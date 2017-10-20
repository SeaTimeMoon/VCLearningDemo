// for each.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	int testArray[]={1,2,3,4};

	for each (auto &i in testArray)
	{
		i=i+10;
	}

	for each (auto i in testArray)
	{
		cout<<i<<endl;
	}


	system("pause");
	return 0;
}

