// Event.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>  
#include <windows.h>

long g_nNum;  
unsigned int __stdcall Fun(void *pPM);  
const int THREAD_NUM = 10;  
//�¼���ؼ���  
HANDLE  g_hThreadEvent;  
CRITICAL_SECTION g_csThreadCode; 

int _tmain(int argc, _TCHAR* argv[])
{
	printf("     �����߳�ͬ�� �¼�Event\n");  
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");  
	//��ʼ���¼��͹ؼ��� �Զ���λ,��ʼ�޴����������¼�  
	g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);   
	InitializeCriticalSection(&g_csThreadCode);  

	HANDLE  handle[THREAD_NUM];   
	g_nNum = 0;  
	int i = 0;  
	while (i < THREAD_NUM)   
	{  
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);  
		WaitForSingleObject(g_hThreadEvent, INFINITE); //�ȴ��¼�������  
		i++;  
	}  
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);  

	//�����¼��͹ؼ���  
	CloseHandle(g_hThreadEvent);  
	DeleteCriticalSection(&g_csThreadCode);

	system("pause");
	return 0;
}

unsigned int __stdcall Fun(void *pPM)  
{  
	int nThreadNum = *(int *)pPM;   
	SetEvent(g_hThreadEvent); //�����¼�  

	Sleep(50);//some work should to do  

	EnterCriticalSection(&g_csThreadCode);  
	g_nNum++;  
	Sleep(0);//some work should to do  
	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum);   
	LeaveCriticalSection(&g_csThreadCode);  
	return 0;  
}