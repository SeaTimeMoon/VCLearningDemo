#include "StdAfx.h"
#include "CustomListBox.h"

CCustomListBox::CCustomListBox(void)
{
	m_nProjectSelCount=1;
}

CCustomListBox::~CCustomListBox(void)
{
}
BEGIN_MESSAGE_MAP(CCustomListBox, CListBoxST)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CCustomListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int curSel = GetCurSel();
	if (curSel<0)
	{
		curSel=0;
	}
	switch (curSel)
	{
	case 0:
		DBConfigure();
		break;
	case 1:
		MonitorConfigure();
		break;
	default:
		break;
	}

	CListBoxST::OnLButtonDblClk(nFlags, point);
}

void CCustomListBox::DBConfigure()
{
	m_oDBdlg.DoModal();

}

void CCustomListBox::MonitorConfigure()
{	
	CDlgMonitor monitorDlg;
	monitorDlg.DoModal();
		
}
