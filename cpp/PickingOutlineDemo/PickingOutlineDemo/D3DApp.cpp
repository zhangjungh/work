///////////////////////////////////////////////////////////////////////////////
// FileName:	D3DApp.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-27
// Purpose:		Implementation of the D3D framework
///////////////////////////////////////////////////////////////////////////////

#include "D3DApp.h"

//init static pointer object
D3DApp* D3DApp::m_pD3DApp = 0;
IDirect3DDevice9* D3DApp::m_pDevice = 0;

//callback function to define message handling
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created.
	if (D3DApp::GetD3DApp())
		return D3DApp::GetD3DApp()->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

void D3DApp::InitBase(HINSTANCE hInstance, string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
{
	m_sMainWndCaption	= winCaption;
	m_eDevType			= devType;
	m_dRequestedVP		= requestedVP;
	
	m_hAppInst			= hInstance;
	m_hMainWnd			= 0;
	m_pD3D				= 0;
	m_bAppPaused		= false;
	ZeroMemory(&m_D3DPrams, sizeof(m_D3DPrams));

	m_iWidth			= 800;
	m_iHeight			= 600;

	InitMainWindow();
	InitDirect3D();
}

D3DApp::~D3DApp()
{
	//release the resources
	ReleaseCOM(m_pD3D);
	ReleaseCOM(m_pDevice);
}

void D3DApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hAppInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	
	int width  = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	RECT rect = {0, 0, m_iWidth, m_iHeight};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	m_hMainWnd = CreateWindow(
		"D3DWndClassName",			//window class name
		m_sMainWndCaption.c_str(),	//window caption
		WS_OVERLAPPEDWINDOW,		//window style
		(width-m_iWidth)/2,			//up position
		(height-m_iHeight)/2,		//left position
		rect.right,					//width
		rect.bottom,				//height
		0,							//parent window
		0,							//menu
		m_hAppInst,					//instance
		0);							//other parameters

	if (!m_hMainWnd)
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);
}

void D3DApp::InitDirect3D()
{
	//init D3D9 object first
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D)
	{
		MessageBox(0, "Direct3DCreate9 FAILED", 0, 0);
		PostQuitMessage(0);
	}

	D3DDISPLAYMODE mode;
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	HR(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, m_eDevType, mode.Format, mode.Format, true));
	HR(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, m_eDevType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));

	//caps checking
	D3DCAPS9 caps;
	HR(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, m_eDevType, &caps));

	DWORD devBehaviorFlags = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		devBehaviorFlags |= m_dRequestedVP;
	}
	else
	{
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// If pure device and HW T&L supported
	if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&	devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;
	}

	m_D3DPrams.BackBufferWidth            = 0; 
	m_D3DPrams.BackBufferHeight           = 0;
	m_D3DPrams.BackBufferFormat           = D3DFMT_UNKNOWN;
	m_D3DPrams.BackBufferCount            = 1;
	m_D3DPrams.MultiSampleType            = D3DMULTISAMPLE_NONE;
	m_D3DPrams.MultiSampleQuality         = 0;
	m_D3DPrams.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	m_D3DPrams.hDeviceWindow              = m_hMainWnd;
	m_D3DPrams.Windowed                   = true;
	m_D3DPrams.EnableAutoDepthStencil     = true; 
	m_D3DPrams.AutoDepthStencilFormat     = D3DFMT_D24S8;
	m_D3DPrams.Flags                      = 0;
	m_D3DPrams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_D3DPrams.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;


	//create the device.
	HR(m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,		//primary adapter
		m_eDevType,				//device type
		m_hMainWnd,				//window associated with device
		devBehaviorFlags,		//vertex processing
	    &m_D3DPrams,            //present parameters
	    &m_pDevice));		//out created device
}

int D3DApp::Run()
{
	MSG  msg;
    msg.message = WM_NULL;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			// If the application is paused then free some CPU cycles to other 
			// applications and then continue on to the next frame.
			if (m_bAppPaused)
			{
				Sleep(20);
				continue;
			}

			if (!IsDeviceLost())
			{
				__int64 currTimeStamp = 0;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
				float dt = (currTimeStamp - prevTimeStamp)*secsPerCnt;

				Update(dt);
				Render();

				// Prepare for next iteration: The current time stamp becomes
				// the previous time stamp for the next iteration.
				prevTimeStamp = currTimeStamp;
			}
        }
    }
	return (int)msg.wParam;
}

