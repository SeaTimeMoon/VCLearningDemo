// decltype.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

template <class T,class U>
auto fun1(T t,U u)->decltype(t*u)
{
	return t*u;
}

int _tmain(int argc, _TCHAR* argv[])
{
	auto i=fun1(1.5,2);
	auto j=fun1(10,1.55f);
	auto k=fun1(1,2);

	cout<<"i="<<i<<" i type:"<<typeid(i).name()<<endl;
	cout<<"j="<<j<<" j type:"<<typeid(j).name()<<endl;
	cout<<"k="<<k<<" k type:"<<typeid(k).name()<<endl;

	decltype(fun1(1.5,2)) var1;
	decltype(fun1(1.5,2.0f)) var2;
	decltype(fun1(2,2)) var3;

	cout<<"var1 type: "<<typeid(var1).name()<<endl;
	cout<<"var2 type: "<<typeid(var2).name()<<endl;
	cout<<"var3 type: "<<typeid(var3).name()<<endl;


	int ni = 10;  
	decltype( ni ) myvar1;  
	decltype( (ni) ) myvar3 = ni;  
	cout<< "Address of    i: " << &ni << endl;  
	cout<< "Address of var1: " << &myvar1 << endl;  
	cout<< "Address of var3: " << &myvar3 << endl; 

	system("pause");

	return 0;
}

