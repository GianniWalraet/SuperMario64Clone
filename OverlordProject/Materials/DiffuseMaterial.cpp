#include "stdafx.h"
#include "DiffuseMaterial.h"


void DiffuseMaterial::UseTransparency(bool enabled)
{
	SetTechnique(enabled ? L"TransparencyTech" : L"Default");
}
void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial::SetInvertNormals(bool enabled)
{
	SetVariable_Scalar(L"gInvertNormals", enabled);
}

void DiffuseMaterial::InitializeEffectVariables()
{
	UseTransparency(false);
}