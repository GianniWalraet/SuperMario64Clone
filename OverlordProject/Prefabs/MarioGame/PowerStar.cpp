#include "stdafx.h"
#include "PowerStar.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/MarioGame/Mario.h"
#include "Singletons/GameState.h"

void PowerStar::Initialize(const SceneContext&)
{
	auto& physX = PxGetPhysics();
	const auto pDefaultMaterial = physX.createMaterial(.5f, .5f, .5f);

	// Model
	auto bodyMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	auto eyesMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	bodyMat->SetDiffuseTexture(L"Textures/Star/body.png");
	eyesMat->SetDiffuseTexture(L"Textures/Star/eye.png");
	auto modelComp = AddComponent(new ModelComponent(L"Meshes/Star.ovm"));
	modelComp->SetMaterial(eyesMat, 0);
	modelComp->SetMaterial(bodyMat, 1);

	// Trigger
	auto rigidBody = AddComponent(new RigidBodyComponent(true));
	auto convexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Star.ovpc");
	rigidBody->AddCollider(PxConvexMeshGeometry(convexMesh, PxMeshScale({ 0.5f,0.5f,0.5f })), *pDefaultMaterial, true);
	SetOnTriggerCallBack([&](GameObject* /*pTrigger*/, GameObject* pOther, PxTriggerAction action)
		{
			if (dynamic_cast<Mario*>(pOther) && action == PxTriggerAction::ENTER)
			{
				if (m_MarkForDelete) return;
				m_MarkForDelete = true;
				dynamic_cast<Mario*>(pOther)->IncrementScore();
				m_pStarsEmitter->Enable();
			}
		}
	);

	// Particles
	auto emitterGO = AddChild(new GameObject());
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,-1.f,0.f };
	settings.minSize = 1.f;
	settings.maxSize = 2.f;
	settings.minEnergy = 1.f;
	settings.maxEnergy = 2.f;
	settings.minScale = 0.5f;
	settings.maxScale = 1.f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .5f;
	settings.color = { 1.f,1.f,1.f, .6f };
	m_pStarsEmitter = emitterGO->AddComponent(new ParticleEmitterComponent(L"Textures/HUD/Star.png", settings));
	m_pStarsEmitter->Disable();

	GetTransform()->Scale(0.5f);

	// Sound
	auto soundManager = SoundManager::Get();
	auto pFmodSystem = soundManager->GetSystem();

	FMOD::Sound* pSound{};
	auto fModResult = pFmodSystem->createSound("Resources/Sounds/Pickup.wav", FMOD_DEFAULT | FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_OFF);
	fModResult = pFmodSystem->playSound(pSound, nullptr, true, &m_pPickUpSound2D);

	fModResult = pFmodSystem->createSound("Resources/Sounds/MarioHaha.wav", FMOD_DEFAULT | FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_OFF);
	fModResult = pFmodSystem->playSound(pSound, nullptr, true, &m_pMarioHaha2D);
}
void PowerStar::Update(const SceneContext& sceneContext)
{
	if (GameState::Get().GetPaused()) return;

	GetTransform()->Rotate(0.f, m_RotationSpeed * sceneContext.pGameTime->GetTotal(), 0.f, false);

	if (m_MarkForDelete)
	{
		PlayPickupAnimation(sceneContext.pGameTime->GetElapsed());

		m_pPickUpSound2D->setPaused(false);
		m_pMarioHaha2D->setPaused(false);

		bool isPlaying;
		m_pPickUpSound2D->isPlaying(&isPlaying);
		if(!isPlaying)
		{
			auto scene = GetScene();
			scene->RemoveChild(this, true);
		}
	}
}

void PowerStar::PlayPickupAnimation(float elapsedTime)
{
	m_RotationSpeed = 20.f;
	const auto& pos = GetTransform()->GetPosition();
	GetTransform()->Translate(pos.x, pos.y + m_FlySpeed * elapsedTime, pos.z);
}
