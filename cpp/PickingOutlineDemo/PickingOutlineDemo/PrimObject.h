///////////////////////////////////////////////////////////////////////////////
// FileName:	PrimObject.h
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Represent the primitive object
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Object.h"

class D3DPrimObject : public D3DObject
{
public:
	D3DPrimObject() {}
	~D3DPrimObject() {}

	virtual void			LoadMesh(const string& filename);
};