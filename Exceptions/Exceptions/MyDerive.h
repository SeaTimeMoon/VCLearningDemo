#pragma once
#include "mybase.h"

class CMyDerive :
	public CMyBase
{
public:
	CMyDerive(string msg);
	virtual ~CMyDerive(void);

	virtual void show();
};
