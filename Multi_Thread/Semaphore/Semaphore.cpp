// Semaphore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>  
#include <windows.h> 

long g_nNum;  
unsigned int __stdcall Fun(void *pPM);  
const int THREAD_NUM = 10;  
//信号量与关键段  
HANDLE            g_hThreadParameter;  
CRITICAL_SECTION  g_csThreadCode;

int _tmain(int argc, _TCHAR* argv[])
{
	printf("     经典线程同步 信号量Semaphore\n");   

	//初始化信号量和关键段  
	g_hThreadParameter = CreateSemaphore(NULL, 0, 1, NULL);//当前0个资源，最大允许1个同时访问  
	InitializeCriticalSection(&g_csThreadCode);  

	HANDLE  handle[THREAD_NUM];   
	g_nNum = 0;  
	int i = 0;  
	while (i < THREAD_NUM)   
	{  
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);  
		WaitForSingleObject(g_hThreadParameter, INFINITE);//等待信号量>0  
		++i;  
	}  
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);  

	//销毁信号量和关键段  
	DeleteCriticalSection(&g_csThreadCode);  
	CloseHandle(g_hThreadParameter);  
	for (i = 0; i < THREAD_NUM; i++)  
		CloseHandle(handle[i]);

	system("pause");

	return 0;
}

unsigned int __stdcall Fun(void *pPM)  
{  
	int nThreadNum = *(int *)pPM;  
	ReleaseSemaphore(g_hThreadParameter, 1, NULL);//信号量++  

	Sleep(50);//some work should to do  

	EnterCriticalSection(&g_csThreadCode);  
	++g_nNum;  
	Sleep(0);//some work should to do  
	printf("线程编号为%d  全局资源值为%d\n", nThreadNum, g_nNum);  
	LeaveCriticalSection(&g_csThreadCode);  
	return 0;  
}

