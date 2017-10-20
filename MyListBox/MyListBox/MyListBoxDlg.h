// MyListBoxDlg.h : header file
//

#pragma once
#include "ListBoxNew.h"


#define WM_SHOWTASK WM_USER+100 //自定义消息，托盘消息


// CMyListBoxDlg dialog
class CMyListBoxDlg : public CDialog
{
// Construction
public:
	CMyListBoxDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MYLISTBOX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam); //消息映射函数，显示托盘

	DECLARE_MESSAGE_MAP()

public:
	void InitListBoxST();//初始化ListBox

public:
	CListBoxNew m_ListBox;
	CImageList m_ImageList;

protected:
	NOTIFYICONDATA m_nid;

protected:
	void ZoomInTaskTray();//缩小到托盘
	void DeleteTray();//销毁托盘图标

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};
