#include "stdafx.h"
#include "SpikyMaterial.h"

void SpikyMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4(1, 0, 0, 1));
}