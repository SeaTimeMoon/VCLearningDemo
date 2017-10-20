// override and final.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

class CBaseClass
{
public:
	CBaseClass(){};
	~CBaseClass(){};
	virtual void fun1(int param){cout<<"Base class: "<<param<<endl;}
	virtual void fun2()const{cout<<"Base class::fun2"<<endl;}
	virtual void fun3()final{}
protected:
private:
};


class CDeriveClass:public CBaseClass
{
public:
	CDeriveClass(){};
	~CDeriveClass(){};
	virtual void fun1(int param)override{cout<<"Derive class: "<<param<<endl;}
	virtual void fun2()const override{cout<<"Derive class::fun2"<<endl;}

protected:	
private:
};


int _tmain(int argc, _TCHAR* argv[])
{
	CBaseClass* pBase=new CBaseClass();
	pBase->fun1(5);
	pBase->fun2();

	CDeriveClass* pDerive=new CDeriveClass();
	pDerive->fun1(5);
	pDerive->fun2();

	CBaseClass* pBase2=new CDeriveClass();
	pBase2->fun1(5);
	pBase2->fun2();

	delete pBase2;
	delete pDerive;
	delete pBase;

	system("pause");

	return 0;
}

