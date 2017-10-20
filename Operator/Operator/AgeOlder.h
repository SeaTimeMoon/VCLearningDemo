#pragma once
#include "Person.h"
class CAgeOlder
{
public:
	CAgeOlder(void);
	CAgeOlder(int age);
	bool operator()(const CPerson &person);
	~CAgeOlder(void);

private:
	int m_nAge;
};
