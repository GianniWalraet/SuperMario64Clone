#include "stdafx.h"
#include "Mario.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Prefabs/Camera/ThirdPersonCamera.h"
#include "Singletons/GameState.h"

Mario::Mario(const CharacterDesc& characterDesc, float cameraOffset)
	: m_CharacterDesc{ characterDesc }
	, m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime)
	, m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
	, m_CamOffset{ cameraOffset }
{
}


void Mario::TakeDamage()
{
	if (m_IsStunned || m_Lives == 0) return;

	m_IsStunned = true;
	--m_Lives;
	m_TotalVelocity = { 0,0,0 };

	SoundManager::Get()->GetSystem()->playSound(m_pPainSound2D, nullptr, false, nullptr);
}
void Mario::Jump()
{
	m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
	m_PreviousJumpCount = m_JumpCount++;
	m_JumpTriggered = true;
}

void Mario::Initialize(const SceneContext& /*sceneContext*/)
{
	// ---- Player ----
	// Model
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));
	m_pControllerComponent->SetCollisionGroup(CollisionGroup::Group1);

	auto marioMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	auto eyesMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	marioMat->SetDiffuseTexture(L"Textures/mariotex.png");
	eyesMat->SetDiffuseTexture(L"Textures/eyetex.png");
	m_pModel = AddChild(new GameObject);
	auto modelComp = m_pModel->AddComponent(new ModelComponent(L"Meshes/Mario.ovm"));
	modelComp->SetMaterial(marioMat);
	modelComp->SetMaterial(eyesMat, 5);
	m_pModel->GetTransform()->Translate(0, -6.f, 0);
	m_pModel->GetTransform()->Rotate(0.f, 180.f, 0.f);


	// Footpos
	auto footPosGO = m_pModel->AddChild(new GameObject());
	footPosGO->GetTransform()->Translate(m_pControllerComponent->GetFootPosition());

	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,0.f,0.f };
	settings.minSize = 0.3f;
	settings.maxSize = 0.7f;
	settings.minEnergy = 0.2f;
	settings.maxEnergy = 0.2f;
	settings.minScale = 0.5f;
	settings.maxScale = 1.f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .5f;
	settings.color = { 1.f,1.f,1.f, .6f };
	m_pJumpEmitter = footPosGO->AddComponent(new ParticleEmitterComponent(L"Textures/cloud.png", settings));
	m_pJumpEmitter->Disable();

	// Animations
	m_pAnimator = modelComp->GetAnimator();
	m_pAnimator->SetAnimation(m_AnimationClipId);

	for (int i = 0; i < static_cast<int>(m_pAnimator->GetClipCount()); i++)
	{
		auto clip = m_pAnimator->GetClip(i);
		m_AnimationIDs[clip.name] = i;
	}

	m_pAnimator->SetAnimation(m_AnimationIDs[L"Idle"]);
	m_pAnimator->Play();
	m_CurrentState = L"Idle";
	m_PreviousState = m_CurrentState;

	// ---- Camera ----
	m_pCamPivot = AddChild(new GameObject());
	m_pCamera = m_pCamPivot->AddChild(new ThirdPersonCamera(this, m_CamOffset));
	m_pCameraComponent = m_pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true);

	m_pCamera->GetTransform()->Translate(0.f, -m_CharacterDesc.controller.height * .5f, 0.f);

	// ---- Sound ----
	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();

	auto fModResult = pFmodSystem->createSound("Resources/Sounds/MarioWa.wav", FMOD_DEFAULT | FMOD_2D, nullptr, &m_pJumpSound2D);
	m_pJumpSound2D->setMode(FMOD_LOOP_OFF);
	if (fModResult != FMOD_OK) std::cerr << "Could not play sound\n";

	fModResult = pFmodSystem->createSound("Resources/Sounds/MarioPain.wav", FMOD_DEFAULT | FMOD_2D, nullptr, &m_pPainSound2D);
	m_pPainSound2D->setMode(FMOD_LOOP_OFF);
	if (fModResult != FMOD_OK) std::cerr << "Could not play sound\n";
}
void Mario::Update(const SceneContext& sceneContext)
{
	if (GameState::Get().GetPaused())
	{
		m_pAnimator->Pause();
		return;
	}
	else if (!m_pAnimator->IsPlaying())
	{
		m_pAnimator->Play();
	}

	HandleInput(sceneContext);
	HandleState();
}

