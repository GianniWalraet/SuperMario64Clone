#include "stdafx.h"
#include "BobOmbRoomScene.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/Post/PostRipple.h"
#include "Prefabs/MarioGame/Mario.h"
#include "Singletons/GameState.h"
#include "Prefabs/MarioGame/PauseHUD.h"
#include "Prefabs/CubePrefab.h"

void BobOmbRoomScene::Initialize()
{
#ifdef NDEBUG
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.showInfoOverlay = false;
#endif
	LoadLevel();
	LoadPlayer();
	LoadPaintingTrigger();
	LoadSound();

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
	AddChild(new PauseHUD());

	m_pPaintingCamComponent = AddChild(new FixedCamera())->GetComponent<CameraComponent>();
	m_pPaintingCamComponent->GetTransform()->Translate(0.f, -1.5f, 5.f);

	m_pPostRipple = MaterialManager::Get()->CreateMaterial<PostRipple>();
	AddPostProcessingEffect(m_pPostRipple);
}

void BobOmbRoomScene::Update()
{
	if (m_PaintingActivated)
	{
		OnPaintingTriggered();
		m_PaintingActivated = false;
		m_StartLevel = true;
	}

	if (m_StartLevel)
	{
		m_ElapsedWaitTime += m_SceneContext.pGameTime->GetElapsed();
		if (m_ElapsedWaitTime > m_LoadLvlWaitTime)
		{
			SceneManager::Get()->SetActiveGameScene(L"BobOmbBattlefieldScene");
			m_StartLevel = false;
			m_ElapsedWaitTime = 0.f;
		}
	}
	else
	{
		TextRenderer::Get()->DrawText(m_pFont, L"Press start or escape for controls", { 0.5f, m_SceneContext.windowHeight });

		bool pauseButtonPressed = InputManager::IsKeyboardKey(InputState::pressed, VK_ESCAPE) ||
			InputManager::IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START);

		if (pauseButtonPressed)
		{
			bool paused = !GameState::Get().GetPaused();
			GameState::Get().SetPaused(paused);
		}

		m_pCastleTheme2D->setPaused(GameState::Get().GetPaused());
	}
}

void BobOmbRoomScene::OnSceneActivated()
{
	m_pCastleTheme2D->setPosition(0, FMOD_TIMEUNIT_MS);
	m_pCastleTheme2D->setPaused(false);

	m_pPaintingCamComponent->SetActive(false);
	m_pMario->GetComponent<CameraComponent>(true)->SetActive(true);
	m_pPostRipple->Reset();
	m_pMario->GetTransform()->Translate(0, 0, 0);
}
void BobOmbRoomScene::OnSceneDeactivated()
{
	m_pCastleTheme2D->setPaused(true);
}

void BobOmbRoomScene::OnPaintingTriggered()
{
	m_pPaintingCamComponent->SetActive(true);
	m_pPostRipple->SetEnabled(true, true);

	m_pCastleTheme2D->setPaused(true);

	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();
	pFmodSystem->playSound(m_pEnterPaintingSound2D, nullptr, false, nullptr);
	pFmodSystem->playSound(m_pGameStartSound2D, nullptr, false, nullptr);

	m_pMario->GetTransform()->Translate(0, 0, 0);
}
void BobOmbRoomScene::LoadPaintingTrigger()
{
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(0.f, 0.f, 0.f);

	m_pPaintingTrigger = AddChild(new GameObject());
	m_pPaintingTrigger->GetTransform()->Translate(0.f, -1.5f, 15.f);

	auto rigidBody = m_pPaintingTrigger->AddComponent(new RigidBodyComponent(true));
	rigidBody->AddCollider(PxBoxGeometry(4.f, 4.f, 0.2f), *pDefaultMaterial, true);
	m_pPaintingTrigger->SetOnTriggerCallBack([&](GameObject* /*pTrigger*/, GameObject* pOther, PxTriggerAction action)
		{
			if (dynamic_cast<Mario*>(pOther) && action == PxTriggerAction::ENTER)
			{
				m_PaintingActivated = true;
			}
		}
	);

}
void BobOmbRoomScene::LoadPlayer()
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

	m_pMario = AddChild(new Mario(characterDesc, 50.f));
	m_pMario->GetTransform()->Scale(0.15f);
	m_pMario->GetTransform()->Translate(0.f, -5.f, -5.f);

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
void BobOmbRoomScene::LoadLevel()
{
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);
	float lvlScale = 3.f;

	m_pLevel = AddChild(new GameObject());
	m_pLevel->GetTransform()->Scale(lvlScale);

	auto rigidBodyComponent = m_pLevel->AddComponent(new RigidBodyComponent(true));
	auto triangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/BobOmbRoom.ovpt");

	rigidBodyComponent->AddCollider(PxTriangleMeshGeometry(triangleMesh, PxMeshScale({ lvlScale,lvlScale,lvlScale }), PxMeshGeometryFlag::eDOUBLE_SIDED), *pDefaultMaterial);

	// Close door gap
	auto doorgo = AddChild(new CubePrefab(3.f, 8.f, 1.f, XMFLOAT4{Colors::Brown}));
	auto rigidBody = doorgo->AddComponent(new RigidBodyComponent(true));
	rigidBody->AddCollider(PxBoxGeometry(3.f / 2.f, 8.f / 2.f, 1.f / 2.f), *pDefaultMaterial, false);
	doorgo->GetTransform()->Translate(5.f, -8.3f, -15.6f);

	auto lvlModelCmp = m_pLevel->AddComponent<ModelComponent>(new ModelComponent(L"Meshes/BobOmbRoom.ovm"));

	auto mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/762BFC9A_c.png");
	lvlModelCmp->SetMaterial(mat, 0);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/38B6ACDB_c.png");
	lvlModelCmp->SetMaterial(mat, 1);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/480F337C_c.png");
	lvlModelCmp->SetMaterial(mat, 2);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/4D23D68_c.png");
	lvlModelCmp->SetMaterial(mat, 3);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/59FF6061_c.png");
	lvlModelCmp->SetMaterial(mat, 4);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/6B88A261_c.png");
	lvlModelCmp->SetMaterial(mat, 5);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/7A7501A5_c.png");
	lvlModelCmp->SetMaterial(mat, 6);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/3C96C6A_c.png");
	lvlModelCmp->SetMaterial(mat, 7);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	mat->SetInvertNormals(true);
	mat->SetDiffuseTexture(L"Textures/Level/3C96C6A_c.png");
	lvlModelCmp->SetMaterial(mat, 8);

}
void BobOmbRoomScene::LoadSound()
{
	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();

	
	pFmodSystem->createSound("Resources/Sounds/EnterPainting.wav", FMOD_DEFAULT | FMOD_2D, nullptr, &m_pEnterPaintingSound2D);
	m_pEnterPaintingSound2D->setMode(FMOD_LOOP_OFF);

	pFmodSystem->createStream("Resources/Sounds/GameStart.mp3", FMOD_DEFAULT | FMOD_2D, nullptr, &m_pGameStartSound2D);
	m_pGameStartSound2D->setMode(FMOD_LOOP_OFF);

	FMOD::Sound* pSound{};
	pFmodSystem->createStream("Resources/Sounds/CastleTheme.mp3", FMOD_DEFAULT | FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_NORMAL);
	pFmodSystem->playSound(pSound, nullptr, true, &m_pCastleTheme2D);
	m_pCastleTheme2D->setVolume(0.5f);
}
