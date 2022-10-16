#include "stdafx.h"
#include "PauseHUD.h"
#include "Prefabs/Button2D.h"
#include "Singletons/GameState.h"

void PauseHUD::Initialize(const SceneContext& sceneContext)
{
	m_pResumeButton = AddChild(new Button2D(L"Textures/HUD/ResumeS.tga", L"Textures/HUD/ResumeNS.tga",{ sceneContext.windowWidth / 2.f,sceneContext.windowHeight / 4.f }));
	m_pMainMenuButton = AddChild(new Button2D(L"Textures/HUD/MainMenuS.tga", L"Textures/HUD/MainMenuNS.tga",{ sceneContext.windowWidth / 2.f ,sceneContext.windowHeight / 4.f * 3.f }));
	
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
	m_pControlSchemeTex = ContentManager::Load<TextureData>(L"Textures/HUD/ControlScheme.tga");
}

void PauseHUD::Update(const SceneContext&)
{
	bool gamePaused = GameState::Get().GetPaused();
	m_pResumeButton->SetEnabled(gamePaused);
	m_pMainMenuButton->SetEnabled(gamePaused);

	if (m_pResumeButton->IsPressed())
	{
		GameState::Get().SetPaused(false);
	}
	else if (m_pMainMenuButton->IsPressed())
	{
		SceneManager::Get()->SetActiveGameScene(L"MainMenuScene");
		GameState::Get().SetPaused(false);
	}
}

void PauseHUD::Draw(const SceneContext& sceneContext)
{
	if (GameState::Get().GetPaused())
	{
		SpriteRenderer::Get()->AppendSprite(m_pControlSchemeTex, { 0.f, sceneContext.windowHeight / 2.f }, XMFLOAT4{ Colors::White }, { 0.f, 0.5f }, { 1.2f, 1.2f });
	}
}
