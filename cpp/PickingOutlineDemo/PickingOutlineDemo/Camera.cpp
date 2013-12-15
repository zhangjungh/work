///////////////////////////////////////////////////////////////////////////////
// FileName:	Camera.cpp
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Implementation of the Camera
///////////////////////////////////////////////////////////////////////////////

#include "Camera.h"
#include "DirectInput.h"

Camera::Camera(DirectInput *p)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mViewProj);

	mPosW   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mRightW = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	mUpW    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	mLookW  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// Client should adjust to a value that makes sense for application's
	// unit scale, and the object the camera is attached to--e.g., car, jet,
	// human walking, etc.
	mSpeed  = 50.0f;

	m_pInput = p;
}

void Camera::LookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& up)
{
	D3DXVECTOR3 L = target - pos;
	D3DXVec3Normalize(&L, &L);

	D3DXVECTOR3 R;
	D3DXVec3Cross(&R, &up, &L);
	D3DXVec3Normalize(&R, &R);

	D3DXVECTOR3 U;
	D3DXVec3Cross(&U, &L, &R);
	D3DXVec3Normalize(&U, &U);

	mPosW   = pos;
	mRightW = R;
	mUpW    = U;
	mLookW  = L;

	BuildView();
	BuildWorldFrustumPlanes();

	mViewProj = mView * mProj;
}

void Camera::SetLens(float fov, float aspect, float nearZ, float farZ)
{
	D3DXMatrixPerspectiveFovLH(&mProj, fov, aspect, nearZ, farZ);
	BuildWorldFrustumPlanes();
	mViewProj = mView * mProj;
}

void Camera::SetSpeed(float s)
{
	mSpeed = s;
}

void Camera::Update(float dt, float offsetHeight)
{
	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
	if( m_pInput->KeyDown(DIK_W) )
		dir += mLookW;
	if( m_pInput->KeyDown(DIK_S) )
		dir -= mLookW;

	//move at mSpeed along net direction.
	D3DXVec3Normalize(&dir, &dir);
	D3DXVECTOR3 newPos = mPosW + dir*mSpeed*dt;
	{
		mPosW = newPos;
	}
	
	//rotate
	float yAngle = 0;
	if( m_pInput->KeyDown(DIK_D) )
		yAngle += 0.005f;
	if( m_pInput->KeyDown(DIK_A) )
		yAngle -= 0.005f;

	D3DXMATRIX R;
	// Rotate camera axes about the world's y-axis.
	D3DXMatrixRotationY(&R, yAngle);
	D3DXVec3TransformCoord(&mRightW, &mRightW, &R);
	D3DXVec3TransformCoord(&mUpW, &mUpW, &R);
	D3DXVec3TransformCoord(&mLookW, &mLookW, &R);


	// Rebuild the view matrix to reflect changes.
	BuildView();
	BuildWorldFrustumPlanes();

	mViewProj = mView * mProj;
}

void Camera::BuildView()
{
	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&mLookW, &mLookW);

	D3DXVec3Cross(&mUpW, &mLookW, &mRightW);
	D3DXVec3Normalize(&mUpW, &mUpW);

	D3DXVec3Cross(&mRightW, &mUpW, &mLookW);
	D3DXVec3Normalize(&mRightW, &mRightW);

	// Fill in the view matrix entries.

	float x = -D3DXVec3Dot(&mPosW, &mRightW);
	float y = -D3DXVec3Dot(&mPosW, &mUpW);
	float z = -D3DXVec3Dot(&mPosW, &mLookW);

	mView(0,0) = mRightW.x; 
	mView(1,0) = mRightW.y; 
	mView(2,0) = mRightW.z; 
	mView(3,0) = x;   

	mView(0,1) = mUpW.x;
	mView(1,1) = mUpW.y;
	mView(2,1) = mUpW.z;
	mView(3,1) = y;  

	mView(0,2) = mLookW.x; 
	mView(1,2) = mLookW.y; 
	mView(2,2) = mLookW.z; 
	mView(3,2) = z;   

	mView(0,3) = 0.0f;
	mView(1,3) = 0.0f;
	mView(2,3) = 0.0f;
	mView(3,3) = 1.0f;
}

void Camera::BuildWorldFrustumPlanes()
{
	// Note: Extract the frustum planes in world space.

	D3DXMATRIX VP = mView * mProj;

	D3DXVECTOR4 col0(VP(0,0), VP(1,0), VP(2,0), VP(3,0));
	D3DXVECTOR4 col1(VP(0,1), VP(1,1), VP(2,1), VP(3,1));
	D3DXVECTOR4 col2(VP(0,2), VP(1,2), VP(2,2), VP(3,2));
	D3DXVECTOR4 col3(VP(0,3), VP(1,3), VP(2,3), VP(3,3));

	// Planes face inward.
	mFrustumPlanes[0] = (D3DXPLANE)(col2);        // near
	mFrustumPlanes[1] = (D3DXPLANE)(col3 - col2); // far
	mFrustumPlanes[2] = (D3DXPLANE)(col3 + col0); // left
	mFrustumPlanes[3] = (D3DXPLANE)(col3 - col0); // right
	mFrustumPlanes[4] = (D3DXPLANE)(col3 - col1); // top
	mFrustumPlanes[5] = (D3DXPLANE)(col3 + col1); // bottom

	for(int i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize(&mFrustumPlanes[i], &mFrustumPlanes[i]);
	}
}