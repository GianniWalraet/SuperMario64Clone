#include "stdafx.h"
#include "PongScene.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/CubePrefab.h"

void PongScene::Initialize()
{
	// Get physX
	auto& physX = PxGetPhysics();

	const auto pBouncyMaterial = physX.createMaterial(0.f, 0.f, 1.f);

	// Set Scene settings
	m_SceneContext.settings.clearColor = XMFLOAT4(Colors::Black);
	m_SceneContext.pCamera->GetTransform()->Translate(XMVECTOR{ 0,50,0 });

	// Ball
	m_pSphere = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Red }));
	auto pSphereActor = m_pSphere->AddComponent(new RigidBodyComponent(false));
	pSphereActor->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);

	m_pSphere->GetTransform()->Translate(0.f, 0.f, 0.f);
	m_pSphere->GetComponent<RigidBodyComponent>()->SetConstraint(RigidBodyConstraint::TransY, false);
	m_pSphere->GetComponent<RigidBodyComponent>()->SetConstraint(RigidBodyConstraint::RotX | RigidBodyConstraint::RotY | RigidBodyConstraint::RotZ, false);

	// paddle size
	XMFLOAT3 size{ 2.f,8.f,1.f };

	// Left paddle
	m_pLeftPaddle = AddChild(new CubePrefab(size, XMFLOAT4(Colors::White)));
	auto pCubeActor = m_pLeftPaddle->AddComponent(new RigidBodyComponent(false));
	pCubeActor->AddCollider(PxBoxGeometry{ size.x / 2.f,size.y / 2.f,size.z / 2.f }, *pBouncyMaterial);
	pCubeActor->SetKinematic(true);

	m_pLeftPaddle->GetTransform()->Translate(-30.f, 0.f, 0.f);
	m_pLeftPaddle->GetTransform()->Rotate(90.f, 0.f, 0.f);

	// Right paddle
	m_pRightPaddle = AddChild(new CubePrefab(size, XMFLOAT4(Colors::White)));
	pCubeActor = m_pRightPaddle->AddComponent(new RigidBodyComponent(false));
	pCubeActor->AddCollider(PxBoxGeometry{ size.x / 2.f,size.y / 2.f,size.z / 2.f }, *pBouncyMaterial);
	pCubeActor->SetKinematic(true);

	m_pRightPaddle->GetTransform()->Translate(30.f, 0.f, 0.f);
	m_pRightPaddle->GetTransform()->Rotate(90.f, 0.f, 0.f);

	// Top wall
	auto pWallActor = PxGetPhysics().createRigidStatic(PxTransform{ PxVec3{0.f,0.f,20.f} });
	PxRigidActorExt::createExclusiveShape(*pWallActor, PxBoxGeometry{ 35.f,1.f,1.f }, *pBouncyMaterial);

	GetPhysxProxy()->AddActor(*pWallActor);

	// Bottom wall
	pWallActor = PxGetPhysics().createRigidStatic(PxTransform{ PxVec3{0.f,0.f,-20.f} });
	PxRigidActorExt::createExclusiveShape(*pWallActor, PxBoxGeometry{ 35.f,1.f,1.f }, *pBouncyMaterial);

	GetPhysxProxy()->AddActor(*pWallActor);

	// Left trigger
	auto pLeftTrigger = AddChild(new GameObject());
	pCubeActor = pLeftTrigger->AddComponent(new RigidBodyComponent(true));
	auto colliderID = pCubeActor->AddCollider(PxBoxGeometry{ 1.f / 2.f, 1.f / 2.f, 40.f / 2.f }, *pBouncyMaterial);

	auto colliderInfo = pCubeActor->GetCollider(colliderID);
	colliderInfo.SetTrigger(true);

	pLeftTrigger->SetOnTriggerCallBack([&](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action) { OnTriggerCallBack(pTrigger, pOther, action); });
	pCubeActor->GetTransform()->Translate(-34.f, 0, 0);

	//right trigger
	auto pRightTrigger = AddChild(new GameObject());
	pCubeActor = pRightTrigger->AddComponent(new RigidBodyComponent(true));
	colliderID = pCubeActor->AddCollider(PxBoxGeometry{ 1.f / 2.f, 1.f / 2.f, 40.f / 2.f }, *pBouncyMaterial);

	colliderInfo = pCubeActor->GetCollider(colliderID);
	colliderInfo.SetTrigger(true);

	pRightTrigger->SetOnTriggerCallBack([&](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action) { OnTriggerCallBack(pTrigger, pOther, action); });
	pCubeActor->GetTransform()->Translate(34.f,0,0);
}
void PongScene::PostInitialize()
{
	m_pSphere->GetComponent<RigidBodyComponent>()->AddForce(m_Speed, PxForceMode::eIMPULSE);
}

void PongScene::Update()
{
	if (m_ResetScene)
	{
		Reset();
		m_pSphere->GetComponent<RigidBodyComponent>()->AddForce(m_Speed, PxForceMode::eIMPULSE);
		m_ResetScene = false;
	}

	const float paddleSpeed = 10.f;
	const auto spherePos = m_pSphere->GetTransform()->GetPosition();

	auto paddlePos = m_pLeftPaddle->GetTransform()->GetPosition();
	if (paddlePos.z < spherePos.z)
		paddlePos.z += paddleSpeed * m_SceneContext.pGameTime->GetElapsed();
	else if (paddlePos.z > spherePos.z)
		paddlePos.z -= paddleSpeed * m_SceneContext.pGameTime->GetElapsed();
	m_pLeftPaddle->GetTransform()->Translate(paddlePos);

	paddlePos = m_pRightPaddle->GetTransform()->GetPosition();
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_UP))
		paddlePos.z += paddleSpeed * m_SceneContext.pGameTime->GetElapsed();
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_DOWN))
		paddlePos.z -= paddleSpeed * m_SceneContext.pGameTime->GetElapsed();
	m_pRightPaddle->GetTransform()->Translate(paddlePos);
}
void PongScene::Draw()
{

}

void PongScene::Reset()
{
	m_pSphere->GetComponent<RigidBodyComponent>()->ClearForce();
	m_pSphere->GetTransform()->Translate(0, 0, 0);
}

void PongScene::OnTriggerCallBack(GameObject* /*pTrigger*/, GameObject* /*pOther*/, PxTriggerAction /*action*/)
{
	m_ResetScene = true;
}
