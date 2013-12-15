///////////////////////////////////////////////////////////////////////////////
// FileName:	SilhouetteEdges.h
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		generate edge information, sending to shader later
///////////////////////////////////////////////////////////////////////////////

#include "Headers.h"

struct EdgeVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 faceNormal1;
	D3DXVECTOR3 faceNormal2;
};

struct MeshVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;
	static const DWORD FVF;
};

class SilhouetteEdges
{
public:
	SilhouetteEdges(
		IDirect3DDevice9* device,
		ID3DXMesh* mesh, 
		ID3DXBuffer* adjBuffer);

	~SilhouetteEdges();

	void Render();

private:
	IDirect3DDevice9*				m_pDevice;
	IDirect3DVertexBuffer9*			m_pVBuf;
	IDirect3DIndexBuffer9*			m_pIBuf;
	IDirect3DVertexDeclaration9*	m_pDecl;

	UINT							m_uNumVerts;
	UINT							m_uNumFaces;

private:
	bool CreateVertexDeclaration();

	void GetFaceNormal(
		ID3DXMesh* mesh,
		DWORD faceIndex,
		D3DXVECTOR3* faceNormal);

	void GetFaceNormals(
		ID3DXMesh* mesh,
		ID3DXBuffer* adjBuffer,
		D3DXVECTOR3* currentFaceNormal,
		D3DXVECTOR3 adjFaceNormals[3],
		DWORD       faceIndex);

	void GenEdgeVertices(ID3DXMesh* mesh, ID3DXBuffer* adjBuffer);
	void GenEdgeIndices(ID3DXMesh* mesh);	
};