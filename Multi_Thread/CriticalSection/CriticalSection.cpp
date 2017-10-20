// CriticalSection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>  
#include <windows.h>

long g_nNum;  
unsigned int __stdcall Fun(void *pPM);  
const int THREAD_NUM = 10;  
//�ؼ��α�������  
CRITICAL_SECTION  g_csThreadParameter, g_csThreadCode; 

int _tmain(int argc, _TCHAR* argv[])
{
	printf("     �����߳�ͬ�� �ؼ���\n");  
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");  

	//�ؼ��γ�ʼ��  
	InitializeCriticalSection(&g_csThreadParameter);  
	InitializeCriticalSection(&g_csThreadCode);  

	HANDLE  handle[THREAD_NUM];   
	g_nNum = 0;   
	int i = 0;  
	while (i < THREAD_NUM)   
	{
		//�����˶���̣߳����̻߳�û�õ�����Ĳ��������߳����޸Ĳ���
		EnterCriticalSection(&g_csThreadParameter);//�������߳���Źؼ�����  
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);  
		++i;  
	}  
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);  

	DeleteCriticalSection(&g_csThreadCode);
	DeleteCriticalSection(&g_csThreadParameter);

	system("pause");

	return 0;
}

unsigned int __stdcall Fun(void *pPM)  
{  
	int nThreadNum = *(int *)pPM;   
	LeaveCriticalSection(&g_csThreadParameter);//�뿪���߳���Źؼ�����  

	Sleep(50);//some work should to do  

	EnterCriticalSection(&g_csThreadCode);//��������̻߳�������  
	g_nNum++;  
	Sleep(0);//some work should to do  
	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", nThreadNum, g_nNum);  
	LeaveCriticalSection(&g_csThreadCode);//�뿪�����̻߳�������  
	return 0;  
}  

