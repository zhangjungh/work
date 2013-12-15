//=============================================================================
// GfxStats.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "Headers.h"
#include "D3DApp.h"
#include "GfxStats.h"
#include <tchar.h>
#include "CPUInfo.h"

GfxStats::GfxStats(IDirect3DDevice9* pDevice)
: mFont(0), mFPS(0.0f), mMilliSecPerFrame(0.0f), mNumTris(0), mNumVertices(0)
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

	HR(D3DXCreateFontIndirect(pDevice, &fontDesc, &mFont));

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
	ReleaseCOM(mFont);
}

void GfxStats::OnLostDevice()
{
	HR(mFont->OnLostDevice());
}

void GfxStats::OnResetDevice()
{
	HR(mFont->OnResetDevice());
}

void GfxStats::addVertices(DWORD n)
{
	mNumVertices += n;
}

void GfxStats::subVertices(DWORD n)
{
	mNumVertices -= n;
}

void GfxStats::addTriangles(DWORD n)
{
	mNumTris += n;
}

void GfxStats::subTriangles(DWORD n)
{
	mNumTris -= n;
}

void GfxStats::setTriCount(DWORD n)
{
	mNumTris = n;
}

void GfxStats::setVertexCount(DWORD n)
{
	mNumVertices = n;
}

void GfxStats::update(float dt)
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

		mFPS = numFrames;

		// Average time, in miliseconds, it took to render a single frame.
		mMilliSecPerFrame = 1000.0f / mFPS;

		// Reset time counter and frame count to prepare for computing
		// the average stats over the next second.
		timeElapsed = 0.0f;
		numFrames   = 0.0f;
	}
}

void GfxStats::display()
{
	// Make static so memory is not allocated every frame.
	static char buffer[1024];

	sprintf_s(buffer, 1024, 
		"CPU = %s\n"
		"MEM = %d MB\n"
		"GPU = %s\n"
		"FPS = %.2f\n"
		"Milliseconds Per Frame = %.4f\n", 
		m_sCPU.c_str(), m_uMem, m_sGPU.c_str(), mFPS, mMilliSecPerFrame);

	RECT R = {5, 5, 0, 0};
	HR(mFont->DrawText(0, buffer, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(255,0,255)));

	
	sprintf_s(buffer, 1024, 
		"W: zoom in\n"
		"S: zoom out\n"
		"A: rotate -Y-axis\n"
		"D: rotate +Y-axis\n"
		"F: full screen\n"
		"Left Mouse to Choose");
	R.left = 600;
	HR(mFont->DrawText(0, buffer, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(255,0,255)));
}