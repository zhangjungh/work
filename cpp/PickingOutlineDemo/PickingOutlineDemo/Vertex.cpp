///////////////////////////////////////////////////////////////////////////////
// FileName:	Vertex.cpp
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		implementation of the Vertex declaration
///////////////////////////////////////////////////////////////////////////////

#include "Vertex.h"
#include "headers.h"

IDirect3DVertexDeclaration9* VertexPNT::Decl = NULL;

void InitAllVertexDeclarations(IDirect3DDevice9* pDevice)
{
	D3DVERTEXELEMENT9 VertexPNTElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};	
	HR(pDevice->CreateVertexDeclaration(VertexPNTElements, &VertexPNT::Decl));
}

void DestroyAllVertexDeclarations()
{
	ReleaseCOM(VertexPNT::Decl);
}