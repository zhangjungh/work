// bridge pattern
//
#pragma once

#include "stdafx.h"

class AbstractionImp
{
public:
	virtual ~AbstractionImp() { cout << "AbstractionImp des" << endl; }
	virtual void Operation() = 0;
protected:
	AbstractionImp() { cout << "AbstractionImp" << endl; }
};

class Abstraction 
{ 
public: 
	virtual ~Abstraction() { cout << "Abstraction des" << endl; } 
	virtual void Operation() = 0; 
protected: 
	Abstraction() { cout << "Abstraction" << endl; }
};

class RefinedAbstraction : public Abstraction
{
public:
	RefinedAbstraction(AbstractionImp* imp) { _imp = imp; cout << "RefinedAbstraction" << endl; }
	~RefinedAbstraction() { cout << "RefinedAbstraction des" << endl; }
	void Operation() { _imp->Operation(); }
private:
	AbstractionImp* _imp;
};

class ConcreteAbstractionImpA : public AbstractionImp
{
public:
	ConcreteAbstractionImpA() { cout << "ConcreteAbstractionImpA" << endl; }
	~ConcreteAbstractionImpA() { cout << "ConcreteAbstractionImpA des" << endl; }
	void Operation() { cout << "operation" << endl; } 
};

class ConcreteAbstractionImpB : public AbstractionImp
{
public:
	ConcreteAbstractionImpB() { cout << "ConcreteAbstractionImpB" << endl; }
	~ConcreteAbstractionImpB() { cout << "ConcreteAbstractionImpB des" << endl; }
	void Operation() { cout << "operation" << endl; } 
};