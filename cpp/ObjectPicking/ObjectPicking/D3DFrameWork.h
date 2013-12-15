///////////////////////////////////////////////////////////////////////////////
// FileName:	D3DFrameWork.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-27
// Purpose:		An abstract base class for D3D frame work, including basical
//				initialization, cap checking and main msg loop (rendering)
//				User should derive from it with its own implementation
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "D3DUtility.h"
#include <string>

using namespace std;

class D3DFrameWork
{
public:
	void InitBase(HINSTANCE hInstance, string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	virtual ~D3DFrameWork();

	HINSTANCE					GetAppInst(void) { return m_hAppInst; }
	HWND						GetMainWnd(void) { return m_hMainWnd; }

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.
	virtual bool				CheckDeviceCaps()		{ return true; }
	virtual void				OnLostDevice()			{}
	virtual void				OnResetDevice()			{}
	virtual void				UpdateFrame(float dt)		{}
	virtual void				RenderFrame()				{}



	void						TouchFullScreenMode(bool enable);
	bool						IsDeviceLost();

	static void					SetD3DFrameWork(D3DFrameWork* p) { m_pD3DFrameWork = p; }				
	static D3DFrameWork*		GetD3DFrameWork(void) { return m_pD3DFrameWork; }
	static IDirect3DDevice9*	GetD3DDevice(void) { return m_pD3DDevice; }


	// Override these methods only if you do not like the default window creation,
	// direct3D device creation, window procedure, or message loop.  In general,
	// for the sample programs of this book, we will not need to modify these.
	virtual void				InitMainWindow();
	virtual void				InitDirect3D();
	virtual int					Run();
	virtual LRESULT				MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	D3DFrameWork() {}
protected:
	// Derived client class can modify these data members in the constructor to 
	// customize the application.  
	string						m_sMainWndCaption;
	D3DDEVTYPE					m_eDevType;
	DWORD						m_dRequestedVP;
	
	int							m_iWidth;	//window width
	int							m_iHeight;	//window height
	// Application, Windows, and Direct3D data members.
	HINSTANCE					m_hAppInst;
	HWND						m_hMainWnd;
	IDirect3D9*					m_pD3D;
	bool						m_bAppPaused;
	D3DPRESENT_PARAMETERS		m_D3DPrams;

	static IDirect3DDevice9*	m_pD3DDevice;
	static D3DFrameWork*		m_pD3DFrameWork;

	string						m_sGPU;
	string						m_sCPU;
	unsigned int				m_uMem;
};