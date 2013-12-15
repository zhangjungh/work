///////////////////////////////////////////////////////////////////////////////
// FileName:	GfxStats.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-27
// Purpose:		display HW, FPS info and show Help KEYs
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <d3dx9.h>
#include <string>

class GfxStats
{
public:
	GfxStats(IDirect3DDevice9* pDevice);
	~GfxStats();

	void OnLostDevice();
	void OnResetDevice();

	void Update(float dt);
	void Render();

private:
	// Prevent copying
	GfxStats(const GfxStats& rhs);
	GfxStats& operator=(const GfxStats& rhs);
	
private:
	ID3DXFont*			m_pFont;
	float				m_fFPS;
	float				m_fMilliSecPerFrame;

	string				m_sCPU;
	string				m_sGPU;
	unsigned int		m_uMem;
};