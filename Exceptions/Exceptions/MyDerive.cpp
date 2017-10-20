#include "StdAfx.h"
#include "MyDerive.h"


CMyDerive::CMyDerive( string msg ):CMyBase(msg)
{

}

CMyDerive::~CMyDerive(void)
{
}

void CMyDerive::show()
{
	cout<<"Derived class"<<m_strMsg<<endl;

}
