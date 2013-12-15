//=============================================================================
// GfxDisp.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "D3DUtility.h"
#include "D3DFrameWork.h"
#include "GfxDisp.h"
#include <tchar.h>

ID3DXSprite*                g_pTextSprite = NULL;

GfxDisp::GfxDisp(IDirect3DDevice9* pD3DDevice, const string& cpu, const string& gpu, const unsigned int mem)
: mFont(0), mFPS(0.0f), mMilliSecPerFrame(0.0f), mNumTris(0), mNumVertices(0)
{
	m_sCPU = cpu;
	m_sGPU = gpu;
	m_uMem = mem;
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
    _tcscpy(fontDesc.FaceName, _T("Arial"));

	HR(D3DXCreateFontIndirect(pD3DDevice, &fontDesc, &mFont));
	
	D3DXCreateSprite( pD3DDevice, &g_pTextSprite );
}

GfxDisp::~GfxDisp()
{
	ReleaseCOM(mFont);
}

void GfxDisp::onLostDevice()
{
	HR(mFont->OnLostDevice());
}

void GfxDisp::onResetDevice()
{
	HR(mFont->OnResetDevice());
}

void GfxDisp::addVertices(DWORD n)
{
	mNumVertices += n;
}

void GfxDisp::subVertices(DWORD n)
{
	mNumVertices -= n;
}

void GfxDisp::addTriangles(DWORD n)
{
	mNumTris += n;
}

void GfxDisp::subTriangles(DWORD n)
{
	mNumTris -= n;
}

void GfxDisp::setTriCount(DWORD n)
{
	mNumTris = n;
}

void GfxDisp::setVertexCount(DWORD n)
{
	mNumVertices = n;
}

void GfxDisp::update(float dt)
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

void GfxDisp::display()
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
}