// Basic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>  
#include <windows.h> 

int g_nCount;
//���̺߳���  
unsigned int __stdcall ThreadFun(PVOID pM)  
{  
	g_nCount++;  
	printf("�߳�ID��Ϊ%4d�����̱߳���%d\n", GetCurrentThreadId(), g_nCount);  
	return 0;  
} 

//����������ν��������ʵ�������߳�ִ�еĺ�����
int _tmain(int argc, _TCHAR* argv[])
{
	printf("     ���̱߳��� \n");  
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");  

	const int THREAD_NUM = 10;  
	HANDLE handle[THREAD_NUM];  

	g_nCount = 0;  
	for (int i = 0; i < THREAD_NUM; i++)  
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);  
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);

	system("pause");	
	return 0;
}

