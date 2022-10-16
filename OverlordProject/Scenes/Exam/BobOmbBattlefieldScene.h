#pragma once

class Mario;
class PauseHUD;
class MainGameHUD;
class Goomba;
class PowerStar;

class BobOmbBattlefieldScene final : public GameScene
{
public:
	BobOmbBattlefieldScene() : GameScene::GameScene(L"BobOmbBattlefieldScene") {}
	virtual ~BobOmbBattlefieldScene() override = default;
	BobOmbBattlefieldScene(const BobOmbBattlefieldScene& other) = delete;
	BobOmbBattlefieldScene(BobOmbBattlefieldScene&& other) noexcept = delete;
	BobOmbBattlefieldScene& operator=(const BobOmbBattlefieldScene& other) = delete;
	BobOmbBattlefieldScene& operator=(BobOmbBattlefieldScene&& other) noexcept = delete;
protected:
	void Initialize() override;
	void Update() override;

	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump
	};

	Mario* m_pMario{};
	GameObject* m_pLevel{};
	GameObject* m_pBridge{};
	PauseHUD* m_pPauseHUD{};
	MainGameHUD* m_pMainGameHUD{};

	std::deque<Goomba*> m_pGoombas{};
	std::deque<PowerStar*> m_pPowerStars{};

	bool m_IsPaused{};
	bool m_HasWon{};
	bool m_HasLost{};

	float m_ElapsedGameEndTime{};
	float m_WinWaitTime{4.f};
	float m_DefeatWaitTime{6.f};

	uint8_t m_NrOfStars{};
	uint8_t m_NrOfEnemies{};

	FMOD::Channel* m_pMainThemeSound2D{};
	FMOD::Sound* m_pMarioHereWeGoSound2D{};
	FMOD::Sound* m_pLevelVictorySound2D{};
	FMOD::Sound* m_pLevelDefeatSound2D{};

	void UpdateBobOmbBalls();
	float m_BallSpawnDelay{10.f}, m_ElapsedBallSpawnDelay{ 10.f };

	void HandleGameEnd();
	void HandleGamePause();

	void GenerateLevelBounds();

	void LoadMap();
	void LoadSounds();
	void LoadPlayer();
	void LoadEnemies();
	void LoadPickups();
	void LoadHUD();
};

