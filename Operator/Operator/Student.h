#pragma once
#include "person.h"

class CStudent :
	public CPerson
{
public:
	CStudent(void);
	~CStudent(void);

	/*virtual bool operator ==(const CPerson &person);
	virtual bool operator !=(const CPerson &person);*/

public:
	int m_nScores;
};
