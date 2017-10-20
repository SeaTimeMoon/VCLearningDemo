// ProducerAndConsumer2.cpp : Defines the entry point for the console application.
//

//1生产者 2消费者 4缓冲区  

#include "stdafx.h"
#include <process.h>  
#include <windows.h>

//设置控制台输出颜色  
BOOL SetConsoleColor(WORD wAttributes)  
{  
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  
	if (hConsole == INVALID_HANDLE_VALUE)  
		return FALSE;  

	return SetConsoleTextAttribute(hConsole, wAttributes);  
}  
const int END_PRODUCE_NUMBER = 8;  //生产产品个数  
const int BUFFER_SIZE = 4;          //缓冲区个数  
int g_Buffer[BUFFER_SIZE];          //缓冲池  
int g_i, g_j;  
//信号量与关键段  
CRITICAL_SECTION g_cs;  
HANDLE g_hSemaphoreBufferEmpty, g_hSemaphoreBufferFull;  
//生产者线程函数  
unsigned int __stdcall ProducerThreadFun(PVOID pM)  
{  
	for (int i = 1; i <= END_PRODUCE_NUMBER; i++)  
	{  
		//等待有空的缓冲区出现  
		WaitForSingleObject(g_hSemaphoreBufferEmpty, INFINITE);  

		//互斥的访问缓冲区  
		EnterCriticalSection(&g_cs);  
		g_Buffer[g_i] = i;  
		printf("生产者在缓冲池第%d个缓冲区中投放数据%d\n", g_i, g_Buffer[g_i]);  
		g_i = (g_i + 1) % BUFFER_SIZE;  
		LeaveCriticalSection(&g_cs);  

		//通知消费者有新数据了  
		ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);  
	}  
	printf("生产者完成任务，线程结束运行\n");  
	return 0;  
}  
//消费者线程函数  
unsigned int __stdcall ConsumerThreadFun(PVOID pM)  
{  
	while (true)  
	{  
		//等待非空的缓冲区出现  
		WaitForSingleObject(g_hSemaphoreBufferFull, INFINITE);  

		//互斥的访问缓冲区  
		EnterCriticalSection(&g_cs);  
		SetConsoleColor(FOREGROUND_GREEN);  
		printf("  编号为%d的消费者从缓冲池中第%d个缓冲区取出数据%d\n", GetCurrentThreadId(), g_j, g_Buffer[g_j]);  
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  
		if (g_Buffer[g_j] == END_PRODUCE_NUMBER)//结束标志  
		{  
			LeaveCriticalSection(&g_cs);  
			//通知其它消费者有新数据了(结束标志)  
			ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);  
			break;  
		}  
		g_j = (g_j + 1) % BUFFER_SIZE;  
		LeaveCriticalSection(&g_cs);  

		Sleep(50); //some other work to do  

		ReleaseSemaphore(g_hSemaphoreBufferEmpty, 1, NULL);  
	}  
	SetConsoleColor(FOREGROUND_GREEN);  
	printf("  编号为%d的消费者收到通知，线程结束运行\n", GetCurrentThreadId());  
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  
	return 0;  
}  


int _tmain(int argc, _TCHAR* argv[])
{
	printf("  生产者消费者问题   1生产者 2消费者 4缓冲区\n");  
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");  

	InitializeCriticalSection(&g_cs);  
	//初始化信号量,一个记录有产品的缓冲区个数,另一个记录空缓冲区个数.  
	g_hSemaphoreBufferEmpty = CreateSemaphore(NULL, 4, 4, NULL);  
	g_hSemaphoreBufferFull  = CreateSemaphore(NULL, 0, 4, NULL);  
	g_i = 0;  
	g_j = 0;  
	memset(g_Buffer, 0, sizeof(g_Buffer));  

	const int THREADNUM = 3;  
	HANDLE hThread[THREADNUM];  
	//生产者线程  
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ProducerThreadFun, NULL, 0, NULL);  
	//消费者线程  
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, ConsumerThreadFun, NULL, 0, NULL);  
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, ConsumerThreadFun, NULL, 0, NULL);  
	WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);  
	for (int i = 0; i < THREADNUM; i++)  
		CloseHandle(hThread[i]);  

	//销毁信号量和关键段  
	CloseHandle(g_hSemaphoreBufferEmpty);  
	CloseHandle(g_hSemaphoreBufferFull);  
	DeleteCriticalSection(&g_cs); 

	system("pause");

	return 0;
}

