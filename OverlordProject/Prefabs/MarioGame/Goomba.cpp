#include "stdafx.h"
#include "Goomba.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/MarioGame/Mario.h"
#include "Singletons/GameState.h"

Goomba::Goomba(const CharacterDesc& characterDesc, float detectionRadius)
	: m_CharacterDesc{ characterDesc }
	, m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime)
	, m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
	, m_DetectionRadius{ detectionRadius }
{
}

void Goomba::Initialize(const SceneContext&)
{
	m_WanderWaitTime = MathHelper::RandomFloat(5.f, 15.f);

	// Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));
	m_pControllerComponent->SetCollisionGroup(CollisionGroup::Group1);

	// model
	m_pModel = AddChild(new GameObject());
	auto mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Goomba/Goomba.png");
	m_pModel->AddComponent(new ModelComponent(L"Meshes/Goomba.ovm"))->SetMaterial(mat);

	m_pModel->GetTransform()->Translate(0, -0.2f, 0);
	m_pModel->GetTransform()->Rotate(90.f, 0.f, 0.f);
	m_pModel->GetTransform()->Scale(0.3f);

	// Triggers
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(0.f, 0.f, 0.f);

	// Kill trigger
	m_pKillTrigger = AddChild(new GameObject());

	auto rigidBody = m_pKillTrigger->AddComponent(new RigidBodyComponent(true));
	UINT ID = rigidBody->AddCollider(PxBoxGeometry(.6f, 0.2f, .6f), *pDefaultMaterial, true);
	m_HeadCollider = rigidBody->GetCollider(ID).GetShape();

	m_pKillTrigger->SetOnTriggerCallBack([&](GameObject* /*pTrigger*/, GameObject* pOther, PxTriggerAction action)
		{
			if (dynamic_cast<Mario*>(pOther) && action == PxTriggerAction::ENTER)
			{
				if (m_MarkedForDelete) return;
				auto mario = dynamic_cast<Mario*>(pOther);
				float marioY = mario->GetComponent<ControllerComponent>()->GetFootPosition().y;
				float goombaY = GetTransform()->GetWorldPosition().y + 0.4f;

				if (marioY > goombaY) 
				{
					m_MarkedForDelete = true;
					mario->Jump();
					FMOD::Channel* c{};
					SoundManager::Get()->GetSystem()->playSound(m_pStompSound2D, nullptr, false, &c);
					c->setVolume(1.5f);
				}

			}
		});

	// Attack trigger
	m_pAttackTrigger = AddChild(new GameObject());

	rigidBody = m_pAttackTrigger->AddComponent(new RigidBodyComponent(true));
	ID = rigidBody->AddCollider(PxBoxGeometry(.7f, 0.6f, .7f), *pDefaultMaterial, true);
	m_BodyCollider = rigidBody->GetCollider(ID).GetShape();

	m_pAttackTrigger->SetOnTriggerCallBack([&](GameObject* /*pTrigger*/, GameObject* pOther, PxTriggerAction action)
		{
			if (dynamic_cast<Mario*>(pOther) && action == PxTriggerAction::ENTER)
			{
				OnDamageGiven(pOther);
			}
		});

	// Sound
	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();

	pFmodSystem->createSound("Resources/Sounds/GoombaStomp.mp3", FMOD_DEFAULT | FMOD_2D, nullptr, &m_pStompSound2D);
	m_pStompSound2D->setMode(FMOD_LOOP_OFF);
}
void Goomba::PostInitialize(const SceneContext&)
{
	m_OriginalPos = GetTransform()->GetPosition();
	m_TargetPos = m_OriginalPos;
}

void Goomba::Update(const SceneContext& sceneContext)
{
	if (GameState::Get().GetPaused()) return;

	if (m_IsDead) return;

	UpdateCollisionBoxes();

	if (m_pTarget)
	{
		if (IsInView(m_pTarget->GetTransform()->GetWorldPosition()))
		{
			if (m_IgnoreTarget)
			{
				Wander(sceneContext.pGameTime->GetElapsed());

				m_ElapsedIgnoreTime += sceneContext.pGameTime->GetElapsed();
				if (m_ElapsedIgnoreTime > m_IgnoreTime)
				{
					m_ElapsedIgnoreTime = 0.f;
					m_IgnoreTarget = false;
				}
			}
			else
			{
				m_TargetPos = m_pTarget->GetTransform()->GetWorldPosition();
				m_MovementSpeed = m_ChaseSpeed;
			}
		}
		else
		{
			Wander(sceneContext.pGameTime->GetElapsed());
		}
	}

	if (m_MarkedForDelete)
	{
		ElapsedTime += sceneContext.pGameTime->GetElapsed();
		OnDie();
		return;
	}

	MoveToTarget();
	HandleRotation(sceneContext.pGameTime->GetTotal());
	HandleMovement(sceneContext.pGameTime->GetElapsed());
}

