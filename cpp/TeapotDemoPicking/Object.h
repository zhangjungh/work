#pragma once

#include "Headers.h"
#include "SilhouetteEdges.h"
#include "Camera.h"
#include "Effect.h"

class D3DObject
{
public:
	D3DObject();
	virtual ~D3DObject();

	static void					SetStatics(IDirect3DDevice9* pd, Camera* pc, Effect* pe) 
								{ 
									m_pDevice = pd; 
									m_pCamera = pc;
									m_pEffect = pe;
								}

	virtual void				LoadMesh(const string& filename) = 0;
	
	virtual void				Update(float dt, const D3DXVECTOR3& v);
	virtual void				Render(void);


	void						SetPosition(float x, float y, float z);
	void						SetRotationY(float d);
	void						SetScale(float s);

	void						SetHitTest(bool b) { m_bHitTest = b; }
	void						SetNotHitted(void) { m_bIsHitted = false; }
	bool						IsHitted(void) { return m_bIsHitted; }
	float						GetDist(void) { return m_fDist; }

protected:

	ID3DXMesh*					m_pMesh;			//mesh of the object
	ID3DXBuffer*				m_pAdjBuffer;		//adjacency info of the mesh for sihouette edge generation 
	D3DFORMAT					m_eFormat;			//16 or 32 bit index?
	vector<D3DMATERIAL9>		m_vMats;			//materials if many
	vector<IDirect3DTexture9*>	m_vTexs;			//textures if many

	D3DXMATRIX					m_mWorld;			//world matrix		

	SilhouetteEdges*			m_pEdgeObj;			//silhouette edge primitives

	D3DXVECTOR3					m_vMinAABBPt;		//AABB box min point, for fast hit test
	D3DXVECTOR3					m_vMaxAABBPt;		//AABB box max point, for fast hit test

	bool						m_bHitTest;			//perform hit test or not, some of the object, like terrain, no need to do test
	bool						m_bIsHitted;		//hitted or not
	float						m_fDist;			//if hitted, the distance recorded for final comparation

	static IDirect3DDevice9*	m_pDevice;			//device pointer for convenience
	static Camera*				m_pCamera;			//camera pointer
	static Effect*				m_pEffect;			//effect pointer
};