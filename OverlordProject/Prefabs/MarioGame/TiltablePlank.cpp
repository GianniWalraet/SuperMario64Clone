#include "stdafx.h"
#include "TiltablePlank.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/MarioGame/Mario.h"
#include "Singletons/GameState.h"

void TiltablePlank::Initialize(const SceneContext&)
{
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);

	XMFLOAT3 scale = { 15.f, 0.5f, 4.f };
	AddChild(new CubePrefab(XMFLOAT3{ scale.x, scale.y, scale.z }, XMFLOAT4{Colors::Brown}));
	auto rigidBody = AddComponent(new RigidBodyComponent(true)); 
	rigidBody->AddCollider(PxBoxGeometry{ scale.x / 2.f, scale.y / 2.f, scale.z / 2.f }, *pDefaultMaterial, false);
}
void TiltablePlank::Update(const SceneContext& sceneContext)
{
	if (GameState::Get().GetPaused()) return;

	GetTransform()->Rotate(0.f, 45.f, sin(sceneContext.pGameTime->GetTotal()) * 20.f, true);
}