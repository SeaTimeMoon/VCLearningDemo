// Mutex2.cpp : Defines the entry point for the console application.
//

//要先启动Mutex1再启动Mutex2

#include "stdafx.h"
#include <conio.h>  
#include <windows.h>


const TCHAR MUTEX_NAME[] = _T("Mutex_MoreWindows"); 
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_NAME); //打开互斥量  
	if (hMutex == NULL)  
	{  
		printf("打开互斥量失败\n");  
		return 0;  
	}  
	printf("等待中....\n");  
	DWORD dwResult = WaitForSingleObject(hMutex, 20 * 1000); //等待互斥量被触发  
	switch (dwResult)  
	{  
	case WAIT_ABANDONED:  
		printf("拥有互斥量的进程意外终止\n");  
		break;  

	case WAIT_OBJECT_0:  
		printf("已经收到信号\n");  
		break;  

	case WAIT_TIMEOUT:  
		printf("信号未在规定的时间内送到\n");  
		break;  
	}  
	CloseHandle(hMutex); 

	system("pause");
	return 0;
}

