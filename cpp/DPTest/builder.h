// builder pattern
//
#pragma once

#include "stdafx.h"

class Product
{
public:
	~Product() { cout << "Product des" << endl; }
	Product(const string& a, const string& b, const string& c) { cout << "Product: " << a.c_str() << b.c_str() << c.c_str() << endl; }
};

class Builder
{
public:
	virtual ~Builder() { cout << "Builder des" << endl; }
	virtual void BuildPartA (const string& para) = 0;
	virtual void BuildPartB (const string& para) = 0;
	virtual void BuildPartC (const string& para) = 0;
	virtual Product* GetProduct() = 0;
protected:
	Builder() { cout <<"Builder" << endl; }
};

class ConcreteBuilder : public Builder
{
public:
	ConcreteBuilder() { cout <<"ConcreteBuilder" << endl; }
	~ConcreteBuilder() { cout <<"ConcreteBuilder des" << endl; }
	virtual void BuildPartA (const string& para) { _a = para; cout << "step a: " << para.c_str() << endl; } 
	virtual void BuildPartB (const string& para) { _b = para; cout << "step b: " << para.c_str() << endl; } 
	virtual void BuildPartC (const string& para) { _c = para; cout << "step c: " << para.c_str() << endl; } 
	virtual Product* GetProduct() {	return new Product(_a, _b, _c); }
private:
	string _a, _b, _c;
};

class Director
{
public:
	Director(Builder* bld) { _bld = bld; }
	~Director() { cout << "Director des" << endl; }
	void Construct() {
		_bld->BuildPartA("director defined");
		_bld->BuildPartB("director defined");
		_bld->BuildPartC("director defined");
	}
private:
	Builder* _bld;
};