#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		passedTicks = fmod(passedTicks, m_CurrentClip.duration);
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		
		//2. 
		//IF m_Reversed is true
		if (m_Reversed)
		{
			//	Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;
			//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if (m_TickCount < 0) m_TickCount += m_CurrentClip.duration;
		}
		else //ELSE
		{
			//	Add passedTicks to m_TickCount
			m_TickCount += passedTicks;
			//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if (m_TickCount > m_CurrentClip.duration) m_TickCount -= m_CurrentClip.duration;
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA{ m_CurrentClip.keys.front() }, keyB{ m_CurrentClip.keys.back() };
		//Iterate all the keys of the clip and find the following keys:
		
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		for (size_t i = 0; i < m_CurrentClip.keys.size(); i++)
		{
			if (m_CurrentClip.keys[i].tick < m_TickCount && m_CurrentClip.keys[i].tick >= keyA.tick)
			{
				keyA = m_CurrentClip.keys[i];
			}
		}
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		for (size_t i = 0; i < m_CurrentClip.keys.size(); i++)
		{
			if (m_CurrentClip.keys[i].tick > m_TickCount && m_CurrentClip.keys[i].tick <= keyB.tick)
			{
				keyB = m_CurrentClip.keys[i];
			}
		}
		

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		auto blendFactor = (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick);
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (size_t i = 0; i < m_pMeshFilter->m_BoneCount; i++)
		{
			//	Retrieve the transform from keyA (transformA)
			auto transformA = keyA.boneTransforms[i];
			// 	Retrieve the transform from keyB (transformB)
			auto transformB = keyB.boneTransforms[i];
			//	Decompose both transforms
			XMVECTOR pos1{}, pos2{};
			XMVECTOR rot1{}, rot2{};
			XMVECTOR scale1{}, scale2{};
			XMMatrixDecompose(&scale1, &rot1, &pos1, XMLoadFloat4x4(&transformA));
			XMMatrixDecompose(&scale2, &rot2, &pos2, XMLoadFloat4x4(&transformB));
			//	Lerp between all the transformations (Position, Scale, Rotation)
			auto lerpedPos = XMVectorLerp(pos1, pos2, blendFactor);
			auto lerpedRot = XMQuaternionSlerp(rot1, rot2, blendFactor);
			auto lerpedScale = XMVectorLerp(scale1, scale2, blendFactor);
			//	Compose a transformation matrix with the lerp-results
			XMFLOAT4X4 newTransform = {};
			XMStoreFloat4x4(&newTransform, XMMatrixAffineTransformation(lerpedScale, g_XMZero, lerpedRot, lerpedPos));
			//	Add the resulting matrix to the m_Transforms vector
			m_Transforms.push_back(newTransform);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for (size_t i = 0; i < m_pMeshFilter->m_AnimationClips.size(); i++)
	{
		//If found,
		if (m_pMeshFilter->m_AnimationClips[i].name == clipName)
		{
			//	Call SetAnimation(Animation Clip) with the found clip
			SetAnimation(m_pMeshFilter->m_AnimationClips[i]);
			return;
		}
	}
	
	Reset();
	Logger::LogWarning(L"Animation not set -> not found in clips");
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{
		SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
		return;
	}

	Reset();
	Logger::LogWarning(L"Invalid clipnumber");
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;

	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause) m_IsPlaying = false;

	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.f;

	//If m_ClipSet is true
	if (m_ClipSet)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		auto BoneTransform = m_CurrentClip.keys[0].boneTransforms;
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(BoneTransform.begin(), BoneTransform.end());
	}
	else //Else
	{
		//	Create an IdentityMatrix
		const auto identityMat = XMMatrixIdentity();
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		XMFLOAT4X4 v{};
		XMStoreFloat4x4(&v, identityMat);
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, v);
	}
}
