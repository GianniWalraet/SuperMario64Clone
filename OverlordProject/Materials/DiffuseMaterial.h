#pragma once


class DiffuseMaterial final : public Material<DiffuseMaterial>
{
public:
	DiffuseMaterial() : Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fx") {}
	~DiffuseMaterial() override = default;
	DiffuseMaterial(const DiffuseMaterial& other) = delete;
	DiffuseMaterial(DiffuseMaterial&& other) noexcept = delete;
	DiffuseMaterial& operator=(const DiffuseMaterial& other) = delete;
	DiffuseMaterial& operator=(DiffuseMaterial&& other) noexcept = delete;

	void UseTransparency(bool enabled);
	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetInvertNormals(bool enabled);

protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pDiffuseTexture{};
};

