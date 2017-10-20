#include "StdAfx.h"
#include "ListBoxNew.h"

CListBoxNew::CListBoxNew(void)
{
}

CListBoxNew::~CListBoxNew(void)
{
}
BEGIN_MESSAGE_MAP(CListBoxNew, CListBoxST)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CListBoxNew::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int curSel = GetCurSel();
	if (curSel<0)
	{
		curSel=0;
	}

	CString str;
	GetText(curSel, str);
	str=_T("ÄúÔÚË«»÷")+str;
	AfxMessageBox(str);
	CListBox::OnLButtonDblClk(nFlags, point);

	CListBoxST::OnLButtonDblClk(nFlags, point);
}
