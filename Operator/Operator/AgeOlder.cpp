#include "StdAfx.h"
#include "AgeOlder.h"

CAgeOlder::CAgeOlder( void )
{

}

CAgeOlder::CAgeOlder(int age)
{
	m_nAge=age;
}

CAgeOlder::~CAgeOlder(void)
{
}

bool CAgeOlder::operator()( const CPerson &person )
{
	if (person.m_nAge>m_nAge)
	{
		return true;
	} 
	else
	{
		return false;
	}

}


