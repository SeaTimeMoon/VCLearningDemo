#pragma once
#include <string>

using namespace std;

class CPerson
{
public:
	CPerson(void);
	CPerson(string name,int age);
	~CPerson(void);
public:
	virtual bool operator ==(const CPerson &person);
	virtual bool operator !=(const CPerson &person);
	virtual bool operator <(const CPerson &person);
	virtual bool operator >(const CPerson &person);
	virtual int operator -(const CPerson &person);
public:
	string m_strName;
	int m_nAge;
};
