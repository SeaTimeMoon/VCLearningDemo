#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#define ERROR  0
#define OK  1

typedef enum{Link, Thread} PointerTag;      //link = 0��ʾָ�����Һ���ָ��
//Thread = 1��ʾָ��ǰ�����̵�����
typedef struct BitNode
{
	char data;                              //�������
	struct BitNode *lchild;                 //���Һ���ָ��
	struct BitNode *rchild; 
	PointerTag ltag;                        //���ұ�־
	PointerTag rtag;
}BitNode, *BiTree;

BiTree pre;                                 //ȫ�ֱ�����ʼ��ָ��ոշ��ʹ��Ľ��

//ǰ�򴴽�������
void CreateTree(BiTree *t)
{
	char ch;
	scanf("%c", &ch);

	if(ch == '#')
	{
		*t = NULL;
	}
	else
	{
		(*t) = (BiTree)malloc(sizeof(BitNode));
		if((*t) == NULL)
		{
			return;
		}
		(*t)->data = ch;
		CreateTree(&((*t)->lchild));
		CreateTree(&((*t)->rchild));
	}
}


//tָ��ͷ��㣬ͷ�������lchildָ�����㣬ͷ�������rchildָ��������������һ����㡣
//�������������������ʾ�Ķ�����t
int InOrderThraverse_Thr(BiTree t)
{
	BiTree p;
	p = t->lchild;           //pָ������
	while(p != t)
	{
		while(p->ltag == Link)   //��ltag = 0ʱѭ�����������еĵ�һ�����
		{
			p = p->lchild;
		}
		printf("%c ", p->data);  //��ʾ������ݣ����Ը���Ϊ�����Խ��Ĳ���
		while(p->rtag == Thread && p->rchild != t)
		{
			p = p->rchild;
			printf("%c ", p->data);
		}

		p = p->rchild;           //p������������
	}

	return OK;
}

//���������������������
void InThreading(BiTree p)
{
	if(p)
	{
		InThreading(p->lchild);              //�ݹ�������������
		if(!p->lchild)                       //û������
		{
			p->ltag = Thread;                //ǰ������
			p->lchild = pre;             //����ָ��ָ��ǰ��,�����ǵ�3��
		}
		if(!pre->rchild)                 //û���Һ���
		{
			pre->rtag = Thread;              //�������
			pre->rchild = p;             //ǰ���Һ���ָ��ָ����(��ǰ���p)
		}
		pre = p;

		InThreading(p->rchild);              //�ݹ�������������
	}
}
//����ͷ��㣬��������������
int InOrderThread_Head(BiTree *h, BiTree t)
{
	(*h) = (BiTree)malloc(sizeof(BitNode));
	if((*h) == NULL)
	{
		return ERROR;
	}

	(*h)->rchild = *h;
	(*h)->rtag = Link;

	if(!t)      //���ΪNULL
	{
		(*h)->lchild = *h;
		(*h)->ltag = Link;
	}
	else
	{
		pre = *h;
		(*h)->lchild = t;        //��һ��
		(*h)->ltag = Link;
		InThreading(t);         //�ҵ����һ�����
		pre->rchild = *h;        //���Ĳ�
		pre->rtag = Thread;
		(*h)->rchild = pre;      //�ڶ���
	}
	return OK;
}

int main(int argc, char **argv)
{
	BiTree t;
	BiTree temp;

	printf("������ǰ�����������չ�������������ݣ��ַ�����:\n");//��չ������
	CreateTree(&t);                 //����������
	InOrderThread_Head(&temp, t);       //����ͷ��㣬��������
	printf("������������������:\n");
	InOrderThraverse_Thr(temp);

	printf("\n");
	system("pause");
	return 0;
}