///////////////////////////////////////////////////////////////////////////////
// FileName:	CharObject.cpp
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Implementation of the CharObject
///////////////////////////////////////////////////////////////////////////////

#include "CharObject.h"
#include "Vertex.h"

void D3DCharObject::LoadMesh(const string& filename)
{
	ID3DXMesh* meshSys      = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls          = 0;

	//load it to system memory first for further optimization 
	HR(D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_SYSTEMMEM, m_pDevice,
		&m_pAdjBuffer,	&mtrlBuffer, 0, &numMtrls, &meshSys));


	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	HR(meshSys->GetDeclaration(elems));

	//check normal info
	bool hasNormals = false;
	D3DVERTEXELEMENT9 term = D3DDECL_END();
	for (int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (elems[i].Stream == 0xff)
			break;

		if (elems[i].Type == D3DDECLTYPE_FLOAT3 &&
			elems[i].Usage == D3DDECLUSAGE_NORMAL &&
			elems[i].UsageIndex == 0 )
		{
			hasNormals = true;
			break;
		}
	}


	//change vertex format to VertexPNT.
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPNT::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, elements, m_pDevice, &temp));
	ReleaseCOM(meshSys);
	meshSys = temp;


	//generate normal after mesh format changed if no normals
	if( hasNormals == false)
		HR(D3DXComputeNormals(meshSys, 0));


	//optimized, note the adjancy buffer need to updated
	HR(meshSys->Optimize(D3DXMESH_MANAGED | 
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, 
		(DWORD*)(m_pAdjBuffer)->GetBufferPointer(), (DWORD*)(m_pAdjBuffer)->GetBufferPointer(), 0, 0, &m_pMesh));
	ReleaseCOM(meshSys);

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

	//build sihouette edge
	m_pEdgeObj = new SilhouetteEdges(m_pDevice, m_pMesh, m_pAdjBuffer);

	//extract the materials and load the textures.
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* d3dxmtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (DWORD i=0; i<numMtrls; ++i)
		{
			m_vMats.push_back(d3dxmtrls[i].MatD3D);
			
			if (d3dxmtrls[i].pTextureFilename)
			{
				IDirect3DTexture9* tex = 0;
				char* texFN = d3dxmtrls[i].pTextureFilename;
				HR(D3DXCreateTextureFromFileEx(m_pDevice, texFN, D3DX_DEFAULT, D3DX_DEFAULT,
					D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
					D3DX_DEFAULT, D3DX_DEFAULT, 0,
					NULL, NULL, &tex));

				m_vTexs.push_back(tex);
			}
			else
			{
				m_vTexs.push_back(0);
			}
		}
	}
	ReleaseCOM(mtrlBuffer);
}
