///////////////////////////////////////////////////////////////////////////////
// FileName:	Object.cpp
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Implementation of D3DOjbect, in Update() the AABB test is performed
//				first to fast exclude object that doesn't intersect with ray
///////////////////////////////////////////////////////////////////////////////


#include "Object.h"


IDirect3DDevice9* D3DObject::m_pDevice = 0;
Camera* D3DObject::m_pCamera = 0;
Effect* D3DObject::m_pEffect = 0;


D3DObject::D3DObject()
{
	m_bHitTest			= true;
	m_bIsHitted			= false;
	m_pEdgeObj			= NULL;

	m_pMesh				= NULL;
	m_pAdjBuffer		= NULL;

	D3DXMatrixIdentity(&m_mWorld);
}

D3DObject::~D3DObject()
{
	DeleteObj(m_pEdgeObj);
	ReleaseCOM(m_pMesh);
	ReleaseCOM(m_pAdjBuffer);

	for (size_t i=0; i<m_vTexs.size(); ++i)
	{
		ReleaseCOM(m_vTexs[i]);
	}
}

void D3DObject::SetPosition(float x, float y, float z)
{
	D3DXMatrixTranslation(&m_mWorld, x, y, z);
}

void D3DObject::SetRotationY(float d)
{
	D3DXMATRIXA16 m;
    D3DXMatrixRotationY(&m, d);
	m_mWorld = m * m_mWorld;
}

void D3DObject::SetScale(float s)
{
	D3DXMATRIXA16 m;
    D3DXMatrixScaling(&m, s, s, s);
	m_mWorld = m * m_mWorld;
}


void D3DObject::Update(float dt, const D3DXVECTOR3& v)
{
	if (m_bHitTest)
	{
		//get the inverse world view matrix
		D3DXMATRIX mWorldView = m_mWorld * m_pCamera->GetView();
		D3DXMATRIX m;
		D3DXMatrixInverse( &m, NULL, &mWorldView );

		D3DXVECTOR3 vPickRayDir, vPickRayOrig;
		//transform the screen space pick ray into 3D space
		vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
		vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
		vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
		vPickRayOrig.x = m._41;
		vPickRayOrig.y = m._42;
		vPickRayOrig.z = m._43;
		
		BOOL bHit = FALSE;
		if (::D3DXBoxBoundProbe(&m_vMinAABBPt, &m_vMaxAABBPt, &vPickRayOrig, &vPickRayDir))//AABB test first
		{
			DWORD dwFace;
			FLOAT fBary1, fBary2;
			D3DXIntersect( m_pMesh, &vPickRayOrig, &vPickRayDir, &bHit, 
				&dwFace, &fBary1, &fBary2, &m_fDist, NULL, NULL ); //intersect test
		}

		//still need further judgement to see if it is hitted at the closest position
		m_bIsHitted = bHit ? true : false; 
	}
	else
	{
		m_bIsHitted = false;
	}
}

void D3DObject::Render(void)
{

	//render object with main effect	
	//set per-pixel light fx parameters
	HR(m_pEffect->GetLightFx()->SetTechnique(m_pEffect->GetLightTech()));
	HR(m_pEffect->GetLightFx()->SetValue(m_pEffect->GetCameraPos(), &m_pCamera->GetPos(), sizeof(D3DXVECTOR3)));
	HR(m_pEffect->GetLightFx()->SetMatrix(m_pEffect->GetWVPMatrix(), &(m_mWorld * m_pCamera->GetViewProj())));
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &m_mWorld);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	HR(m_pEffect->GetLightFx()->SetMatrix(m_pEffect->GetWorldInvTrans(), &worldInvTrans));
	HR(m_pEffect->GetLightFx()->SetMatrix(m_pEffect->GetWorldMatrix(), &m_mWorld));

	UINT numPasses = 0;
	HR(m_pEffect->GetLightFx()->Begin(&numPasses, 0));
	for (UINT i=0; i<numPasses; ++i)
	{
		HR(m_pEffect->GetLightFx()->BeginPass(i));
		for(UINT j = 0; j < m_vMats.size(); ++j)
		{
			HR(m_pEffect->GetLightFx()->SetValue(m_pEffect->GetMaterial(), &m_vMats[j], sizeof(D3DMATERIAL9)));
			
			// If there is a texture, then use.
			if(!m_vTexs.empty() && m_vTexs[j] != 0)
			{
				HR(m_pEffect->GetLightFx()->SetTexture(m_pEffect->GetTexture(), m_vTexs[j]));
			}
			else
			{
				HR(m_pEffect->GetLightFx()->SetTexture(m_pEffect->GetTexture(), 0));
			}
			
			HR(m_pEffect->GetLightFx()->CommitChanges());
			HR(m_pMesh->DrawSubset(j));
		} 
		HR(m_pEffect->GetLightFx()->EndPass());
	}
	HR(m_pEffect->GetLightFx()->End());
	
	//render outlines if it is hitted
	if (m_bIsHitted)
	{
		//disable cull to draw triangle trip, or some of the faces would be cut!
		HR(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

		HR(m_pEffect->GetOutlineFx()->SetTechnique(m_pEffect->GetOLTech()));
		HR(m_pEffect->GetOutlineFx()->SetMatrix(m_pEffect->GetOLWVMatrix(), &(m_mWorld*m_pCamera->GetView())));
		HR(m_pEffect->GetOutlineFx()->SetMatrix(m_pEffect->GetOLPMatrix(), &(m_pCamera->GetProj())));


		HR(m_pEffect->GetOutlineFx()->Begin(&numPasses, 0));
		HR(m_pEffect->GetOutlineFx()->BeginPass(0)); //only has one pass

		m_pEdgeObj->Render();

		HR(m_pEffect->GetOutlineFx()->EndPass());
		HR(m_pEffect->GetOutlineFx()->End());

		HR(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW));
	}
}