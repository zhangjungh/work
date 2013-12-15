#pragma once

#include "D3DFrameWork.h"
#include "GfxDisp.h"
#include "Singleton.h"
#include <vector>

class ObjectPicking : public D3DFrameWork, public Singleton<ObjectPicking>
{
public:
	void Init(HINSTANCE hInstance, string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~ObjectPicking();

	// implement specific application requirements.
	virtual bool				CheckDeviceCaps();
	virtual void				OnLostDevice();
	virtual void				OnResetDevice();
	virtual void				UpdateFrame(float dt);
	virtual void				RenderFrame();

	ObjectPicking() {}
private:

	GfxDisp*					m_GfxDisp;

	ID3DXMesh* mSceneMesh;
	D3DXMATRIX mSceneWorld;
	std::vector<Mtrl> mSceneMtrls;
	std::vector<IDirect3DTexture9*> mSceneTextures;
};