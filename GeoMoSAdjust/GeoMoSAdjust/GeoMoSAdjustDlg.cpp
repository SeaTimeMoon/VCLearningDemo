// GeoMoSAdjustDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeoMoSAdjust.h"
#include "GeoMoSAdjustDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGeoMoSAdjustDlg dialog




CGeoMoSAdjustDlg::CGeoMoSAdjustDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeoMoSAdjustDlg::IDD, pParent)
	, m_nFaceTypeMeas(Face_I_II)
	, m_nCoordTypeRef(Coordinate_Reference)
	, m_nCoordTypeStn(Coordinate_Setup)
	, m_nCoordTypeMeas(Coordinate_Current)
	, m_nCoordTypeScan(Coordinate_Scan)
	, m_nCurrAdjIndex(-1)
{
	/*m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);*/
	 m_hIcon = AfxGetApp()->LoadIcon(IDI_Config);
	
}

void CGeoMoSAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_BtnStart,m_btnStart);
	DDX_Control(pDX,IDC_BtnStop,m_btnStop);


}

BEGIN_MESSAGE_MAP(CGeoMoSAdjustDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BtnStart, &CGeoMoSAdjustDlg::OnBnClickedBtnstart)
	ON_BN_CLICKED(IDC_BtnStop, &CGeoMoSAdjustDlg::OnBnClickedBtnstop)
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)//显示主对话框的消息
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGeoMoSAdjustDlg message handlers

BOOL CGeoMoSAdjustDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if (FAILED(::CoInitialize(NULL)))
	{
		return FALSE;
	}

	// Failed to regasm TunnelDll
	if (RegAsmTunnelDll() == FALSE)
	{
		AfxMessageBox(_T("请先注册动态库"), MB_OK);
		SendMessage(WM_CLOSE);
	}

	SetDialogWindowPostition(ZTop);

	InitListBoxST();
	InitButtonST();

	GetDlgItem(IDC_BtnStart)->EnableWindow(TRUE);
	GetDlgItem(IDC_BtnStop)->EnableWindow(FALSE);

	ZoomInTaskTray();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGeoMoSAdjustDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGeoMoSAdjustDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGeoMoSAdjustDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CGeoMoSAdjustDlg::OnShowTask( WPARAM wParam,LPARAM lParam )
{
	if(wParam != IDI_Config) 
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP:               //右键起来时弹出菜单
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);                    // 得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();                    // 声明一个弹出式菜单
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("退出"));//Appends a new item to the end of this menu
			//与退出消息进行绑定
			//Displays a floating pop-up menu at the specified location and tracks the selection of items on the pop-up menu
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x ,lpoint->y,this); //定义弹出菜单的位置，与鼠标位置有光
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
			break;
		}
	case WM_LBUTTONDBLCLK:                            // 双击左键的处理
		ShowWindow(SW_SHOWNORMAL);         // 显示主窗口
		/*SetDialogWindowPostition(ZTopMost);*/
		break;

	}
	return 0;
}

void CGeoMoSAdjustDlg::InitListBoxST()
{
	m_ListBox.SubclassDlgItem(IDC_LIST1,this);

	HICON hIcon=NULL;
	m_ImageList.Create(32,32,ILC_COLOR32|ILC_MASK,2,1);
	hIcon=AfxGetApp()->LoadIcon(IDI_DataBase);
	m_ImageList.Add(hIcon);
	hIcon=AfxGetApp()->LoadIcon(IDI_Monitor);
	m_ImageList.Add(hIcon);

	m_ListBox.SetImageList(&m_ImageList);

	m_ListBox.AddString(_T("数据库设置"),0);
	m_ListBox.AddString(_T("监测设置"),1);
	

	m_ListBox.EnableToolTips(TRUE);

}

void CGeoMoSAdjustDlg::InitButtonST()
{
	// Calculate a color effect for hilighting the button
	short	shBtnColor = 15;
	m_btnStart.SetIcon(IDI_Start);
	m_btnStart.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnStop.SetIcon(IDI_Stop);
	m_btnStop.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);

}



void CGeoMoSAdjustDlg::SetDialogWindowPostition(DialogWindowsPos windowspos)
{
	int cx=GetSystemMetrics(SM_CXSCREEN);  
	int cy=GetSystemMetrics(SM_CYSCREEN);
	CRect rectWindow;  
	GetWindowRect(&rectWindow);
	
	if (windowspos==ZTop)
	{
		SetWindowPos(&wndTop,cx-rectWindow.Width(),cy-rectWindow.Height()*1.5,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);  
	} 
	else
	{
		//窗口始终放在z轴顶层，桌面右边偏下
		SetWindowPos(&wndTopMost,cx-rectWindow.Width(),cy-rectWindow.Height()*1.5,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);  

	}
	
}

