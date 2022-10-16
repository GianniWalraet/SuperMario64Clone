#include "stdafx.h"
#include "BobOmbBattlefieldScene.h"
#include "Singletons/GameState.h"

#pragma region PrefabIncludes
#include "Prefabs/Camera/ThirdPersonCamera.h"
#include "Prefabs/Button2D.h"
#include "Prefabs/MarioGame/Mario.h"
#include "Prefabs/MarioGame/PowerStar.h"
#include "Prefabs/MarioGame/Goomba.h"
#include "Prefabs/MarioGame/MainGameHUD.h"
#include "Prefabs/MarioGame/PauseHUD.h"
#include "Prefabs/MarioGame/TiltablePlank.h"
#include "Prefabs/MarioGame/BobOmbBall.h"
#pragma endregion
#pragma region MaterialIncludes
#include "Materials/ColorMaterial.h"
#include "Materials/Post/PostScanline.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Post/PostRipple.h"
#include "Materials/SkyboxMaterial.h"
#pragma endregion


std::deque<XMFLOAT3> g_BallMoveTargets1
{
	{-7.8f, 39.4f, -48.6f},
	{-5.6f, 38.4f, -52.3f},
	{-22.3f, 37.2f, -61.6f},
	{-42.f, 32.9f, -50.2f },
	{-42.f, 31.5f, -27.4f},
	{-22.5f, 30.f, -16.f}
};
std::deque<XMFLOAT3> g_BallMoveTargets2
{
	{3.f, 27.9f, -47.5f},
	{5.4f, 26.8f, -51.6f},
	{-22.3f, 25.7f, -66.7f},
	{-48.8f, 21.4f, -54.3f},
	{-48.8f, 19.9f, -23.4f},
	{-21.4f, 17.3f, -6.9f}
};


void BobOmbBattlefieldScene::Initialize()
{
#ifdef NDEBUG
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.showInfoOverlay = false;
#endif
	GenerateLevelBounds();
	LoadMap();
	LoadPlayer();
	LoadEnemies();
	LoadPickups();
	LoadSounds();

	// Init hud later since it's attached to the player we created
	LoadHUD();
}

void BobOmbBattlefieldScene::Update()
{
	if (m_HasWon || m_HasLost)
	{
		HandleGameEnd();
		return;
	}

	HandleGamePause();
	UpdateBobOmbBalls();

	bool isPlaying;
	m_pMainThemeSound2D->isPlaying(&isPlaying);
	if (!isPlaying)
	{
		m_pMainThemeSound2D->setPosition(0, FMOD_TIMEUNIT_MS);
		m_pMainThemeSound2D->setPaused(false);
		m_pMainThemeSound2D->setVolume(1);
	}

	if (static_cast<uint8_t>(m_pMario->GetScore()) == m_NrOfStars)
	{
		m_pMainThemeSound2D->setPaused(true);
		SoundManager::Get()->GetSystem()->playSound(m_pLevelVictorySound2D, nullptr, false, nullptr);
		m_HasWon = true;
	}
	else if (m_pMario->GetLives() == 0)
	{
		m_pMainThemeSound2D->setPaused(true);
		SoundManager::Get()->GetSystem()->playSound(m_pLevelDefeatSound2D, nullptr, false, nullptr);
		m_HasLost = true;
	}
}

void BobOmbBattlefieldScene::OnSceneActivated()
{
	m_pMainThemeSound2D->setPosition(0, FMOD_TIMEUNIT_MS);
	m_pMainThemeSound2D->setPaused(false);
	m_ElapsedGameEndTime = 0.f;

	if (m_HasWon || m_HasLost)
	{
		m_HasWon = false;
		m_HasLost = false;

		m_pMario->SetScore(0);
		m_pMario->SetLives(3);
		m_pMario->GetTransform()->Translate(65.f, 10.f, 62.f);

		LoadPickups();
		LoadEnemies();
	}

	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();
	pFmodSystem->playSound(m_pMarioHereWeGoSound2D, nullptr, false, nullptr);
}
void BobOmbBattlefieldScene::OnSceneDeactivated()
{
	m_pMainThemeSound2D->setPaused(true);
	GameState::Get().SetPaused(false);
}

void BobOmbBattlefieldScene::UpdateBobOmbBalls()
{
	m_ElapsedBallSpawnDelay += m_SceneContext.pGameTime->GetElapsed();
	if (m_ElapsedBallSpawnDelay > m_BallSpawnDelay)
	{
		AddChild(new BobOmbBall(g_BallMoveTargets1, 5.f));
		AddChild(new BobOmbBall(g_BallMoveTargets2, 5.f));
		m_ElapsedBallSpawnDelay = 0.f;
	}
}

