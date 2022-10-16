#include "stdafx.h"
#include "MainMenuScene.h"
#include "Prefabs/Button2D.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/ColorMaterial.h"
#include "Materials/Post/PostRipple.h"
#include "Materials/SkyboxMaterial.h"
#include "Materials/DiffuseMaterial.h"

void MainMenuScene::Initialize()
{
#ifdef NDEBUG
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.showInfoOverlay = false;
#endif
	// Camera reset to center
	auto cam = AddChild(new FixedCamera());
	cam->GetComponent<CameraComponent>()->SetActive(true);
	cam->GetTransform()->Translate(0, 0, -60);
	cam->GetTransform()->Rotate(0, 0, 0);

	// Buttons
	m_pStartButton = AddChild(new Button2D(L"Textures/HUD/StartS.tga", L"Textures/HUD/StartNS.tga",{m_SceneContext.windowWidth / 4.f,m_SceneContext.windowHeight / 4.f * 3.f}));
	m_pQuitButton = AddChild(new Button2D(L"Textures/HUD/QuitS.tga", L"Textures/HUD/QuitNS.tga", { m_SceneContext.windowWidth / 4.f * 3.f,m_SceneContext.windowHeight / 4.f * 3.f }));

	// Skybox
	m_pSkyBox = AddChild(new GameObject());
	auto skyboxMat = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	skyboxMat->SetDiffuseTexture(L"Textures/sky.dds");
	auto modelComp = m_pSkyBox->AddComponent(new ModelComponent(L"Meshes/Cube.ovm", false));
	modelComp->SetMaterial(skyboxMat);

	// Banner
	auto bannerGO = AddChild(new GameObject());
	bannerGO->AddComponent(new SpriteComponent(L"Textures/HUD/MainMenuBar.png", { 0,1 }));
	bannerGO->GetTransform()->Translate(0.f, m_SceneContext.windowHeight, 0.f);

	LoadHeadModel();
	LoadLevelModel();
	LoadLogo();
	LoadSound();
}

void MainMenuScene::Update()
{
	HandleAnimation();

	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		if (const auto pPickedObject = m_SceneContext.pCamera->Pick())
		{
			if (pPickedObject == m_pMarioHead)
			{
				m_IsDragged = true;
			}
		}
	}
	else if (InputManager::IsMouseButton(InputState::released, VK_LBUTTON))
	{
		m_IsDragged = false;
	}

	if (m_IsDragged)
	{
		auto mousePos = InputManager::GetMousePosition();
		m_pMarioHead->GetTransform()->Rotate(float(mousePos.y) - 90.f, float(mousePos.x) + 90.f, 0.f);
	}

	if (m_pStartButton->IsPressed())
	{
		SceneManager::Get()->SetActiveGameScene(L"BobOmbRoomScene");
	}
	else if (m_pQuitButton->IsPressed())
	{
		PostQuitMessage(WM_QUIT);
	}
}

void MainMenuScene::OnSceneActivated()
{
	m_pMainMenuSound2D->setPosition(0, FMOD_TIMEUNIT_MS);
	m_pMainMenuSound2D->setPaused(false);

	FMOD::Channel* channel;
	SoundManager::Get()->GetSystem()->playSound(m_pMarioSound2D, nullptr, true, &channel);
	channel->setPaused(false);
}
void MainMenuScene::OnSceneDeactivated()
{
	m_pMainMenuSound2D->setPaused(true);
}

void MainMenuScene::HandleAnimation()
{
	// Rotate mario head
	const auto& totalTime = m_SceneContext.pGameTime->GetTotal();
	float angleX = sin(totalTime) * 20.f;
	float angleY = sin(totalTime * 1.5f) * 30.f;
	float angleZ = cos(totalTime) * 30.f;
	m_pMarioHead->GetTransform()->Rotate(angleX, angleY, angleZ);

	// Rotate lvl
	angleY = sin(totalTime / 3.f) * 30.f;
	m_pCastle->GetTransform()->Rotate(10.f, 180.f + angleY, 0.f);
	m_pSkyBox->GetTransform()->Rotate(0.f, angleY, 0.f);

	// Rotate logo
	m_pTitle->GetTransform()->Rotate(-20.f, 0.f, sin(totalTime) * 10.f);
	m_pTitle->GetTransform()->Scale(std::lerp(1.f, 1.2f, sin(totalTime * 2.f)));

}

