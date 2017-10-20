// ChartExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartExample.h"
#include "ChartExampleDlg.h"
#include <string>

using namespace std;

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


// CChartExampleDlg dialog


CChartExampleDlg::CChartExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartExampleDlg::IDD, pParent),
	m_pMouseListener(NULL),
	m_bIsSerieVisible(TRUE)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChartExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MyChart1, m_ChartCtrl);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
}

BEGIN_MESSAGE_MAP(CChartExampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONSaveToBmp, &CChartExampleDlg::OnBnClickedButtonsavetobmp)
	ON_BN_CLICKED(IDC_CHECK1, &CChartExampleDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CChartExampleDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CChartExampleDlg::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CChartExampleDlg message handlers

BOOL CChartExampleDlg::OnInitDialog()
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
	InitCtrl(m_ChartCtrl);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChartExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChartExampleDlg::OnPaint()
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
HCURSOR CChartExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChartExampleDlg::InitCtrl( CChartCtrl &ChartCtrl )
{
	srand((unsigned int)time(NULL));
	//Disable the refresh
	ChartCtrl.EnableRefresh(false);

	//////////////////////设置坐标轴////////////////////////////////////////////////////	
	//横轴（时间轴）设置
	CChartDateTimeAxis* pBottomAxis=ChartCtrl.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	COleDateTime Min(2008,1,1,0,0,0);
	COleDateTime Max(2008,12,1,0,0,0);
	pBottomAxis->SetMinMax(Min,Max);//设置时间轴的显示时间范围
	pBottomAxis->SetTickIncrement(false,tiMonth,1); //每个刻度代表1个月
	pBottomAxis->SetTickLabelFormat(false,_T("%m-%d"));//显示刻度格式为“月-日”
	pBottomAxis->SaveAxisInfoBeforeZoom();//保存放大前的坐标信息
	//纵轴（数字轴）设置
	CChartStandardAxis* pLeftAxis=ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(0,100);
	pLeftAxis->GetLabel()->SetText(_T("收入/dollar"));
	pLeftAxis->SaveAxisInfoBeforeZoom();


	/////////////////////Configure the legend设置线条标记///////////////////////////////////	
	ChartCtrl.GetLegend()->SetVisible(true);
	ChartCtrl.GetLegend()->SetHorizontalMode(false);//曲线标记为纵向排列
	CRect rect;   
	GetDlgItem(IDC_MyChart1)->GetWindowRect(&rect);
	//ChartCtrl.GetLegend()->UndockLegend(rect.Width()-100,0); //曲线标记的位置
	ChartCtrl.GetLegend()->DockLegend(CChartLegend::dsDockTop);

	
	///////////////////////设置标题//////////////////////////////////
	//Add next to the title and set the font&color
	ChartCtrl.GetTitle()->AddString(_T("Income over 2008"));//标题名称
	CChartFont titleFont;
	titleFont.SetFont(_T("Arial Black"),120,true,false,true);//字体设置
	ChartCtrl.GetTitle()->SetFont(titleFont);
	ChartCtrl.GetTitle()->SetColor(RGB(0,0,128));
	//背景色设置为白色
	ChartCtrl.SetBackGradient(RGB(255,255,255),RGB(255,255,255),gtVertical);


	////////////////////设置曲线//////////////////////////////////////
	//Create three line series and three point series and populate them with data
	//创建三条曲线，每条曲线实际有一条line Serie和一条Point Serie组合而成
	CChartLineSerie* pLSeries1=ChartCtrl.CreateLineSerie();//线line serie
	CChartPointsSerie* pPtSeries1=ChartCtrl.CreatePointsSerie();//点线point serie
	CChartLineSerie* pLSeries2=ChartCtrl.CreateLineSerie();
	CChartPointsSerie* pPtSeries2=ChartCtrl.CreatePointsSerie();
	CChartLineSerie* pLSeries3=ChartCtrl.CreateLineSerie();
	CChartPointsSerie* pPtSeries3=ChartCtrl.CreatePointsSerie();

	//曲线添加数据
	int lowIndex=-1;
	int lowVal=99;
	int end=12;
	COleDateTime TimeVal;
	int DesktopVal,LaptopVal,Income;
	for (int i=1;i<=end;i++)
	{
		int dayend=30;
		switch(i)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			{
				dayend=31;
			}
			break;
		case 2:
			{
				dayend=29;
			}
			break;
		default:
			{
				dayend=30;
			}
			break;
		}
		
		for(int j=1;j<=dayend;j++)
		{
			
			TimeVal=COleDateTime(2008,i,j,6,8,0);	

			DesktopVal=20+rand()%(100-30);
			pLSeries1->AddPoint(TimeVal,DesktopVal);
			pPtSeries1->AddPoint(TimeVal,DesktopVal);

			LaptopVal=10+rand()%(80-20);
			pLSeries2->AddPoint(TimeVal,LaptopVal);
			pPtSeries2->AddPoint(TimeVal,LaptopVal);

			Income=DesktopVal+LaptopVal;
			if (Income<lowVal)
			{
				lowVal=Income;
				lowIndex=i;
			}
			pLSeries3->AddPoint(TimeVal,Income);
			pPtSeries3->AddPoint(TimeVal,Income);

		}
		
	}
	//最后一个点加空点以显示光滑曲线，横坐标=倒数第二个点的横坐标，纵坐标=倒数第二个点的纵坐标	
	pLSeries1->AddPoint(TimeVal,LaptopVal);
	pPtSeries1->AddPoint(TimeVal,LaptopVal);
	pLSeries2->AddPoint(TimeVal,DesktopVal);
	pPtSeries2->AddPoint(TimeVal,DesktopVal);
	pLSeries3->AddPoint(TimeVal,Income);
	pPtSeries3->AddPoint(TimeVal,Income);

	//设置为平滑曲线
	pLSeries1->SetSmooth(true);
	pLSeries2->SetSmooth(true);
	pLSeries3->SetSmooth(true);		

	//设置点线的形式和点的大小
	pPtSeries1->SetPointType(CChartPointsSerie::ptRectangle);//矩形
	pPtSeries1->SetPointSize(8,8);
	pPtSeries2->SetPointType(CChartPointsSerie::ptEllipse);//圆形
	pPtSeries2->SetPointSize(8,8);
	pPtSeries3->SetPointType(CChartPointsSerie::ptTriangle);//三角形
	pPtSeries3->SetPointSize(8,8);
	
	//设置曲线“Desktops”
	TChartString szlegend=_T("Desktops");//标记名称
	CString legendtmp=_T("Desktops");
	COLORREF clrDesktops=RGB(255,0,0);//线条颜色
	pLSeries1->SetColor(clrDesktops);
	pLSeries1->SetName(szlegend);
	pLSeries1->SetWidth(4);//线条宽度	
	//checkbox文字、线条颜色、是否勾选设置
	m_check1.SetWindowText(legendtmp);
	m_check1.SetLineColor(clrDesktops);
	m_check1.SetCheck(TRUE);

	//设置曲线“Laptops”
	szlegend=_T("Laptops");
	legendtmp=_T("Laptops");
	COLORREF clrLaptops=RGB(68,68,255);
	pLSeries2->SetColor(clrLaptops);
	pLSeries2->SetName(szlegend);
	pLSeries2->SetWidth(4);
	m_check2.SetWindowText(legendtmp);
	m_check2.SetLineColor(clrLaptops);
	m_check2.SetCheck(TRUE);
	
	//设置曲线“Total income”
	szlegend=_T("Total income");
	legendtmp=_T("Total income");
	COLORREF clrIncome=RGB(0,180,0);
	pLSeries3->SetColor(clrIncome);
	pLSeries3->SetName(szlegend);
	pLSeries3->SetWidth(4);	
	m_check3.SetWindowText(legendtmp);
	m_check3.SetLineColor(clrIncome);
	m_check3.SetCheck(TRUE);

	//对曲线进行监听
	//******一定要对point serie和line serie同时进行监听，
	//否则可能出现鼠标移上去没有balloon label的情况*******
	m_pMouseListener=new CCustomMouseListener();
	pLSeries1->EnableMouseNotifications(false,true);
	pLSeries1->RegisterMouseListener(m_pMouseListener);
	pPtSeries1->EnableMouseNotifications(false,true); //允许鼠标移动，禁止鼠标点击
	pPtSeries1->RegisterMouseListener(m_pMouseListener);//Desktop;

	pLSeries2->EnableMouseNotifications(false,true);
	pLSeries2->RegisterMouseListener(m_pMouseListener);
	pPtSeries2->EnableMouseNotifications(false,true); //
	pPtSeries2->RegisterMouseListener(m_pMouseListener);//laptop	

	pLSeries3->EnableMouseNotifications(false,true);
	pLSeries3->RegisterMouseListener(m_pMouseListener);
	pPtSeries3->EnableMouseNotifications(false,true); //
	pPtSeries3->RegisterMouseListener(m_pMouseListener);//Income


	//Re enable the refresh
	ChartCtrl.EnableRefresh(true);

}

