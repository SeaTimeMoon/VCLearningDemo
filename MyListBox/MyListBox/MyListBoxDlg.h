// MyListBoxDlg.h : header file
//

#pragma once
#include "ListBoxNew.h"


#define WM_SHOWTASK WM_USER+100 //�Զ�����Ϣ��������Ϣ


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
	
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam); //��Ϣӳ�亯������ʾ����

	DECLARE_MESSAGE_MAP()

public:
	void InitListBoxST();//��ʼ��ListBox

public:
	CListBoxNew m_ListBox;
	CImageList m_ImageList;

protected:
	NOTIFYICONDATA m_nid;

protected:
	void ZoomInTaskTray();//��С������
	void DeleteTray();//��������ͼ��

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};
