#pragma once


class ThirdPersonCamera : public GameObject
{
public:
	ThirdPersonCamera(GameObject* pTarget, float offset = 50.f);
	~ThirdPersonCamera() override = default;
	ThirdPersonCamera(const ThirdPersonCamera& other) = delete;
	ThirdPersonCamera(ThirdPersonCamera&& other) noexcept = delete;
	ThirdPersonCamera& operator=(const ThirdPersonCamera& other) = delete;
	ThirdPersonCamera& operator=(ThirdPersonCamera&& other) noexcept = delete;

	const float GetOffset() const { return m_DefaultOffset; }
	void SetOffset(float offset) { m_DefaultOffset = offset; }
protected:
	void Update(const SceneContext& sceneContext) override;
private:
	GameObject* m_pTarget;

	float m_DefaultOffset{};
	float m_CurrentOffset{};

	float m_FollowSpeed{ 1.f };
};

