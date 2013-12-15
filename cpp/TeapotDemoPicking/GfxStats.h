//=============================================================================
// GfxStats.h by Frank Luna (C) 2005 All Rights Reserved.
//
// Class used for keeping track of and displaying the frames rendered
// per second, milliseconds per frame, and vertex and triangle counts.
//=============================================================================

#ifndef GFX_STATS_H
#define GFX_STATS_H

#include <d3dx9.h>
#include <string>

using namespace std;

class GfxStats
{
public:
	GfxStats(IDirect3DDevice9* pDevice);
	~GfxStats();

	void OnLostDevice();
	void OnResetDevice();

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
	GfxStats(const GfxStats& rhs);
	GfxStats& operator=(const GfxStats& rhs);
	
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