// GridControlDlg.h : header file
//

#pragma once

#include "GridCtrl.h" 
// CGridControlDlg dialog
class CGridControlDlg : public CDialog
{
// Construction
public:
	CGridControlDlg(CWnd* pParent = NULL);	// standard constructor

	CGridCtrl m_pGrid; //Grid控件
	

// Dialog Data
	enum { IDD = IDD_GRIDCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void GridCtrlInit(); //初始化Grid控件


// Implementation
protected:
	HICON m_hIcon;
	CImageList m_ImageList;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
