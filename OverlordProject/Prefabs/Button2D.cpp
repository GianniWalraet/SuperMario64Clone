#include "stdafx.h"
#include "Button2D.h"

Button2D::Button2D(const std::wstring& pathSelected, const std::wstring& pathNotSelected, const XMFLOAT2& pos)
{
	GetTransform()->Translate(pos.x, pos.y, 0.f);

	m_Pos = pos;

	m_pTextureNotSelected = ContentManager::Load<TextureData>(pathNotSelected);
	m_pTextureSelected = ContentManager::Load<TextureData>(pathSelected);
}

void Button2D::Draw(const SceneContext&)
{
	if (!m_IsEnabled) return;

	auto toRender = m_IsSelected ? m_pTextureSelected : m_pTextureNotSelected;
	SpriteRenderer::Get()->AppendSprite(toRender, m_Pos, XMFLOAT4{Colors::White}, { 0.5f,0.5f }, { m_Scale,m_Scale }, 0.f, 0.6f);
}

void Button2D::Update(const SceneContext&)
{
	if (!m_IsEnabled) return;

	auto buttonWH = m_pTextureSelected->GetDimension();
	buttonWH.x *= m_Scale;
	buttonWH.y *= m_Scale;
	const auto& buttonPos = GetTransform()->GetPosition();
	const auto& mousePos = InputManager::GetMousePosition();

	// check if mouse is in button area
	m_IsSelected = (mousePos.x > (buttonPos.x - buttonWH.x / 2.f) && mousePos.x < (buttonPos.x + buttonWH.x / 2.f) && mousePos.y >(buttonPos.y - buttonWH.y / 2.f) && mousePos.y < (buttonPos.y + buttonWH.y / 2.f));

	m_IsPressed = false;
	if (m_IsSelected)
	{
		m_IsPressed = InputManager::IsMouseButton(InputState::released, VK_LBUTTON);
	}
}