#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/UberMaterial.h"

void UberMaterialScene::Initialize()
{
	// SceneSettings
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.pCamera->GetTransform()->Translate(0, 25, -10);

	// UberSphere
	m_pSphere = AddChild(new GameObject());
	
	m_pUberMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	m_pUberMat->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	m_pUberMat->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	m_pUberMat->SetOpacityTexture(L"Textures/Specular_Level.jpg");
	m_pUberMat->SetEnvironmentTexture(L"Textures/Sunol_Cubemap.dds");

	auto modelComp = new ModelComponent(L"Meshes/Sphere.ovm");
	modelComp->SetMaterial(m_pUberMat);
	m_pSphere->AddComponent<ModelComponent>(modelComp);

	m_pSphere->GetTransform()->Translate(0, 20, 0);
	m_pSphere->GetTransform()->Scale(2,2,2);
}

void UberMaterialScene::Update()
{
	m_pSphere->GetTransform()->Rotate(XMFLOAT3{ 0,m_SceneContext.pGameTime->GetTotal() * m_RotationSpeed,0 });
}
void UberMaterialScene::Draw()
{
}

void UberMaterialScene::OnGUI()
{
	m_pUberMat->DrawImGui();
}
