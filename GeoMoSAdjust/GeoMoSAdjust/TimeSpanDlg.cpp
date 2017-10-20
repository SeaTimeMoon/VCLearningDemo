// TimeSpanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeoMoSAdjust.h"
#include "TimeSpanDlg.h"


// CTimeSpanDlg dialog

IMPLEMENT_DYNAMIC(CTimeSpanDlg, CDialog)

CTimeSpanDlg::CTimeSpanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSpanDlg::IDD, pParent)
	, m_timeProcess(2000,1,1,0,0,0)
{

}

CTimeSpanDlg::~CTimeSpanDlg()
{
}

void CTimeSpanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_timeSpanCtrl);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_timeProcess);
}


BEGIN_MESSAGE_MAP(CTimeSpanDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTimeSpanDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTimeSpanDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTimeSpanDlg message handlers

void CTimeSpanDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strTimeGrid=m_timeProcess.Format(_T("%H:%M:%S"));
	m_strTimeFile=m_timeProcess.Format(_T("%H%M%S"));
	OnOK();
}

void CTimeSpanDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
