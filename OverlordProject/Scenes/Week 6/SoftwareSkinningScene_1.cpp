#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	auto colorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	auto pRoot = AddChild(new GameObject);
	m_pBone0 = pRoot->AddChild(new BoneObject(colorMat, 15.f));
	m_pBone1 = new BoneObject(colorMat, 15.f);
	m_pBone0->AddBone(m_pBone1);
}

void SoftwareSkinningScene_1::Update()
{
	if (m_AutoRotate)
	{
		static float speed = 45.f;
		m_BoneRotation += (speed * m_SceneContext.pGameTime->GetElapsed()) * m_RotationSign;

		if (abs(m_BoneRotation) > speed) m_RotationSign = -m_RotationSign;

		m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
		m_pBone1->GetTransform()->Rotate(0, 0, -m_BoneRotation * 2.f);
	}
	else
	{
		m_pBone0->GetTransform()->Rotate(m_Bone0Rot.x, m_Bone0Rot.y, m_Bone0Rot.z, false);
		m_pBone1->GetTransform()->Rotate(m_Bone1Rot.x, m_Bone1Rot.y, m_Bone1Rot.z, false);
	}
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::SliderFloat3("Bone 0 - ROT", &m_Bone0Rot.x, 0.f, XM_2PI);
	ImGui::SliderFloat3("Bone 1 - ROT", &m_Bone1Rot.x, 0.f, XM_2PI);
	ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
}
