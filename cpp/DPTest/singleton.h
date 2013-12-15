// singleton pattern
//
#pragma once

#include "stdafx.h"

class Singleton
{
public:
	static Singleton* getSingleten() { if (_m == NULL) _m = new Singleton; return _m; };
	~Singleton() { cout << "singleton des" << endl; }
protected:
	Singleton() { cout << "singleton" << endl; }
private:
	static Singleton* _m;
};

Singleton* Singleton::_m = NULL;