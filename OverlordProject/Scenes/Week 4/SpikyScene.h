#pragma once

class SpikyMaterial;

class SpikyScene final : public GameScene
{
public:
	SpikyScene() : GameScene::GameScene(L"SpikyScene") {}
	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;
	virtual ~SpikyScene() override = default;

protected:
	void Initialize() override;

	void OnGUI() override;
private:
	GameObject* m_pSphere{};
	SpikyMaterial* m_pSpikyMat{};
};

