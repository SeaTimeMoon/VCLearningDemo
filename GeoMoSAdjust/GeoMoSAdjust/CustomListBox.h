#pragma once
#include "ListBoxST.h"
#include "DBCfgDlg.h"
#include "DlgMonitor.h"

#include "GeoAdjustStruct.h"


//�������б�ؼ�
class CCustomListBox :
	public CListBoxST
{
public:
	CCustomListBox(void);
	~CCustomListBox(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

public:
	void DBConfigure(); //���ݿ����öԻ���
	void MonitorConfigure(); //�������

public:
	int m_nProjectSelCount;//ѡ�����Ŀ����

	CDBCfgDlg m_oDBdlg;
	CDlgMonitor m_oMonitorDlg;

	vector<GeoPojectStruct>m_vecGeoProject;//project�ļ���
};
