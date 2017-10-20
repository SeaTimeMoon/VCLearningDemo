// ProducerAndConsumer.cpp : Defines the entry point for the console application.
//

//1生产者 1消费者 1缓冲区  
//使用二个事件，一个表示缓冲区空，一个表示缓冲区满。  
//再使用一个关键段来控制缓冲区的访问 

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
const int END_PRODUCE_NUMBER = 10;   //生产产品个数  
int g_Buffer;                        //缓冲区  
//事件与关键段  
CRITICAL_SECTION g_cs;  
HANDLE g_hEventBufferEmpty, g_hEventBufferFull;  
//生产者线程函数  
unsigned int __stdcall ProducerThreadFun(PVOID pM)  
{  
	for (int i = 1; i <= END_PRODUCE_NUMBER; i++)  
	{  
		//等待缓冲区为空  
		WaitForSingleObject(g_hEventBufferEmpty, INFINITE);  

		//互斥的访问缓冲区  
		EnterCriticalSection(&g_cs);  
		g_Buffer = i;  
		printf("生产者将数据%d放入缓冲区\n", i);  
		LeaveCriticalSection(&g_cs);  

		//通知缓冲区有新数据了  
		SetEvent(g_hEventBufferFull);  
	}  
	return 0;  
}  
//消费者线程函数  
unsigned int __stdcall ConsumerThreadFun(PVOID pM)  
{  
	volatile bool flag = true;  
	while (flag)  
	{  
		//等待缓冲区中有数据  
		WaitForSingleObject(g_hEventBufferFull, INFINITE);  

		//互斥的访问缓冲区  
		EnterCriticalSection(&g_cs);  
		SetConsoleColor(FOREGROUND_GREEN);  
		printf("  消费者从缓冲区中取数据%d\n", g_Buffer);  
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  
		if (g_Buffer == END_PRODUCE_NUMBER)  
			flag = false;  
		LeaveCriticalSection(&g_cs);  

		//通知缓冲区已为空  
		SetEvent(g_hEventBufferEmpty);  

		Sleep(10); //some other work should to do  
	}  
	return 0;  
}  
int _tmain(int argc, _TCHAR* argv[])
{
	printf("  生产者消费者问题   1生产者 1消费者 1缓冲区\n");  
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");  

	InitializeCriticalSection(&g_cs);  
	//创建二个自动复位事件，一个表示缓冲区是否为空，另一个表示缓冲区是否已经处理  
	g_hEventBufferEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);  
	g_hEventBufferFull = CreateEvent(NULL, FALSE, FALSE, NULL);  

	const int THREADNUM = 2;  
	HANDLE hThread[THREADNUM];  

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ProducerThreadFun, NULL, 0, NULL);  
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, ConsumerThreadFun, NULL, 0, NULL);  
	WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);  
	CloseHandle(hThread[0]);  
	CloseHandle(hThread[1]);  

	//销毁事件和关键段  
	CloseHandle(g_hEventBufferEmpty);  
	CloseHandle(g_hEventBufferFull);  
	DeleteCriticalSection(&g_cs); 


	system("pause");
	return 0;
}

