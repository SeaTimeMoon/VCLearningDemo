// ProducerAndConsumer2.cpp : Defines the entry point for the console application.
//

//1������ 2������ 4������  

#include "stdafx.h"
#include <process.h>  
#include <windows.h>

//���ÿ���̨�����ɫ  
BOOL SetConsoleColor(WORD wAttributes)  
{  
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  
	if (hConsole == INVALID_HANDLE_VALUE)  
		return FALSE;  

	return SetConsoleTextAttribute(hConsole, wAttributes);  
}  
const int END_PRODUCE_NUMBER = 8;  //������Ʒ����  
const int BUFFER_SIZE = 4;          //����������  
int g_Buffer[BUFFER_SIZE];          //�����  
int g_i, g_j;  
//�ź�����ؼ���  
CRITICAL_SECTION g_cs;  
HANDLE g_hSemaphoreBufferEmpty, g_hSemaphoreBufferFull;  
//�������̺߳���  
unsigned int __stdcall ProducerThreadFun(PVOID pM)  
{  
	for (int i = 1; i <= END_PRODUCE_NUMBER; i++)  
	{  
		//�ȴ��пյĻ���������  
		WaitForSingleObject(g_hSemaphoreBufferEmpty, INFINITE);  

		//����ķ��ʻ�����  
		EnterCriticalSection(&g_cs);  
		g_Buffer[g_i] = i;  
		printf("�������ڻ���ص�%d����������Ͷ������%d\n", g_i, g_Buffer[g_i]);  
		g_i = (g_i + 1) % BUFFER_SIZE;  
		LeaveCriticalSection(&g_cs);  

		//֪ͨ����������������  
		ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);  
	}  
	printf("��������������߳̽�������\n");  
	return 0;  
}  
//�������̺߳���  
unsigned int __stdcall ConsumerThreadFun(PVOID pM)  
{  
	while (true)  
	{  
		//�ȴ��ǿյĻ���������  
		WaitForSingleObject(g_hSemaphoreBufferFull, INFINITE);  

		//����ķ��ʻ�����  
		EnterCriticalSection(&g_cs);  
		SetConsoleColor(FOREGROUND_GREEN);  
		printf("  ���Ϊ%d�������ߴӻ�����е�%d��������ȡ������%d\n", GetCurrentThreadId(), g_j, g_Buffer[g_j]);  
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  
		if (g_Buffer[g_j] == END_PRODUCE_NUMBER)//������־  
		{  
			LeaveCriticalSection(&g_cs);  
			//֪ͨ��������������������(������־)  
			ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);  
			break;  
		}  
		g_j = (g_j + 1) % BUFFER_SIZE;  
		LeaveCriticalSection(&g_cs);  

		Sleep(50); //some other work to do  

		ReleaseSemaphore(g_hSemaphoreBufferEmpty, 1, NULL);  
	}  
	SetConsoleColor(FOREGROUND_GREEN);  
	printf("  ���Ϊ%d���������յ�֪ͨ���߳̽�������\n", GetCurrentThreadId());  
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  
	return 0;  
}  


int _tmain(int argc, _TCHAR* argv[])
{
	printf("  ����������������   1������ 2������ 4������\n");  
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");  

	InitializeCriticalSection(&g_cs);  
	//��ʼ���ź���,һ����¼�в�Ʒ�Ļ���������,��һ����¼�ջ���������.  
	g_hSemaphoreBufferEmpty = CreateSemaphore(NULL, 4, 4, NULL);  
	g_hSemaphoreBufferFull  = CreateSemaphore(NULL, 0, 4, NULL);  
	g_i = 0;  
	g_j = 0;  
	memset(g_Buffer, 0, sizeof(g_Buffer));  

	const int THREADNUM = 3;  
	HANDLE hThread[THREADNUM];  
	//�������߳�  
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ProducerThreadFun, NULL, 0, NULL);  
	//�������߳�  
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, ConsumerThreadFun, NULL, 0, NULL);  
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, ConsumerThreadFun, NULL, 0, NULL);  
	WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);  
	for (int i = 0; i < THREADNUM; i++)  
		CloseHandle(hThread[i]);  

	//�����ź����͹ؼ���  
	CloseHandle(g_hSemaphoreBufferEmpty);  
	CloseHandle(g_hSemaphoreBufferFull);  
	DeleteCriticalSection(&g_cs); 

	system("pause");

	return 0;
}

