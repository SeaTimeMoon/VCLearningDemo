// Mutex1.cpp : Defines the entry point for the console application.
//

//要先启动Mutex1再启动Mutex2

#include "stdafx.h"
#include <conio.h>  
#include <windows.h>

const TCHAR MUTEX_NAME[] = _T("Mutex_MoreWindows");
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, MUTEX_NAME); //创建互斥量  
	printf("互斥量已经创建，现在按任意键触发互斥量\n");  
	getch();  
	//exit(0);  
	ReleaseMutex(hMutex);  
	printf("互斥量已经触发\n");  
	CloseHandle(hMutex);

	system("pause");
	return 0;
}