void MainMenuScene::LoadLevelModel()
{
	m_pCastle = AddChild(new GameObject());
	m_pCastle->GetTransform()->Scale(20.f);
	m_pCastle->GetTransform()->Translate(0, -10.f , 50.f);
	m_pCastle->GetTransform()->Rotate(20.f, 180.f, 0.f);

	auto modelComponent = m_pCastle->AddComponent(new ModelComponent(L"Meshes/PeachsCastle.ovm"));

	auto mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 259
	mat->SetDiffuseTexture(L"Textures/Castle/6DAF90F6_c.png");
	modelComponent->SetMaterial(mat, 0);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 258
	mat->SetDiffuseTexture(L"Textures/Castle/6DAF90F6_c.png");
	modelComponent->SetMaterial(mat, 1);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 254
	mat->SetDiffuseTexture(L"Textures/Castle/114626C6_c.png");
	modelComponent->SetMaterial(mat, 2);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 253
	mat->SetDiffuseTexture(L"Textures/Castle/114626C6_c.png");
	modelComponent->SetMaterial(mat, 3);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 249
	mat->SetDiffuseTexture(L"Textures/Castle/7EB1C6A1_c.png");
	modelComponent->SetMaterial(mat, 4);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 233
	mat->SetDiffuseTexture(L"Textures/Castle/3C6851DA_c.png");
	modelComponent->SetMaterial(mat, 5);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 232
	mat->SetDiffuseTexture(L"Textures/Castle/148EB7B9_c.png");
	modelComponent->SetMaterial(mat, 6);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 231
	mat->SetDiffuseTexture(L"Textures/Castle/1FEE462_c.png");
	modelComponent->SetMaterial(mat, 7);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 230
	mat->SetDiffuseTexture(L"Textures/Castle/5EDC83BD_c.png");
	modelComponent->SetMaterial(mat, 8);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 229
	mat->SetDiffuseTexture(L"Textures/Castle/19D5421D_c.png");
	modelComponent->SetMaterial(mat, 9);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 228
	mat->SetDiffuseTexture(L"Textures/Castle/31BEEA74_c.png");
	modelComponent->SetMaterial(mat, 10);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 227
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 11);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();  // 226
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 12);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 225
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 13);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 224
	mat->SetDiffuseTexture(L"Textures/Castle/31BEEA74_c.png");
	modelComponent->SetMaterial(mat, 14);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 223
	mat->SetDiffuseTexture(L"Textures/Castle/31BEEA74_c.png");
	modelComponent->SetMaterial(mat, 15);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 222
	mat->SetDiffuseTexture(L"Textures/Castle/31BEEA74_c.png");
	modelComponent->SetMaterial(mat, 16);

	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); // 221
	mat->SetDiffuseTexture(L"Textures/Castle/31BEEA74_c.png");
	modelComponent->SetMaterial(mat, 17);

	// 220
	// 219
	// 218
	// 217
	// 216
	// 215
	// 214
	// 213
	// 212
	// 211
	// 210
	
	// 209
	// 208
	// 207
	// 206
	// 205
	// 204

	// 194
	// 193
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 36);
	
	// 192
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/61BE951F_c.png");
	modelComponent->SetMaterial(mat, 37);
	 
	// 168
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 38);

	// 167
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 39);

	// 166
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 40);

	// 165
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 41);

	// 164
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1916F8C5_c.png");
	modelComponent->SetMaterial(mat, 42);

	// 163
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1916F8C5_c.png");
	modelComponent->SetMaterial(mat, 43);

	// 162
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/20EF7F7C_c.png");
	modelComponent->SetMaterial(mat, 44);

	// 161
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/20EF7F7C_c.png");
	modelComponent->SetMaterial(mat, 45);

	// 160
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1916F8C5_c.png");
	modelComponent->SetMaterial(mat, 46);
	
	// 159
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1916F8C5_c.png");
	modelComponent->SetMaterial(mat, 46);

	// 159
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1D0CA178_c.png");
	modelComponent->SetMaterial(mat, 47);

	// 158
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1D0CA178_c.png");
	modelComponent->SetMaterial(mat, 48);

	// 157
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1D0CA178_c.png");
	modelComponent->SetMaterial(mat, 49);

	// 156
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1D0CA178_c.png");
	modelComponent->SetMaterial(mat, 50);

	// 155
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1D0CA178_c.png");
	modelComponent->SetMaterial(mat, 51);

	// 154
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1D0CA178_c.png");
	modelComponent->SetMaterial(mat, 52);

	// 153
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1D0CA178_c.png");
	modelComponent->SetMaterial(mat, 53);

	// 152
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/41EDCCB_c.png");
	modelComponent->SetMaterial(mat, 54);

	// 151
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1FFBC780_c.png");
	modelComponent->SetMaterial(mat, 55);

	// 150
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1FFBC780_c.png");
	modelComponent->SetMaterial(mat, 56);
	

	// 149
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/6AE21407_c.png");
	modelComponent->SetMaterial(mat, 57);

	// 148
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/6AE21407_c.png");
	modelComponent->SetMaterial(mat, 58);

	// 147
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/6AE21407_c.png");
	modelComponent->SetMaterial(mat, 59);

	// 146
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/2BC808F1_c.png");
	modelComponent->SetMaterial(mat, 60);

	// 145
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/3FBAB8A0_c.png");
	modelComponent->SetMaterial(mat, 61);

	// 144
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/154B175A_c.png");
	modelComponent->SetMaterial(mat, 62);

	// 143
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/36B9F48_c.png");
	modelComponent->SetMaterial(mat, 63);

	// 142
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/36B9F48_c.png");
	modelComponent->SetMaterial(mat, 64);

	// 141
	
	// 140
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/20EF7F7C_c.png");
	modelComponent->SetMaterial(mat, 66);
	
	// 139
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/49254A53_c.png");
	modelComponent->SetMaterial(mat, 67);

	// 138
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1D0CA178_c.png");
	modelComponent->SetMaterial(mat, 68);

	// 137
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/1916F8C5_c.png");
	modelComponent->SetMaterial(mat, 69);

	// 136
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/2BC808F1_c.png");
	modelComponent->SetMaterial(mat, 70);

	// 135
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/19D5421D_c.png");
	modelComponent->SetMaterial(mat, 71);

	// 134
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/61BE951F_c.png");
	modelComponent->SetMaterial(mat, 72);

	// 133
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/3AB7B2BC_c.png");
	modelComponent->SetMaterial(mat, 73);

	// 132
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/31BEEA74_c.png");
	modelComponent->SetMaterial(mat, 74);

	// 131
	mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	mat->SetDiffuseTexture(L"Textures/Castle/60A64656_c.png");
	modelComponent->SetMaterial(mat, 75);
}
void MainMenuScene::LoadHeadModel()
{
	m_pMarioHead = AddChild(new GameObject());
	m_pMarioHead->GetTransform()->Scale(2.f);
	m_pMarioHead->GetTransform()->Translate(0, -5, -10.f);

	auto& physX = PxGetPhysics();
	auto pDefaultMat = physX.createMaterial(0.5f, 0.5f, 0.5f);

	auto rigidBody = m_pMarioHead->AddComponent(new RigidBodyComponent(true));
	rigidBody->AddCollider(PxSphereGeometry(5.f), *pDefaultMat, false);

	auto modelComponent = m_pMarioHead->AddComponent(new ModelComponent(L"Meshes/MarioHead.ovm"));

	auto matBLACK = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matBLACK->SetDiffuseTexture(L"Textures/Head/BLACK.png");

	auto matSKIN = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matSKIN->SetDiffuseTexture(L"Textures/Head/SKIN.png");

	auto matHAIR = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matHAIR->SetDiffuseTexture(L"Textures/Head/HAIR.png");

	auto matRED = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matRED->SetDiffuseTexture(L"Textures/Head/RED.png");

	auto matWHITE = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matWHITE->SetDiffuseTexture(L"Textures/Head/WHITE.png");

	auto matBLUE = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	matBLUE->SetDiffuseTexture(L"Textures/Head/BLUE.png");

	modelComponent->SetMaterial(matBLACK, 1);
	modelComponent->SetMaterial(matBLACK, 2);
	modelComponent->SetMaterial(matWHITE, 3);
	modelComponent->SetMaterial(matBLACK, 4);
	modelComponent->SetMaterial(matBLUE, 5);
	modelComponent->SetMaterial(matWHITE, 6);
	modelComponent->SetMaterial(matBLACK, 7);
	modelComponent->SetMaterial(matBLUE, 8);
	modelComponent->SetMaterial(matRED, 9);
	modelComponent->SetMaterial(matRED, 10);
	modelComponent->SetMaterial(matHAIR, 11);
	modelComponent->SetMaterial(matWHITE, 12);
	modelComponent->SetMaterial(matWHITE, 13);
	modelComponent->SetMaterial(matHAIR, 14);
	modelComponent->SetMaterial(matSKIN, 15);
	modelComponent->SetMaterial(matWHITE, 16);

}
void MainMenuScene::LoadLogo()
{
	m_pTitle = AddChild(new GameObject());
	m_pTitle->GetTransform()->Translate(0, 10, 5);

	auto modelComponent = m_pTitle->AddComponent(new ModelComponent(L"Meshes/Logo.ovm", false));

	auto mat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	mat->SetColor(XMFLOAT4{ Colors::LightBlue });
	modelComponent->SetMaterial(mat, 0);

	mat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	mat->SetColor(XMFLOAT4{ Colors::Red });
	modelComponent->SetMaterial(mat, 1);

	mat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	mat->SetColor(XMFLOAT4{ Colors::Blue });
	modelComponent->SetMaterial(mat, 2);

	mat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	mat->SetColor(XMFLOAT4{ Colors::Green });
	modelComponent->SetMaterial(mat, 3);

	mat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	mat->SetColor(XMFLOAT4{ Colors::Pink });
	modelComponent->SetMaterial(mat, 4);
}
void MainMenuScene::LoadSound()
{
	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();

	FMOD::Sound* pSound{};
	pFmodSystem->createStream("Resources/Sounds/TitleTheme.mp3", FMOD_DEFAULT, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_NORMAL);
	pFmodSystem->playSound(pSound, nullptr, true, &m_pMainMenuSound2D);

	pFmodSystem->createStream("Resources/Sounds/MarioPressStart.wav", FMOD_DEFAULT, nullptr, &m_pMarioSound2D);
	m_pMarioSound2D->setMode(FMOD_LOOP_OFF);

	m_pMainMenuSound2D->setVolume(0.5f);
}
