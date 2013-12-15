///////////////////////////////////////////////////////////////////////////////
// FileName:	SceneGraph.cpp
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Implementation of SceneGraph class, basically everything is here
///////////////////////////////////////////////////////////////////////////////


#include "SceneGraph.h"
#include "DirectInput.h"
#include "Vertex.h"
#include "CharObject.h"
#include "PrimObject.h"

SceneGraph::~SceneGraph()
{
	DestroyAllVertexDeclarations();
	DeleteObj(m_pGfxStats);
	DeleteObj(m_pEffect);
	DeleteObj(m_pCamera);
	DeleteObj(m_pInput);

	for (size_t i=0; i<m_vSceneObjs.size(); ++i)
	{
		DeleteObj(m_vSceneObjs[i]);
	}
	m_vSceneObjs.clear();
}

bool SceneGraph::CheckDeviceCaps(void)
{
	D3DCAPS9 caps;
	HR(m_pDevice->GetDeviceCaps(&caps));

	// Check for vertex shader version 2.0 support.
	if( caps.VertexShaderVersion < D3DVS_VERSION(2, 0) )
		return false;

	// Check for pixel shader version 2.0 support.
	if( caps.PixelShaderVersion < D3DPS_VERSION(2, 0) )
		return false;

	return true;
}

void SceneGraph::OnLostDevice()
{
	m_pGfxStats->OnLostDevice();
	HR(m_pEffect->GetLightFx()->OnLostDevice());
	HR(m_pEffect->GetOutlineFx()->OnLostDevice());
}

void SceneGraph::OnResetDevice()
{
	m_pGfxStats->OnResetDevice();
	HR(m_pEffect->GetLightFx()->OnResetDevice());
	HR(m_pEffect->GetOutlineFx()->OnResetDevice());

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	float w = (float)m_D3DPrams.BackBufferWidth;
	float h = (float)m_D3DPrams.BackBufferHeight;
	m_pCamera->SetLens(D3DX_PI * 0.25f, w/h, 0.01f, 5000.0f);
}

void SceneGraph::Update(float dt)
{
	//update stats
	m_pGfxStats->Update(dt);

	//update input devices.
	m_pInput->Poll();

	//update camera
	m_pCamera->Update(dt, 0);

	if (m_pInput->MouseButtonDown(0))
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hMainWnd, &pt);
		// Compute the vector of the pick ray in screen space
		float w = (float)m_D3DPrams.BackBufferWidth;
		float h = (float)m_D3DPrams.BackBufferHeight;
		D3DXVECTOR3 v;
      	v.x = ( ( ( 2.0f * pt.x ) / w ) - 1 ) / m_pCamera->GetProj()._11;
        v.y = -( ( ( 2.0f * pt.y ) / h ) - 1 ) / m_pCamera->GetProj()._22;
        v.z = 1.0f;

		for (size_t i=0; i<m_vSceneObjs.size(); ++i)
		{
			m_vSceneObjs[i]->Update(dt, v);
		}

		//check for multi object hitted
		float min = FLT_MAX;
		int index = -1;
		for (size_t i=0; i<m_vSceneObjs.size(); ++i)
		{
			if (m_vSceneObjs[i]->IsHitted())
			{
				if (min > m_vSceneObjs[i]->GetDist())
				{
					min = m_vSceneObjs[i]->GetDist();
					index = i;
				}
			}
		}
		if (index != -1)
		{
			for (size_t i=0; i<m_vSceneObjs.size(); ++i)
			{
				if (i != index)
				{
					m_vSceneObjs[i]->SetNotHitted();
				}
			}
		}
	}
}

void SceneGraph::Init()
{
	//check device caps
	if(!CheckDeviceCaps())
	{
		MessageBox(0, "CheckDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	//init vertex declaration
	InitAllVertexDeclarations(m_pDevice);


	//init onscreen info
	m_pGfxStats = new GfxStats(m_pDevice);

	//init input
	m_pInput = new DirectInput(m_pD3DApp, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	//init camera.
	m_pCamera = new Camera(m_pInput);
	m_pCamera->GetPos() = D3DXVECTOR3(-0.0f, 2.0f, -15.0f);
	m_pCamera->SetSpeed(40.0f);

	// Load the default texture.
	//HR(D3DXCreateTextureFromFile(gd3dDevice, "whitetex.dds", &mWhiteTex));

	// Init a light.
	m_sLight.dirW    = D3DXVECTOR3(-0.707f, 0.0f, 0.707f);
	D3DXVec3Normalize(&m_sLight.dirW, &m_sLight.dirW);
	m_sLight.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	m_sLight.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_sLight.spec    = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	//init effect
	m_pEffect = new Effect();
	m_pEffect->BuildEffect(m_pDevice);

	HR(m_pEffect->GetLightFx()->SetValue(m_pEffect->GetLight(), &m_sLight, sizeof(DirLight)));

	
	//load all object
	D3DObject::SetStatics(m_pDevice, m_pCamera, m_pEffect);

	for (int i=0; i<5; ++i) //should load different object, only has this model for testing
	{
		D3DObject* p = new D3DCharObject();
		p->LoadMesh("tiger.x");
		p->SetPosition(-(i%5)*1.0f, 0.0f, (i/5)*3.0f);
		m_vSceneObjs.push_back(p);
	}
	for (int i=0; i<3; ++i)
	{
		D3DObject* p = new D3DCharObject();
		p->LoadMesh("tiger.x");
		p->SetPosition(((i+6)%5)*2.0f, 0.0f, -1.0f);
		p->SetRotationY(D3DX_PI/(2+i));
		m_vSceneObjs.push_back(p);
	}

	D3DObject* p = new D3DPrimObject();
	p->LoadMesh("teapot");
	p->SetPosition(0.0f, 0.5f, 10.0f);
	m_vSceneObjs.push_back(p);

	p = new D3DPrimObject();
	p->LoadMesh("box");
	p->SetPosition(-2.0f, 0.5f, 4.0f);
	m_vSceneObjs.push_back(p);

	
	p = new D3DPrimObject();
	p->LoadMesh("terrain");
	p->SetHitTest(false);
	p->SetPosition(0.0f, -1.0f, 0.0f);
	m_vSceneObjs.push_back(p);

	OnResetDevice();
}

void SceneGraph::Render()
{
	//clear the backbuffer and depth buffer.
	HR(m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x7f7f7fff, 1.0f, 0));
	HR(m_pDevice->BeginScene());
	
	//iterator all objects and draw
	for (size_t i=0; i<m_vSceneObjs.size(); ++i)
	{
		m_vSceneObjs[i]->Render();
	}

	//display onscreen info
	m_pGfxStats->Render();

	HR(m_pDevice->EndScene());
	//present the backbuffer.
	HR(m_pDevice->Present(0, 0, 0, 0));

}