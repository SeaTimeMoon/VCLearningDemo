#pragma once
#include "ListBoxST.h"
#include "DBCfgDlg.h"
#include "DlgMonitor.h"

#include "GeoAdjustStruct.h"


//主界面列表控件
class CCustomListBox :
	public CListBoxST
{
public:
	CCustomListBox(void);
	~CCustomListBox(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

public:
	void DBConfigure(); //数据库设置对话框
	void MonitorConfigure(); //监控设置

public:
	int m_nProjectSelCount;//选择的项目数量

	CDBCfgDlg m_oDBdlg;
	CDlgMonitor m_oMonitorDlg;

	vector<GeoPojectStruct>m_vecGeoProject;//project文件集
};
