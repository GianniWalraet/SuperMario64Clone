#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: m_pMaterial{ pMaterial }
	, m_Length{ length }
{

}

void BoneObject::CalculateBindPose()
{
	const auto& float4x4World = GetTransform()->GetWorld();
	const auto matrixWorld = XMLoadFloat4x4(&float4x4World);
	auto determinant = XMMatrixDeterminant(matrixWorld);
	const auto invMatrix = XMMatrixInverse(&determinant, matrixWorld);

	XMStoreFloat4x4(&m_BindPose, invMatrix);
	const auto& children = GetChildren<BoneObject>();
	for (auto child : children) child->CalculateBindPose();
}

void BoneObject::Initialize(const SceneContext&)
{
	auto pEmty = AddChild(new GameObject());
	auto modelComp = pEmty->AddComponent<ModelComponent>(new ModelComponent(L"Meshes/Bone.ovm"));
	modelComp->SetMaterial(m_pMaterial);
	pEmty->GetTransform()->Rotate(0,-90, 0);
	pEmty->GetTransform()->Scale(m_Length);
}

void BoneObject::AddBone(BoneObject* pBone)
{
	auto bonePos = pBone->GetTransform()->GetPosition();
	pBone->GetTransform()->Translate(GetTransform()->GetPosition().x + m_Length, bonePos.y, bonePos.z);
	AddChild(pBone);
}
