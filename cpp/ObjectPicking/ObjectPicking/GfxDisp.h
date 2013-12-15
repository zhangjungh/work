//=============================================================================
// GfxDisp.h by Frank Luna (C) 2005 All Rights Reserved.
//
// Class used for keeping track of and displaying the frames rendered
// per second, milliseconds per frame, and vertex and triangle counts.
//=============================================================================

#ifndef GFX_STATS_H
#define GFX_STATS_H

#include <d3dx9.h>
#include <string>

using namespace std;

class GfxDisp
{
public:
	GfxDisp(IDirect3DDevice9* pD3DDevice, const string& cpu, const string& gpu, const unsigned int mem);
	~GfxDisp();

	void onLostDevice();
	void onResetDevice();

	void addVertices(DWORD n);
	void subVertices(DWORD n);
	void addTriangles(DWORD n);
	void subTriangles(DWORD n);

	void setTriCount(DWORD n);
	void setVertexCount(DWORD n);

	void update(float dt);
	void display();

private:
	// Prevent copying
	GfxDisp(const GfxDisp& rhs);
	GfxDisp& operator=(const GfxDisp& rhs);
	
private:
	ID3DXFont* mFont;
	float mFPS;
	float mMilliSecPerFrame;
	DWORD mNumTris;
	DWORD mNumVertices;

	string m_sCPU;
	string m_sGPU;
	unsigned int m_uMem;
};
#endif // GFX_STATS_H