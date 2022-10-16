#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/MarioGame/Mario.h"

ThirdPersonCamera::ThirdPersonCamera(GameObject* pTarget, float offset)
	: m_pTarget{ pTarget }
	, m_DefaultOffset{ offset }
	, m_CurrentOffset{ offset }
{
	AddComponent(new CameraComponent);
}

void ThirdPersonCamera::Update(const SceneContext& sceneContext)
{
	m_CurrentOffset = m_DefaultOffset;

	const auto& pos = GetTransform()->GetPosition();
	const auto& targetForward = m_pTarget->GetTransform()->GetForward();
	const auto& elapsedTime = sceneContext.pGameTime->GetElapsed();

	XMFLOAT3 newPos{};
	newPos.x = std::lerp(pos.x, targetForward.x, m_FollowSpeed * elapsedTime);
	newPos.y = std::lerp(pos.y, targetForward.y, m_FollowSpeed * elapsedTime);
	newPos.z = std::lerp(pos.z, -targetForward.z * m_CurrentOffset, m_FollowSpeed * elapsedTime);

	GetTransform()->Translate(newPos);
}