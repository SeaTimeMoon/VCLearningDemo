// Semaphore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>  
#include <windows.h> 

long g_nNum;  
unsigned int __stdcall Fun(void *pPM);  
const int THREAD_NUM = 10;  
//�ź�����ؼ���  
HANDLE            g_hThreadParameter;  
CRITICAL_SECTION  g_csThreadCode;

int _tmain(int argc, _TCHAR* argv[])
{
	printf("     �����߳�ͬ�� �ź���Semaphore\n");   

	//��ʼ���ź����͹ؼ���  
	g_hThreadParameter = CreateSemaphore(NULL, 0, 1, NULL);//��ǰ0����Դ���������1��ͬʱ����  
	InitializeCriticalSection(&g_csThreadCode);  

	HANDLE  handle[THREAD_NUM];   
	g_nNum = 0;  
	int i = 0;  
	while (i < THREAD_NUM)   
	{  
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);  
		WaitForSingleObject(g_hThreadParameter, INFINITE);//�ȴ��ź���>0  
		++i;  
	}  
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);  

	//�����ź����͹ؼ���  
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
	ReleaseSemaphore(g_hThreadParameter, 1, NULL);//�ź���++  

	Sleep(50);//some work should to do  

	EnterCriticalSection(&g_csThreadCode);  
	++g_nNum;  
	Sleep(0);//some work should to do  
	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum);  
	LeaveCriticalSection(&g_csThreadCode);  
	return 0;  
}

