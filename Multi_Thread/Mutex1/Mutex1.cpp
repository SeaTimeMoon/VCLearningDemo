// Mutex1.cpp : Defines the entry point for the console application.
//

//Ҫ������Mutex1������Mutex2

#include "stdafx.h"
#include <conio.h>  
#include <windows.h>

const TCHAR MUTEX_NAME[] = _T("Mutex_MoreWindows");
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, MUTEX_NAME); //����������  
	printf("�������Ѿ����������ڰ����������������\n");  
	getch();  
	//exit(0);  
	ReleaseMutex(hMutex);  
	printf("�������Ѿ�����\n");  
	CloseHandle(hMutex);

	system("pause");
	return 0;
}

