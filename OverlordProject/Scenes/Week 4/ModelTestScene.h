#pragma once


class ModelTestScene : public GameScene
{
public:
	ModelTestScene() : GameScene::GameScene(L"ModelTestScene") {}
	ModelTestScene(const ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;
	virtual ~ModelTestScene() override = default;

protected:
	void Initialize() override;
private:
	GameObject* m_pConvexChair{};
};