void BobOmbBattlefieldScene::HandleGameEnd()
{
	m_ElapsedGameEndTime += m_SceneContext.pGameTime->GetElapsed();

	if (m_HasWon)
	{
		if (m_ElapsedGameEndTime > m_WinWaitTime)
		{
			SceneManager::Get()->SetActiveGameScene(L"VictoryScene");
		}
		return;
	}

	if (m_HasLost)
	{
		if (m_ElapsedGameEndTime > m_DefeatWaitTime)
		{
			SceneManager::Get()->SetActiveGameScene(L"DefeatScene");
		}
		return;
	}
}
void BobOmbBattlefieldScene::HandleGamePause()
{
	bool pauseButtonPressed = InputManager::IsKeyboardKey(InputState::pressed, VK_ESCAPE) ||
		InputManager::IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START);

	if (pauseButtonPressed)
	{
		bool paused = !GameState::Get().GetPaused();
		GameState::Get().SetPaused(paused);
	}

	m_pMainThemeSound2D->setPaused(GameState::Get().GetPaused());
}

void BobOmbBattlefieldScene::GenerateLevelBounds()
{
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);

	// Z1
	auto pActor = PxGetPhysics().createRigidStatic(PxTransform{ PxVec3{-19.f, 17.f, 86.f}, PxQuat{PxPiDivTwo, PxVec3{0.f,1.f,0.f}} });
	PxRigidActorExt::createExclusiveShape(*pActor, PxPlaneGeometry{}, *pDefaultMaterial);
	GetPhysxProxy()->AddActor(*pActor);

	// Z2
	pActor = PxGetPhysics().createRigidStatic(PxTransform{ PxVec3{14.f, 30.1f, -72.f}, PxQuat{PxPiDivTwo, PxVec3{0.f,-1.f,0.f}} });
	PxRigidActorExt::createExclusiveShape(*pActor, PxPlaneGeometry{}, *pDefaultMaterial);
	GetPhysxProxy()->AddActor(*pActor);

	// X1
	pActor = PxGetPhysics().createRigidStatic(PxTransform{ PxVec3{85.6f, 14.f, -29.f}, PxQuat{PxPi, PxVec3{0.f,1.f,0.f}} });
	PxRigidActorExt::createExclusiveShape(*pActor, PxPlaneGeometry{}, *pDefaultMaterial);
	GetPhysxProxy()->AddActor(*pActor);

	// X2
	pActor = PxGetPhysics().createRigidStatic(PxTransform{ PxVec3{-71.1f, 19.3f, 15.1f}, PxQuat{PxPi, PxVec3{1.f,0.f,0.f}} });
	PxRigidActorExt::createExclusiveShape(*pActor, PxPlaneGeometry{}, *pDefaultMaterial);
	GetPhysxProxy()->AddActor(*pActor);
}

