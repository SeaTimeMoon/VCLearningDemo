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

	//////////////////////����������////////////////////////////////////////////////////	
	//���ᣨʱ���ᣩ����
	CChartDateTimeAxis* pBottomAxis=ChartCtrl.CreateDateTimeAxis(CChartCtrl::BottomAxis);
	COleDateTime Min(2008,1,1,0,0,0);
	COleDateTime Max(2008,12,1,0,0,0);
	pBottomAxis->SetMinMax(Min,Max);//����ʱ�������ʾʱ�䷶Χ
	pBottomAxis->SetTickIncrement(false,tiMonth,1); //ÿ���̶ȴ���1����
	pBottomAxis->SetTickLabelFormat(false,_T("%m-%d"));//��ʾ�̶ȸ�ʽΪ����-�ա�
	pBottomAxis->SaveAxisInfoBeforeZoom();//����Ŵ�ǰ��������Ϣ
	//���ᣨ�����ᣩ����
	CChartStandardAxis* pLeftAxis=ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(0,100);
	pLeftAxis->GetLabel()->SetText(_T("����/dollar"));
	pLeftAxis->SaveAxisInfoBeforeZoom();


	/////////////////////Configure the legend�����������///////////////////////////////////	
	ChartCtrl.GetLegend()->SetVisible(true);
	ChartCtrl.GetLegend()->SetHorizontalMode(false);//���߱��Ϊ��������
	CRect rect;   
	GetDlgItem(IDC_MyChart1)->GetWindowRect(&rect);
	//ChartCtrl.GetLegend()->UndockLegend(rect.Width()-100,0); //���߱�ǵ�λ��
	ChartCtrl.GetLegend()->DockLegend(CChartLegend::dsDockTop);

	
	///////////////////////���ñ���//////////////////////////////////
	//Add next to the title and set the font&color
	ChartCtrl.GetTitle()->AddString(_T("Income over 2008"));//��������
	CChartFont titleFont;
	titleFont.SetFont(_T("Arial Black"),120,true,false,true);//��������
	ChartCtrl.GetTitle()->SetFont(titleFont);
	ChartCtrl.GetTitle()->SetColor(RGB(0,0,128));
	//����ɫ����Ϊ��ɫ
	ChartCtrl.SetBackGradient(RGB(255,255,255),RGB(255,255,255),gtVertical);


	////////////////////��������//////////////////////////////////////
	//Create three line series and three point series and populate them with data
	//�����������ߣ�ÿ������ʵ����һ��line Serie��һ��Point Serie��϶���
	CChartLineSerie* pLSeries1=ChartCtrl.CreateLineSerie();//��line serie
	CChartPointsSerie* pPtSeries1=ChartCtrl.CreatePointsSerie();//����point serie
	CChartLineSerie* pLSeries2=ChartCtrl.CreateLineSerie();
	CChartPointsSerie* pPtSeries2=ChartCtrl.CreatePointsSerie();
	CChartLineSerie* pLSeries3=ChartCtrl.CreateLineSerie();
	CChartPointsSerie* pPtSeries3=ChartCtrl.CreatePointsSerie();

	//�����������
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
	//���һ����ӿյ�����ʾ�⻬���ߣ�������=�����ڶ�����ĺ����꣬������=�����ڶ������������	
	pLSeries1->AddPoint(TimeVal,LaptopVal);
	pPtSeries1->AddPoint(TimeVal,LaptopVal);
	pLSeries2->AddPoint(TimeVal,DesktopVal);
	pPtSeries2->AddPoint(TimeVal,DesktopVal);
	pLSeries3->AddPoint(TimeVal,Income);
	pPtSeries3->AddPoint(TimeVal,Income);

	//����Ϊƽ������
	pLSeries1->SetSmooth(true);
	pLSeries2->SetSmooth(true);
	pLSeries3->SetSmooth(true);		

	//���õ��ߵ���ʽ�͵�Ĵ�С
	pPtSeries1->SetPointType(CChartPointsSerie::ptRectangle);//����
	pPtSeries1->SetPointSize(8,8);
	pPtSeries2->SetPointType(CChartPointsSerie::ptEllipse);//Բ��
	pPtSeries2->SetPointSize(8,8);
	pPtSeries3->SetPointType(CChartPointsSerie::ptTriangle);//������
	pPtSeries3->SetPointSize(8,8);
	
	//�������ߡ�Desktops��
	TChartString szlegend=_T("Desktops");//�������
	CString legendtmp=_T("Desktops");
	COLORREF clrDesktops=RGB(255,0,0);//������ɫ
	pLSeries1->SetColor(clrDesktops);
	pLSeries1->SetName(szlegend);
	pLSeries1->SetWidth(4);//�������	
	//checkbox���֡�������ɫ���Ƿ�ѡ����
	m_check1.SetWindowText(legendtmp);
	m_check1.SetLineColor(clrDesktops);
	m_check1.SetCheck(TRUE);

	//�������ߡ�Laptops��
	szlegend=_T("Laptops");
	legendtmp=_T("Laptops");
	COLORREF clrLaptops=RGB(68,68,255);
	pLSeries2->SetColor(clrLaptops);
	pLSeries2->SetName(szlegend);
	pLSeries2->SetWidth(4);
	m_check2.SetWindowText(legendtmp);
	m_check2.SetLineColor(clrLaptops);
	m_check2.SetCheck(TRUE);
	
	//�������ߡ�Total income��
	szlegend=_T("Total income");
	legendtmp=_T("Total income");
	COLORREF clrIncome=RGB(0,180,0);
	pLSeries3->SetColor(clrIncome);
	pLSeries3->SetName(szlegend);
	pLSeries3->SetWidth(4);	
	m_check3.SetWindowText(legendtmp);
	m_check3.SetLineColor(clrIncome);
	m_check3.SetCheck(TRUE);

	//�����߽��м���
	//******һ��Ҫ��point serie��line serieͬʱ���м�����
	//������ܳ����������ȥû��balloon label�����*******
	m_pMouseListener=new CCustomMouseListener();
	pLSeries1->EnableMouseNotifications(false,true);
	pLSeries1->RegisterMouseListener(m_pMouseListener);
	pPtSeries1->EnableMouseNotifications(false,true); //��������ƶ�����ֹ�����
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

	CString filter=_T("λͼ�ļ�|*.bmp|JPEG ͼ���ļ�|*.jpg|GIF ͼ���ļ�|*.gif|PNG ͼ���ļ�|*.png||"); 
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
