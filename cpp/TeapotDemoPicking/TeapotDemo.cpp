//=============================================================================
// TeapotDemo.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Demonstrates material alpha blending and transparency.
//
// Controls: Use mouse to orbit and zoom; use the 'W' and 'S' keys to 
//           alter the height of the camera.
//=============================================================================

#include "d3dApp.h"
#include "DirectInput.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>
#include "Vertex.h"
#include "silhouetteEdges.h"
class TeapotDemo : public D3DApp
{
public:
	TeapotDemo(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~TeapotDemo();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float dt);
	void drawScene();

	// Helper methods
	void buildBoxGeometry();
	void buildFX();
	void buildViewMtx();
	void buildProjMtx();

	void drawCrate();
	void drawTeapot();
	void drawTiger();
	void genSphericalTexCoords();

private:
	GfxStats* mGfxStats;
	
	IDirect3DVertexBuffer9* mBoxVB;
	IDirect3DIndexBuffer9* mBoxIB;
	ID3DXMesh*             mTeapot;
	IDirect3DTexture9*     mCrateTex;
	IDirect3DTexture9*     mTeapotTex;
	SilhouetteEdges*		mTeapotOutline;

	
	ID3DXMesh* mSceneMesh;
	D3DXMATRIX mSceneWorld;
	std::vector<Mtrl> mSceneMtrls;
	std::vector<IDirect3DTexture9*> mSceneTextures;
	SilhouetteEdges*             mOutline;

	ID3DXEffect* mFX;
	D3DXHANDLE   mhTech;
	D3DXHANDLE   mhWVP;
	D3DXHANDLE   mhWorldInvTrans;
	D3DXHANDLE   mhLightVecW;
	D3DXHANDLE   mhDiffuseMtrl;
	D3DXHANDLE   mhDiffuseLight;
	D3DXHANDLE   mhAmbientMtrl;
	D3DXHANDLE   mhAmbientLight;
	D3DXHANDLE   mhSpecularMtrl;
	D3DXHANDLE   mhSpecularLight;
	D3DXHANDLE   mhSpecularPower;
	D3DXHANDLE   mhEyePos;
	D3DXHANDLE   mhWorld;
	D3DXHANDLE   mhTex;

	ID3DXEffect* mFXoutline;
	D3DXHANDLE   mhTechoutline;
	D3DXHANDLE   mhWVoutline;
	D3DXHANDLE   mhProjoutline;

	Mtrl mCrateMtrl;
	Mtrl mTeapotMtrl;

	D3DXVECTOR3 mLightVecW;
	D3DXCOLOR   mAmbientLight;
	D3DXCOLOR   mDiffuseLight;
	D3DXCOLOR   mSpecularLight;

	float mCameraRotationY;
	float mCameraRadius;
	float mCameraHeight;