void BobOmbBattlefieldScene::LoadMap()
{
	// Plank
	m_pBridge = AddChild(new TiltablePlank());
	m_pBridge->GetTransform()->Translate(31.4f, 7.5f, 17.3f);

	// Skybox
	auto skyBox = AddChild(new GameObject());
	auto skyboxMat = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	skyboxMat->SetDiffuseTexture(L"Textures/sky.dds");
	auto modelComp = skyBox->AddComponent(new ModelComponent(L"Meshes/Cube.ovm", false));
	modelComp->SetMaterial(skyboxMat);

	// World
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);

	m_pLevel = AddChild(new GameObject());
	m_pLevel->GetTransform()->Scale(10);

	auto rigidBodyComponent = m_pLevel->AddComponent(new RigidBodyComponent(true));
	auto triangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/BobOmbBattlefield.ovpt");

	rigidBodyComponent->AddCollider(PxTriangleMeshGeometry(triangleMesh, PxMeshScale({ 10.f,10.f,10.f }), PxMeshGeometryFlag::eDOUBLE_SIDED), *pDefaultMaterial);

	auto lvlModelCmp = m_pLevel->AddComponent<ModelComponent>(new ModelComponent(L"Meshes/BobOmbBattlefield.ovm"));

	auto mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/574B138E_c.png");
	lvlModelCmp->SetMaterial(mat, 0);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/41A41EE3_c.png");
	lvlModelCmp->SetMaterial(mat, 1);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/6E3A21B_c.png");
	lvlModelCmp->SetMaterial(mat, 2);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/359289F2_c.png");
	lvlModelCmp->SetMaterial(mat, 3);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/6E3A21B_c.png");
	lvlModelCmp->SetMaterial(mat, 4);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/6B1A233B_c.png");
	lvlModelCmp->SetMaterial(mat, 5);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/6B2D96F_c.png");
	lvlModelCmp->SetMaterial(mat, 6);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/12436720_c.png");
	lvlModelCmp->SetMaterial(mat, 7);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/1FAAE88D_c.png");
	lvlModelCmp->SetMaterial(mat, 8);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/1FAAE88D_c.png");
	lvlModelCmp->SetMaterial(mat, 9);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/4020CDFE_c.png");
	lvlModelCmp->SetMaterial(mat, 10);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/4020CDFE_c.png");
	lvlModelCmp->SetMaterial(mat, 11);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/1B46C8C_c.png");
	lvlModelCmp->SetMaterial(mat, 12);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/6C631877_c.png");
	lvlModelCmp->SetMaterial(mat, 13);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/3D49A9D5_c.png");
	lvlModelCmp->SetMaterial(mat, 14);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/C1DF883_c.png");
	lvlModelCmp->SetMaterial(mat, 15);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/3F485258_c.png");
	lvlModelCmp->SetMaterial(mat, 16);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/10E99677_c.png");
	lvlModelCmp->SetMaterial(mat, 17);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/359289F2_c.png");
	lvlModelCmp->SetMaterial(mat, 18);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/6B2D96F_c.png");
	lvlModelCmp->SetMaterial(mat, 19);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Level/12436720_c.png");
	lvlModelCmp->SetMaterial(mat, 20);
}
void BobOmbBattlefieldScene::LoadSounds()
{
	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();

	FMOD::Sound* pSound{};
	pFmodSystem->createStream("Resources/Sounds/MainTheme.mp3", FMOD_DEFAULT | FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_NORMAL);
	pFmodSystem->playSound(pSound, nullptr, true, &m_pMainThemeSound2D);

	pFmodSystem->createSound("Resources/Sounds/MarioHereWeGo.wav", FMOD_DEFAULT | FMOD_2D, nullptr, &m_pMarioHereWeGoSound2D);
	m_pMarioHereWeGoSound2D->setMode(FMOD_LOOP_OFF);

	pFmodSystem->createSound("Resources/Sounds/LevelComplete.mp3", FMOD_DEFAULT | FMOD_2D, nullptr, &m_pLevelVictorySound2D);
	m_pLevelVictorySound2D->setMode(FMOD_LOOP_OFF);

	pFmodSystem->createSound("Resources/Sounds/GameOver.mp3", FMOD_DEFAULT | FMOD_2D, nullptr, &m_pLevelDefeatSound2D);
	m_pLevelDefeatSound2D->setMode(FMOD_LOOP_OFF);

	m_pMainThemeSound2D->setVolume(0.5f);
}
void BobOmbBattlefieldScene::LoadPlayer()
{
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);

	// Mario
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;
	characterDesc.rotationSpeed = 100.f;
	characterDesc.maxMoveSpeed = 8.f;
	characterDesc.controller.height = 1.f;
	characterDesc.controller.radius = 0.3f;

	m_pMario = AddChild(new Mario(characterDesc));
	m_pMario->GetTransform()->Scale(0.15f);
	m_pMario->GetTransform()->Translate(65.f, 10.f, 62.f);

	// Input
	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

}
void BobOmbBattlefieldScene::LoadEnemies()
{
	m_NrOfEnemies = 0;
	while (m_pGoombas.size() > 0)
	{
		if (!m_pGoombas.front()->IsDead())
		{
			m_pGoombas.front()->Kill();
		}
		m_pGoombas.pop_front();
	}

	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);
	CharacterDesc desc{ pDefaultMaterial };
	desc.controller.height = 0.1f;
	desc.controller.radius = 0.6f;


	std::ifstream file("Resources/Parser/GoombaPositions.txt");
	if (!file) return;

	float yOffset = 1.f;
	std::string line;
	while (getline(file, line))
	{
		if (line.empty()) continue;

		std::istringstream v(line);
		float x, y, z;
		v >> x >> y >> z;
		XMFLOAT3 pos = { x, y, z };

		auto goomba = AddChild(new Goomba(desc, 15.f));
		pos.y += yOffset;
		goomba->GetTransform()->Translate(pos);
		goomba->SetTarget(m_pMario);
		m_pGoombas.emplace_back(goomba);
		++m_NrOfEnemies;
	}

}
void BobOmbBattlefieldScene::LoadPickups()
{
	m_NrOfStars = 0;
	while (m_pPowerStars.size() > 0)
	{
		if (HasChild(m_pPowerStars.front()))
		{
			RemoveChild(m_pPowerStars.front(), true);
		}
		m_pPowerStars.pop_front();
	}

	float yOffset = 1.f;

	std::ifstream file("Resources/Parser/StarPositions.txt");
	if (!file) return;

	std::string line;
	while (getline(file, line))
	{
		if (line.empty()) continue;

		std::istringstream v(line);
		float x, y, z;
		v >> x >> y >> z;
		XMFLOAT3 pos = { x, y, z };

		auto star = AddChild(new PowerStar());
		pos.y += yOffset;
		star->GetTransform()->Translate(pos);
		m_pPowerStars.emplace_back(star);
		++m_NrOfStars;
	}
}
void BobOmbBattlefieldScene::LoadHUD()
{
	m_pMainGameHUD = AddChild(new MainGameHUD(m_pMario));
	m_pPauseHUD = AddChild(new PauseHUD());
}
