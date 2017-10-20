#include "StdAfx.h"
#include "MyBase.h"


CMyBase::CMyBase( string msg )
{
	m_strMsg=msg;

}

CMyBase::~CMyBase(void)
{
}

void CMyBase::show()
{
	cout<<"Base class:"<<m_strMsg<<endl;

}
