#pragma once

#include "Object.h"

class D3DPrimObject : public D3DObject
{
public:
	D3DPrimObject() {}
	~D3DPrimObject() {}

	virtual void			LoadMesh(const string& filename);
};