// BiTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std ;

struct BiNode 
{
	char data ;
	BiNode *lchild , *rchild ;
} ;
BiNode *BiTree ;

int NodeID ;

BiNode *CreateBiTree (char *c , int n)
{
	BiNode *T ;
	NodeID ++ ;
	if (NodeID > n)
	{
		return (NULL) ;
	}
	if (c[NodeID] == '#')
	{
		return (NULL) ;
	}
	T = new BiNode ;
	T -> data = c[NodeID] ;
	T -> lchild = CreateBiTree (c , n) ;
	T -> rchild = CreateBiTree (c , n) ;
	return (T) ;
}

void PreOrderTraverse (BiNode *T)
{
	if (T)
	{
		cout << T -> data << " ";
		PreOrderTraverse (T -> lchild) ;
		PreOrderTraverse (T -> rchild) ;
	}
}

void InOrderTraverse (BiNode *T)
{
	if (T)
	{
		InOrderTraverse (T -> lchild) ;
		cout << T -> data << " ";
		InOrderTraverse (T -> rchild) ;
	}
}

void PostOrderTraverse (BiNode *T)
{
	if (T)
	{
		PostOrderTraverse (T -> lchild) ;
		PostOrderTraverse (T -> rchild) ;
		cout << T -> data << " ";
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	int i , SampleNum ;
	char c[100] ;
	cout<<"Please input a node count: ";
	cin >> SampleNum ;
	for (i = 1 ; i <= SampleNum ; i ++)
	{
		cout<<"Please input a node name: ";
		cin >> c[i] ;
	}
	NodeID = 0 ;
	BiTree = CreateBiTree (c , SampleNum) ;
	cout<<"PreOrderTraverse: ";
	PreOrderTraverse (BiTree) ;
	cout << endl ;
	cout<<"InOrderTraverse: ";
	InOrderTraverse (BiTree) ;
	cout << endl ;
	cout<<"PostOrderTraverse: ";
	PostOrderTraverse (BiTree) ;

	system("pause");
	return 0;
}

