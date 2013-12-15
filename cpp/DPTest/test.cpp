// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "factory.h"
//#include "abstractfactory.h"
//#include "singleton.h"
#include "builder.h"
#include "prototype.h"
#include "bridge.h"
#include "mystring.h"
#include "cast.h"

class EmptyClass
{
public:
	EmptyClass();
	~EmptyClass();
	int size;
	virtual int add();
	virtual int sub();
};

class EmptyClass2
{
public:
	EmptyClass2();
	virtual ~EmptyClass2();
};

class EmptyClass1 : public EmptyClass, public EmptyClass2
{
public:
	EmptyClass1();
	~EmptyClass1();
	virtual int mix() = 0;
};

void printFloat(float f)
{
	char *p = (char*) &f;
	int cnt = 0;
	for (int i=sizeof(f)-1; i>=0; --i)
	{
		unsigned char b = *(p+i);
		for (int j=0; j<8; ++j)
		{
			int v = b & 128;
			v >>= 7;
			cout << v;
			if (cnt == 0 || cnt == 8) cout << " ";
			b <<= 1;
			cnt++;
		}
	}
	cout << endl;
}

void printBytes(char *p, size_t s)
{
	int cnt = 0;
	for (int i=s-1; i>=0; --i)
	{
		unsigned char b = *(p+i);
		for (int j=0; j<8; ++j)
		{
			int v = b & 128;
			v >>= 7;
			cout << v;
			//if (cnt == 0 || cnt == 8) cout << " ";
			b <<= 1;
			cnt++;
		}
	}
	cout << endl;
}

#define MEM_PAGE_SIZE            0x1000
#define MEM_PAGE_MASK            (~(MEM_PAGE_SIZE - 1))
 
char * my_malloc(size_t n)
{
        size_t size = (n+MEM_PAGE_SIZE);
        char * ptr = (char*)malloc(size);
        if (NULL == ptr)
        {
			return NULL;
        }
        //printf("%08x, %zd\r\n", ptr, size);
        return (char *)(((long)ptr+MEM_PAGE_SIZE -1)& MEM_PAGE_MASK);
}
#pragma pack(2)
struct test111
{
char x1;
short x2;
float x3;
char x4;
};

class Stack
{
public:
	Stack();
	~Stack();
	void Push(int data);
	void Pop (int &data);
	bool empty();
};

class Queue
{
private:
	Stack s;
public:
	Queue();
	~Queue();
	void Enqueue(int data) {//12345,
		Stack temp;
		int t;
		while (!s.empty())
		{
			s.Pop(t);
			temp.Push(t);
		}
		temp.Push(data);
		while(!temp.empty())
		{
			temp.Pop(t);
			s.Push(t);
		}
	}
	void Dequeue(int &data) {
		if (!s.empty())
			s.Pop(data);
	}
};
class Person
{
public:
	Person():m_x(0) {}
	~Person() {}
	Person(int x) { m_x = x; }
	friend bool operator==(const Person &left, const Person &right)
	{
		return left.m_x == right.m_x;
	}
private:
	int m_x;
};
#include <set>
int _tmain(int argc, _TCHAR* argv[])
{
	set<int> myset;
	for (int i=0; i<10; ++i)
		myset.insert( rand() );
	set<int>::iterator iter = myset.begin();
	for (; iter != myset.end(); ++iter)
		cout << *iter << endl;
	cout << sizeof(test111) << endl;
	cout << "hello world" << endl;
	cout << sizeof(EmptyClass1) << endl;
	/*
	AbstractFactory *f1 = new ConcreteFactory1;
	AbstractFactory *f2 = new ConcreteFactory2;
	f1->createProductA();
	f1->createProductB();
	f2->createProductA();
	f2->createProductB();
	Singleton *p = Singleton::getSingleten();
	*/

	//int i = 3;
	//int j = 4;
	//int a = i++ + i++ + i++;
	//int b = j++ + ++j + ++j + ++j + j++ + j++;
	//cout << a << " " << i << " " << b << " " << j << endl;
	Person p1(10);
	Person p2(10);
	if (p1 == p2) cout << "hahaaha" << endl;
	if (10 == p1) cout << "hahahah" << endl;
	if (p2 == 10) cout << "hehhhhhh" << endl;

	TestAll(); 
	MyString my1;
	MyString my2("hello world");
	MyString my3(" haha");
	my1 = my2 + my3;
	my1.PrintString();
	MyString my4 = my2 + my3;
	my4.PrintString();
	MyString my5 = my4;
	my5.PrintString();
	MyString *my6 = new MyString;
	my6->PrintString();

	int test = 0x1000;
	printBytes((char*)&test, 4);
	test = ~(test-1);
	printBytes((char*)&test, 4);
	printFloat(5.0f);
	printFloat(6.0f);
	printFloat(5.1f);
	printFloat(5.2f);
	printFloat(-5.0f);
	printFloat(178.125f);
	printFloat(0.0f);

	TestNodeClass();

	char str[] = "helloworld";
	ReverseAndPrint(str);

	int qsort[] = {8, 4, 3, 5, 9, 0, 10, 33, 43, 53, 2};
	quick_sort(qsort, 0, sizeof(qsort)/sizeof(int) - 1);

	TestNumber();

	float f;
	for (f=0; f != f+1; f++)
		printFloat(f);
	Builder* b = new ConcreteBuilder;
	Director* d = new Director(b);
	d->Construct();
	b->GetProduct();

	Prototype *p = new ConcretePrototype;
	Prototype *q = p->Clone();

	AbstractionImp* imp = new ConcreteAbstractionImpA;
	Abstraction* abs = new RefinedAbstraction(imp);
	abs->Operation();

	system("pause");
	return 0;
}

