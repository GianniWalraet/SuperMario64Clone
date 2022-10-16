#include "stdafx.h"
#include "UberMaterial.h"

void UberMaterial::InitializeEffectVariables()
{
	UseDiffuseTexture(true);
	UseSpecularTexture(false);
	UseOpacityTexture(false);
	UseNormalTexture(true);
	UseEnvironmentTexture(true);
	UseTransparency(true);

	SetColor(XMFLOAT4{ 1,0,0,1 });
	SetLightDirection(XMFLOAT3(-0.577f, -0.577f, 0.577f));

	SetVariable_Scalar(L"gReflectionStrength", 0.2f);
	SetVariable_Scalar(L"gRefractionStrength", 0.1f);
	SetVariable_Scalar(L"gUseSpecularPhong", true);

	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Scalar(L"gFresnelPower", 10.f);
	SetVariable_Scalar(L"gFresnelMultiplier", 10.f);
	SetVariable_Scalar(L"gFresnelHardness", 10.f);
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);
}
void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureNormal", m_pNormalTexture);
}
void UberMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacityTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureOpacity", m_pOpacityTexture);
}
void UberMaterial::SetEnvironmentTexture(const std::wstring& assetFile)
{
	m_pEnvironmentTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gCubeEnvironment", m_pEnvironmentTexture);
}

void UberMaterial::SetColor(const XMFLOAT4& color)
{
	SetVariable_Vector(L"gColorDiffuse", color);
}
void UberMaterial::SetLightDirection(const XMFLOAT3& dir)
{
	SetVariable_Vector(L"gLightDirection", dir);
}

void UberMaterial::UseDiffuseTexture(bool enabled)
{
	SetVariable_Scalar(L"gUseTextureDiffuse", enabled);
}
void UberMaterial::UseSpecularTexture(bool enabled)
{
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", enabled);
}
void UberMaterial::UseOpacityTexture(bool enabled)
{
	SetVariable_Scalar(L"gTextureOpacityIntensity", enabled);
}
void UberMaterial::UseNormalTexture(bool enabled)
{
	SetVariable_Scalar(L"gUseTextureNormal", enabled);
}
void UberMaterial::UseEnvironmentTexture(bool enabled)
{
	SetVariable_Scalar(L"gUseEnvironmentMapping", enabled);
}
void UberMaterial::UseTransparency(bool enabled)
{
	SetTechnique(enabled ? L"WithAlphaBlending" : L"WithoutAlphaBlending");
}