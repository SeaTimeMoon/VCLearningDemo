// ChartExampleDlg.h : header file
//

#pragma once
#include "ChartCtrl\ChartCtrl.h"
#include "ChartCtrl\ChartTitle.h"
#include "ChartCtrl\ChartLineSerie.h"
#include "ChartCtrl\ChartAxisLabel.h"
#include "ChartCtrl\ChartPointsSerie.h"
#include "CustomMouseListener.h"
#include "CustomCommonMouseListener.h"
#include "afxwin.h"
#include "CCheckSK.h"


// CChartExampleDlg dialog
class CChartExampleDlg : public CDialog
{
// Construction
public:
	CChartExampleDlg(CWnd* pParent = NULL);	// standard constructor
	~CChartExampleDlg();

// Dialog Data
	enum { IDD = IDD_CHARTEXAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	
private:
	void InitCtrl(CChartCtrl &ChartCtrl);//初始化为折线图
	afx_msg void OnBnClickedButtonsavetobmp(); //保存为图像格式
	//显示或隐藏曲线
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();

	
private:
	CChartCtrl m_ChartCtrl;	
	CCustomMouseListener* m_pMouseListener;

	bool m_bIsSerieVisible;
	
public:

	CCheckSK m_check1;
	CCheckSK m_check2;
	CCheckSK m_check3;


};
