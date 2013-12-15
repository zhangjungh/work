#pragma once

//dx9 headers
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>

#include <tchar.h>

//stl headers
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

//for free resource
#define ReleaseCOM(x) { if(x){ x->Release();x=0; } }
#define DeleteObj(x) { if(x) { delete x; x=0; }}

struct DirLight
{
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	D3DXVECTOR3 dirW;
};

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if(FAILED(hr))                                 \
		{                                              \
			DXTrace(__FILE__, __LINE__, hr, #x, TRUE); \
		}                                              \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) x;
	#endif
#endif