#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

void SpikyScene::Initialize()
{
	// SceneSettings
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.pCamera->GetTransform()->Translate(0, 25, -10);

	// SpikySphere
	m_pSphere = AddChild(new GameObject());

	m_pSpikyMat = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();
	auto modelComp = new ModelComponent(L"Meshes/OctaSphere.ovm");
	modelComp->SetMaterial(m_pSpikyMat);
	m_pSphere->AddComponent<ModelComponent>(modelComp);

	m_pSphere->GetTransform()->Translate(0, 20, 0);
}

void SpikyScene::OnGUI()
{
	m_pSpikyMat->DrawImGui();
}
