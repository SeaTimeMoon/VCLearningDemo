// ProducerAndConsumer.cpp : Defines the entry point for the console application.
//

//1������ 1������ 1������  
//ʹ�ö����¼���һ����ʾ�������գ�һ����ʾ����������  
//��ʹ��һ���ؼ��������ƻ������ķ��� 

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
const int END_PRODUCE_NUMBER = 10;   //������Ʒ����  
int g_Buffer;                        //������  
//�¼���ؼ���  
CRITICAL_SECTION g_cs;  
HANDLE g_hEventBufferEmpty, g_hEventBufferFull;  
//�������̺߳���  
unsigned int __stdcall ProducerThreadFun(PVOID pM)  
{  
	for (int i = 1; i <= END_PRODUCE_NUMBER; i++)  
	{  
		//�ȴ�������Ϊ��  
		WaitForSingleObject(g_hEventBufferEmpty, INFINITE);  

		//����ķ��ʻ�����  
		EnterCriticalSection(&g_cs);  
		g_Buffer = i;  
		printf("�����߽�����%d���뻺����\n", i);  
		LeaveCriticalSection(&g_cs);  

		//֪ͨ����������������  
		SetEvent(g_hEventBufferFull);  
	}  
	return 0;  
}  
//�������̺߳���  
unsigned int __stdcall ConsumerThreadFun(PVOID pM)  
{  
	volatile bool flag = true;  
	while (flag)  
	{  
		//�ȴ���������������  
		WaitForSingleObject(g_hEventBufferFull, INFINITE);  

		//����ķ��ʻ�����  
		EnterCriticalSection(&g_cs);  
		SetConsoleColor(FOREGROUND_GREEN);  
		printf("  �����ߴӻ�������ȡ����%d\n", g_Buffer);  
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  
		if (g_Buffer == END_PRODUCE_NUMBER)  
			flag = false;  
		LeaveCriticalSection(&g_cs);  

		//֪ͨ��������Ϊ��  
		SetEvent(g_hEventBufferEmpty);  

		Sleep(10); //some other work should to do  
	}  
	return 0;  
}  
int _tmain(int argc, _TCHAR* argv[])
{
	printf("  ����������������   1������ 1������ 1������\n");  
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");  

	InitializeCriticalSection(&g_cs);  
	//���������Զ���λ�¼���һ����ʾ�������Ƿ�Ϊ�գ���һ����ʾ�������Ƿ��Ѿ�����  
	g_hEventBufferEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);  
	g_hEventBufferFull = CreateEvent(NULL, FALSE, FALSE, NULL);  

	const int THREADNUM = 2;  
	HANDLE hThread[THREADNUM];  

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ProducerThreadFun, NULL, 0, NULL);  
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, ConsumerThreadFun, NULL, 0, NULL);  
	WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);  
	CloseHandle(hThread[0]);  
	CloseHandle(hThread[1]);  

	//�����¼��͹ؼ���  
	CloseHandle(g_hEventBufferEmpty);  
	CloseHandle(g_hEventBufferFull);  
	DeleteCriticalSection(&g_cs); 


	system("pause");
	return 0;
}

