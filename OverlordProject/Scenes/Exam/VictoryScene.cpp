#include "stdafx.h"
#include "VictoryScene.h"
#include "Materials/SkyboxMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Prefabs/MarioGame/VictoryScreenHUD.h"

void VictoryScene::Initialize()
{
#ifdef NDEBUG
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.showInfoOverlay = false;
#endif

	auto cam = AddChild(new FixedCamera());
	cam->GetComponent<CameraComponent>()->SetActive(true);
	cam->GetTransform()->Translate(-26.5f, -6.2f, -56.6f);
	cam->GetTransform()->Rotate(-10.f, 10.f, 0);

	AddChild(new VictoryScreenHUD());

	LoadPlayer();
	LoadVictoryStar();
	LoadLevel();
	LoadSound();
}

void VictoryScene::Update()
{
	m_pVictoryStar->GetTransform()->Rotate(0.f, m_SceneContext.pGameTime->GetTotal() * 2.f, 0.f, false);
}
void VictoryScene::OnSceneActivated()
{
	m_pMainMenuSound2D->setPosition(0, FMOD_TIMEUNIT_MS);
	m_pMainMenuSound2D->setPaused(false);
}
void VictoryScene::OnSceneDeactivated()
{
	m_pMainMenuSound2D->setPaused(true);
}

void VictoryScene::LoadPlayer()
{
	auto marioMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	auto eyesMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	marioMat->SetDiffuseTexture(L"Textures/mariotex.png");
	eyesMat->SetDiffuseTexture(L"Textures/eyetex.png");
	m_pMario = AddChild(new GameObject);
	auto modelComp = m_pMario->AddComponent(new ModelComponent(L"Meshes/Mario.ovm"));
	modelComp->SetMaterial(marioMat);
	modelComp->SetMaterial(eyesMat, 5);
	m_pMario->GetTransform()->Translate(-28.f, -8.f, -40.f);
	m_pMario->GetTransform()->Rotate(0.f, 0.f, 0.f);
	m_pMario->GetTransform()->Scale(0.6f);

	modelComp->GetAnimator()->SetAnimation(L"Dance");
	modelComp->GetAnimator()->Play();

}
void VictoryScene::LoadVictoryStar()
{
	// Model
	m_pVictoryStar = AddChild(new GameObject());
	auto bodyMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	auto eyesMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	bodyMat->SetDiffuseTexture(L"Textures/Star/body.png");
	eyesMat->SetDiffuseTexture(L"Textures/Star/eye.png");
	auto modelComp = m_pVictoryStar->AddComponent(new ModelComponent(L"Meshes/Star.ovm"));
	modelComp->SetMaterial(eyesMat, 0);
	modelComp->SetMaterial(bodyMat, 1);

	m_pVictoryStar->GetTransform()->Translate(-28.f, 1.f, -40.f);
	m_pVictoryStar->GetTransform()->Scale(1.5f);
}
void VictoryScene::LoadLevel()
{
	// Skybox
	auto skyBox = AddChild(new GameObject());
	auto skyboxMat = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	skyboxMat->SetDiffuseTexture(L"Textures/sky.dds");
	auto modelComp = skyBox->AddComponent(new ModelComponent(L"Meshes/Cube.ovm"));
	modelComp->SetMaterial(skyboxMat);

	m_pCastle = AddChild(new GameObject());
	m_pCastle->GetTransform()->Scale(20.f);
	m_pCastle->GetTransform()->Translate(0, -15.f, 10.f);
	m_pCastle->GetTransform()->Rotate(0.f, 180.f, 0.f);

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
void VictoryScene::LoadSound()
{
	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();

	FMOD::Sound* pSound{};
	pFmodSystem->createStream("Resources/Sounds/WinDance.mp3", FMOD_DEFAULT | FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_NORMAL);
	pFmodSystem->playSound(pSound, nullptr, true, &m_pMainMenuSound2D);

	m_pMainMenuSound2D->setVolume(0.5f);
}
