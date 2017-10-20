// DateTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeoMoSAdjust.h"
#include "DateTimeDlg.h"


// CDateTimeDlg dialog

IMPLEMENT_DYNAMIC(CDateTimeDlg, CDialog)

CDateTimeDlg::CDateTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDateTimeDlg::IDD, pParent)
	, m_dateTimeSel(2000,1,1,0,0,0)
{

}

CDateTimeDlg::~CDateTimeDlg()
{
}

void CDateTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dateTimeSel);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateTimeCtrl);
}


BEGIN_MESSAGE_MAP(CDateTimeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDateTimeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDateTimeDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDateTimeDlg message handlers

BOOL CDateTimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_dateTimeCtrl.SetFormat(_T("yyyy-MM-dd   HH:mm:ss"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDateTimeDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strTimeGrid=m_dateTimeSel.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strTimeFile=m_dateTimeSel.Format(_T("%Y%m%d%H%M%S"));
	OnOK();
}

void CDateTimeDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
