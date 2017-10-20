// MyListBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyListBox.h"
#include "MyListBoxDlg.h"

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


// CMyListBoxDlg dialog




CMyListBoxDlg::CMyListBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyListBoxDlg::IDD, pParent)
{
	/*m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);*/
	m_hIcon = AfxGetApp()->LoadIcon(IDI_Config);

}

void CMyListBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyListBoxDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_SHOWTASK,OnShowTask)//消息映射
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMyListBoxDlg message handlers

BOOL CMyListBoxDlg::OnInitDialog()
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
	InitListBoxST();
	ZoomInTaskTray();



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyListBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyListBoxDlg::OnPaint()
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
HCURSOR CMyListBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CMyListBoxDlg::OnShowTask( WPARAM wParam,LPARAM lParam )
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
		break;

	}

	return 0;



}

void CMyListBoxDlg::InitListBoxST()
{
	m_ListBox.SubclassDlgItem(IDC_LIST1,this);

	HICON hIcon=NULL;
	m_ImageList.Create(16,16,ILC_COLOR32|ILC_MASK,6,1);
	hIcon=AfxGetApp()->LoadIcon(IDI_Page01);
	m_ImageList.Add(hIcon);
	hIcon=AfxGetApp()->LoadIcon(IDI_Page02);
	m_ImageList.Add(hIcon);
	hIcon=AfxGetApp()->LoadIcon(IDI_Page03);
	m_ImageList.Add(hIcon);
	hIcon=AfxGetApp()->LoadIcon(IDI_Page04);
	m_ImageList.Add(hIcon);
	hIcon=AfxGetApp()->LoadIcon(IDI_Page05);
	m_ImageList.Add(hIcon);
	hIcon=AfxGetApp()->LoadIcon(IDI_Page06);
	m_ImageList.Add(hIcon);
	hIcon=AfxGetApp()->LoadIcon(IDI_Page07);
	m_ImageList.Add(hIcon);
	hIcon=AfxGetApp()->LoadIcon(IDI_Page08);
	m_ImageList.Add(hIcon);

	m_ListBox.SetImageList(&m_ImageList);

	m_ListBox.AddString(_T("Java is one of the most popular programming languages in the world"),0);
	m_ListBox.AddString(_T("C++ is a classic,powerful programming language"),1);
	m_ListBox.AddString(_T("C# is an object-oriented,high-level programming language"),2);
	m_ListBox.AddString(_T("Python is a programming language that lets you work quickly and integrate systems more effectively"),3);
	m_ListBox.AddString(_T("Delphi is famous Windows platform of rapid application development tools"),4);
	m_ListBox.AddString(_T("VB is a Visual programming language"),5);
	m_ListBox.AddString(_T("Objective-C is a programming language for iOS platform"),6);
	m_ListBox.AddString(_T("Swift is a programming language for iOS platform"),7);

	m_ListBox.EnableToolTips(TRUE);

}

void CMyListBoxDlg::ZoomInTaskTray()
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDI_Config;
	m_nid.uFlags = NIF_ICON|NIF_MESSAGE |NIF_TIP;
	m_nid.uCallbackMessage = WM_SHOWTASK;
	m_nid.hIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1));
	wcscpy_s(m_nid.szTip,wcslen(_T("对话框"))+1,_T("对话框")); //提示信息
	Shell_NotifyIcon(NIM_ADD,&m_nid); // 在托盘区添加图标

}



void CMyListBoxDlg::DeleteTray()
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);

}

void CMyListBoxDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED)
	{

		ShowWindow(SW_HIDE);//隐藏最小化到任务栏的图标

	}

	// TODO: Add your message handler code here
}

void CMyListBoxDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	DeleteTray();
}
