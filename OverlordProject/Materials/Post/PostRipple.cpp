#include "stdafx.h"
#include "PostRipple.h"

PostRipple::PostRipple()
	: PostProcessingMaterial(L"Effects/Post/Ripple.fx")
{
}

void PostRipple::SetEnabled(bool isEnabled, bool fadeToWhite)
{
	m_pBaseEffect->GetVariableByName("gEnabled")->AsScalar()->SetBool(isEnabled);
	m_pBaseEffect->GetVariableByName("gFadeOut")->AsScalar()->SetBool(fadeToWhite);
	m_IsEnabled = true;
	m_TimeSinceEnabled = 0.f;
}

void PostRipple::Reset()
{
	m_TimeSinceEnabled = 0.f;
	m_IsEnabled = false;

	m_pBaseEffect->GetVariableByName("gEnabled")->AsScalar()->SetBool(false);
	m_pBaseEffect->GetVariableByName("gFadeOut")->AsScalar()->SetBool(false);
	m_pBaseEffect->GetVariableByName("gTimeSinceEnabled")->AsScalar()->SetFloat(m_TimeSinceEnabled);
}

void PostRipple::Initialize(const GameContext&)
{
	
}

void PostRipple::Draw(const SceneContext& sceneContext, RenderTarget* pSource)
{
	PostProcessingMaterial::Draw(sceneContext, pSource);
}

void PostRipple::UpdateBaseEffectVariables(const SceneContext& sceneContext, RenderTarget* pSource)
{
	PostProcessingMaterial::UpdateBaseEffectVariables(sceneContext, pSource);
	m_pBaseEffect->GetVariableByName("gTotalTime")->AsScalar()->SetFloat(sceneContext.pGameTime->GetTotal());

	if (m_IsEnabled)
	{
		m_TimeSinceEnabled += sceneContext.pGameTime->GetElapsed();
		m_pBaseEffect->GetVariableByName("gTimeSinceEnabled")->AsScalar()->SetFloat(m_TimeSinceEnabled);
	}
}