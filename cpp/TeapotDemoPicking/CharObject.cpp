#include "CharObject.h"
#include "Vertex.h"

void D3DCharObject::LoadMesh(const string& filename)
{
	// Step 1: Load the .x file from file into a system memory mesh.
	ID3DXMesh* meshSys      = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls          = 0;

	//load it to system memory first for further optimization 
	HR(D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_SYSTEMMEM, m_pDevice,
		&m_pAdjBuffer,	&mtrlBuffer, 0, &numMtrls, &meshSys));

	// Step 2: Find out if the mesh already has normal info?

	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	HR(meshSys->GetDeclaration(elems));

	bool hasNormals = false;
	D3DVERTEXELEMENT9 term = D3DDECL_END();
	for (int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		// Did we reach D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0}?
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


	// Step 3: Change vertex format to VertexPNT.

	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPNT::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, elements, m_pDevice, &temp));
	ReleaseCOM(meshSys);
	meshSys = temp;


	// Step 4: If the mesh did not have normals, generate them.

	if( hasNormals == false)
		HR(D3DXComputeNormals(meshSys, 0));


	// Step 5: Optimize the mesh.

	HR(meshSys->Optimize(D3DXMESH_MANAGED | 
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, 
		(DWORD*)(m_pAdjBuffer)->GetBufferPointer(), (DWORD*)(m_pAdjBuffer)->GetBufferPointer(), 0, 0, &m_pMesh));
	ReleaseCOM(meshSys); // Done w/ system mesh.

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

	// Step 6: Extract the materials and load the textures.
	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* d3dxmtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (DWORD i=0; i<numMtrls; ++i)
		{
			// Save the ith material.  Note that the MatD3D property does not have an ambient
			// value set when its loaded, so just set it to the diffuse value.
			m_vMats.push_back(d3dxmtrls[i].MatD3D);

			// Check if the ith material has an associative texture
			if (d3dxmtrls[i].pTextureFilename)
			{
				// Yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				char* texFN = d3dxmtrls[i].pTextureFilename;
				HR(D3DXCreateTextureFromFileEx(m_pDevice, texFN, D3DX_DEFAULT, D3DX_DEFAULT,
					D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
					D3DX_DEFAULT, D3DX_DEFAULT, 0,
					NULL, NULL, &tex));
				// Save the loaded texture
				m_vTexs.push_back(tex);
			}
			else
			{
				// No texture for the ith subset
				m_vTexs.push_back(0);
			}
		}
	}
	ReleaseCOM(mtrlBuffer); // done w/ buffer
}
