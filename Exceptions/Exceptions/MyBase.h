#pragma once
#include <string>

using namespace std;

class CMyBase
{
public:
	CMyBase(string msg);
	virtual ~CMyBase(void);

	virtual void show();

protected:
	string m_strMsg;
};