void CGeoMoSAdjustDlg::OnBnClickedBtnstart()
{
	// TODO: Add your control notification handler code here
	if (FALSE == theApp.CheckDongleValidity())
	{
		CDialog::OnCancel();
		return;
	}

	// 读取数据库配置
	m_strIpAddress = AfxGetApp()->GetProfileString(_T("GeoMoSAdjust"), _T("IpAddress"), _T(""));
	m_strDbName	   = AfxGetApp()->GetProfileString(_T("GeoMoSAdjust"), _T("DbName"),	  _T(""));
	m_strUserName  = AfxGetApp()->GetProfileString(_T("GeoMoSAdjust"), _T("UserName"),  _T(""));
	m_strPassword  = AfxGetApp()->GetProfileString(_T("GeoMoSAdjust"), _T("Password"),  _T(""));
	if (m_strIpAddress.IsEmpty() || m_strDbName.IsEmpty() || m_strUserName.IsEmpty() || m_strPassword.IsEmpty())
	{
		AfxMessageBox(_T("请先进行GeoMoS数据库设置！"), MB_OK);
		return;
	}

	// 读取所有INI有效文件
	if (FALSE == GetAllValidIniFiles())
	{
		AfxMessageBox(_T("请先进行INI文件设置！"), MB_OK);
		return;
	}

	UpdateData(TRUE);

	CTime currTime = CTime::GetCurrentTime();
	m_timeEndAll = currTime;
	for (int i=0; i<(int)m_vecAdjustment.size(); i++)
	{
		if (m_timeEndAll < m_vecAdjustment.at(i).m_oProjectInfo.m_timeEnd)
		{
			m_timeEndAll = m_vecAdjustment.at(i).m_oProjectInfo.m_timeEnd;
		}
	}
	if (currTime >= m_timeEndAll) {
		AfxMessageBox(_T("当前时间晚于所有INI文件的结束时间！"));
		return;
	}


	UpdateStatus(_T("自动平差功能已启动!"));

	for (int i=0; i<(int)m_vecAdjustment.size(); i++)
	{
		CAdjustment adjustTmp = m_vecAdjustment.at(i);
		CString strIniFileName = adjustTmp.m_oProjectInfo.m_strFilePath;
		adjustTmp.ReadIniFile(adjustTmp.m_oProjectInfo.m_strFilePath);
		adjustTmp.WriteLogFile(CTime::GetCurrentTime(), _T("GeoMoS自动平差功能已启动 ") + strIniFileName);

		CTime timeBegin = adjustTmp.m_oProjectInfo.m_timeBegin;
		while (currTime >= timeBegin)
		{
			timeBegin += adjustTmp.m_oProjectInfo.m_timeSpan;
			m_vecAdjustment.at(i).m_oProjectInfo.m_timeBegin = timeBegin;
			if (timeBegin >= adjustTmp.m_oProjectInfo.m_timeEnd)
			{
				break;
			}
		}
	}

	SetTimer(TimerId_Adjust, Time_1_Sec, NULL);
	RefreshControls(FALSE);

	GetDlgItem(IDC_BtnStop)->EnableWindow(TRUE);
	GetDlgItem(IDC_BtnStart)->EnableWindow(FALSE);
}

void CGeoMoSAdjustDlg::OnBnClickedBtnstop()
{
	// TODO: Add your control notification handler code here
	StopAutoAdjustment(TRUE);

	GetDlgItem(IDC_BtnStop)->EnableWindow(FALSE);
	GetDlgItem(IDC_BtnStart)->EnableWindow(TRUE);
}

void CGeoMoSAdjustDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (nType == SIZE_MINIMIZED)
	{

		ShowWindow(SW_HIDE);//隐藏最小化到任务栏的图标

	}
}

void CGeoMoSAdjustDlg::ZoomInTaskTray()
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDI_Config;
	m_nid.uFlags = NIF_ICON|NIF_MESSAGE |NIF_TIP;
	m_nid.uCallbackMessage = WM_SHOWTASK;
	m_nid.hIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_Config));
	wcscpy_s(m_nid.szTip,wcslen(_T("GeoMos自动平差工具"))+1,_T("GeoMos自动平差工具")); //提示信息
	Shell_NotifyIcon(NIM_ADD,&m_nid); // 在托盘区添加图标


}

void CGeoMoSAdjustDlg::DeleteTray()
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

void CGeoMoSAdjustDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	DeleteTray();

	for (int i=0; i<(int)m_vecAdjustment.size(); i++)
	{
		CAdjustment adjustTmp = m_vecAdjustment.at(i);
		adjustTmp.WriteLogFile(CTime::GetCurrentTime(), _T("GeoMoS自动平差工具已退出"));
	}
	if(m_pConnection != NULL)
	{
		//m_pConnection->Close();  // 关闭连接
		m_pConnection.Release(); // 释放对象
	}

	::CoUninitialize();
}

void CGeoMoSAdjustDlg::StopAutoAdjustment(BOOL bAll)
{
	// TODO: 在此添加控件通知处理程序代码
	theAdjustment.WriteLogFile(CTime::GetCurrentTime(), _T("GeoMoS自动平差功能已停止"));

	if (bAll)
	{
		KillTimer(TimerId_Adjust);
		UpdateData(TRUE);
		RefreshControls(TRUE);
	}
}

void CGeoMoSAdjustDlg::UpdateStatus(CString strStatus)
{
	CString strText;
	strText.Format(_T(" %s At %s"), strStatus, theAdjustment.ConvertTime(CTime::GetCurrentTime()));

	theAdjustment.WriteLogFile(CTime::GetCurrentTime(), strStatus);
}

