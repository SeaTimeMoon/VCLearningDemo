// EnumClass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EnumClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum class StrongTypeEnum1
{
	First=0,
	Second,
	Third
};

enum class StrongTypeEnum2
{
	First=0,
	Second,
	Third
};

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

			StrongTypeEnum1 myEnum1;
			myEnum1=StrongTypeEnum1::First;

			StrongTypeEnum2 myEnum2;
			myEnum2=StrongTypeEnum2::Second;
			
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
