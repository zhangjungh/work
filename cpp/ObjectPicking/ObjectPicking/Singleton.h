///////////////////////////////////////////////////////////////////////////////
// FileName:	Singleton.h
// Author:		JZhang
// Created:		2012-11-27
// Modified:	2012-11-27
// Purpose:		singleton pattern to create only one instance of the class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

template <typename T>
class Singleton
{
public:
	static T* GetInstance() 
	{
		static T t;
		return &t;
	}
protected:
	Singleton() {}
private:
	Singleton(const Singleton& s);
	Singleton& operator=(const Singleton& s);
};