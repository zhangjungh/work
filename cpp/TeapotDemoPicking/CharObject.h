#pragma once

#include "Object.h"

class D3DCharObject : public D3DObject
{
public:
	D3DCharObject() {}
	~D3DCharObject() {}

	virtual void			LoadMesh(const string& filename);
};