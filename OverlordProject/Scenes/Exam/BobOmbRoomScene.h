#pragma once

class PostRipple;

class BobOmbRoomScene : public GameScene
{
public:
	BobOmbRoomScene() : GameScene::GameScene(L"BobOmbRoomScene") {}
	virtual ~BobOmbRoomScene() override = default;
	BobOmbRoomScene(const BobOmbRoomScene& other) = delete;
	BobOmbRoomScene(BobOmbRoomScene&& other) noexcept = delete;
	BobOmbRoomScene& operator=(const BobOmbRoomScene& other) = delete;
	BobOmbRoomScene& operator=(BobOmbRoomScene&& other) noexcept = delete;
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

	GameObject* m_pMario{};
	GameObject* m_pLevel{};
	GameObject* m_pPaintingTrigger{};
	CameraComponent* m_pPaintingCamComponent{};

	PostRipple* m_pPostRipple{};
	bool m_PaintingActivated{ false };
	bool m_StartLevel{};

	float m_ElapsedWaitTime{};
	float m_LoadLvlWaitTime{3.f};

	SpriteFont* m_pFont{};

	FMOD::Sound* m_pEnterPaintingSound2D{};
	FMOD::Sound* m_pGameStartSound2D{};
	FMOD::Channel* m_pCastleTheme2D{};

	void OnPaintingTriggered();

	void LoadPaintingTrigger();
	void LoadPlayer();
	void LoadLevel();
	void LoadSound();
};

