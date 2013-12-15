///////////////////////////////////////////////////////////////////////////////
// FileName:	GfxStats.cpp
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-27
// Purpose:		inplementation of statistics
///////////////////////////////////////////////////////////////////////////////

#include "Headers.h"
#include "GfxStats.h"
#include "CPUInfo.h"

#define PURPLE D3DCOLOR_XRGB(255, 0, 255)

GfxStats::GfxStats(IDirect3DDevice9* pDevice)
: m_pFont(0), m_fFPS(0.0f), m_fMilliSecPerFrame(0.0f)
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 15;
    fontDesc.Width           = 0;
    fontDesc.Weight          = FW_BOLD;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    _tcscpy_s(fontDesc.FaceName, 32, _T("Arial"));

	HR(D3DXCreateFontIndirect(pDevice, &fontDesc, &m_pFont));

	//get hardware info here
	IDirect3D9* pD3D;
	pDevice->GetDirect3D(&pD3D);
	
	D3DADAPTER_IDENTIFIER9 ident;
	pD3D->GetAdapterIdentifier(0, 0, &ident);
	m_sGPU = string(ident.Description);

	MEMORYSTATUS mem;
	GlobalMemoryStatus(&mem);
	m_uMem = mem.dwTotalPhys / 1024 / 1024;
	
	m_sCPU = GetCPUInfo();
}

GfxStats::~GfxStats()
{
	ReleaseCOM(m_pFont);
}

void GfxStats::OnLostDevice()
{
	HR(m_pFont->OnLostDevice());
}

void GfxStats::OnResetDevice()
{
	HR(m_pFont->OnResetDevice());
}

void GfxStats::Update(float dt)
{
	// Make static so that their values persist accross function calls.
	static float numFrames   = 0.0f;
	static float timeElapsed = 0.0f;

	// Increment the frame count.
	numFrames += 1.0f;

	// Accumulate how much time has passed.
	timeElapsed += dt;

	// Has one second passed?--we compute the frame statistics once 
	// per second.  Note that the time between frames can vary so 
	// these stats are averages over a second.
	if( timeElapsed >= 1.0f )
	{
		// Frames Per Second = numFrames / timeElapsed,
		// but timeElapsed approx. equals 1.0, so 
		// frames per second = numFrames.

		m_fFPS = numFrames;

		// Average time, in miliseconds, it took to render a single frame.
		m_fMilliSecPerFrame = 1000.0f / m_fFPS;

		// Reset time counter and frame count to prepare for computing
		// the average stats over the next second.
		timeElapsed = 0.0f;
		numFrames   = 0.0f;
	}
}

void GfxStats::Render()
{
	// Make static so memory is not allocated every frame.
	static char buffer[1024];

	sprintf_s(buffer, 1024, 
		"CPU = %s\n"
		"MEM = %d MB\n"
		"GPU = %s\n"
		"FPS = %.2f\n"
		"Milliseconds Per Frame = %.4f\n", 
		m_sCPU.c_str(), m_uMem, m_sGPU.c_str(), m_fFPS, m_fMilliSecPerFrame);

	RECT R = {5, 5, 0, 0};
	HR(m_pFont->DrawText(0, buffer, -1, &R, DT_NOCLIP, PURPLE));

	char* strHelp = 
		"W: zoom in\n"
		"S: zoom out\n"
		"A: rotate -Y-axis\n"
		"D: rotate +Y-axis\n"
		"F: full screen\n"
		"ESC: windowed\n"
		"Left Mouse to Choose";

	R.left = 600;
	HR(m_pFont->DrawText(0, strHelp, -1, &R, DT_NOCLIP, PURPLE));
}