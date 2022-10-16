#include "stdafx.h"
#include "MainGameHUD.h"
#include "Prefabs/MarioGame/Mario.h"
#include "Singletons/GameState.h"

MainGameHUD::MainGameHUD(Mario* attachedPlayer)
	: m_pMario{ attachedPlayer }
{
}

void MainGameHUD::Initialize(const SceneContext& sceneContext)
{
	auto starGO = AddChild(new GameObject);
	auto spriteComp = starGO->AddComponent<>(new SpriteComponent(L"Textures/HUD/Star.png"));
	spriteComp->SetPivot({ 1, 0 });
	starGO->GetTransform()->Scale(0.1f);
	starGO->GetTransform()->Translate(sceneContext.windowWidth, 0.f, 0.f);

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
	m_MarioHead2D = ContentManager::Load<TextureData>(L"Textures/HUD/MarioHead.png");

}
void MainGameHUD::Update(const SceneContext&)
{
}

void MainGameHUD::PostDraw(const SceneContext& sceneContext)
{
	if (!m_pMario) return;

	TextRenderer::Get()->DrawText(m_pFont, L"Press start or escape for controls", { 0.5f, sceneContext.windowHeight });

	std::string txt = std::to_string(m_pMario->GetScore()) + "/7";
	const float windowW = sceneContext.windowWidth;
	const float windowH = sceneContext.windowHeight;
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(txt), { windowW - (windowW / 12.f), windowH / 20.f }, XMFLOAT4{Colors::BlanchedAlmond});
	
	const float addedW = windowW / 25.f;
	for (size_t i = 0; i < m_pMario->GetLives(); ++i)
	{
		SpriteRenderer::Get()->AppendSprite(m_MarioHead2D, { addedW * i,  0.1f }, XMFLOAT4{ Colors::White }, { 0,0 }, {0.1f, 0.1f});
	}
}