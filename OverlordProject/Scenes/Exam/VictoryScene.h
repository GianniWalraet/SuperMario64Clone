#pragma once


class VictoryScene : public GameScene
{
public:
	VictoryScene() : GameScene::GameScene(L"VictoryScene") {}
	virtual ~VictoryScene() override = default;
	VictoryScene(const VictoryScene& other) = delete;
	VictoryScene(VictoryScene&& other) noexcept = delete;
	VictoryScene& operator=(const VictoryScene& other) = delete;
	VictoryScene& operator=(VictoryScene&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	GameObject* m_pCastle{};
	GameObject* m_pMario{};
	GameObject* m_pVictoryStar{};

	FMOD::Channel* m_pMainMenuSound2D{};

	void LoadPlayer();
	void LoadVictoryStar();
	void LoadLevel();
	void LoadSound();
};

