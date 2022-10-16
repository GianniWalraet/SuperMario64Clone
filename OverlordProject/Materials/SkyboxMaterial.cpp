#include "stdafx.h"
#include "SkyboxMaterial.h"

void SkyboxMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTexture", m_pDiffuseTexture);
}

void SkyboxMaterial::InitializeEffectVariables()
{
}