LRESULT D3DApp::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Is the application in a minimized or maximized state?
	static bool minOrMaxed = false;

	RECT clientRect = {0, 0, 0, 0};
	switch (msg)
	{

	// WM_ACTIVE is sent when the window is activated or deactivated.
	// We pause the game when the main window is deactivated and 
	// unpause it when it becomes active.
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
			m_bAppPaused = true;
		else
			m_bAppPaused = false;
		return 0;


	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		if (m_pDevice)
		{
			m_D3DPrams.BackBufferWidth  = LOWORD(lParam);
			m_D3DPrams.BackBufferHeight = HIWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				m_bAppPaused = true;
				minOrMaxed = true;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_bAppPaused = false;
				minOrMaxed = true;
				OnLostDevice();
				HR(m_pDevice->Reset(&m_D3DPrams));
				OnResetDevice();
			}
			// Restored is any resize that is not a minimize or maximize.
			// For example, restoring the window to its default size
			// after a minimize or maximize, or from dragging the resize
			// bars.
			else if (wParam == SIZE_RESTORED)
			{
				m_bAppPaused = false;

				// Are we restoring from a mimimized or maximized state, 
				// and are in windowed mode?  Do not execute this code if 
				// we are restoring to full screen mode.
				if( minOrMaxed && m_D3DPrams.Windowed )
				{
					OnLostDevice();
					HR(m_pDevice->Reset(&m_D3DPrams));
					OnResetDevice();
				}
				else
				{
					// No, which implies the user is resizing by dragging
					// the resize bars.  However, we do not reset the device
					// here because as the user continuously drags the resize
					// bars, a stream of WM_SIZE messages is sent to the window,
					// and it would be pointless (and slow) to reset for each
					// WM_SIZE message received from dragging the resize bars.
					// So instead, we reset after the user is done resizing the
					// window and releases the resize bars, which sends a
					// WM_EXITSIZEMOVE message.
				}
				minOrMaxed = false;
			}
		}
		return 0;


	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		GetClientRect(m_hMainWnd, &clientRect);
		m_D3DPrams.BackBufferWidth  = clientRect.right;
		m_D3DPrams.BackBufferHeight = clientRect.bottom;
		OnLostDevice();
		HR(m_pDevice->Reset(&m_D3DPrams));
		OnResetDevice();

		return 0;

	// WM_CLOSE is sent when the user presses the 'X' button in the
	// caption bar menu.
	case WM_CLOSE:
		DestroyWindow(m_hMainWnd);
		return 0;

	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	//keyboard message
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			TouchFullScreenMode(false);
		else if (wParam == 'F')
			TouchFullScreenMode(true);
		return 0;
	}
	return DefWindowProc(m_hMainWnd, msg, wParam, lParam);
}

void D3DApp::TouchFullScreenMode(bool enable)
{
	int width  = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	// Switch to fullscreen mode.
	if (enable)
	{
		//are we already in fullscreen mode?
		if (!m_D3DPrams.Windowed) 
			return;

		m_D3DPrams.BackBufferFormat = D3DFMT_X8R8G8B8;
		m_D3DPrams.BackBufferWidth  = width;
		m_D3DPrams.BackBufferHeight = height;
		m_D3DPrams.Windowed         = false;

		// Change the window style to a more fullscreen friendly style.
		SetWindowLongPtr(m_hMainWnd, GWL_STYLE, WS_POPUP);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(m_hMainWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);	
	}
	// Switch to windowed mode.
	else
	{
		// Are we already in windowed mode?
		if (m_D3DPrams.Windowed) 
			return;

		RECT rect = {0, 0, m_iWidth, m_iHeight};
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		m_D3DPrams.BackBufferFormat = D3DFMT_UNKNOWN;
		m_D3DPrams.BackBufferWidth  = m_iWidth;
		m_D3DPrams.BackBufferHeight = m_iHeight;
		m_D3DPrams.Windowed         = true;
	
		// Change the window style to a more windowed friendly style.
		SetWindowLongPtr(m_hMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(m_hMainWnd, HWND_TOP, (width-m_iWidth)/2, (height-m_iHeight)/2, rect.right, rect.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	// Reset the device with the changes.
	OnLostDevice();
	HR(m_pDevice->Reset(&m_D3DPrams));
	OnResetDevice();
}

bool D3DApp::IsDeviceLost()
{
	// Get the state of the graphics device.
	HRESULT hr = m_pDevice->TestCooperativeLevel();

	// If the device is lost and cannot be reset yet then
	// sleep for a bit and we'll try again on the next 
	// message loop cycle.
	if (hr == D3DERR_DEVICELOST)
	{
		Sleep(20);
		return true;
	}
	// Driver error, exit.
	else if (hr == D3DERR_DRIVERINTERNALERROR)
	{
		MessageBox(0, "Internal Driver Error...Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	// The device is lost but we can reset and restore it.
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		OnLostDevice();
		HR(m_pDevice->Reset(&m_D3DPrams));
		OnResetDevice();
		return false;
	}
	else
		return false;
}