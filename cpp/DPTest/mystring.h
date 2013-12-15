// mystring template class with a bunch of c++ features

#pragma once

#include "stdafx.h"

class MyString 
{
public:
	virtual ~MyString(); //des
	MyString(); //cons
	MyString(const char *str); //cons with parameter
	MyString(const MyString &other); //copy cons

	void PrintString(void);

	//friend
	friend MyString operator+(const MyString &left, const MyString &right);

	MyString& operator=(const MyString &other);
	
	friend bool operator==(const MyString &left, const MyString &right);

	static void* operator new (size_t size);
	static void operator delete(void *mem);
private:
	char *m_data;
};

MyString::MyString()
{
	m_data = (char*) malloc(1);
	if (m_data != NULL)
		m_data[0] = '\0';
	cout << "cons string" << endl;
}

MyString::~MyString()
{
	if (m_data != NULL)
		free(m_data);
	m_data = NULL;
	cout << "des string" << endl;
}

void MyString::PrintString(void)
{
	cout << m_data << " " << strlen(m_data) << endl;
}

MyString::MyString(const char *str)
{
	if (str == NULL)
	{
		m_data = (char*) malloc(1);
		if (m_data != NULL)
			m_data[0] = '\0';
	}
	else 
	{
		size_t size = strlen(str) + 1;
		m_data = (char*) malloc(size);
		if (m_data != NULL)
			strcpy_s(m_data, size, str);
	}
	cout << "para cons string" << endl;
}

MyString::MyString(const MyString &other)
{
	size_t size = strlen(other.m_data) + 1;
	m_data = (char*) malloc(size);
	if (m_data != NULL)
		strcpy_s(m_data, size, other.m_data);
	cout << "copy cons string" << endl;
}

MyString& MyString::operator=(const MyString &other)
{
	cout << "= called" << endl;
	if (this == &other)
		return *this;
	if (m_data != NULL)
		free(m_data);
	size_t size = strlen(other.m_data) + 1;
	m_data = (char*) malloc(size);
	strcpy_s(m_data, size, other.m_data);
	return *this;
}

MyString operator+(const MyString &left, const MyString &right)
{
	size_t size = strlen(left.m_data) + strlen(right.m_data) + 1;
	char *temp = (char*) malloc(size);
	strcpy_s(temp, size, left.m_data);
	strcat_s(temp, size, right.m_data);
	return MyString(temp);
}

bool operator==(const MyString &left, const MyString &right)
{
	return (strcmp(left.m_data, right.m_data) == 0);
}

void* MyString::operator new(size_t size)
{
	cout << "overloaded new " << size << endl;
	return malloc(size);
}

void MyString::operator delete(void* mem)
{
	cout << "overloaded delete " << endl;
	if (mem != NULL)
		free(mem);
}