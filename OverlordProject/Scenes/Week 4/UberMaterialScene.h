#pragma once

class UberMaterial;

class UberMaterialScene : public GameScene
{
public:
	UberMaterialScene() : GameScene::GameScene(L"UberMaterialScene") {}
	UberMaterialScene(const UberMaterialScene& other) = delete;
	UberMaterialScene(UberMaterialScene&& other) noexcept = delete;
	UberMaterialScene& operator=(const UberMaterialScene& other) = delete;
	UberMaterialScene& operator=(UberMaterialScene&& other) noexcept = delete;
	virtual ~UberMaterialScene() override = default;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void OnGUI() override;
private:
	GameObject* m_pSphere{};
	UberMaterial* m_pUberMat{};

	inline const static float m_RotationSpeed = 20.f;
};

