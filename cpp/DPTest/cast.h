#pragma once

class Base
{
public:
	Base() : m_x(0) {}
	virtual ~Base() {}
	int m_x;
};

class Derived : public Base
{
public:
	Derived() : m_y(1) {}
	~Derived() {}
	int m_y;
};


template <typename T>
T MaxValue (T x, T y)
{
	return (x>y ? x : y);
}

template <typename T>
class MyVector
{
public:
	MyVector() : size(0) {}
	~MyVector() {}
	int GetSize() const { return size; }
	T& operator[](int idx) { 
		if (idx < size) 
			return m_data[idx]; 
		else 
			cout <<"out of range" << endl; 
	}
	void append(T v) { 
		if (size+1 <= 100) {
			m_data[size] = v; 
			size += 1;
		}
	}

private:
	T m_data[100];
	int size;
};
void TestAll()
{
	Base *pb = new Derived;

	Derived *psd = static_cast<Derived*>(pb);
	if (psd != NULL) cout << "static get psd " << psd->m_y << endl;

	Derived *pdd = dynamic_cast<Derived*>(pb);
	if (pdd != NULL) cout << "dynamic get pdd " << pdd->m_y << endl;

	const Base b;
	Base *b1 = const_cast<Base*>(&b);
	b1->m_x = 10;
	cout << "const cast " << b.m_x << " " << b1->m_x << endl;


	Base *b2 = reinterpret_cast<Base*>(pb);
	cout << "reinterpret cast " << b2->m_x << endl;

	Derived *pd = new Derived;

	Base *pbb = static_cast<Base*>(pd);
	if (pbb != NULL) cout << "static get pbd " << pbb->m_x << endl;

	Base *pdb = dynamic_cast<Base*>(pb);
	if (pdb != NULL) cout << "dynamic get pdd " << pdb->m_x << endl;

	cout << MaxValue<float>(5.0f, 10.0f) << endl;
	cout << MaxValue<int>(18, 7) << endl;

	MyVector<int> vint;
	cout << vint.GetSize() << endl;
	vint.append(5);
	cout << vint[0] << endl;

	MyVector<float> vfloat;
	vfloat.append(1.2f);
	vfloat.append(2.2f);
	vfloat[1] = 4.2f;
	cout << vfloat[0] << " " << vfloat[1] << endl;
}
//union A
//{
//	int a[5];
//	char b;
//	double c;
//};
//
//struct B
//{
//	int n; //4+4
//	A a; //24
//	char c[10]; //10+6
//};
//
//struct C{ 
//	int n; //4
//	char c[10]; //10+2
//	union { 
//	int a[5]; //20 + 4
//	char b; //1
//	double cc; //8
//	};
//}; 

    union AA 
    {  
        int t;   //4  
        short m; //2  
        char p;  //1  
    } ;  
struct BB    
{  
    AA a;
    double c;    //8  
    char p2;     //1  
};  
struct CC
{
    struct A
    {  
        int t;   //4  
        short m; //2  
        char p;  //1  
    } ;  
	double c;    //8  
    char p2;     //1  
};
void ReverseAndPrint(char* str)
{
	size_t sz = strlen(str);
	int i = 0, j = sz-1;
	while (j > i)
	{
		char t = str[i];
		str[i] = str[j];
		str[j] = t;
		--j;
		++i;
	}
	cout /*<< sizeof(A) << " " << sizeof(B) << " " << sizeof(C) << " "*/ << sizeof(BB) << " " << sizeof(CC) << endl;
	cout << str << endl;
}

int portion(int *arr, int p, int r)
{
	int x = arr[r];
	int i = p-1;
	for (int j=p; j<r; ++j)
	{
		if (arr[j] <= x)
		{
			i += 1;
			int t = arr[i];
			arr[i] = arr[j];
			arr[j] = t;
		}
	}
	int t = arr[i+1];
	arr[i+1] = arr[r];
	arr[r] = t;

	return i+1;
}

void quick_sort(int *arr, int p, int r)
{
	if (p < r)
	{
		int q = portion(arr, p, r);
		quick_sort(arr, p, q-1);
		quick_sort(arr, q+1, r);
	}
}