_RecordsetPtr& CGeoMoSAdjustDlg::GetRS(CString strSQL) 
{
	try
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open((_bstr_t)strSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch(_com_error e)
	{
		CString errormessage;
		errormessage.Format(_T("创建数据库失败!错误信息:%s"), e.ErrorMessage());
		UpdateStatus(errormessage);
	}

	return m_pRecordset;
}

void CGeoMoSAdjustDlg::RefreshControls(BOOL bEnabled)
{
	m_btnStart.EnableWindow(bEnabled);
	m_btnStop.EnableWindow(!bEnabled);
}


UINT ProcThreadAdjustment(LPVOID pParam)
{
	CGeoMoSAdjustDlg *pMainDialog = (CGeoMoSAdjustDlg*)pParam;

	pMainDialog->EnableWindow(FALSE);
	pMainDialog->StartAutoAdjustment();
	pMainDialog->EnableWindow(TRUE);

	return 0;
}

void CGeoMoSAdjustDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TimerId_Adjust)
	{
		CTime currTime = CTime::GetCurrentTime();
		for (int i=0; i<(int)m_vecAdjustment.size(); i++)
		{
			CAdjustment adjustTmp = m_vecAdjustment.at(i);
			if (FALSE == adjustTmp.m_bAdjStopped)
			{
				if (currTime >= adjustTmp.m_oProjectInfo.m_timeEnd)
				{
					m_nCurrAdjIndex = i;
					theAdjustment = adjustTmp;
					m_vecAdjustment.at(i).m_bAdjStopped = TRUE;
					UpdateStatus(_T("自动平差功能已停止!"));
					StopAutoAdjustment(currTime >= m_timeEndAll);
				}
				else if (currTime >= adjustTmp.m_oProjectInfo.m_timeBegin)
				{
					m_nCurrAdjIndex = i;
					theAdjustment = adjustTmp;
					EnableWindow(FALSE);
					StartAutoAdjustment();
					EnableWindow(TRUE);
					// use a new thread for adjustment
					//::AfxBeginThread(ProcThreadAdjustment, this);			
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void CGeoMoSAdjustDlg::StartAutoAdjustment()
{
	KillTimer(TimerId_Adjust);
	//////////////////////////////////////////////////////////////////////////
	// start to adjustment
	BeginWaitCursor(); 
	theAdjustment.m_nAdjustTimes ++ ;
	CString strText;
	strText.Format(_T("测量周期 %d 平差"), theAdjustment.m_nAdjustTimes);
	UpdateStatus(strText + _T("进行中!"));
	theAdjustment.SetLstTestResult(_T(""));
	if (StartAdjustment())
		strText.Append(_T("成功"));
	else	
		strText.Append(_T("失败"));
	if (theAdjustment.GetLstTestResult().IsEmpty())
	{
		strText.Append(_T("\nWarning: 未成功获取有效数据:访问数据库失败或者调用StarNet失败(无软件狗),未生成lst文件"));
	}
	else
	{
		strText.AppendFormat(_T("\n%s"), theAdjustment.GetLstTestResult().TrimLeft());
	}
	theAdjustment.WriteLogFile(CTime::GetCurrentTime(), strText);
	CString strStatus;
	strStatus.Format(_T(" 测量周期 %d 平差完成!"), theAdjustment.m_nAdjustTimes);
	UpdateStatus(strStatus);
	Sleep(5000);
	EndWaitCursor(); // end adjustment
	//////////////////////////////////////////////////////////////////////////
	theAdjustment.m_oProjectInfo.m_timeBegin += theAdjustment.m_oProjectInfo.m_timeSpan;
	if (m_nCurrAdjIndex >= 0 && m_nCurrAdjIndex < (int)m_vecAdjustment.size())
	{
		m_vecAdjustment.at(m_nCurrAdjIndex) = theAdjustment;
	}
	SetTimer(TimerId_Adjust, Time_1_Sec, NULL);
}

BOOL CGeoMoSAdjustDlg::StartAdjustment()
{
	// 1. connect to geomos database
	HRESULT hr = m_pConnection.CreateInstance("ADODB.Connection");
	CString strCn = _T("driver={SQL Server};Server=") + m_strIpAddress + _T(";DATABASE=") + m_strDbName + 
		_T(";UID=") + m_strUserName + _T(";PWD=") + m_strPassword;
	_variant_t bcnstr  = _variant_t(strCn);
	_variant_t bunstr  = _variant_t(m_strUserName);
	_variant_t bpwdstr = _variant_t(m_strPassword);
	try
	{
		m_pConnection->Open(_bstr_t(bcnstr), _bstr_t(bunstr), _bstr_t(bpwdstr), adModeUnknown);
	}
	catch(_com_error e)
	{
		CString errormessage;
		errormessage.Format(_T("Warning: 连接数据库发生异常. 错误信息: %s; 行: %d\n"), e.ErrorMessage(), __LINE__);
		UpdateStatus(errormessage);
		return FALSE;
	}
	catch(...)
	{
		UpdateStatus(_T("Warning: 连接数据库时发生未知错误"));
		return FALSE;
	}

	BOOL bSucceed = FALSE;
	// 2. read ini file
	theAdjustment.Reset();
	if (theAdjustment.ReadIniFile(theAdjustment.m_oProjectInfo.m_strFilePath))
	{
		// 3. get ref and stn coordinates
		if (GetCoordinates())
		{
			// 4. get tps measurement
			CTime currTime = theAdjustment.m_oProjectInfo.m_timeBegin;
			////////////////////////////////////////////////////////////////////////////
			//if (m_checkTest.GetCheck()) // tested by xra
			//{
			//	currTime = CTime(2014,7,28,18,10,3); // tested by xra
			//	//currTime = CTime(2014,9,10,18,10,3); // tested by xra
			//}
			////////////////////////////////////////////////////////////////////////////
			if (GetMeasurementValid(currTime))
			{
				if (theAdjustment.IsStarNetUsed())
				{
					// 5. write dat file
					if (theAdjustment.WriteDatFile(currTime))
					{
						// 6. write bat file and call starnet for adjustment
						if (theAdjustment.WriteBatFile(currTime))
						{
							ShellExecute(NULL, _T("open"), theAdjustment.GetBatFileName(), NULL, NULL, SW_HIDE);
							// 7. read lst file
							BOOL bReadLst = FALSE;
							for (int i=0; i<15; i++)
							{
								Sleep(1000);
								bReadLst = theAdjustment.ReadLstFile();
								if (bReadLst)
								{
									// 8. update coordinates 
									if (theAdjustment.IsTestLowerBound())
									{
										if (theAdjustment.IsUpdateLowerBound())
										{
											bSucceed = SetCoordinates();
										}
									}
									else
									{
										bSucceed = SetCoordinates();
									}
									break;
								}
							}
							if (!bReadLst)
							{
								CString strErrFileName = theAdjustment.GetRptFileName(theAdjustment.GetErrFileName(), currTime);
								MoveFile(theAdjustment.GetErrFileName(), strErrFileName);
							}
						}
					}
				}
				////////////////////////////////////////////////////////////////////////////
				// write TunnelDll dat file and call TunnelDll for adjustment
				if (theAdjustment.WriteTunnelDllDatFile(currTime))
				{
					_ComClstunnelPtr ptrTunnelAdj = NULL;
					HRESULT hr = ptrTunnelAdj.CreateInstance(__uuidof(ComClstunnel));
					if (SUCCEEDED(hr)) 
					{
						BeginWaitCursor();
						CString strDataFile = theAdjustment.GetRptFileName(theAdjustment.GetDatFileName(), currTime);
						CString strFlnIn  = strDataFile + _T(".in");
						CString strFlnOut = strDataFile + _T(".out");
						long long Ier = 0;
						VARIANT_BOOL b_retVal = VARIANT_FALSE;
						BSTR bstrIn  = strFlnIn.AllocSysString();
						BSTR bstrOut = strFlnOut.AllocSysString();
						ptrTunnelAdj->TunnelDll(bstrIn, bstrOut, CalcPassword(), &Ier, &b_retVal);
						SysFreeString(bstrIn);
						SysFreeString(bstrOut);
						//AfxMessageBox(theAdjustment.GetIerErrorMessage((int)Ier));
						if (theAdjustment.GetLstTestResult().IsEmpty())
						{
							theAdjustment.WriteLogFile(CTime::GetCurrentTime(), _T("\nAdj:") + theAdjustment.GetIerErrorMessage((int)Ier));
						}
						else
						{
							theAdjustment.SetLstTestResult(theAdjustment.GetLstTestResult() + _T("\nAdj:") + theAdjustment.GetIerErrorMessage((int)Ier));
						}
						if (Ier == Ier_OK)
						{
							if (theAdjustment.ReadTunnelDllOutFile(strFlnOut))
							{
								if (theAdjustment.IsStarNetUsed())
								{
									theAdjustment.WriteTwoResultsForTest(strDataFile + _T(".txt"));
								}
								else
								{
									if (theAdjustment.ConvertTunnelDllOutResult())
									{
										bSucceed = SetCoordinates();
									}
								}
							}
						}
						EndWaitCursor();
					}
					////////////////////////////////////////////////////////////////////////////
				}
			}
		}
	}

	// 9. close geomose database connection
	if(m_pConnection != NULL)
	{
		m_pConnection->Close();  // 关闭连接
	}

	return bSucceed;
}

BOOL CGeoMoSAdjustDlg::GetCoordinates()
{
	if (GetCoordinates(theAdjustment.GetPointRef(), m_nCoordTypeRef))
	{
		(void)GetCoordinates(theAdjustment.GetPointMeas(), m_nCoordTypeMeas);
		(void)GetCoordinates(theAdjustment.GetPointScan(), m_nCoordTypeScan);
		return GetCoordinates(theAdjustment.GetPointStn(), m_nCoordTypeStn);
	}

	return FALSE;
}

BOOL CGeoMoSAdjustDlg::GetCoordinates(POINT_3DList * pList, short coordType)
{
	for (int i=0; i<(int)pList->size(); i++)
	{
		int idPoint = GetIdByNameDesc(_T("Points"), pList->at(i).Name);
		if (idPoint == 0)
			break;
		CString strSqlCmd = SQL_Select_Coordinates(idPoint, coordType);

		m_pRecordset = GetRS(strSqlCmd);
		try
		{
			if (!m_pRecordset->adoEOF)
			{
				CString strTemp;
				_variant_t var;
				pList->at(i).Type = coordType;

				var = m_pRecordset->GetCollect(_variant_t((long)0)); // ID
				strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
				pList->at(i).ID = _wtoi(strTemp);

				var = m_pRecordset->GetCollect(_variant_t((long)1)); // Easting
				strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
				pList->at(i).EY = _wtof(strTemp);

				var = m_pRecordset->GetCollect(_variant_t((long)2)); // Northing
				strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
				pList->at(i).NX = _wtof(strTemp);

				var = m_pRecordset->GetCollect(_variant_t((long)3)); // Height
				strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
				pList->at(i).HH = _wtof(strTemp);
			}
		}
		catch(_com_error e)	
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CGeoMoSAdjustDlg::GetMeasurementValid(CTime currTime)
{
	TpsMeasurementList * pList = theAdjustment.GetTpsMeasurement();
	for (int i=0; i<(int)pList->size(); i++)
	{
		// get point id according to point name
		int idPointFrom = GetIdByNameDesc(_T("Points"), pList->at(i).From);
		if (idPointFrom == 0)
			break;

		// get setup id according to point id
		GetSetupIdByPointIdDesc(idPointFrom, m_nCoordTypeStn, currTime, theAdjustment.GetRetrieveMin());
		if (m_veccTpsSetupId.size() == 0)
			break;
		// get measured point id according to point name
		int idPointTo = GetIdByNameDesc(_T("Points"), pList->at(i).To);
		if (idPointTo == 0)
			break;

		TpsMeasurementList vecMeasList;
		vecMeasList.clear();
		CTimeSpan timeSpan(0, 0, theAdjustment.GetRetrieveMin(), 0);
		CTime lastTime = currTime - timeSpan;

		for (int j=0; j<(int)m_veccTpsSetupId.size(); j++)
		{
			// get the last tps measurement until the time is out of back retrieve time
			CString strSqlCmd = SQL_Select_TpsMeasurents(m_veccTpsSetupId.at(j), idPointTo, m_nFaceTypeMeas);
			m_pRecordset = GetRS(strSqlCmd);
			try
			{
				while (!m_pRecordset->adoEOF)
				{
					CString strTemp;
					_variant_t var;
					TpsMeasurement measLast;
					var = m_pRecordset->GetCollect(_variant_t((long)0)); // Epoch
					strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
					measLast.Epoch = strTemp;

					var = m_pRecordset->GetCollect(_variant_t((long)1)); // HzAngle
					strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
					measLast.Hz = _wtof(strTemp);

					var = m_pRecordset->GetCollect(_variant_t((long)2)); // VAngle
					strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
					measLast.V = _wtof(strTemp);

					var = m_pRecordset->GetCollect(_variant_t((long)3)); // SlopeDistance
					strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
					measLast.SD = _wtof(strTemp);

					var = m_pRecordset->GetCollect(_variant_t((long)4)); // ReflectorConstant
					strTemp = (var.vt == VT_NULL) ? _T("") : (_bstr_t)var;
					measLast.Prism = _wtof(strTemp);

					var = m_pRecordset->GetCollect(_variant_t((long)5)); // State
					strTemp = (var.vt == VT_NULL) ? _T("0") : (_bstr_t)var;
					measLast.State = _wtoi(strTemp);

					CTime measTime = ConvertTime(measLast.Epoch);
					if (measTime > lastTime && measTime <= currTime)
					{
						vecMeasList.push_back(measLast);
					}
					else if (measTime <= lastTime)
					{
						m_pRecordset->Cancel();
						break;
					}
					m_pRecordset->MoveNext();
				}
			}
			catch(_com_error e)	
			{
				return FALSE;
			}
		}
		//
		if (vecMeasList.size() > 0)
		{
			TpsMeasurement measAverage = GetAverageMeasurement(vecMeasList);
			pList->at(i).Epoch = measAverage.Epoch;
			pList->at(i).State = measAverage.State;
			pList->at(i).Hz    = measAverage.Hz;
			pList->at(i).V     = measAverage.V;
			pList->at(i).SD    = measAverage.SD;
			pList->at(i).Prism = measAverage.Prism;
		}
	}

	return theAdjustment.CheckTpsMeasurement(currTime);
}

BOOL CGeoMoSAdjustDlg::SetCoordinates()
{
	POINT_3DList * pListResult = theAdjustment.GetPointRes();
	POINT_3DList * pListMeasStn = theAdjustment.GetPointMeas();
	for (int i=0; i<(int)theAdjustment.GetPointStn()->size(); i++)
	{
		pListMeasStn->push_back(theAdjustment.GetPointStn()->at(i));
	}
	// update reference coordinates
	(void)SetCoordinates234(pListResult, pListMeasStn,  m_nCoordTypeRef);
	// update meas and scan coordinates
	(void)SetCoordinates234(pListResult, pListMeasStn,  m_nCoordTypeMeas);
	(void)SetCoordinates234(pListResult, pListMeasStn,  m_nCoordTypeScan);
	// update setup coordinates
	return SetCoordinates(pListResult, theAdjustment.GetPointStn(),  m_nCoordTypeStn);
}

BOOL CGeoMoSAdjustDlg::SetCoordinates234(POINT_3DList * pListResult, POINT_3DList * pList, short coordType)
{
	for (int i=0; i<(int)pListResult->size(); i++)
	{
		POINT_3D pointResult = pListResult->at(i);
		for (int j=0; j<(int)pList->size(); j++)
		{
			if (pointResult.Name == pList->at(j).Name)
			{
				if (CalcDistance(pointResult, pList->at(j), 3) < MAX_DIFF_ALLOWED)
				{
					// get point id according to point name
					int idPoint = GetIdByNameDesc(_T("Points"), pList->at(j).Name);
					// update coordinates according to point_id and type
					CString strSqlCmd = SQL_Update_Coordinates234(idPoint, coordType, pointResult);
					m_pRecordset = GetRS(strSqlCmd);
					try	{
					} catch(_com_error e) {
						return FALSE;
					}
					break;
				}
			}
		}
	}

	return TRUE;
}

BOOL CGeoMoSAdjustDlg::SetCoordinates(POINT_3DList * pListResult, POINT_3DList * pList, short coordType)
{
	for (int i=0; i<(int)pListResult->size(); i++)
	{
		POINT_3D pointResult = pListResult->at(i);
		for (int j=0; j<(int)pList->size(); j++)
		{
			if (pointResult.Name == pList->at(j).Name)
			{
				if (CalcDistance(pointResult, pList->at(j), 3) < MAX_DIFF_ALLOWED)
				{
					CString strSqlCmd = SQL_Update_Coordinates(pList->at(j).ID, coordType, pointResult);
					m_pRecordset = GetRS(strSqlCmd);
					try	{
					} catch(_com_error e) {
						return FALSE;
					}
					break;
				}
			}
		}
	}

	return TRUE;
}

int CGeoMoSAdjustDlg::GetIdByNameDesc(CString tableName, CString pointName)
{
	int id = 0;
	CString strSqlCmd = SQL_Select_IdByName(tableName, pointName);
	m_pRecordset = GetRS(strSqlCmd);
	try
	{
		if (!m_pRecordset->adoEOF)
		{
			_variant_t var = m_pRecordset->GetCollect(_variant_t((long)0)); // ID	
			if (var.vt != VT_NULL)
			{
				id = _wtoi((_bstr_t)var);
			}
		}
	}
	catch(_com_error e)	
	{
		return 0;
	}

	return id;
}

void CGeoMoSAdjustDlg::GetSetupIdByPointIdDesc(int pointId, short type, CTime currTime, int nRetrieveMin)
{
	m_veccTpsSetupId.clear();
	if (nRetrieveMin > 0)
		nRetrieveMin *= -1;

	CString strSqlCmd = SQL_Select_SetupIdByPointId(pointId, type, currTime, nRetrieveMin);
	m_pRecordset = GetRS(strSqlCmd);
	try
	{
		while (!m_pRecordset->adoEOF)
		{
			_variant_t var = m_pRecordset->GetCollect(_variant_t((long)0)); // TPSSetup_ID
			if (var.vt != VT_NULL)
			{
				m_veccTpsSetupId.push_back(_wtoi((_bstr_t)var));
			}
			m_pRecordset->MoveNext();
		}
	}
	catch(_com_error e)	
	{
		return;
	}
}

CString CGeoMoSAdjustDlg::SQL_Select_IdByName(CString tableName, CString pointName)
{
	CString strFormat = _T("select ID from %s where Name=\'%s\' ORDER by Epoch DESC");
	CString strSqlCmd = _T("");
	strSqlCmd.Format(strFormat, tableName, pointName);

	return strSqlCmd;
}

CString CGeoMoSAdjustDlg::ConvertTime(CTime currTime)
{
	// 2008/8/8 8:08:08
	CString strTime;
	strTime.Format(_T("%4d/%d/%d %d:%02d:%02d"), currTime.GetYear(), currTime.GetMonth(), 
		currTime.GetDay(), currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond());

	return strTime;
}

CString CGeoMoSAdjustDlg::SQL_Select_SetupIdByPointId(int pointId, short type, CTime currTime, int nRetrieveMin)
{
	CString strTime = ConvertTime(currTime);
	CString strFormat = _T("select TPSSetup_ID from Coordinates where Point_ID=%d and Type=%d and (Epoch<\'%s\' and Epoch>DATEADD(n,%d,\'%s\')) ORDER by Epoch DESC");
	CString strSqlCmd = _T("");
	strSqlCmd.Format(strFormat, pointId, type, strTime, nRetrieveMin, strTime);

	return strSqlCmd;
}

CString CGeoMoSAdjustDlg::SQL_Select_Coordinates(int pointId, short type)
{
	CString strFormat = _T("select ID,Easting,Northing,Height from Coordinates where Point_ID=%d And Type=%d ORDER by Epoch DESC");
	CString strSqlCmd = _T("");
	strSqlCmd.Format(strFormat, pointId, type);

	return strSqlCmd;
}

CString CGeoMoSAdjustDlg::SQL_Select_TpsMeasurents(int id1, int id2, short face)
{
	CString strFormat = _T("select Epoch,HzAngle,VAngle,SlopeDistance,ReflectorConstant,State from TpsMeasurements where TpsSetup_ID=%d And Point_ID=%d And Face=%d And Type=1 ORDER by Epoch DESC");
	CString strSqlCmd = _T("");
	strSqlCmd.Format(strFormat, id1, id2, face);

	return strSqlCmd;
}

CString CGeoMoSAdjustDlg::SQL_Update_Coordinates234(int id, short type, POINT_3D point)
{	
	CString strFormat = _T("update Coordinates SET Easting=%.4f,Northing=%.4f,Height=%.4f where Point_ID=%d And Type=%d");
	CString strSqlCmd = _T("");
	strSqlCmd.Format(strFormat,  point.EY, point.NX, point.HH, id, type);

	return strSqlCmd;
}

CString CGeoMoSAdjustDlg::SQL_Update_Coordinates(int id, short type, POINT_3D point)
{	
	CString strFormat = _T("update Coordinates SET Easting=%.4f,Northing=%.4f,Height=%.4f where ID=%d And Type=%d");
	CString strSqlCmd = _T("");
	strSqlCmd.Format(strFormat,  point.EY, point.NX, point.HH, id, type);

	return strSqlCmd;
}

TpsMeasurement CGeoMoSAdjustDlg::GetAverageMeasurement(TpsMeasurementList measList)
{
	int nListCount = (int)measList.size();
	int nValidCount = 0;
	int i = 0;
	for (i=0; i<nListCount; i++)
	{
		if (measList[i].State == 1)
		{
			nValidCount ++;
		}
	}

	TpsMeasurement measAverage;
	measAverage.Hz    = 0.0;
	measAverage.V     = 0.0;
	measAverage.SD    = 0.0;
	measAverage.Prism = 0.0;

	measAverage.Epoch = measList[0].Epoch;
	if (nValidCount == 0)
	{
		measAverage.State = measList[0].State;
		measAverage.Hz    = measList[0].Hz;
		measAverage.V     = measList[0].V;
		measAverage.SD    = measList[0].SD;
		measAverage.Prism = measList[0].Prism;
	}
	else {
		measAverage.State = 1;
		for (i=0; i<nListCount; i++)
		{
			if (measList[i].State == 1)
			{
				if (i > 0)
				{
					//解决跨界数据，形如：359.5958,0.0002
					double dHzCorr = measList[i].Hz;
					if (IsHzCorrected(measAverage.Hz, measList[i].Hz, dHzCorr))
					{
						measList[i].Hz = dHzCorr;
					}
				}
				measAverage.Hz    += measList[i].Hz / nValidCount;
				measAverage.V     += measList[i].V / nValidCount;
				measAverage.SD    += measList[i].SD / nValidCount;
				measAverage.Prism += measList[i].Prism / nValidCount;
			}
		}
	}

	measAverage.Hz = theAdjustment.Rad2DMS(measAverage.Hz);
	measAverage.V  = theAdjustment.Rad2DMS(measAverage.V);

	return measAverage;
}

CTime CGeoMoSAdjustDlg::ConvertTime(CString strEpoch)
{
	//2014/8/8 8:08:08 --> 2014 8 8 8:8:8
	int nYear = 2000;
	int nMonth = 1;
	int nDay = 1;
	int nHour = 0;
	int nMin = 0;
	int nSec = 0;

	int pos = strEpoch.Find('/');
	if (pos != -1)
	{
		nYear = _wtoi(strEpoch.Left(pos));
		strEpoch = strEpoch.Mid(pos + 1);
		pos = strEpoch.Find('/');
		if (pos != -1)
		{
			nMonth = _wtoi(strEpoch.Left(pos));
			strEpoch = strEpoch.Mid(pos + 1);
			pos = strEpoch.Find(' ');
			if (pos != -1)
			{
				nDay = _wtoi(strEpoch.Left(pos));
				strEpoch = strEpoch.Mid(pos + 1);
				pos = strEpoch.Find(':');
				if (pos != -1)
				{
					nHour = _wtoi(strEpoch.Left(pos));
					strEpoch = strEpoch.Mid(pos + 1);
					pos = strEpoch.Find(':');
					if (pos != -1)
					{
						nMin = _wtoi(strEpoch.Left(pos));
						strEpoch = strEpoch.Mid(pos + 1);
						strEpoch.TrimRight();
						nSec = _wtoi(strEpoch);
					}
				}
			}
		}
	}

	return CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
}

__int64 CGeoMoSAdjustDlg::CalcPassword()
{
	CTime oTime = CTime::GetCurrentTime();
	int psswd = oTime.GetYear() + oTime.GetMonth() + oTime.GetDay();

	return (__int64)psswd;
}

BOOL CGeoMoSAdjustDlg::IsHzCorrected(double dHzTrue, double dHzMeas, double &dHzCorr)
{
	double diff = fabs(dHzMeas - dHzTrue);
	if (diff > 1.5*PI && diff <= 2.0*PI)
	{
		if (dHzMeas > dHzTrue) // nearby 360
		{
			dHzCorr = dHzMeas - 2.0*PI;
		}
		else // nearby 0
		{
			dHzCorr = dHzMeas + 2.0*PI;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CGeoMoSAdjustDlg::RegAsmTunnelDll()
{
	_ComClstunnelPtr ptrTunnelAdj = NULL;
	HRESULT hr = ptrTunnelAdj.CreateInstance(__uuidof(ComClstunnel));
	if (FAILED(hr)) // never RegAsm
	{
		// RegAsm TunnelDll
		CString strPath = GetCurrentPath();
		CString strFile = _T("RegAsm.bat");
		if (WriteRegAsmBatFile(strPath, strFile))
		{
			// run bat file
			ShellExecute(NULL, _T("open"), strPath + strFile, NULL, NULL, SW_HIDE);
			// sleep 
			Sleep(1500);
		}

		// create again after RegAsm
		if (FAILED(ptrTunnelAdj.CreateInstance(__uuidof(ComClstunnel)))) 
		{
			return FALSE;
		}
	}

	return TRUE;
}

CString CGeoMoSAdjustDlg::GetCurrentPath()
{
	CString strCurrPath; 
	GetModuleFileName(NULL, strCurrPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strCurrPath.ReleaseBuffer(); 

	strCurrPath = strCurrPath.Left(strCurrPath.ReverseFind( '\\')+1);

	return strCurrPath;
}

BOOL CGeoMoSAdjustDlg::WriteRegAsmBatFile(CString strPath, CString strFileName)
{
	CStdioFile mFile;  
	if (mFile.Open(strPath + strFileName, CFile::modeCreate|CFile::modeWrite))
	{
		mFile.WriteString(_T("@echo off\n"));
		// register
		CString strTemp;
		strTemp.Format(_T("\"%s%s\" %s\n"), strPath, _T("RegAsm.exe"), _T("/codebase TunnelDll.dll /tlb:TunnelDll.tlb"));
		mFile.WriteString(strTemp);

		// delete bat file
		strTemp.Format(_T("del \"%s%s\"\n"), strPath, strFileName);
		mFile.WriteString(strTemp);

		mFile.Close();
		return TRUE;
	}

	return FALSE;
}

double CGeoMoSAdjustDlg::CalcDistance(POINT_3D oPoint1, POINT_3D oPoint2, int nSize)
{
	double ddX = oPoint2.NX - oPoint1.NX;
	double ddY = oPoint2.EY - oPoint1.EY;
	double ddZ = oPoint2.HH - oPoint1.HH;

	if (nSize > 3) 
		nSize = 3;

	if (nSize == 1)
		return ddZ;
	else if (nSize == 2)
		return sqrt(ddX * ddX + ddY * ddY);
	else if (nSize == 3)
		return sqrt(ddX * ddX + ddY * ddY + ddZ * ddZ);

	return 0.0;
}

BOOL CGeoMoSAdjustDlg::GetAllValidIniFiles()
{
	m_vecAdjustment.clear();

	CFileFind finder;
	CString filePath =_T("INI\\*.ini"); // 和程序文件同级的INI目录
	BOOL bFindSuc = finder.FindFile(filePath);
	while (bFindSuc)
	{
		bFindSuc = finder.FindNextFile();
		CString fileName = finder.GetFilePath();

		CAdjustment adjustTmp;
		if (IsValidIniFile(fileName, adjustTmp.m_oProjectInfo))
		{
			adjustTmp.m_nAdjustTimes = 0;
			m_vecAdjustment.push_back(adjustTmp);
		}
	}

	return ((int)m_vecAdjustment.size() > 0);
}

BOOL CGeoMoSAdjustDlg::IsValidIniFile(CString filePath, ProjectInfo &projectInfo)
{
	BOOL bValid = FALSE;

	CStdioFile mFile;
	if (mFile.Open(filePath, CFile::modeRead))
	{
		CString strTemp;
		BOOL bReadSucc = mFile.ReadString(strTemp);
		CString strItemText;
		if (bReadSucc && (strTemp.Find(IsSelectedKey) != -1))
		{
			strTemp.MakeUpper();//全部转换为大写
			strItemText = strTemp.Mid(strTemp.Find('=') + 1);
			if (strItemText.Find(_T("YES")) != -1)
			{
				projectInfo.m_strFilePath = filePath;
				while (bReadSucc)
				{
					if (strTemp.Find(_T("Start Time=")) != -1) //开始时间
					{
						strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
						if (strItemText.GetLength() >= 14)
						{
							projectInfo.m_timeBegin = ConvertToTime(strItemText);
						}
					}
					else if (strTemp.Find(_T("Interval Time=")) != -1) //处理时间
					{
						strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
						if (strItemText.GetLength() >= 6)
						{
							projectInfo.m_timeSpan = ConvertToTimeSpan(strItemText);
						}
					}
					else if (strTemp.Find(_T("End Time=")) != -1) //结束时间
					{
						strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
						if (strItemText.GetLength() >= 14)
						{
							projectInfo.m_timeEnd = ConvertToTime(strItemText);
							bValid = (projectInfo.m_timeBegin <= projectInfo.m_timeEnd);
						}
						break;
					}
					bReadSucc = mFile.ReadString(strTemp);
				}
			}		
		}
		mFile.Close();
	}

	return bValid;
}

CTime CGeoMoSAdjustDlg::ConvertToTime(CString strText)
{
	if (strText.GetLength() >= 14)
	{
		int nYear  = _wtoi(strText.Mid(0,  4));
		int nMonth = _wtoi(strText.Mid(4,  2));
		int nDay   = _wtoi(strText.Mid(6,  2));
		int nHour  = _wtoi(strText.Mid(8,  2));
		int nMin   = _wtoi(strText.Mid(10, 2));
		int nSec   = _wtoi(strText.Mid(12, 2));

		return CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
	}

	return CTime();
}

CTimeSpan CGeoMoSAdjustDlg::ConvertToTimeSpan(CString strText)
{
	if (strText.GetLength() >= 6)
	{
		int nHour = _wtoi(strText.Mid(0, 2));
		int nMin  = _wtoi(strText.Mid(2, 2));
		int nSec  = _wtoi(strText.Mid(4, 2));

		return CTimeSpan(0, nHour, nMin, nSec);
	}

	return CTimeSpan();
}