void Mario::HandleInput(const SceneContext& sceneContext)
{
	if (!m_pCameraComponent->IsActive()) return;

	if (m_IsStunned)
	{
		m_StunnedTime += sceneContext.pGameTime->GetElapsed();
		if (m_StunnedTime > m_MaxStunnedTime)
		{
			m_IsStunned = false;
			m_StunnedTime = 0.f;
		}
	}

	constexpr float epsilon{ 0.01f };
	bool xPressed = InputManager::IsGamepadButton(InputState::down, XINPUT_GAMEPAD_X);
	m_CurrentState = xPressed ? L"Dance" : L"Idle";
	m_IdleTime = xPressed ? 0.f : m_IdleTime;
	//***************
	//HANDLE INPUT

	XMFLOAT2 move{ 0,0 };
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
		move.y += 1;
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
		move.y -= 1;

	if (abs(move.y) < epsilon)  move.y = InputManager::GetThumbstickPosition().y;

	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
		move.x += 1;
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft))
		move.x -= 1;

	if (abs(move.x) < epsilon) move.x = InputManager::GetThumbstickPosition().x;

	//## Input Gathering (look)
	XMFLOAT2 look{ 0.f, 0.f };
	if (InputManager::IsMouseButton(InputState::down, VK_LBUTTON))
	{
		auto movement = InputManager::GetMouseMovement();
		look = XMFLOAT2(static_cast<float>(movement.y), static_cast<float>(movement.x));
	}
	if (abs(look.x) < epsilon && abs(look.y) < epsilon)
	{
		look = InputManager::GetThumbstickPosition(false);
		std::swap(look.x, look.y);
	}

	//************************
	//GATHERING TRANSFORM INFO

	auto transform = m_pCamPivot->GetTransform();

	auto forward = XMVectorSet(transform->GetForward().x, transform->GetForward().y, transform->GetForward().z, 0.f);
	auto right = XMVectorSet(transform->GetRight().x, transform->GetRight().y, transform->GetRight().z, 0.f);

	//***************
	//CAMERA ROTATION

	m_TotalPitch += look.x * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();
	m_TotalYaw += look.y * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();
	MathHelper::Clamp(m_TotalPitch, 50.f, 0.f);
	transform->Rotate(m_TotalPitch, m_TotalYaw, 0.f);

	//********
	//MOVEMENT
	//## Horizontal Velocity (Forward/Backward/Right/Left)
	if (!m_IsStunned)
	{
		auto moveAcceleration = m_MoveAcceleration * sceneContext.pGameTime->GetElapsed();
		if (abs(move.x) > epsilon || abs(move.y) > epsilon)
		{
			m_CurrentState = std::max(abs(move.x), abs(move.y)) < 0.5f ? L"Walk" : L"Run";
			XMStoreFloat3(&m_CurrentDirection, forward * move.y + right * move.x);
			m_MoveSpeed += moveAcceleration;
			if (m_MoveSpeed > m_CharacterDesc.maxMoveSpeed) m_MoveSpeed = m_CharacterDesc.maxMoveSpeed;
		}
		else
		{
			m_MoveSpeed -= moveAcceleration;
			if (m_MoveSpeed < 0.f) m_MoveSpeed = 0.f;
		}


		auto horVelocityX = m_CurrentDirection.x * m_MoveSpeed;
		auto horVelocityZ = m_CurrentDirection.z * m_MoveSpeed;

		m_TotalVelocity.x = horVelocityX;
		m_TotalVelocity.z = horVelocityZ;
	}
	else
	{
		m_CurrentState = L"Stunned";
	}

	//## Vertical Movement (Jump/Fall)
	XMFLOAT3 pos = m_pControllerComponent->GetFootPosition();

	PxRaycastBuffer hit{};
	XMFLOAT3 castDir{ 0, 1, 0 };
	if (GetScene()->GetPhysxProxy()->Raycast(PhysxHelper::ToPxVec3(pos), { 0,-1,0 }, 1.f, hit, PxHitFlag::eNORMAL))
	{
		castDir = MathHelper::ToFloat3(hit.block.normal);
		//const auto newRot = MathHelper::FromToRotation(m_pControllerComponent->GetTransform()->GetUp(), castDir);
		//m_pModel->GetTransform()->Rotate(XMLoadFloat4(&newRot));
	}

	castDir = { -castDir.x, -castDir.y, -castDir.z };
	bool isGrounded = GetScene()->GetPhysxProxy()->Raycast(PhysxHelper::ToPxVec3(pos), PhysxHelper::ToPxVec3(castDir), 0.01f, hit, PxHitFlag::eMESH_BOTH_SIDES) ||
		m_pControllerComponent->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN);

	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump) && m_JumpCount < m_MaxJumps)
	{
		if (!m_IsStunned)
		{
			m_IdleTime = 0.f;
			m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
			m_PreviousJumpCount = m_JumpCount++;
			m_pJumpEmitter->Burst(0.2f);

			FMOD::Channel* channel;
			SoundManager::Get()->GetSystem()->playSound(m_pJumpSound2D, nullptr, true, &channel);
			channel->setPaused(false);
		}
	}
	else if (!isGrounded)
	{
		m_IdleTime = 0.f;
		m_TotalVelocity.y -= m_FallAcceleration * sceneContext.pGameTime->GetElapsed();
		//if (m_TotalVelocity.y < -m_CharacterDesc.maxFallSpeed) m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;
	}
	else
	{
		if (!m_JumpTriggered)
		{
			m_TotalVelocity.y = m_GravityForce;
			m_JumpCount = 0;
			m_PreviousJumpCount = 0;
		}
		m_JumpTriggered = false;
	}


	//************
	//DISPLACEMENT
	float minFallSpeed = -3.f;
	if (m_TotalVelocity.y > epsilon)
	{
		m_CurrentState = L"Jump";
	}
	else if (m_TotalVelocity.y < minFallSpeed)
	{
		m_CurrentState = L"Fall";
	}

	float displacementX = m_TotalVelocity.x * sceneContext.pGameTime->GetElapsed();
	float displacementY = m_TotalVelocity.y * sceneContext.pGameTime->GetElapsed();
	float displacementZ = m_TotalVelocity.z * sceneContext.pGameTime->GetElapsed();
	m_pControllerComponent->Move(XMFLOAT3(displacementX, displacementY, displacementZ));

	if (m_IsStunned) return;

	if (abs(move.x) > epsilon || abs(move.y) > epsilon)
	{
		m_IdleTime = 0.f;

		auto camForward = XMLoadFloat3(&m_pCameraComponent->GetTransform()->GetForward());
		auto camRight = XMLoadFloat3(&m_pCameraComponent->GetTransform()->GetRight());

		auto camRotForward = XMVector3Normalize({ XMVectorGetX(camForward), 0, XMVectorGetZ(camForward) });
		auto camRotRight = XMVector3Normalize({ XMVectorGetX(camRight), 0, XMVectorGetZ(camRight) });

		auto movementDir = XMVector2Normalize({ move.x, move.y });
		auto movementForward = XMVectorSet(move.y * XMVectorGetX(camRotForward), 0.f, move.y * XMVectorGetZ(camRotForward), 0.f);
		auto movementRight = XMVectorSet(move.x * XMVectorGetX(camRotRight), 0.f, move.x * XMVectorGetZ(camRotRight), 0.f);
		auto movementTotal = movementForward + movementRight;
		float rotAngle = -atan2(XMVectorGetZ(movementTotal), XMVectorGetX(movementTotal));

		rotAngle = XMConvertToDegrees(rotAngle);
		m_pModel->GetTransform()->Rotate(0.f, rotAngle - 90.f, 0.f);
	}
	else
	{
		m_IdleTime += sceneContext.pGameTime->GetElapsed();
		if (m_IdleTime > m_MaxIdleTime)
		{
			m_CurrentState = L"SitIdle";
		}
	}
}

void Mario::HandleState()
{
	if (m_CurrentState == m_PreviousState && m_PreviousJumpCount == m_JumpCount) return;

	m_PreviousJumpCount = m_JumpCount;
	m_pAnimator->SetAnimation(m_AnimationIDs[m_CurrentState]);
	m_PreviousState = m_CurrentState;
}


