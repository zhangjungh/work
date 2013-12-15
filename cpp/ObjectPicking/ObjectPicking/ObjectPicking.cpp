
#include "ObjectPicking.h"
#include "Vertex.h"
//#include <d3dx9mesh.h>
void ObjectPicking::Init(HINSTANCE hInstance, string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
{

	InitBase(hInstance, winCaption, devType, requestedVP);

	InitAllVertexDeclarations(m_pD3DDevice);

	m_GfxDisp = new GfxDisp(m_pD3DDevice, m_sCPU, m_sGPU, m_uMem);

	
	LoadXFile(m_pD3DDevice, "..\\Media\\tiny.x", &mSceneMesh, mSceneMtrls, mSceneTextures);
	D3DXMatrixIdentity(&mSceneWorld);

	srand(time_t(0));

	if(!CheckDeviceCaps())
	{
		MessageBox(0, "CheckDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}
}


ObjectPicking::~ObjectPicking()
{
	delete m_GfxDisp;
}

bool ObjectPicking::CheckDeviceCaps()
{
	// Nothing to check.
	return true;
}

void ObjectPicking::OnLostDevice()
{
	m_GfxDisp->onLostDevice();
}

void ObjectPicking::OnResetDevice()
{
	m_GfxDisp->onResetDevice();
}

void ObjectPicking::UpdateFrame(float dt)
{
	m_GfxDisp->update(dt);
}

void ObjectPicking::RenderFrame()
{
	HR(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0));

	RECT formatRect;
	GetClientRect(m_hMainWnd, &formatRect);

	HR(m_pD3DDevice->BeginScene());

	m_GfxDisp->display();
	
	for(UINT j = 0; j < mSceneMtrls.size(); ++j)
	{
		HR(mSceneMesh->DrawSubset(j));
	}

	HR(m_pD3DDevice->EndScene());
	HR(m_pD3DDevice->Present(0, 0, 0, 0));
}