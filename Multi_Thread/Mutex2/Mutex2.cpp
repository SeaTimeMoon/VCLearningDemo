// Mutex2.cpp : Defines the entry point for the console application.
//

//Ҫ������Mutex1������Mutex2

#include "stdafx.h"
#include <conio.h>  
#include <windows.h>


const TCHAR MUTEX_NAME[] = _T("Mutex_MoreWindows"); 
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_NAME); //�򿪻�����  
	if (hMutex == NULL)  
	{  
		printf("�򿪻�����ʧ��\n");  
		return 0;  
	}  
	printf("�ȴ���....\n");  
	DWORD dwResult = WaitForSingleObject(hMutex, 20 * 1000); //�ȴ�������������  
	switch (dwResult)  
	{  
	case WAIT_ABANDONED:  
		printf("ӵ�л������Ľ���������ֹ\n");  
		break;  

	case WAIT_OBJECT_0:  
		printf("�Ѿ��յ��ź�\n");  
		break;  

	case WAIT_TIMEOUT:  
		printf("�ź�δ�ڹ涨��ʱ�����͵�\n");  
		break;  
	}  
	CloseHandle(hMutex); 

	system("pause");
	return 0;
}

