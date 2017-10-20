#pragma once
#include "afxdtctl.h"


// CDateTimeDlg dialog

class CDateTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CDateTimeDlg)

public:
	CDateTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDateTimeDlg();

// Dialog Data
	enum { IDD = IDD_DateTimeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_dateTimeSel;
	CDateTimeCtrl m_dateTimeCtrl;

	CString m_strTimeFile;//INI�ļ���ʾʱ��
	CString m_strTimeGrid;//�����ʾʱ��
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