	D3DXMATRIX mCrateWorld;
	D3DXMATRIX mTeapotWorld;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	int mHitted;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	TeapotDemo app(hInstance, "Transparent Teapot Demo", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

    return gd3dApp->run();
}

TeapotDemo::TeapotDemo(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	InitAllVertexDeclarations();

	mGfxStats = new GfxStats();
	
	ID3DXBuffer* adjBuffer;
	//D3DXCreateTeapot(gd3dDevice, &mSceneMesh, &adjBuffer);
	//D3DXCreateCylinder(gd3dDevice, 0.5f, 0.5f, 2.0f, 20, 20, &mSceneMesh, &adjBuffer);
	//D3DXCreateBox(gd3dDevice, 2.0f, 2.0f, 2.0f, &mSceneMesh, &adjBuffer);
	LoadXFile("tiger.x", &mSceneMesh, &adjBuffer, mSceneMtrls, mSceneTextures);
	//D3DXCreateBuffer(mSceneMesh->GetNumFaces()*3*sizeof(DWORD), &adjBuffer);
	//mSceneMesh->GenerateAdjacency(1e-6f, (DWORD*)adjBuffer);

    D3DXVECTOR3* pData;
    D3DXVECTOR3 vCenter;
    FLOAT fObjectRadius;
    HR( mSceneMesh->LockVertexBuffer( 0, ( LPVOID* )&pData ) );
    HR( D3DXComputeBoundingSphere( pData, mSceneMesh->GetNumVertices(),
                                  D3DXGetFVFVertexSize( mSceneMesh->GetFVF() ), &vCenter, &fObjectRadius ) );
    HR( mSceneMesh->UnlockVertexBuffer() );

    D3DXMatrixTranslation( &mSceneWorld, -vCenter.x, -vCenter.y, -vCenter.z );
    D3DXMATRIXA16 m;
    D3DXMatrixRotationY( &m, D3DX_PI );
    mSceneWorld *= m;
    //D3DXMatrixRotationX( &m, D3DX_PI / 2.0f );
    //mSceneWorld *= m;
	
	mOutline = new SilhouetteEdges(gd3dDevice, mSceneMesh, adjBuffer);
	ReleaseCOM(adjBuffer);

	mCameraRadius    = 6.0f;
	mCameraRotationY = 1.2 * D3DX_PI;
	mCameraHeight    = 3.0f;

	mLightVecW     = D3DXVECTOR3(0.0, 0.0f, -1.0f);
	mDiffuseLight  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mAmbientLight  = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	mSpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	mCrateMtrl.ambient   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mCrateMtrl.diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mCrateMtrl.spec      = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	mCrateMtrl.specPower = 8.0f;

	mTeapotMtrl.ambient   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mTeapotMtrl.diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	mTeapotMtrl.spec      = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mTeapotMtrl.specPower = 16.0f;

	// Set the crate back a bit.
	D3DXMatrixTranslation(&mCrateWorld, 0.0f, 0.0f, 2.0f);
	D3DXMatrixTranslation(&mTeapotWorld, -2.0f, 0.0f, 0.0f);

	HR(D3DXCreateTextureFromFile(gd3dDevice, "crate.jpg", &mCrateTex));
	HR(D3DXCreateTextureFromFile(gd3dDevice, "brick1.dds", &mTeapotTex));

	ID3DXBuffer* teaAdjbuffer;
	HR(D3DXCreateTeapot(gd3dDevice, &mTeapot, &teaAdjbuffer));

	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPNT::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(mTeapot->CloneMesh(D3DXMESH_SYSTEMMEM, 
		elements, gd3dDevice, &temp));
	ReleaseCOM(mTeapot);
	mTeapot = temp;

	mTeapotOutline = new SilhouetteEdges(gd3dDevice, mTeapot, teaAdjbuffer);
	ReleaseCOM(teaAdjbuffer);

	// Generate texture coordinates for the teapot.
	//genSphericalTexCoords();

	mGfxStats->addVertices(24);
	mGfxStats->addTriangles(12);
	mGfxStats->addVertices(mTeapot->GetNumVertices());
	mGfxStats->addTriangles(mTeapot->GetNumFaces());

	buildBoxGeometry();
	buildFX();

	mHitted = 0;

	onResetDevice();
}

TeapotDemo::~TeapotDemo()
{
	delete mGfxStats;
	ReleaseCOM(mBoxVB);
	ReleaseCOM(mBoxIB);
	ReleaseCOM(mTeapot);
	ReleaseCOM(mCrateTex);
	ReleaseCOM(mTeapotTex);
	ReleaseCOM(mFX);

	DestroyAllVertexDeclarations();
}

bool TeapotDemo::checkDeviceCaps()
{
	D3DCAPS9 caps;
	HR(gd3dDevice->GetDeviceCaps(&caps));

	// Check for vertex shader version 2.0 support.
	if( caps.VertexShaderVersion < D3DVS_VERSION(2, 0) )
		return false;

	// Check for pixel shader version 2.0 support.
	if( caps.PixelShaderVersion < D3DPS_VERSION(2, 0) )
		return false;

	return true;
}

void TeapotDemo::onLostDevice()
{
	mGfxStats->onLostDevice();
	HR(mFX->OnLostDevice());
}

void TeapotDemo::onResetDevice()
{
	mGfxStats->onResetDevice();
	HR(mFX->OnResetDevice());


	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void TeapotDemo::updateScene(float dt)
{
	mGfxStats->update(dt);

	// Get snapshot of input devices.
	gDInput->poll();

	// Check input.
	if( gDInput->keyDown(DIK_W) )	 
		mCameraHeight   += 25.0f * dt;
	if( gDInput->keyDown(DIK_S) )	 
		mCameraHeight   -= 25.0f * dt;

	// Divide by 50 to make mouse less sensitive. 
	//mCameraRotationY += gDInput->mouseDX() / 100.0f;
	//mCameraRadius    += gDInput->mouseDY() / 25.0f;

	// If we rotate over 360 degrees, just roll back to 0
	if( fabsf(mCameraRotationY) >= 2.0f * D3DX_PI ) 
		mCameraRotationY = 0.0f;

	// Don't let radius get too small.
	if( mCameraRadius < 3.0f )
		mCameraRadius = 3.0f;

	// The camera position/orientation relative to world space can 
	// change every frame based on input, so we need to rebuild the
	// view matrix every frame with the latest changes.
	buildViewMtx();


	if (gDInput->mouseButtonDown(0))
	{
		float x = gDInput->mouseDX();
		float y = gDInput->mouseDY();
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(mhMainWnd, &pt);
		float z = gDInput->mouseDZ();
// Compute the vector of the pick ray in screen space
		
		float w = (float)md3dPP.BackBufferWidth;
		float h = (float)md3dPP.BackBufferHeight;
        D3DXVECTOR3 v;
        v.x = ( ( ( 2.0f * pt.x ) / w ) - 1 ) / mProj._11;
        v.y = -( ( ( 2.0f * pt.y ) / h ) - 1 ) / mProj._22;
        v.z = 1.0f;

        // Get the inverse view matrix
       // const D3DXMATRIX matView = *g_Camera.GetViewMatrix();
       // const D3DXMATRIX matWorld = *g_Camera.GetWorldMatrix();
        D3DXMATRIX mWorldView = mSceneWorld * mView;
        D3DXMATRIX m;
        D3DXMatrixInverse( &m, NULL, &mWorldView );

		D3DXVECTOR3 vPickRayDir, vPickRayOrig;
        // Transform the screen space pick ray into 3D space
        vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
        vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
        vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
        vPickRayOrig.x = m._41;
        vPickRayOrig.y = m._42;
        vPickRayOrig.z = m._43;

		BOOL bHit1 = FALSE;
		BOOL bHit2 = FALSE;
        DWORD dwFace;
                FLOAT fBary1, fBary2, fDist1, fDist2;
                D3DXIntersect( mSceneMesh, &vPickRayOrig, &vPickRayDir, &bHit1, &dwFace, &fBary1, &fBary2, &fDist1,
                               NULL, NULL );

		mWorldView = mTeapotWorld * mView;
        D3DXMatrixInverse( &m, NULL, &mWorldView );

        // Transform the screen space pick ray into 3D space
        vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
        vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
        vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
        vPickRayOrig.x = m._41;
        vPickRayOrig.y = m._42;
        vPickRayOrig.z = m._43;
		D3DXIntersect( mTeapot, &vPickRayOrig, &vPickRayDir, &bHit2, &dwFace, &fBary1, &fBary2, &fDist2,
                               NULL, NULL );

		if (bHit1 && bHit2)
		{
			if (fDist1 < fDist2)
			{
				mHitted = 1;
			}
			else
			{
				mHitted = 2;
			}
		}
		else if (bHit1)
		{
			mHitted = 1;
		}
		else if (bHit2)
		{
			mHitted = 2;
		}
		else
			mHitted = 0;
	}

}


void TeapotDemo::drawScene()
{
	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffeeeeee, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	HR(mFX->SetTechnique(mhTech));
	HR(mFX->SetValue(mhLightVecW, &mLightVecW, sizeof(D3DXVECTOR3)));
	HR(mFX->SetValue(mhDiffuseLight, &mDiffuseLight, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhAmbientLight, &mAmbientLight, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhSpecularLight, &mSpecularLight, sizeof(D3DXCOLOR)));

	//drawCrate();
	drawTeapot();
	drawTiger();

	mGfxStats->display();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void TeapotDemo::buildBoxGeometry()
{
	// Create the vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(24 * sizeof(VertexPNT), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED,	&mBoxVB, 0));

	// Write box vertices to the vertex buffer.
	VertexPNT* v = 0;
	HR(mBoxVB->Lock(0, 0, (void**)&v, 0));

	// Fill in the front face vertex data.
	v[0] = VertexPNT(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = VertexPNT(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = VertexPNT( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = VertexPNT( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = VertexPNT(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[5] = VertexPNT( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = VertexPNT( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[7] = VertexPNT(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8]  = VertexPNT(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = VertexPNT(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[10] = VertexPNT( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = VertexPNT( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = VertexPNT(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[13] = VertexPNT( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = VertexPNT( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[15] = VertexPNT(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = VertexPNT(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[17] = VertexPNT(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[18] = VertexPNT(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[19] = VertexPNT(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = VertexPNT( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[21] = VertexPNT( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[22] = VertexPNT( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[23] = VertexPNT( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	HR(mBoxVB->Unlock());


	// Create the index buffer.
	HR(gd3dDevice->CreateIndexBuffer(36 * sizeof(WORD),	D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,	D3DPOOL_MANAGED, &mBoxIB, 0));

	// Write box indices to the index buffer.
	WORD* i = 0;
	HR(mBoxIB->Lock(0, 0, (void**)&i, 0));

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	HR(mBoxIB->Unlock());
}

void TeapotDemo::buildFX()
{
	// Create the FX from a .fx file.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, "DirLightTex.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors));
	if( errors )
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	mhTech          = mFX->GetTechniqueByName("DirLightTexTech");
	mhWVP           = mFX->GetParameterByName(0, "gWVP");
	mhWorldInvTrans = mFX->GetParameterByName(0, "gWorldInvTrans");
	mhLightVecW     = mFX->GetParameterByName(0, "gLightVecW");
	mhDiffuseMtrl   = mFX->GetParameterByName(0, "gDiffuseMtrl");
	mhDiffuseLight  = mFX->GetParameterByName(0, "gDiffuseLight");
	mhAmbientMtrl   = mFX->GetParameterByName(0, "gAmbientMtrl");
	mhAmbientLight  = mFX->GetParameterByName(0, "gAmbientLight");
	mhSpecularMtrl  = mFX->GetParameterByName(0, "gSpecularMtrl");
	mhSpecularLight = mFX->GetParameterByName(0, "gSpecularLight");
	mhSpecularPower = mFX->GetParameterByName(0, "gSpecularPower");
	mhEyePos        = mFX->GetParameterByName(0, "gEyePosW");
	mhWorld         = mFX->GetParameterByName(0, "gWorld");
	mhTex           = mFX->GetParameterByName(0, "gTex");

	HR(D3DXCreateEffectFromFile(gd3dDevice, "outline.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &mFXoutline, &errors));
	if( errors )
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	mhTechoutline   = mFXoutline->GetTechniqueByName("OutLineTech");
	mhWVoutline           = mFXoutline->GetParameterByName(0, "WorldViewMatrix");
	mhProjoutline           = mFXoutline->GetParameterByName(0, "ProjMatrix");

}

void TeapotDemo::buildViewMtx()
{
	float x = mCameraRadius * cosf(mCameraRotationY);
	float z = mCameraRadius * sinf(mCameraRotationY);
	D3DXVECTOR3 pos(x, mCameraHeight, z);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);

	HR(mFX->SetValue(mhEyePos, &pos, sizeof(D3DXVECTOR3)));
}

void TeapotDemo::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}

void TeapotDemo::drawCrate()
{
	HR(mFX->SetValue(mhAmbientMtrl, &mCrateMtrl.ambient, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhDiffuseMtrl, &mCrateMtrl.diffuse, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhSpecularMtrl, &mCrateMtrl.spec, sizeof(D3DXCOLOR)));
	HR(mFX->SetFloat(mhSpecularPower, mCrateMtrl.specPower));

	HR(mFX->SetMatrix(mhWVP, &(mCrateWorld*mView*mProj)));
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &mCrateWorld);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	HR(mFX->SetMatrix(mhWorldInvTrans, &worldInvTrans));
	HR(mFX->SetMatrix(mhWorld, &mCrateWorld));
	HR(mFX->SetTexture(mhTex, mCrateTex));

	HR(gd3dDevice->SetVertexDeclaration(VertexPNT::Decl));
	HR(gd3dDevice->SetStreamSource(0, mBoxVB, 0, sizeof(VertexPNT)));
	HR(gd3dDevice->SetIndices(mBoxIB));

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12));
		HR(mFX->EndPass());
	}
	HR(mFX->End());
}

void TeapotDemo::drawTeapot()
{
	// Cylindrically interpolate texture coordinates.
	HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAPCOORD_0));

	// Enable alpha blending.
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true));
	HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	HR(mFX->SetValue(mhAmbientMtrl, &mTeapotMtrl.ambient, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhDiffuseMtrl, &mTeapotMtrl.diffuse, sizeof(D3DXCOLOR)));
	HR(mFX->SetValue(mhSpecularMtrl, &mTeapotMtrl.spec, sizeof(D3DXCOLOR)));
	HR(mFX->SetFloat(mhSpecularPower, mTeapotMtrl.specPower));

	HR(mFX->SetMatrix(mhWVP, &(mTeapotWorld*mView*mProj)));
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &mTeapotWorld);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	HR(mFX->SetMatrix(mhWorldInvTrans, &worldInvTrans));
	HR(mFX->SetMatrix(mhWorld, &mTeapotWorld));
	HR(mFX->SetTexture(mhTex, mTeapotTex));

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		HR(mTeapot->DrawSubset(0));
		HR(mFX->EndPass());
	}
	HR(mFX->End());

	// Disable wrap.
	HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, 0));

	// Disable alpha blending.
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false));

		//draw outline
	if (mHitted == 2)
	{
	gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	HR(mFXoutline->SetTechnique(mhTechoutline));
	mFXoutline->SetMatrix(mhWVoutline, &(mTeapotWorld*mView));
	mFXoutline->SetMatrix(mhProjoutline, &(mProj));

	
	HR(mFXoutline->Begin(&numPasses, 0));
	mFXoutline->BeginPass(0);

	mTeapotOutline->render();

	mFXoutline->EndPass();
	mFXoutline->End();

	gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}


void TeapotDemo::drawTiger()
{
	// Cylindrically interpolate texture coordinates.
	HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAPCOORD_0));

	// Enable alpha blending.
	//HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true));
	//HR(gd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
	//HR(gd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));


	HR(mFX->SetMatrix(mhWVP, &(mSceneWorld*mView*mProj)));
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &mSceneWorld);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	HR(mFX->SetMatrix(mhWorldInvTrans, &worldInvTrans));
	HR(mFX->SetMatrix(mhWorld, &mSceneWorld));

	// Begin passes.
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mFX->BeginPass(i));
		
		for(UINT j = 0; j < mSceneMtrls.size(); ++j)
		{
	
			HR(mFX->SetValue(mhAmbientMtrl, &mSceneMtrls[j].ambient, sizeof(D3DXCOLOR)));
			HR(mFX->SetValue(mhDiffuseMtrl, &mSceneMtrls[j].diffuse, sizeof(D3DXCOLOR)));
			HR(mFX->SetValue(mhSpecularMtrl, &mSceneMtrls[j].spec, sizeof(D3DXCOLOR)));
			HR(mFX->SetFloat(mhSpecularPower, mSceneMtrls[j].specPower));
			if(mSceneTextures[j] != 0)
			{
				HR(mFX->SetTexture(mhTex, mSceneTextures[j]));
			}
			else
			{
				//HR(mFX->SetTexture(mhTex, mWhiteTex));
			}
	
			HR(mFX->CommitChanges());
			HR(mSceneMesh->DrawSubset(j));
		}

		HR(mFX->EndPass());
	}
	HR(mFX->End());

	// Disable wrap.
	HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, 0));


	//draw outline
	if (mHitted == 1)
	{
	gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	HR(mFXoutline->SetTechnique(mhTechoutline));
	mFXoutline->SetMatrix(mhWVoutline, &(mSceneWorld*mView));
	mFXoutline->SetMatrix(mhProjoutline, &(mProj));

	
	HR(mFXoutline->Begin(&numPasses, 0));
	mFXoutline->BeginPass(0);

	mOutline->render();

	mFXoutline->EndPass();
	mFXoutline->End();

	gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	// Disable alpha blending.
	//HR(gd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false));
}

void TeapotDemo::genSphericalTexCoords()
{
	// D3DXCreate* functions generate vertices with position 
	// and normal data.  But for texturing, we also need
	// tex-coords.  So clone the mesh to change the vertex
	// format to a format with tex-coords.

	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	VertexPNT::Decl->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = 0;
	HR(mTeapot->CloneMesh(D3DXMESH_SYSTEMMEM, 
		elements, gd3dDevice, &temp));

	ReleaseCOM(mTeapot);

	// Now generate texture coordinates for each vertex.
	VertexPNT* vertices = 0;
	HR(temp->LockVertexBuffer(0, (void**)&vertices));

	for(UINT i = 0; i < temp->GetNumVertices(); ++i)
	{
		// Convert to spherical coordinates.
		D3DXVECTOR3 p = vertices[i].pos;
	
		float theta = atan2f(p.z, p.x);
		float phi   = acosf(p.y / sqrtf(p.x*p.x+p.y*p.y+p.z*p.z));

		// Phi and theta give the texture coordinates, but are not in 
		// the range [0, 1], so scale them into that range.

		float u = theta / (2.0f*D3DX_PI);
		float v = phi   / D3DX_PI;
		
		// Save texture coordinates.
		
		vertices[i].tex0.x = u;
		vertices[i].tex0.y = v;
	}
	HR(temp->UnlockVertexBuffer());

	// Clone back to a hardware mesh.
	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		elements, gd3dDevice, &mTeapot));

	ReleaseCOM(temp);
}