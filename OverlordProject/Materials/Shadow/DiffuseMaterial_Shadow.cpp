#include "stdafx.h"
#include "DiffuseMaterial_Shadow.h"

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow.fx")
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow::SetIsTransparent(bool transparent)
{
	m_IsTransparent = transparent;
}

void DiffuseMaterial_Shadow::InitializeEffectVariables()
{
	SetIsTransparent(false);
}

void DiffuseMaterial_Shadow::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	
	//  TODO_W8
	//  Update The Shader Variables
	//  1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	  	// = model_world * light_viewprojection
	XMMATRIX matWorld = XMLoadFloat4x4(&pModel->GetTransform()->GetWorld());
	XMMATRIX matLightVP = XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP());
	XMMATRIX lightWVP = matWorld * matLightVP;
	XMFLOAT4X4 storedWVP{};
	XMStoreFloat4x4(&storedWVP, lightWVP);
	SetVariable_Matrix(L"gWorldViewProj_Light", storedWVP);
	 
	//  2. Update the ShadowMap texture
	SetVariable_Texture(L"gShadowMap", ShadowMapRenderer::Get()->GetShadowMap());

	// 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	//Update Shadow Variables
	//const auto& pShadowMapRenderer = ShadowMapRenderer::Get();
}
