///////////////////////////////////////////////////////////////////////////////
// FileName:	Effect.cpp
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Implementation of Effect
///////////////////////////////////////////////////////////////////////////////

#include "Effect.h"

void Effect::BuildEffect(IDirect3DDevice9* pDevice)
{
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(pDevice, "DirLightTex.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &m_pLightFX, &errors));
	if (errors)
	{
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
		return;
	}

	// Obtain handles.
	m_hLightTech		= m_pLightFX->GetTechniqueByName("DirLightTexTech");
	m_hWVPMatrix		= m_pLightFX->GetParameterByName(0, "gWVP");
	m_hWorldInvTrans	= m_pLightFX->GetParameterByName(0, "gWorldInvTrans");
	m_hCamera			= m_pLightFX->GetParameterByName(0, "gCameraPos");
	m_hWorld			= m_pLightFX->GetParameterByName(0, "gWorld");
	m_hMtrl				= m_pLightFX->GetParameterByName(0, "gMtrl");
	m_hLight			= m_pLightFX->GetParameterByName(0, "gLight");
	m_hTex				= m_pLightFX->GetParameterByName(0, "gTex");

	HR(D3DXCreateEffectFromFile(pDevice, "outline.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &m_pOutlineFX, &errors));
	if (errors)
	{
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
		return;
	}

	// Obtain handles.
	m_hOutlineTech		= m_pOutlineFX->GetTechniqueByName("OutLineTech");
	m_hWVMatrix			= m_pOutlineFX->GetParameterByName(0, "WorldViewMatrix");
	m_hProjMatrix		= m_pOutlineFX->GetParameterByName(0, "ProjMatrix");
}