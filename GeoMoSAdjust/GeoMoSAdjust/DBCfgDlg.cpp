// DBCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeoMoSAdjust.h"
#include "DBCfgDlg.h"


// CDBCfgDlg dialog

IMPLEMENT_DYNAMIC(CDBCfgDlg, CDialog)

CDBCfgDlg::CDBCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBCfgDlg::IDD, pParent)
	, m_strDBName(_T(""))
	, m_strUsrName(_T("geomos_admin2"))
	, m_strPassword(_T("geomos_admin_2006"))
	, m_DwIpAddress(0)
{
	/*m_IPAddressCtrl.setA*/
}

CDBCfgDlg::~CDBCfgDlg()
{
}

void CDBCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_editDBName, m_strDBName);
	DDX_Text(pDX, IDC_editUserName, m_strUsrName);
	DDX_Text(pDX, IDC_editPassword, m_strPassword);
	DDX_Control(pDX, IDC_IPADRS, m_IPAddressCtrl);
	DDX_IPAddress(pDX, IDC_IPADRS, m_DwIpAddress);
}




BEGIN_MESSAGE_MAP(CDBCfgDlg, CDialog)
	ON_BN_CLICKED(IDC_BtnTestConnect, &CDBCfgDlg::OnBnClickedBtntestconnect)
	ON_BN_CLICKED(IDOK, &CDBCfgDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDBCfgDlg message handlers

BOOL CDBCfgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// 读取数据库配置
	m_DwIpAddress = AfxGetApp()->GetProfileInt(_T("GeoMoSAdjust"),    _T("IpAdrInt"), 0);
	m_strDBName	  = AfxGetApp()->GetProfileString(_T("GeoMoSAdjust"), _T("DbName"),	  _T(""));
	m_strUsrName  = AfxGetApp()->GetProfileString(_T("GeoMoSAdjust"), _T("UserName"), _T("geomos_admin2"));
	m_strPassword = AfxGetApp()->GetProfileString(_T("GeoMoSAdjust"), _T("Password"), _T("geomos_admin_2006"));	
	if (m_DwIpAddress == 0)
	{
		GetLocalIPAddress(m_IPAddressCtrl);
		m_IPAddressCtrl.GetAddress(m_DwIpAddress);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDBCfgDlg::GetLocalIPAddress( CIPAddressCtrl &ipCtrl )
{
	AfxSocketInit(NULL);

	char sHostName[100];
	gethostname(sHostName,sizeof(sHostName));
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		MessageBox(_T("error"));
	}
	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
				
		DWORD  dw1=inet_addr(inet_ntoa(inAddr));
		WORD  wd1=LOWORD(dw1);//取高十六位
		WORD  wd2=HIWORD(dw1);//取低十六位
		BYTE  bte1=LOBYTE(wd1);
		BYTE  bte2=HIBYTE(wd1);
		BYTE  bte3=LOBYTE(wd2);
		BYTE  bte4=HIBYTE(wd2);

		ipCtrl.SetAddress(bte1,bte2,bte3,bte4);
		UpdateData(TRUE);
	}
}

void CDBCfgDlg::OnBnClickedBtntestconnect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strIpAddress = GetIpAddress();
	if (strIpAddress.IsEmpty() || m_strDBName.IsEmpty() || m_strUsrName.IsEmpty() || m_strPassword.IsEmpty())
	{
		AfxMessageBox(_T("请输入GeoMoS数据库相关配置！"), MB_ICONSTOP);
		return;
	}

	if (FAILED(::CoInitialize(NULL)))   
		return;

	BeginWaitCursor(); 
	_ConnectionPtr pConnection;
	_RecordsetPtr  pRecordset;
	HRESULT hr = pConnection.CreateInstance("ADODB.Connection");

	CString strCn;

	strCn = _T("driver={SQL Server};Server=") + strIpAddress + _T(";DATABASE=") + m_strDBName + 
		_T(";UID=") + m_strUsrName + _T(";PWD=") + m_strPassword;

	_variant_t bcnstr  = _variant_t(strCn);
	_variant_t bunstr  = _variant_t(m_strUsrName);
	_variant_t bpwdstr = _variant_t(m_strPassword);

	//打开一个连接
	try
	{
		pConnection->Open(_bstr_t(bcnstr), _bstr_t(bunstr), _bstr_t(bpwdstr), adModeUnknown);
	}
	catch(_com_error e)
	{
		EndWaitCursor();
		CString errormessage;
		errormessage.Format(_T("Warning: 连接数据库发生异常. 错误信息: %s; 行: %d\n"), e.ErrorMessage(), __LINE__);
		AfxMessageBox(errormessage);
		return;
	}
	catch(...)
	{
		EndWaitCursor();
		AfxMessageBox(_T("Warning: 连接数据库时发生未知错误"));
		return;
	}
	EndWaitCursor();
	AfxMessageBox(_T("GeoMoS数据库连接正常"));

	if(pConnection != NULL)
	{
		pConnection->Close();  // 关闭连接
		pConnection.Release(); // 释放对象
	}

	::CoUninitialize();
}

void CDBCfgDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strIpAddress = GetIpAddress();
	if (strIpAddress.IsEmpty() || m_strDBName.IsEmpty() || m_strUsrName.IsEmpty() || m_strPassword.IsEmpty())
	{
		AfxMessageBox(_T("请输入GeoMoS数据库相关配置！"), MB_ICONSTOP);
		return;
	}

	// 保存数据库配置
	AfxGetApp()->WriteProfileString(_T("GeoMoSAdjust"), _T("IpAddress"), strIpAddress);
	AfxGetApp()->WriteProfileInt(_T("GeoMoSAdjust"),    _T("IpAdrInt"),  m_DwIpAddress);
	AfxGetApp()->WriteProfileString(_T("GeoMoSAdjust"), _T("DbName"),	 m_strDBName);
	AfxGetApp()->WriteProfileString(_T("GeoMoSAdjust"), _T("UserName"),  m_strUsrName);
	AfxGetApp()->WriteProfileString(_T("GeoMoSAdjust"), _T("Password"),  m_strPassword);

	OnOK();
}

CString CDBCfgDlg::GetIpAddress()
{
	CString strIp = _T("");

	if (m_DwIpAddress > 0)
	{
		BYTE  nField0 = 0;
		BYTE  nField1 = 0;
		BYTE  nField2 = 0;
		BYTE  nField3 = 0;

		m_IPAddressCtrl.GetAddress(nField0, nField1, nField2, nField3);
		strIp.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
	}

	return strIp;
}