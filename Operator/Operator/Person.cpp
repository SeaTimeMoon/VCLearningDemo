
#include "StdAfx.h"
#include "Person.h"

CPerson::CPerson(void):m_strName("")
						,m_nAge(0)
{
}

CPerson::CPerson( string name,int age )
{
	m_strName=name;
	m_nAge=age;

}

CPerson::~CPerson(void)
{
}


int CPerson::operator-( const CPerson &person )
{
	return (this->m_nAge-person.m_nAge);

}

bool CPerson::operator==( const CPerson &person )
{
	if (this->m_nAge==person.m_nAge)
	{
		return true;
	} 
	else
	{
		return false;
	}

}

bool CPerson::operator!=( const CPerson &person )
{
	if (this==&person)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool CPerson::operator<( const CPerson &person )
{
	if (this->m_nAge<person.m_nAge)
	{
		return true;
	} 
	else
	{
		return false;
	}

}

bool CPerson::operator>( const CPerson &person )
{
	if ((*this<person)||(*this==person))
	{
		return false;
	}
	else
	{
		return true;
	}

}
