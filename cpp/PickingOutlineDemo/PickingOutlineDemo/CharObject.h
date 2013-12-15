///////////////////////////////////////////////////////////////////////////////
// FileName:	CharObject.h
// Author:		JZhang
// Created:		2012-11-28
// Modified:	2012-11-29
// Purpose:		Character object, basically it is loaded from x file
//				which is different from the primitive objects
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Object.h"

class D3DCharObject : public D3DObject
{
public:
	D3DCharObject() {}
	~D3DCharObject() {}

	virtual void			LoadMesh(const string& filename);
};