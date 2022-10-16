#pragma once
class PostRipple : public PostProcessingMaterial
{
public:
	PostRipple();
	~PostRipple() override = default;
	PostRipple(const PostRipple& other) = delete;
	PostRipple(PostRipple&& other) noexcept = delete;
	PostRipple& operator=(const PostRipple& other) = delete;
	PostRipple& operator=(PostRipple&& other) noexcept = delete;

	void SetEnabled(bool isEnabled, bool fadeToWhite);
	void Reset();
protected:
	void Initialize(const GameContext& /*gameContext*/) override;
	void Draw(const SceneContext& sceneContext, RenderTarget* pSource) override;
	void UpdateBaseEffectVariables(const SceneContext& sceneContext, RenderTarget* pSource) override;

	bool m_IsEnabled{};
	float m_TimeSinceEnabled{};
};


