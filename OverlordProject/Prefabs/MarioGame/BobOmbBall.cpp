#include "stdafx.h"
#include "BobOmbBall.h"
#include "Materials/ColorMaterial.h"
#include "Prefabs/MarioGame/Mario.h"
#include "Singletons/GameState.h"

BobOmbBall::BobOmbBall(const std::deque<XMFLOAT3>& moveTargets, float speed)
	: m_MoveTargets{ moveTargets }
	, m_MovementSpeed{ speed, speed, speed }
{
	if (m_MoveTargets.empty() || m_MoveTargets.size() == 1)
	{
		m_MarkedForDelete = true;
		return;
	}

	for (auto& target : m_MoveTargets)
	{
		target.y += m_YOffset;
	}

	auto startPos = m_MoveTargets.front();
	m_MoveTargets.pop_front();
	GetTransform()->Translate(startPos);
	m_CurrentTarget = m_MoveTargets.front();
	m_MoveTargets.pop_front();
	m_MoveDir = MathHelper::Normalized(MathHelper::SubtractFloat3(m_CurrentTarget, startPos));
}

void BobOmbBall::Initialize(const SceneContext&)
{
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);

	float scale{ 1.5f };

	m_pModel = AddChild(new GameObject());
	m_pModel->GetTransform()->Scale(scale);
	auto modelComp = m_pModel->AddComponent(new ModelComponent(L"Meshes/Sphere.ovm"));
	auto mat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	mat->SetColor(XMFLOAT4(Colors::Black));
	modelComp->SetMaterial(mat);

	auto rigidBody = AddComponent(new RigidBodyComponent());
	rigidBody->AddCollider(PxSphereGeometry(scale), *pDefaultMaterial, true);

	SetOnTriggerCallBack([&](GameObject* /*pTrigger*/, GameObject* pOther, PxTriggerAction action)
		{
			if (dynamic_cast<Mario*>(pOther) && action == PxTriggerAction::ENTER)
			{
				auto mario = dynamic_cast<Mario*>(pOther);
				mario->TakeDamage();
			}
		});
}
void BobOmbBall::Update(const SceneContext& sceneContext)
{
	if (GameState::Get().GetPaused()) return;

	auto curPos = GetTransform()->GetPosition();
	auto elapsedSec = sceneContext.pGameTime->GetElapsed();

	curPos.x += (m_MovementSpeed.x * m_MoveDir.x) * elapsedSec;
	curPos.y += (m_MovementSpeed.y * m_MoveDir.y) * elapsedSec;
	curPos.z += (m_MovementSpeed.z * m_MoveDir.z) * elapsedSec;

	GetTransform()->Translate(curPos);

	CheckNextPointReached();

	if (m_MarkedForDelete)
	{
		GetScene()->RemoveChild(this, true);
	}
}

void BobOmbBall::SetNextTarget()
{
	if (m_MoveTargets.empty())
	{
		m_MarkedForDelete = true;
		return;
	}
	m_CurrentTarget = m_MoveTargets.front();
	m_MoveTargets.pop_front();

	m_MoveDir = MathHelper::Normalized(MathHelper::SubtractFloat3(m_CurrentTarget, GetTransform()->GetPosition()));
	m_PrevDistance = FLT_MAX;
}
void BobOmbBall::CheckNextPointReached()
{
	auto distSqrd = MathHelper::DistanceSquared(m_CurrentTarget, GetTransform()->GetPosition());

	if (distSqrd > m_PrevDistance)
	{
		SetNextTarget();
	}
	else
	{
		m_PrevDistance = distSqrd;
	}
}