void Goomba::OnDamageGiven(GameObject* pOther)
{
	if (m_MarkedForDelete) return;

	m_IgnoreTarget = true;
	m_WanderElapsedTime = m_WanderWaitTime;

	const auto& player = dynamic_cast<Mario*>(pOther);
	const auto& playerPos = player->GetTransform()->GetWorldPosition();
	const auto& pos = GetTransform()->GetWorldPosition();

	auto dir = MathHelper::Normalized(MathHelper::SubtractFloat3(playerPos, pos));
	player->TakeDamage();
}
void Goomba::OnDie()
{
	m_pModel->GetTransform()->Scale(0.3f, 0.3f, 0.1f);
	if (ElapsedTime > m_DeathAnimationTime)
	{
		m_pControllerComponent->GetTransform()->Translate(0, -50, 0);
		m_HeadCollider->setLocalPose(PxTransform({ 0, -50, 0 }));
		m_BodyCollider->setLocalPose(PxTransform({ 0, -50, 0 }));
		m_IsDead = true;
	}
}

void Goomba::Wander(float dt)
{
	m_WanderElapsedTime += dt;
	if (m_WanderElapsedTime > m_WanderWaitTime)
	{
		CalculateWanderTarget();
		m_WanderElapsedTime = 0.f;
		m_WanderWaitTime = MathHelper::RandomFloat(5.f, 15.f);
	}
}

void Goomba::UpdateCollisionBoxes()
{
	auto pos = GetTransform()->GetPosition();
	m_BodyCollider->setLocalPose(PxTransform(PhysxHelper::ToPxVec3(pos)));

	float yOffset = 0.8f;
	pos.y += yOffset;
	m_HeadCollider->setLocalPose(PxTransform(PhysxHelper::ToPxVec3(pos)));

		
}

bool Goomba::IsInView(const XMFLOAT3& pos)
{
	auto dirToTarget = MathHelper::SubtractFloat3(GetTransform()->GetWorldPosition(), pos);
	auto angle = XMConvertToDegrees(MathHelper::Angle(m_pModel->GetTransform()->GetUp(), MathHelper::Normalized(dirToTarget)));
	if (abs(angle) > m_DetectionAngle) return false;

	if (MathHelper::DistanceSquared(pos, GetTransform()->GetPosition()) < m_DetectionRadius * m_DetectionRadius)
	{
		return true;
	}

	return false;
}
void Goomba::HandleRotation(float totalTime)
{
	const auto& modelTransform = m_pModel->GetTransform();

	auto move = MathHelper::Normalized(m_TotalVelocity);
	auto movementForward = MathHelper::SubtractFloat3(m_TargetPos, GetTransform()->GetWorldPosition());
	float rotAngleY{};

	const auto tiltAngle = cos(totalTime * m_WiggleSpeed) * m_MaxTiltAngle;

	rotAngleY = -atan2(movementForward.z, movementForward.x);
	rotAngleY = XMConvertToDegrees(rotAngleY);
	modelTransform->Rotate(90.f + tiltAngle, rotAngleY - 90.f, 0.f);

}
void Goomba::HandleMovement(float dt)
{
	const auto& footPos = m_pControllerComponent->GetFootPosition();

	PxRaycastBuffer hit{};
	bool isGrounded = GetScene()->GetPhysxProxy()->Raycast(PhysxHelper::ToPxVec3(footPos), { 0,-1,0 }, 0.1f, hit, PxHitFlag::eMESH_BOTH_SIDES) ||
		m_pControllerComponent->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN);

	if (!isGrounded)
		m_TotalVelocity.y -= m_FallAcceleration * dt;
	else
		m_TotalVelocity.y = 0.f;

	float displacementX = m_TotalVelocity.x * dt;
	float displacementY = m_TotalVelocity.y * dt;
	float displacementZ = m_TotalVelocity.z * dt;
	m_pControllerComponent->Move(XMFLOAT3(displacementX, displacementY, displacementZ));
}

void Goomba::MoveToTarget()
{
	auto targetDir = MathHelper::SubtractFloat3(m_TargetPos, GetTransform()->GetWorldPosition());
	targetDir = MathHelper::Normalized(targetDir);

	m_TotalVelocity.x = targetDir.x * m_MovementSpeed;
	m_TotalVelocity.z = targetDir.z * m_MovementSpeed;
}
void Goomba::CalculateWanderTarget()
{
	float dx = MathHelper::RandomFloat(-m_WanderRadius, m_WanderRadius);
	float dz = MathHelper::RandomFloat(-m_WanderRadius, m_WanderRadius);

	XMFLOAT3 newTarget{};
	newTarget.x = m_OriginalPos.x + dx;
	newTarget.z = m_OriginalPos.z + dz;

	m_TargetPos = newTarget;

	m_MovementSpeed = m_WanderSpeed;
}
