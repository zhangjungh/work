///////////////////////////////////////////////////////////////////////////////
// FileName:	PrimObject.cpp
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Implementation of the PrimObject, only the init is different
///////////////////////////////////////////////////////////////////////////////


#include "PrimObject.h"
#include "Vertex.h"

void D3DPrimObject::LoadMesh(const string& filename)
{
	//create mesh according to filename
	if (filename == "teapot")
	{
		HR(D3DXCreateTeapot(m_pDevice, &m_pMesh, &m_pAdjBuffer));
	}
	else if (filename == "box")
	{
		HR(D3DXCreateBox(m_pDevice, 2.0f, 2.0f, 2.0f, &m_pMesh, &m_pAdjBuffer));
	}
	else if (filename == "terrain")
	{
		HR(D3DXCreateBox(m_pDevice, 14.0f, 0.1f, 10.0f, &m_pMesh, &m_pAdjBuffer));
		IDirect3DTexture9* tex;
		D3DXCreateTextureFromFile(m_pDevice, "floor_color.bmp", &tex);
		m_vTexs.push_back(tex);
	}
	else 
	{
		MessageBox(0, "Parameter NOT support", 0, 0);
		return;
	}

	//change format to VertexPNT
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPNT::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(m_pMesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, m_pDevice, &temp));
	ReleaseCOM(m_pMesh);

	// Now generate texture coordinates for each vertex.
	VertexPNT* vertices = 0;
	HR(temp->LockVertexBuffer(0, (void**)&vertices));

	for(UINT i = 0; i < temp->GetNumVertices(); ++i)
	{
		// Convert to spherical coordinates.
		D3DXVECTOR3 p = vertices[i].pos;
		// Save texture coordinates.		
		vertices[i].tex0.x = (p.x - 7)/14.0f;
		vertices[i].tex0.y = 1.0f - (p.z - 5)/10.0f;
	}
	HR(temp->UnlockVertexBuffer());

	// Clone back to a hardware mesh.
	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		elements, m_pDevice, &m_pMesh));

	ReleaseCOM(temp);

	//check format
	LPDIRECT3DINDEXBUFFER9 ib;
	m_pMesh->GetIndexBuffer (&ib);
	D3DINDEXBUFFER_DESC ibd;
	ib->GetDesc (&ibd);
	m_eFormat = ibd.Format; //used to calculate the sihouette edge

	//build AABB
	VertexPNT* v = 0;
	HR(m_pMesh->LockVertexBuffer(0, (void**)&v));
	HR(D3DXComputeBoundingBox((D3DXVECTOR3*)v, m_pMesh->GetNumVertices(), 
		sizeof(VertexPNT), &m_vMinAABBPt, &m_vMaxAABBPt));
	HR(m_pMesh->UnlockVertexBuffer());

	//build silhouette edge
	m_pEdgeObj = new SilhouetteEdges(m_pDevice, m_pMesh, m_pAdjBuffer);

	//assign default texture and material
	D3DMATERIAL9 mtrl;
	mtrl.Ambient	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mtrl.Diffuse	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	mtrl.Emissive	= D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	mtrl.Specular	= D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mtrl.Power		= 16.0f;

	m_vMats.push_back(mtrl);
}
