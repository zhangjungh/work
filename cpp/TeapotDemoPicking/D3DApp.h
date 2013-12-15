///////////////////////////////////////////////////////////////////////////////
// FileName:	D3DApp.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-27
// Purpose:		An abstract base class for D3D frame work, including basical
//				initialization, cap checking and main msg loop (rendering)
//				User should derive from it with its own implementation
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Headers.h"

class D3DApp
{
public:
	void InitBase(HINSTANCE hInstance, string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	virtual ~D3DApp();

	HINSTANCE					GetAppInst(void) { return m_hAppInst; }
	HWND						GetMainWnd(void) { return m_hMainWnd; }

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.
	virtual bool				CheckDeviceCaps()		{ return true; }
	virtual void				OnLostDevice()			{}
	virtual void				OnResetDevice()			{}
	virtual void				Update(float dt)		{}
	virtual void				Render()				{}


	void						TouchFullScreenMode(bool enable);
	bool						IsDeviceLost();

	static void					SetD3DApp(D3DApp* p) { m_pD3DApp = p; }				
	static D3DApp*				GetD3DApp(void) { return m_pD3DApp; }
	static IDirect3DDevice9*	GetD3DDevice(void) { return m_pDevice; }


	// Override these methods only if you do not like the default window creation,
	// direct3D device creation, window procedure, or message loop.  In general,
	// for the sample programs of this book, we will not need to modify these.
	virtual void				InitMainWindow();
	virtual void				InitDirect3D();
	virtual int					Run();
	virtual LRESULT				MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	D3DApp() {}
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

	static IDirect3DDevice9*	m_pDevice;
	static D3DApp*				m_pD3DApp;
};