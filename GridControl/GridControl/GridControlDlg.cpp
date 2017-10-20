// GridControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GridControl.h"
#include "GridControlDlg.h"

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


// CGridControlDlg dialog




CGridControlDlg::CGridControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGridControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX,IDC_MY_GRIDCTRL,m_pGrid);
}


BEGIN_MESSAGE_MAP(CGridControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CGridControlDlg message handlers

BOOL CGridControlDlg::OnInitDialog()
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
	m_ImageList.Create(MAKEINTRESOURCE(IDB_MyBITMAP), 16, 1, RGB(255,255,255));//图像宽度为16，每次只控制1个图标，图像宽度=图标数X图标宽度
	m_pGrid.SetImageList(&m_ImageList);
	GridCtrlInit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGridControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGridControlDlg::OnPaint()
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
HCURSOR CGridControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGridControlDlg::GridCtrlInit()
{
	m_pGrid.SetEditable(true);
	m_pGrid.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));//黄色背景

	m_pGrid.SetRowCount(9); //初始为9行
	m_pGrid.SetColumnCount(8); //初始化为8列
	m_pGrid.SetFixedRowCount(1); //表头为一行
	m_pGrid.SetFixedColumnCount(1); //表头为一列 

	for (int row = 0; row < m_pGrid.GetRowCount(); row++)
		for (int col = 0; col < m_pGrid.GetColumnCount(); col++)
		{ 
			//设置表格显示属性
			GV_ITEM Item; 
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			m_pGrid.SetRowHeight(row,25); //设置各行高          
			m_pGrid.SetColumnWidth(0,64); //设置0列宽 
			m_pGrid.SetColumnWidth(col,64); //设置各列宽

			if(row==0&&col==0) //第(0，0)格
			{
				Item.nFormat = DT_CENTER|DT_WORDBREAK;
				Item.strText.Format(_T("报表显示"),col);
			}
			else if (row < 1) //设置0行表头显示
			{        
				Item.nFormat = DT_CENTER|DT_WORDBREAK;
				Item.strText.Format(_T(" 项目%d"),col);
			}
			else if (col < 1) //设置0列表头显示
			{
				if(row< m_pGrid.GetRowCount())
				{
					Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
					Item.strText.Format(_T("第%d次"),row);
				}
			}
			else
			{
				Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
				Item.strText.Format(_T(""),2);
			}

			if (col < m_pGrid.GetFixedColumnCount())
			{
				/*Item.iImage = rand()%m_ImageList.GetImageCount();*/
				Item.iImage = row%9;
				Item.mask  |= GVIF_IMAGE;
				
			}

			m_pGrid.SetItem(&Item); 
		}

}


