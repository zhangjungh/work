// factory pattern
//
#pragma once

#include "stdafx.h"

class Product 
{
public:
	virtual	~Product() = 0;
protected:
	Product();
};

class ConcreteProduct : public Product
{
public:
	ConcreteProduct();
	virtual ~ConcreteProduct();
};

Product::Product() { cout << "Product" << endl; }
Product::~Product() { cout << "Product des" << endl; }
ConcreteProduct::ConcreteProduct() { cout << "ConcreteProduct" << endl; }
ConcreteProduct::~ConcreteProduct() { cout << "ConcreteProduct des" << endl; }

class Factory
{
public:
	virtual ~Factory() = 0;
	virtual Product* createProduct();
protected:
	Factory();
};

class ConcreteFactory : public Factory
{
public:
	virtual ~ConcreteFactory();
	ConcreteFactory();
private:
	virtual Product* createProduct();
};

Factory::Factory() { cout << "Factory" << endl; }
Factory::~Factory() { cout << "Factory des" << endl; }
Product* Factory::createProduct() { cout << "Factory create Product" << endl; return 0; }
ConcreteFactory::ConcreteFactory() { cout << "ConcreteFactory" << endl; }
ConcreteFactory::~ConcreteFactory() { cout << "ConcreteFactory des" << endl; }
Product* ConcreteFactory::createProduct() { cout << "ConcreteFactory create Product" << endl; return new ConcreteProduct; }
