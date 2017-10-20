#pragma once
#include "afxdtctl.h"


// CTimeSpanDlg dialog

class CTimeSpanDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeSpanDlg)

public:
	CTimeSpanDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTimeSpanDlg();

// Dialog Data
	enum { IDD = IDD_TimeSpanDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_timeSpanCtrl;
	COleDateTime m_timeProcess;
	CString m_strTimeFile;//INI文件显示时间
	CString m_strTimeGrid;//表格显示时间

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
