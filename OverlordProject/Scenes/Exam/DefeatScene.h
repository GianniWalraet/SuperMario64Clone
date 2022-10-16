#pragma once


class DefeatScene : public GameScene
{
public:
	DefeatScene() : GameScene::GameScene(L"DefeatScene") {}
	virtual ~DefeatScene() override = default;
	DefeatScene(const DefeatScene& other) = delete;
	DefeatScene(DefeatScene&& other) noexcept = delete;
	DefeatScene& operator=(const DefeatScene& other) = delete;
	DefeatScene& operator=(DefeatScene&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	GameObject* m_pCastle{};
	GameObject* m_pMario{};
	GameObject* m_pGoomba{};

	ModelAnimator* m_Animator{};

	FMOD::Channel* m_pMainMenuSound2D{};

	float m_ElapsedTime{};
	float m_AnimationPlayTime{3.f};

	void LoadPlayer();
	void LoadEnemy();
	void LoadLevel();
	void LoadSound();
};