#include <vector>
#include <stack>
#include <fstream>
#include <string>
class FileParser {
public:
	FileParser() : m_root(0) {}
	virtual ~FileParser()
	{
		if (m_root) 
		{
			//travel and delete
		}
	}

	void ParseFile(const string &filename)
	{
		fstream file;
		file.open(filename, ios::in);
		if (!file.is_open())
			return;

		Node* current;
		stack<Node*> path;
		string line;
		while (getline(file, line))
		{
			int pos = line.find_first_not_of('.');
			if (!m_root)
			{
				if (pos == 0)
				{
					m_root = new Node;
					m_root->data = line;
					path.push(m_root);
				}
			}
			else
			{
				Node *newNode = new Node;
				newNode->data = line.substr(pos, line.size()-1);
				int result = pos - path.size();
				if (result > 0)
				{
					delete newNode;
				}
				else
				{
					while (result++ != 0)
					{
						path.pop();
					}
					current = path.top();
					current->nextGen.push_back(newNode);
				}
				path.push(newNode);
			}
		}
	}

	void TravelDepth(vector<string> &all) const
	{
		all.clear();
		
		stack<Node*> path;
		Node *current = m_root;
		while (current)
		{
			string str(path.size(), '.');
			str += current->data;
			all.push_back(str);
			
			if (!current->nextGen.empty())
			{
				path.push(current);
				current = current->nextGen[0];
			}
			else 
			{
				bool nextFound = false;
				while (!path.empty())
				{
					Node *parent = path.top(); 
					int i = 0;
					for (; i<parent->nextGen.size(); ++i)
					{
						if (current == parent->nextGen[i])
							break;
					}
					if (i >= parent->nextGen.size()-1)
					{
						current = path.top();
						path.pop();
					}
					else
					{
						current = parent->nextGen[i+1];
						nextFound = true;
						break;
					}
				}
				if (!nextFound)
					current = NULL;
			}
		}
	}

	void TravelWidth(vector<string> &all) const
	{
		all.clear();
		
		stack<Node*> path;
		Node *current = m_root;
		while (current)
		{
			string str(path.size(), '.');
			str += current->data;
			all.push_back(str);
		
			bool nextFound = false;
			while (!path.empty())
			{
				Node *parent = path.top(); 
				int i = 0;
				for (; i<parent->nextGen.size(); ++i)
				{
					if (current == parent->nextGen[i])
						break;
				}
				if (i >= parent->nextGen.size()-1)
				{
					current = path.top();
					path.pop();
				}
				else
				{
					current = parent->nextGen[i+1];
					nextFound = true;
					break;
				}
			}
			if (!nextFound)
			{	
				current = NULL;
				if (!current->nextGen.empty())
					{
						path.push(current);
						current = current->nextGen[0];
					}
			}
		}
	}
private:
	typedef struct _Node {
		string data;
		vector<_Node*> nextGen;
	} Node;

	Node *m_root;
};

void TestNodeClass()
{
	FileParser parser;
	parser.ParseFile("C:\\Users\\jzhang\\Desktop\\input.txt");
	vector<string> all;
	parser.TravelDepth(all);
	for (int i=0; i<all.size(); ++i)
		cout << all[i] << endl;
	//parser.TravelWidth(all);
	for (int i=0; i<all.size(); ++i)
		cout << all[i] << endl;
}

class B
{
public:
	B() { cout << "cons b" << endl; }
	~B() { cout << "des b" << endl; }
	B(int i) : data(i) { cout << "cons by data " << data << endl; }
private:
	int data;
};

B Play(B b)
{
	return b;
}

void TestB()
{
	B t1 = Play(5); //cons by data
	B t2 = Play(t1);
}

class Number
{
public:
	string type;

	Number() : type("void") {}
	explicit Number(short) : type("short") {}
	Number(int) : type("int") {}
};

void Show(const Number &n) { cout << n.type << endl; }

void TestNumber()
{
	char *p = "%s";
	printf(p);
	short s = 42;
	Show(s);

	for (int i=0; i<8; ++i)
	{
		for (int k=0; k<=i; ++k)
		{
			cout << '*';
			for (int j=0; j<i; ++j)
			{
				cout << '.';
			}
		}
		cout << endl;
	}
}