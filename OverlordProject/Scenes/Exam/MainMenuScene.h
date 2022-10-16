#pragma once

class Button2D;
class PostRipple;

class MainMenuScene final : public GameScene
{
public:
	MainMenuScene() : GameScene::GameScene(L"MainMenuScene") {}
	virtual ~MainMenuScene() override = default;
	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;

	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	GameObject* m_pSkyBox{};
	GameObject* m_pTitle{};
	GameObject* m_pMarioHead{};
	GameObject* m_pCastle{};

	Button2D* m_pStartButton{};
	Button2D* m_pQuitButton{};

	FMOD::Channel* m_pMainMenuSound2D{};
	FMOD::Sound* m_pMarioSound2D{};

	bool m_IsDragged{};

	void HandleAnimation();

	void LoadLevelModel();
	void LoadHeadModel();
	void LoadLogo();
	void LoadSound();
};


