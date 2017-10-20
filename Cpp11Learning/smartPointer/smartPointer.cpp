// smartPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>

using namespace std;

class CMyClass
{
public:
	CMyClass(int i);
	~CMyClass();

private:

	int m_nVal;
};

CMyClass::CMyClass(int i):m_nVal(i)
{
	cout<<"CMyClass(int i) i="<<m_nVal<<endl;
}

CMyClass::~CMyClass()
{
	cout<<"~CMyClass() i="<<m_nVal<<endl;
}


class CBaseClass  
{  
public:  
	CBaseClass(){}  
	~CBaseClass(){}  
public:  
	virtual void Func() { cout<<"BaseClass::Func()\n";}  
};  

class CDeriveClass : public CBaseClass  
{  
public:  
	CDeriveClass(){}  
	~CDeriveClass(){}  
public:  
	virtual void Func() override { cout << "DeriveClass::Func()\n";}  
};

//Test for smart pointer of C++11
void TestAutoPointer();
void TestSharedPointer();
void TestCastPointer();


int _tmain(int argc, _TCHAR* argv[])
{
	
	/*TestAutoPointer();*/
	/*TestSharedPointer();*/
	TestCastPointer();

	system("pause");
	return 0;
}

void TestAutoPointer()
{
	auto_ptr<CMyClass>obj(new CMyClass(5));
	auto_ptr<CMyClass>obj2=obj;

	if (obj.get())// if obj binds a pointer
	{
		cout<<"obj exist"<<endl;
	}

	if (obj2.get())
	{
		cout<<"obj2 exist"<<endl;
	}

	obj2.reset(new CMyClass(10));

}

void TestSharedPointer()
{
	shared_ptr<CMyClass>sObj1(new CMyClass(20));

	if (sObj1)
	{
		cout<<"1. use count = "<<sObj1.use_count()<<endl;

		shared_ptr<CMyClass>sObj2=sObj1;
		cout<<"2. use count = "<<sObj2.use_count()<<endl;

		shared_ptr<CMyClass>sObj3(sObj1);
		cout<<"3. use count = "<<sObj3.use_count()<<endl;

		sObj1.reset();
		cout<<"4. After reset, use count = "<<sObj3.use_count()<<endl;
	}

	cout<<"5. sObj2 and sObj3 are out of scope"<<endl;
	cout<<"6. use count = "<<sObj1.use_count()<<endl;

}

void TestCastPointer()
{
	shared_ptr<CBaseClass> baseObj(new CBaseClass());
	baseObj->Func();

	shared_ptr<CDeriveClass> deriveObj=make_shared<CDeriveClass>();
	deriveObj->Func();

	shared_ptr<CBaseClass> obj3=dynamic_pointer_cast<CBaseClass>(deriveObj);
	obj3->Func();

	shared_ptr<CBaseClass> obj4=static_pointer_cast<CBaseClass>(deriveObj);
	obj4->Func();

	shared_ptr<int> foo;
	shared_ptr<const int> bar;

	foo = make_shared<int>(10);

	bar =const_pointer_cast<const int>(foo);

	std::cout << "*bar: " << *bar << '\n';
	*foo = 20;
	std::cout << "*bar: " << *bar << '\n';
	
}

