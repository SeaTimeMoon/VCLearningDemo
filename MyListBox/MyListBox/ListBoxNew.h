#pragma once
#include "ListBoxST.h"

class CListBoxNew :
	public CListBoxST
{
public:
	CListBoxNew(void);
	~CListBoxNew(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
