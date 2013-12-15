// absract factory pattern
//
#pragma once

#include "stdafx.h"

class AbstractProductA 
{
public:
	virtual	~AbstractProductA() { cout <<"AbstractProductA des" << endl; }
protected:
	AbstractProductA() { cout <<"AbstractProductA" << endl; }
};

class AbstractProductB 
{
public:
	virtual	~AbstractProductB() { cout <<"AbstractProductB des" << endl; }
protected:
	AbstractProductB() { cout <<"AbstractProductB" << endl; }
};

class ProductA1 : public AbstractProductA
{
public:
	ProductA1() { cout << "ProductA1" << endl; }
	virtual ~ProductA1() { cout << "ProductA1 des" << endl; }
};

class ProductA2 : public AbstractProductA
{
public:
	ProductA2() { cout << "ProductA2" << endl; }
	virtual ~ProductA2() { cout << "ProductA2 des" << endl; }
};

class ProductB1 : public AbstractProductB
{
public:
	ProductB1() { cout << "ProductB1" << endl; }
	virtual ~ProductB1() { cout << "ProductB1 des" << endl; }
};

class ProductB2 : public AbstractProductB
{
public:
	ProductB2() { cout << "ProductB2" << endl; }
	virtual ~ProductB2() { cout << "ProductB2 des" << endl; }
};


class AbstractFactory
{
public:
	virtual ~AbstractFactory() { cout << "AbstractFactory des" << endl; }
	virtual AbstractProductA* createProductA() = 0;
	virtual AbstractProductB* createProductB() = 0;
protected:
	AbstractFactory() { cout << "AbstractFactory" << endl; };
};

class ConcreteFactory1 : public AbstractFactory
{
public:
	virtual ~ConcreteFactory1() { cout << "ConcreteFactory1 des" << endl; }
	ConcreteFactory1() { cout << "ConcreteFactory1" << endl; } 
	AbstractProductA* createProductA() { cout <<"create product a1 in 1" << endl; return new ProductA1; }
	AbstractProductB* createProductB() { cout <<"create product b1 in 1" << endl; return new ProductB1; }
};

class ConcreteFactory2 : public AbstractFactory
{
public:
	virtual ~ConcreteFactory2() { cout << "ConcreteFactory2 des" << endl; }
	ConcreteFactory2() { cout << "ConcreteFactory2" << endl; } 
	AbstractProductA* createProductA() { cout <<"create product a2 in 2" << endl; return new ProductA2; }
	AbstractProductB* createProductB() { cout <<"create product b2 in 2" << endl; return new ProductB2; }
};