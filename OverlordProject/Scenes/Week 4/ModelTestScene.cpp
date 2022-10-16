#include "stdafx.h"
#include "ModelTestScene.h"
#include "Materials/DiffuseMaterial.h"

void ModelTestScene::Initialize()
{
	// Ground Plane
	auto& physX = PxGetPhysics();
	auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);

	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	// Convex Chair
	m_pConvexChair = AddChild(new GameObject());
	auto diffuseMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	diffuseMat->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	auto modelComponent = new ModelComponent(L"Meshes/Chair.ovm");
	modelComponent->SetMaterial(diffuseMat);
	m_pConvexChair->AddComponent<ModelComponent>(modelComponent);

	// Physics
	auto rigidBodyComponent = new RigidBodyComponent();
	auto convexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
	rigidBodyComponent->AddCollider(PxConvexMeshGeometry{ convexMesh }, *pDefaultMaterial);

	m_pConvexChair->AddComponent<RigidBodyComponent>(rigidBodyComponent);

	m_pConvexChair->GetTransform()->Translate(0.f, 5.f, 0.f);
}
