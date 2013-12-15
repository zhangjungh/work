///////////////////////////////////////////////////////////////////////////////
// FileName:	Effect.h
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Effect that used in game, the basic per-pixel light effect
//				and outline effect for object been chosen
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Headers.h"

class Effect
{
public:
	Effect() : m_pLightFX(0), m_pOutlineFX(0) {}
	~Effect() { ReleaseCOM(m_pLightFX); ReleaseCOM(m_pOutlineFX); }
	
	void						BuildEffect(IDirect3DDevice9* pDevice);

	ID3DXEffect*				GetLightFx(void) { return m_pLightFX; }
	ID3DXEffect*				GetOutlineFx(void) { return m_pOutlineFX; }
	D3DXHANDLE					GetLightTech(void) { return m_hLightTech; }
	D3DXHANDLE					GetWVPMatrix(void) { return m_hWVPMatrix; }
	D3DXHANDLE					GetWorldInvTrans(void) { return m_hWorldInvTrans; }
	D3DXHANDLE					GetWorldMatrix(void) { return m_hWorld; }
	D3DXHANDLE					GetCameraPos(void) { return m_hCamera; }
	D3DXHANDLE					GetTexture(void) { return m_hTex; }
	D3DXHANDLE					GetMaterial(void) { return m_hMtrl; }
	D3DXHANDLE					GetLight(void) { return m_hLight; }
	D3DXHANDLE					GetOLTech(void) { return m_hOutlineTech; }
	D3DXHANDLE					GetOLWVMatrix(void) { return m_hWVMatrix; }
	D3DXHANDLE					GetOLPMatrix(void) { return m_hProjMatrix; }
private:
	ID3DXEffect*				m_pLightFX;			//main shader effect, per-pixel directional light
	D3DXHANDLE					m_hLightTech;
	D3DXHANDLE					m_hWVPMatrix;
	D3DXHANDLE					m_hWorldInvTrans;
	D3DXHANDLE					m_hCamera;
	D3DXHANDLE					m_hWorld;
	D3DXHANDLE					m_hTex;
	D3DXHANDLE					m_hMtrl;
	D3DXHANDLE					m_hLight;

	ID3DXEffect*				m_pOutlineFX;		//sihouette edge rendering effect
	D3DXHANDLE					m_hOutlineTech;
	D3DXHANDLE					m_hWVMatrix;
	D3DXHANDLE					m_hProjMatrix;
};