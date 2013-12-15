// prototype pattern
//
#pragma once

#include "stdafx.h"

class Prototype
{
public:
	virtual ~Prototype() { cout << "Prototype des" << endl; }
	virtual Prototype* Clone() const = 0;
protected:
	Prototype() { cout << "Prototype" << endl; }
};

class ConcretePrototype : public Prototype
{
public:
	ConcretePrototype() { cout << "ConcretePrototype" << endl; }
	ConcretePrototype(const ConcretePrototype &c) { cout << "ConcretePrototype copy" << endl; }
	virtual ~ConcretePrototype() { cout << "ConcretePrototype" << endl; }
	virtual Prototype* Clone() const { return new ConcretePrototype(*this); }
};
