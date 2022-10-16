#pragma once


class SpikyMaterial final : public Material<SpikyMaterial>
{
public:
	SpikyMaterial() : Material<SpikyMaterial>(L"Effects/SpikyShader.fx") {}
	~SpikyMaterial() override = default;
	SpikyMaterial(const SpikyMaterial& other) = delete;
	SpikyMaterial(SpikyMaterial&& other) noexcept = delete;
	SpikyMaterial& operator=(const SpikyMaterial& other) = delete;
	SpikyMaterial& operator=(SpikyMaterial&& other) noexcept = delete;

protected:
	void InitializeEffectVariables() override;
};

