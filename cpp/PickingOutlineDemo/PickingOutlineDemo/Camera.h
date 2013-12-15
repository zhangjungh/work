///////////////////////////////////////////////////////////////////////////////
// FileName:	Camera.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-29
// Purpose:		Game Camera
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Headers.h"

class DirectInput;

class Camera
{
public:
	Camera(DirectInput *p);

	const D3DXMATRIX& GetView() const { return mView; }
	const D3DXMATRIX& GetProj() const { return mProj; } 
	const D3DXMATRIX& GetViewProj() const { return mViewProj; }

	const D3DXVECTOR3& GetRight() const { return mRightW; }
	const D3DXVECTOR3& GetUp() const { return mUpW; }
	const D3DXVECTOR3& GetLook() const { return mLookW; }

	D3DXVECTOR3& GetPos() { return mPosW; }

	void LookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& up);
	void SetLens(float fov, float aspect, float nearZ, float farZ);
	void SetSpeed(float s);

	void Update(float dt, float offsetHeight);

protected:
	void BuildView();
	void BuildWorldFrustumPlanes();

protected:
	DirectInput*				m_pInput;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mViewProj;

	// Relative to world space.
	D3DXVECTOR3 mPosW;
	D3DXVECTOR3 mRightW;
	D3DXVECTOR3 mUpW;
	D3DXVECTOR3 mLookW;

	float mSpeed;

	// Frustum Planes
	D3DXPLANE mFrustumPlanes[6]; // [0] = near
	                             // [1] = far
	                             // [2] = left
	                             // [3] = right
	                             // [4] = top
	                             // [5] = bottom
};