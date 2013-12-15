///////////////////////////////////////////////////////////////////////////////
// FileName:	Vertex.h
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Vertex format that used in game, the PNT
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <d3dx9.h>

void InitAllVertexDeclarations(IDirect3DDevice9* pDevice);
void DestroyAllVertexDeclarations();

struct VertexPNT
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;

	static IDirect3DVertexDeclaration9* Decl;
};