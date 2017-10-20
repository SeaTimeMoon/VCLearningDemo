#pragma once
#include "resource.h"
#include "afxcmn.h"
#include <afxsock.h>
//数据库设置对话框
// CDBCfgDlg dialog

class CDBCfgDlg : public CDialog
{
	DECLARE_DYNAMIC(CDBCfgDlg)

public:
	CDBCfgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDBCfgDlg();

// Dialog Data
	enum { IDD = IDD_DlgDBCfg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void GetLocalIPAddress(CIPAddressCtrl &ipCtrl); //获取本机地址

public:
	CString m_strDBName;
	CString m_strUsrName;
	CString m_strPassword;
	CIPAddressCtrl m_IPAddressCtrl;
	DWORD m_DwIpAddress;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtntestconnect();
	afx_msg void OnBnClickedOk();
	CString GetIpAddress();
};
