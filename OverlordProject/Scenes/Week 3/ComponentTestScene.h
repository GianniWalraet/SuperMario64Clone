#pragma once

class ComponentTestScene final : public GameScene
{
public:
	ComponentTestScene();
	ComponentTestScene(const ComponentTestScene& other) = delete;
	ComponentTestScene(ComponentTestScene&& other) noexcept = delete;
	ComponentTestScene& operator=(const ComponentTestScene& other) = delete;
	ComponentTestScene& operator=(ComponentTestScene&& other) noexcept = delete;
	virtual ~ComponentTestScene() = default;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
};

