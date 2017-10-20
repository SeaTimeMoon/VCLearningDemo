// Auto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Auto.h"
#include <iostream>
#include <vector>
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CMyClass::CMyClass():a(0)
	,b(0)
{

}

CMyClass::~CMyClass()
{

}

// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			auto i=2;
			auto j=3.4f;
			auto k=4.5;
			auto c='c';

			CMyClass myclass;
			auto obj=myclass;

			const auto n1 = 1;  
			auto const n2 = 2;  
			auto n3 = 3;  
			const int n4 = 4;  
			auto p1 = new int(4);  
			auto* p2 = new int(5);

			

			cout<<"type of i:"<<typeid(i).name()<<endl;
			cout<<"type of j:"<<typeid(j).name()<<endl;
			cout<<"type of k:"<<typeid(k).name()<<endl;
			cout<<"type of c:"<<typeid(c).name()<<endl;
			cout<<"type of obj:"<<typeid(obj).name()<<endl;
			cout<<"type of n1:"<<typeid(n1).name()<<endl;
			cout<<"type of n2:"<<typeid(n2).name()<<endl;
			cout<<"type of n4:"<<typeid(n4).name()<<endl;
			cout<<"type of p1:"<<typeid(p1).name()<<endl;
			cout<<"type of p2:"<<typeid(p2).name()<<endl;

			vector<string>vecStrs;
			for (int i=0;i<5;i++)
			{
				string str;
				str="p";
				vecStrs.push_back(str);
			}

			for (auto iter=vecStrs.begin();iter!=vecStrs.end();iter++)
			{
				cout<<*(iter)<<endl;
			}

			system("pause");

			
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}


