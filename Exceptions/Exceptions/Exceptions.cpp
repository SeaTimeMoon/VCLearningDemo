// Exceptions.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Exceptions.h"
#include "MyDerive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

float divide(float firstNum,float secondNum);
void showClass();
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
		float firstNum,secondNum,resultNum;
		cout<<"请输入除数"<<endl;
		cin>>firstNum;
		cout<<"请输入被除数"<<endl;
		cin>>secondNum;
		resultNum=divide(firstNum,secondNum);
		cout<<"结果为:"<<resultNum<<endl;
		
		showClass();

	}

	system("pause");
	return nRetCode;
}

float divide(float firstNum,float secondNum)
{
	float result=0.0;
	try
	{
		if (secondNum==0)
		{
			throw invalid_argument("除数不能为0");
		}
		result=firstNum/secondNum;
	}
	catch (invalid_argument e)
	{
		cout<<e.what()<<endl;
		
	}

	return result;

	

}

void showClass()
{
	try
	{
		throw CMyDerive("123");
	}
	catch(CMyBase &e)
	{
		e.show();
	}

}
