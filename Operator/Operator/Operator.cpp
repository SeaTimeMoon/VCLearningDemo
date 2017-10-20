// Operator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Operator.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include "Person.h"
#include "AgeOlder.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	CPerson oPerson1("Anna",19);
	CPerson oPerson2("Mike",22);
	if (oPerson1!=oPerson2)
	{
		cout<<"different age"<<endl;
		if (oPerson1>oPerson2)
		{
			cout<<oPerson1.m_strName<<"年纪比"<<oPerson2.m_strName<<"大"<<oPerson1-oPerson2<<"岁"<<endl;
		}
		else
		{
			cout<<oPerson1.m_strName<<"年纪比"<<oPerson2.m_strName<<"小"<<oPerson2-oPerson1<<"岁"<<endl;

		}

	}
	else
	{
		cout<<"same age"<<endl;
	}


	vector<CPerson>vecPeople;
	srand(time(NULL));
	for (int i=0;i<10;i++)
	{
		int age=rand()%10+20;
		string name="";
		cout<<"Please input a name"<<endl;
		cin>>name;
		CPerson person(name,age);
		vecPeople.push_back(person);
	}
	
	for (vector<CPerson>::iterator iter=vecPeople.begin();iter!=vecPeople.end();iter++)
	{
		cout<<"name:"<<iter->m_strName<<"\t"<<"age:"<<iter->m_nAge<<endl;
	}

	int agebound=20;
	cout<<"----Please input a age bound----"<<endl;
	cin>>agebound;
	vector<CPerson>::iterator iter1=find_if(vecPeople.begin(),vecPeople.end(),CAgeOlder(agebound));
	while(iter1!=vecPeople.end())
	{
		cout<<"name:"<<iter1->m_strName<<"\t"<<"age:"<<iter1->m_nAge<<endl;
		iter1++;
		iter1=find_if(iter1,vecPeople.end(),CAgeOlder(agebound));
	}
		
	system("pause");
	return nRetCode;
}
