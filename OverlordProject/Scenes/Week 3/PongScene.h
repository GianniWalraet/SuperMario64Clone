#pragma once

class PongScene final : public GameScene
{
public:
	PongScene() : GameScene::GameScene(L"PongScene") {}
	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;
	virtual ~PongScene() = default;

protected:
	void Initialize() override;
	void PostInitialize() override;

	void Update() override;
	void Draw() override;
private:
	GameObject* m_pSphere{};
	GameObject* m_pLeftPaddle{};
	GameObject* m_pRightPaddle{};

	XMFLOAT3 m_Speed = XMFLOAT3{ 5.f,0.f,5.f };

	void Reset();
	void OnTriggerCallBack(GameObject* pTrigger, GameObject* pOther, PxTriggerAction action);

	bool m_ResetScene{false};
};

