#pragma once



class UberMaterial : public Material<UberMaterial>
{
public:
	UberMaterial() : Material<UberMaterial>(L"Effects/UberShader.fx") {}
	~UberMaterial() override = default;
	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetNormalTexture(const std::wstring& assetFile);
	void SetOpacityTexture(const std::wstring& assetFile);
	void SetEnvironmentTexture(const std::wstring& assetFile);

	void UseDiffuseTexture(bool enabled);
	void UseSpecularTexture(bool enabled);
	void UseOpacityTexture(bool enabled);
	void UseNormalTexture(bool enabled);
	void UseEnvironmentTexture(bool enabled);

	void SetColor(const XMFLOAT4& color);
	void SetLightDirection(const XMFLOAT3& dir);

	void UseTransparency(bool enabled);
protected:
	void InitializeEffectVariables() override;
private:
	TextureData* m_pDiffuseTexture{};
	TextureData* m_pNormalTexture{};
	TextureData* m_pOpacityTexture{};
	TextureData* m_pEnvironmentTexture{};
};