CChartExampleDlg::~CChartExampleDlg()
{
	if (m_pMouseListener!=NULL)
	{
		delete m_pMouseListener;
		m_pMouseListener=NULL;
	}

}


void CChartExampleDlg::OnBnClickedButtonsavetobmp()
{
	// TODO: Add your control notification handler code here

	CString filter=_T("位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||"); 
	CFileDialog fileDlg(false,_T("*.bmp"), _T("Pic"), OFN_EXPLORER|OFN_OVERWRITEPROMPT,filter);
	if (fileDlg.DoModal()==IDOK)
	{
		CRect rect;
		TChartString filepath=fileDlg.GetPathName();
		m_ChartCtrl.SaveAsImage(filepath,rect,16);

	}
	else
	{
		return;
	}
	
}


void CChartExampleDlg::OnBnClickedCheck1()
{
	static bool bIsON = FALSE;
	m_check1.SetCheck(bIsON?TRUE:FALSE);
	bIsON=!bIsON;

	// TODO: Add your control notification handler code here
	m_ChartCtrl.GetSerie(0)->SetVisible(m_check1.GetCheck());
	m_ChartCtrl.GetSerie(1)->SetVisible(m_check1.GetCheck());

}

void CChartExampleDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	static bool bIsON = FALSE;
	m_check2.SetCheck(bIsON?TRUE:FALSE);
	bIsON=!bIsON;

	// TODO: Add your control notification handler code here
	m_ChartCtrl.GetSerie(2)->SetVisible(m_check2.GetCheck());
	m_ChartCtrl.GetSerie(3)->SetVisible(m_check2.GetCheck());
}

void CChartExampleDlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	static bool bIsON = FALSE;
	m_check3.SetCheck(bIsON?TRUE:FALSE);
	bIsON=!bIsON;

	// TODO: Add your control notification handler code here
	m_ChartCtrl.GetSerie(4)->SetVisible(m_check3.GetCheck());
	m_ChartCtrl.GetSerie(5)->SetVisible(m_check3.GetCheck());
}
