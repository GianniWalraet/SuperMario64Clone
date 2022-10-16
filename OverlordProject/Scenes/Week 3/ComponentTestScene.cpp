#include "stdafx.h"
#include "ComponentTestScene.h"
#include "Prefabs/SpherePrefab.h"

ComponentTestScene::ComponentTestScene()
	: GameScene::GameScene(L"ComponentTestScene") {}

void ComponentTestScene::Initialize()
{
	//m_SceneContext.settings.enableOnGUI = true;
	auto& physX = PxGetPhysics();

	const auto pBouncyMaterial = physX.createMaterial(.5f, .5f, .9f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	// Sphere 1 (Group 1 & 2)
	auto pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Red }));
	auto pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent(false));
	pSphereActor->AddCollider(PxSphereGeometry{1.f}, *pBouncyMaterial);
	pSphereActor->SetCollisionGroup(CollisionGroup::Group1 | CollisionGroup::Group2);

	pSphereObject->GetTransform()->Translate(0.f, 40.f, 0.f);

	// Sphere 2 (Group 0 + ignore group 1)
	pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Green }));
	pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent(false));
	pSphereActor->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);

	pSphereObject->GetTransform()->Translate(0.f, 30.f, 0.f);

	// Sphere 3
	pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Blue }));
	pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent(false));
	pSphereActor->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	pSphereActor->SetCollisionGroup(CollisionGroup::Group5 | CollisionGroup::Group6);

	pSphereObject->GetTransform()->Translate(0.f, 20.f, 0.f);
}

void ComponentTestScene::Update()
{

}
void ComponentTestScene::Draw()
{

}
