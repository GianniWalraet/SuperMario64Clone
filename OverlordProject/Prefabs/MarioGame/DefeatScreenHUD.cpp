#include "stdafx.h"
#include "DefeatScreenHUD.h"
#include "Prefabs/Button2D.h"

void DefeatScreenHUD::Initialize(const SceneContext& sceneContext)
{
	m_pQuitButton = AddChild(new Button2D(L"Textures/HUD/QuitS.tga", L"Textures/HUD/QuitNS.tga", { sceneContext.windowWidth / 4.f * 3.f,sceneContext.windowHeight / 4.f * 3.f }));
	m_pMainMenuButton = AddChild(new Button2D(L"Textures/HUD/MainMenuS.tga", L"Textures/HUD/MainMenuNS.tga", { sceneContext.windowWidth / 4.f * 3.f,sceneContext.windowHeight / 4.f }));
}

void DefeatScreenHUD::Update(const SceneContext&)
{
	if (m_pQuitButton->IsPressed())
	{
		PostQuitMessage(WM_QUIT);
	}
	else if (m_pMainMenuButton->IsPressed())
	{
		SceneManager::Get()->SetActiveGameScene(L"MainMenuScene");
	}
}
