#pragma once


class Button2D final : public GameObject
{
public:
	Button2D(const std::wstring& pathSelected, const std::wstring& pathNotSelected,const XMFLOAT2& pos);
	~Button2D() override = default;
	Button2D(const Button2D& other) = delete;
	Button2D(Button2D&& other) noexcept = delete;
	Button2D& operator=(const Button2D& other) = delete;
	Button2D& operator=(Button2D&& other) noexcept = delete;

	const bool IsPressed() const { return m_IsPressed; }

	void SetEnabled(bool isEnabled) { m_IsEnabled = isEnabled; }
protected:
	void Draw(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	XMFLOAT2 m_Pos{};
	XMFLOAT2 m_Pivot{};
	std::string m_Text{};

	TextureData* m_pTextureSelected{};
	TextureData* m_pTextureNotSelected{};

	float m_Scale{ 0.5f };

	bool m_IsSelected{ false };
	bool m_IsPressed{ false };
	bool m_IsEnabled{ true };
};

