// DlgMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "GeoMoSAdjust.h"
#include "DlgMonitor.h"
#include "SelectFolderDlg.h"


// CDlgMonitor dialog

IMPLEMENT_DYNAMIC(CDlgMonitor, CDialog)

CDlgMonitor::CDlgMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMonitor::IDD, pParent)
	, m_strReportPath(_T(""))
	, m_strDataPath(_T(""))
	, m_strPrjPath(_T(""))
	, m_strStarNetPath(_T(""))
	, m_bUsingStarNet(FALSE)
	, m_nBackTime(60)
	, m_bFailConvergaeWriteToDB(FALSE)
	, m_bUpdateCoordIfLowerBound(FALSE)
	, m_nSelectedRowNum(-1)
	, m_bDuplicateName(false)
{
	
}

CDlgMonitor::~CDlgMonitor()
{

}

void CDlgMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ProjectSelect, m_GridProjectSel);
	DDX_Text(pDX, IDC_editReportPath, m_strReportPath);
	DDX_Text(pDX, IDC_editDataPath, m_strDataPath);
	DDX_Text(pDX, IDC_editProjectPath, m_strPrjPath);
	DDX_Text(pDX, IDC_editStarNetPath, m_strStarNetPath);
	DDX_Check(pDX, IDC_CheckDifference, m_bUsingStarNet);
	DDX_Text(pDX, IDC_editSeachTime, m_nBackTime);
	DDX_Check(pDX, IDC_CheckWriteDBFail, m_bFailConvergaeWriteToDB);
	DDX_Check(pDX, IDC_CheckAxisLow, m_bUpdateCoordIfLowerBound);
}




BEGIN_MESSAGE_MAP(CDlgMonitor, CDialog)
	ON_BN_CLICKED(IDC_BtnNewPrj, &CDlgMonitor::OnBnClickedBtnnewprj)
	ON_BN_CLICKED(IDC_BtnDeletePrj, &CDlgMonitor::OnBnClickedBtndeleteprj)
	ON_BN_CLICKED(IDC_BtnObserveCfg, &CDlgMonitor::OnBnClickedBtnobservecfg)
	ON_BN_CLICKED(IDC_BtnReportPathSel, &CDlgMonitor::OnBnClickedBtnreportpathsel)
	ON_BN_CLICKED(IDC_BtnDataPathSel, &CDlgMonitor::OnBnClickedBtndatapathsel)
	ON_BN_CLICKED(IDC_BtnProjectPathSel, &CDlgMonitor::OnBnClickedBtnprojectpathsel)
	ON_BN_CLICKED(IDC_BtnStarNetPathSel, &CDlgMonitor::OnBnClickedBtnstarnetpathsel)
	ON_BN_CLICKED(IDC_CheckDifference, &CDlgMonitor::OnBnClickedCheckdifference)
	ON_BN_CLICKED(IDC_BtnSave, &CDlgMonitor::OnBnClickedBtnsave)

	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_ProjectSelect, OnGridStartEdit)
	ON_NOTIFY(GVN_SELCHANGED, IDC_ProjectSelect, OnGridEndSelChange)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_ProjectSelect, OnGridEndEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_ProjectSelect, OnGridDblClick)
	ON_NOTIFY(NM_CLICK, IDC_ProjectSelect, OnGridClick)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_editReportPath, &CDlgMonitor::OnEnChangeeditreportpath)
	ON_EN_CHANGE(IDC_editDataPath, &CDlgMonitor::OnEnChangeeditdatapath)
	ON_EN_CHANGE(IDC_editProjectPath, &CDlgMonitor::OnEnChangeeditprojectpath)
	ON_EN_CHANGE(IDC_editStarNetPath, &CDlgMonitor::OnEnChangeeditstarnetpath)
	ON_EN_CHANGE(IDC_editSeachTime, &CDlgMonitor::OnEnChangeeditseachtime)
	ON_BN_CLICKED(IDC_BtnSaveAll, &CDlgMonitor::OnBnClickedBtnsaveall)
	ON_BN_CLICKED(IDC_CheckWriteDBFail, &CDlgMonitor::OnBnClickedCheckwritedbfail)
	ON_BN_CLICKED(IDC_CheckAxisLow, &CDlgMonitor::OnBnClickedCheckaxislow)
END_MESSAGE_MAP()


// CDlgMonitor message handlers

BOOL CDlgMonitor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	FindINIFiles();
	ReadINIFiles(m_vecINIFilePath,m_mapGeoProject);

	InitGridCtrl();
	m_GridProjectSel.SetHeaderSort(true);

	if (m_mapGeoProject.empty())
	{
		GetDlgItem(IDC_editDataPath)->EnableWindow(FALSE);
		GetDlgItem(IDC_editProjectPath)->EnableWindow(FALSE);
		GetDlgItem(IDC_editStarNetPath)->EnableWindow(FALSE);
		GetDlgItem(IDC_BtnDataPathSel)->EnableWindow(FALSE);
		GetDlgItem(IDC_BtnProjectPathSel)->EnableWindow(FALSE);
		GetDlgItem(IDC_BtnStarNetPathSel)->EnableWindow(FALSE);
	} 
	else
	{
		MAPGeoProject::iterator iter=m_mapGeoProject.begin();
		m_strReportPath=iter->second.strReportPath;
		m_bUsingStarNet=iter->second.bUsingStarNet;
		m_strDataPath=iter->second.strDataPath;
		m_strPrjPath=iter->second.strPrjPath;
		m_strStarNetPath=iter->second.strStarNetPath;
		m_nBackTime=iter->second.nBackTime;
		m_bFailConvergaeWriteToDB=iter->second.bFailConvergaeWriteToDB;
		m_bUpdateCoordIfLowerBound=iter->second.bUpdateCoordIfLowerBound;
		m_vecNodes=iter->second.vecGeoNodes;
		m_vecOrientation=iter->second.vecGeoOrientations;
		m_vecOffset=iter->second.vecGeoOffsets;

		SetRowSelected(m_GridProjectSel,1);
		m_nSelectedRowNum=1;//默认选择第1行
		UpdateData(FALSE);

	}
	

	SetAllCtrlState();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMonitor::OnBnClickedBtnnewprj()
{
	// TODO: Add your control notification handler code here


	m_GridProjectSel.InsertRow(_T(""));
	
	m_nProjectCount=m_GridProjectSel.GetRowCount();
    
	//1列为combox形式
	vector<CString>vecstring;
	vecstring.push_back(_T("是"));
	vecstring.push_back(_T("否"));
	SetComboBoxCell(m_GridProjectSel,1,vecstring);
	//3,4,5列为ReadOnly
	SetReadOnlyCell(m_GridProjectSel,3);
	SetReadOnlyCell(m_GridProjectSel,4);
	SetReadOnlyCell(m_GridProjectSel,5);
	//0列设置为序号
	SetFirstColumnText(m_GridProjectSel);
	
	SetDefaultProject(m_nSelectedRowNum); //设置默认的数据

	int selectIndex=m_mapGeoProject.size();
	CString projectName=m_GridProjectSel.GetItemText(selectIndex,2);
	GeoPojectStruct selGeoPrj=m_mapGeoProject[projectName];//选中的项目

	//刷新数据格式和路径
	Refresh(selGeoPrj);

	SetRowSelected(m_GridProjectSel,selectIndex);
	m_nSelectedRowNum=selectIndex;//选中行
	UpdateData(FALSE);

	SetAllCtrlState();
	m_GridProjectSel.Refresh();
	
}

void CDlgMonitor::OnBnClickedBtndeleteprj()
{
	// TODO: Add your control notification handler code here
	int rowCount=m_GridProjectSel.GetRowCount();//行数
	int selRowNum=m_nSelectedRowNum;//选中的行

	if (rowCount>1)
	{
		CString projectName=m_GridProjectSel.GetItemText(selRowNum,2);
		m_GridProjectSel.DeleteRow(selRowNum);
		m_mapGeoProject.erase(projectName);

		selRowNum=selRowNum-1;
		if (selRowNum!=0)
		{
			SetRowSelected(m_GridProjectSel,selRowNum);
			CString projectName=m_GridProjectSel.GetItemText(selRowNum,2);
			GeoPojectStruct geo=m_mapGeoProject[projectName];
			Refresh(geo);
			m_nSelectedRowNum=selRowNum;
		} 
		else
		{
			if (m_GridProjectSel.GetRowCount()>1)
			{
				selRowNum=1;
				SetRowSelected(m_GridProjectSel,selRowNum);
				CString projectName=m_GridProjectSel.GetItemText(selRowNum,2);
				GeoPojectStruct geo=m_mapGeoProject[projectName];
				Refresh(geo);
				m_nSelectedRowNum=selRowNum;
			}
		}
	}
	else
	{
		AfxMessageBox(_T("没有数据删除！"));
	}
	
	m_GridProjectSel.Refresh();
	m_nProjectCount=m_GridProjectSel.GetRowCount();
	//0列设置为序号
	SetFirstColumnText(m_GridProjectSel);

	SetAllCtrlState();
}

void CDlgMonitor::InitGridCtrl()
{
	m_GridProjectSel.SetEditable(true);
	m_GridProjectSel.SetTextBkColor(RGB(0xFF, 0xFF, 0xFF));//白色背景

	m_nProjectCount=m_mapGeoProject.size();
	
	m_GridProjectSel.SetRowCount(m_nProjectCount+1); //初始行
	m_GridProjectSel.SetColumnCount(6); //初始化为6列
	m_GridProjectSel.SetFixedRowCount(1); //表头为1行
	m_GridProjectSel.SetFixedColumnCount(1);//表头为1列


	//设置第一行标题和各行高、各列宽
	for (int row = 0; row < m_GridProjectSel.GetRowCount(); row++)
		for (int col = 0; col < m_GridProjectSel.GetColumnCount(); col++)
		{ 
			//设置表格显示属性
			GV_ITEM Item; 
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			m_GridProjectSel.SetRowHeight(row,25); //设置各行高
			//设置列宽
			switch(col)
			{
			case 0:
				m_GridProjectSel.SetColumnWidth(col,36);
				break;
			case 1:
				m_GridProjectSel.SetColumnWidth(col,48);
				break;
			case 2:
				m_GridProjectSel.SetColumnWidth(col,96);
				break;
			case 3:
			case 5:
				m_GridProjectSel.SetColumnWidth(col,128);
				break;
			case 4:
				m_GridProjectSel.SetColumnWidth(col,96);
				break;
			default:
				m_GridProjectSel.SetColumnWidth(col,72);
				break;
			}

			if (row<1) //设置第1行的表头
			{

				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				switch (col)
				{
				case 0:
					Item.strText.Format(_T("序号"));
					break;
				case 1:
					Item.strText.Format(_T("选用"));
					break;
				case 2:
					Item.strText.Format(_T("项目名"));
					break;
				case 3:
					Item.strText.Format(_T("开始时间"));
					break;
				case 4:
					Item.strText.Format(_T("处理时间"));
					break;
				case 5:
					Item.strText.Format(_T("结束时间"));
					break;
				}
			}
			m_GridProjectSel.SetItem(&Item);
		}


	MAPGeoProject::iterator iter=m_mapGeoProject.begin();
	int row,col;

	for (row = 1,iter=m_mapGeoProject.begin(); row < m_GridProjectSel.GetRowCount(),iter!=m_mapGeoProject.end(); row++,iter++)
		for (col = 0; col < m_GridProjectSel.GetColumnCount(); col++)
		{ 
			//设置表格显示属性
			GV_ITEM Item; 
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
						
			Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
			bool bSelected;
			CString strTime;				
			switch (col)
			{
			case 0:
				Item.strText.Format(_T(""));
				break;
			case 1:
				bSelected=iter->second.bSelectedPrj;
				if (bSelected)
				{
					Item.strText=_T("是");
				} 
				else
				{
					Item.strText=_T("否");
				}
				break;
			case 2:
				Item.strText=iter->second.strPrjName;
				break;
			case 3:
				strTime=iter->second.strStartTime;
				if (strTime.Find('-')==-1)
				{
					strTime=ConvertDateTimeFormat(strTime);
				}
				Item.strText=strTime;
				break;
			case 4:
				strTime=iter->second.strIntervalTime;
				if (strTime.Find(':')==-1)
				{
					strTime=ConvertTimeFormat(strTime);
				}
				Item.strText=strTime;
				break;
			case 5:
				strTime=iter->second.strEndTime;
				if (strTime.Find('-')==-1)
				{
					strTime=ConvertDateTimeFormat(strTime);
				}
				Item.strText=strTime;
				break;
			}

			m_GridProjectSel.SetItem(&Item);
			if (col!=0)
			{
				m_GridProjectSel.SetItemBkColour(row,col,GREEN);
			}			
			if (col==3||col==4||col==5)
			{
				m_GridProjectSel.SetItemState(row,col,GVIS_READONLY);
			}
		}
		vector<CString>vecOptionStr;
		vecOptionStr.push_back(_T("是"));
		vecOptionStr.push_back(_T("否"));
		SetComboBoxCell(m_GridProjectSel,1,vecOptionStr);

		//0列设置为序号
		SetFirstColumnText(m_GridProjectSel);

}

void CDlgMonitor::OnBnClickedBtnobservecfg()
{
	// TODO: Add your control notification handler code here
	CString projectName;
	projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	CObserveDlg ObserveDlg;
	ObserveDlg.m_strSelProjectName=projectName;
	ObserveDlg.m_vecNodes=m_vecNodes;
	ObserveDlg.m_vecOrientation=m_vecOrientation;
	ObserveDlg.m_vecOffset=m_vecOffset;
	if (IDOK==ObserveDlg.DoModal())
	{
		if (!m_vecNodes.empty())//如果非空，清空数据
		{
			vector<GeoNode>().swap(m_vecNodes);
		}
		if (!m_vecOrientation.empty())//如果非空，清空数据
		{
			vector<GeoOrientation>().swap(m_vecOrientation);
		}
		if (!m_vecOffset.empty()) //如果非空，清空数据
		{
			vector<GeoOffset>().swap(m_vecOffset);
		}

		m_vecNodes=ObserveDlg.m_vecNodes;
		m_vecOrientation=ObserveDlg.m_vecOrientation;
		m_vecOffset=ObserveDlg.m_vecOffset;

		//状态变化
		RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	}

	
}

void CDlgMonitor::OnBnClickedBtnreportpathsel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSelectFolderDlg dlg;
	dlg.m_hwndOwner=this->m_hWnd;

	m_strReportPath=dlg.Show();
	//数据更新
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].strReportPath=m_strReportPath;
	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);

	UpdateData(FALSE);

}

void CDlgMonitor::OnBnClickedBtndatapathsel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle=_T("打开DAT文件");
	fileDlg.m_ofn.lpstrFilter=_T("DAT Files(*.dat)\0*.dat\0All Files(*.*)\0*.*\0\0");

	if(IDOK==fileDlg.DoModal()) 
	{  
		m_strDataPath=fileDlg.GetPathName();
		//数据更新
		CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
		m_mapGeoProject[projectName].strDataPath=m_strDataPath;
		//状态变化
		RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);

	}


	UpdateData(FALSE);

}

void CDlgMonitor::OnBnClickedBtnprojectpathsel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle=_T("打开工程文件");
	fileDlg.m_ofn.lpstrFilter=_T("Project Files(*.prj)\0*.prj\0All Files(*.*)\0*.*\0\0");

	if(IDOK==fileDlg.DoModal()) 
	{  
		m_strPrjPath=fileDlg.GetPathName();
		//数据更新
		CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
		m_mapGeoProject[projectName].strPrjPath=m_strPrjPath;
		//状态变化
		RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);


	}

	UpdateData(FALSE);
}

void CDlgMonitor::OnBnClickedBtnstarnetpathsel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle=_T("打开应用程序");
	fileDlg.m_ofn.lpstrFilter=_T("StarNet EXE(*.exe)\0*.exe\0All Files(*.*)\0*.*\0\0");

	if(IDOK==fileDlg.DoModal()) 
	{  
		m_strStarNetPath=fileDlg.GetPathName();
		//数据更新
		CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
		m_mapGeoProject[projectName].strStarNetPath=m_strStarNetPath;
		//状态变化
		RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);

	}

	UpdateData(FALSE);
}


void CDlgMonitor::OnBnClickedCheckdifference()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//数据更新
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].bUsingStarNet=m_bUsingStarNet;

	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	
	EnableStarNetDiff(m_bUsingStarNet);

}

void CDlgMonitor::OnBnClickedCheckwritedbfail()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].bFailConvergaeWriteToDB=m_bFailConvergaeWriteToDB;

	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	UpdateData(FALSE);
}

void CDlgMonitor::OnBnClickedCheckaxislow()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].bUpdateCoordIfLowerBound=m_bUpdateCoordIfLowerBound;

	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	UpdateData(FALSE);
}

void CDlgMonitor::OnGridEndSelChange( NMHDR *pNotifyStruct, LRESULT* pResult )
{


}


void CDlgMonitor::OnBnClickedBtnsave()
{
	// TODO: Add your control notification handler code here
	if (m_nSelectedRowNum==-1)
	{
		AfxMessageBox(_T("请选择要保存的项目"));
		return ;
	}
	
	CString strName;
	strName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	if (strName==_T(""))
	{
		AfxMessageBox(_T("项目名不能为空，请输入项目名"));
		return;
	}
		
	GeoPojectStruct selectedProject;
	SaveFlag isWrite=GetSelectedGeoProject(m_nSelectedRowNum,selectedProject);//得到最新的project数据
	selectedProject.bSaveSuc=m_mapGeoProject[selectedProject.strPrjName].bSaveSuc;
	m_mapGeoProject[selectedProject.strPrjName]=selectedProject;//project数据更新
	//保存文件
	if (isWrite==SaveSuc)
	{
		//m_mapGeoProject[selectedProject.strPrjName]=selectedProject;//project数据更新
		WriteGeoPrjToFile(selectedProject,_T("###"),true);
		ClearNonexistentINIFiles();
		m_mapGeoProject[selectedProject.strPrjName].bSaveSuc=true;
	}
	ClearRowSelectdColor(m_GridProjectSel);

	
	
}

void CDlgMonitor::OnBnClickedBtnsaveall()
{
	// TODO: 在此添加控件通知处理程序代码
	CString msg;
	for(MAPGeoProject::iterator iter=m_mapGeoProject.begin();iter!=m_mapGeoProject.end();iter++)
	{
		GeoPojectStruct geo=iter->second;

		CString strStartTime=geo.strStartTime;
		CString strEndTime=geo.strEndTime;

		if (ConvertTime(strEndTime)<ConvertTime(strStartTime))
		{
			msg=_T("项目")+iter->first+MsgTimeValueErr;
			MessageBox(msg);
			return;
		}

		strStartTime.Remove('-');//删除-
		strStartTime.Remove(':');//删除:
		strStartTime.Replace(_T(" "),_T(""));//删除空格
		geo.strStartTime=strStartTime;

		strEndTime.Remove('-');//删除-
		strEndTime.Remove(':');//删除:
		strEndTime.Replace(_T(" "),_T(""));//删除空格
		geo.strEndTime=strEndTime;

		CString strIntervalTime=geo.strIntervalTime;
		strIntervalTime.Remove(':');//删除:
		geo.strIntervalTime=strIntervalTime;

		CString reportPath=geo.strReportPath;
		if (reportPath==_T(""))
		{
			msg=_T("项目")+iter->first+MsgReportErr;
			MessageBox(msg);
			return;
		}
		bool bUsingStarNet=geo.bUsingStarNet;
		if (bUsingStarNet)
		{
			if (geo.strDataPath==_T(""))
			{
				msg=_T("项目")+iter->first+MsgDataFileErr;
				MessageBox(msg);
				return;
			}
			if (geo.strPrjPath==_T(""))
			{
				msg=_T("项目")+iter->first+MsgPrjFileErr;
				MessageBox(msg);
				return;
			}
			if (geo.strStarNetPath==_T(""))
			{
				msg=_T("项目")+iter->first+MsgStarNetErr;
				MessageBox(msg);
				return;
			}
		}


		
		WriteGeoPrjToFile(geo,_T("###"),false);
		iter->second.bSaveSuc=true;
		ClearNonexistentINIFiles();
		ClearRowSelectdColor(m_GridProjectSel);
	}
	MessageBox(_T("保存成功"));
}


	 



CString CDlgMonitor::CombineKeyAndValue( CString key,CString strValue )
{
	CString combineStr;
	combineStr = key+_T("=");
	combineStr+=strValue;
	return combineStr;
}
void CDlgMonitor::FormatStr( CString &orginStr,CString mark )
{
	orginStr=orginStr+mark;

}

void CDlgMonitor::WriteToFile( vector<CString>vecStr,CString FilePath,bool MsgFlag)
{
	if (vecStr.empty())
	{
		AfxMessageBox(_T("没有数据保存"));
		return;
	}

	//设置语言为中文,否则在Unicode编码下读出中文字符为乱码
	char* old_locale=_strdup( setlocale(LC_CTYPE,NULL) ); 
	setlocale( LC_CTYPE,"chs"); 

	CStdioFile file;
	file.Open(FilePath,CFile::modeCreate|CFile::modeWrite);
	for (vector<CString>::iterator iter=vecStr.begin();iter!=vecStr.end();iter++)
	{
		CString str=*iter;
		FormatStr(str,_T("\r\n"));
		file.WriteString(str);		
	}
	file.Close();
	if (MsgFlag)
	{
		AfxMessageBox(_T("数据保存成功"));
	}
	

	setlocale( LC_CTYPE, old_locale ); //还原语言区域的设置 
	free( old_locale );//还原区域设定

}

void CDlgMonitor::WriteGeoPrjToFile( GeoPojectStruct geoprj,CString separateStr,bool MsgFlag)
{
	CString INIFileDirectory=_T("INI"); //判断与程序文件同级的文件夹是否存在，不存在，则创建
	if (!PathIsDirectory(INIFileDirectory))
	{
		CreateDirectory(INIFileDirectory,0);
	}

	CString fileName=geoprj.strPrjName;
	CString FilePath;
	FilePath=_T("INI\\")+fileName+_T(".ini"); //路径名

	vector<CString>vecStr;

	CString strSelectState;//选用状态,是或否
	bool bIsSelected=geoprj.bSelectedPrj;
	if (bIsSelected)
	{
		strSelectState=CombineKeyAndValue(IsSelectedKey,_T("Yes"));
	} 
	else
	{
		strSelectState=CombineKeyAndValue(IsSelectedKey,_T("No"));
	}
	vecStr.push_back(strSelectState);

	//报告存放的目录
	CString strRptPath;
	strRptPath=CombineKeyAndValue(ReportPathKey,geoprj.strReportPath);
	vecStr.push_back(strRptPath);

	//使用StarNet进行平差
	CString strUsingStarnet;
	bool bUsingStarnet=geoprj.bUsingStarNet;
	if (bUsingStarnet)
	{
		strUsingStarnet=CombineKeyAndValue(UsingStarNetKey,_T("Yes"));
	} 
	else
	{
		strUsingStarnet=CombineKeyAndValue(UsingStarNetKey,_T("No"));
	}
	vecStr.push_back(strUsingStarnet);

	//dat文件存放目录
	CString strDataPath;
	strDataPath=CombineKeyAndValue(DataFilePathKey,geoprj.strDataPath);
	vecStr.push_back(strDataPath);

	//工程文件选择
	CString strprjPath;
	strprjPath=CombineKeyAndValue(PrjPathKey,geoprj.strPrjPath);
	vecStr.push_back(strprjPath);

	//StarNet应用程序选择
	CString strStarNetPath;
	strStarNetPath=CombineKeyAndValue(StarNetPathKey,geoprj.strStarNetPath);
	vecStr.push_back(strStarNetPath);

	//回滚检索数据时间（分钟）
	CString strBackTime;
	strBackTime.Format(_T("%d"),geoprj.nBackTime);
	strBackTime=CombineKeyAndValue(BackTimeKey,strBackTime);
	vecStr.push_back(strBackTime);

	//收敛失败仍将结果写入GeoMos数据库
	CString strFailConvergaeWriteToDB;
	bool bFailConvergaeWriteToDB=geoprj.bFailConvergaeWriteToDB;
	if (bFailConvergaeWriteToDB)
	{
		strFailConvergaeWriteToDB=CombineKeyAndValue(FailConvergaeWriteToDBKey,_T("Yes"));
	} 
	else
	{
		strFailConvergaeWriteToDB=CombineKeyAndValue(FailConvergaeWriteToDBKey,_T("No"));
	}
	vecStr.push_back(strFailConvergaeWriteToDB);

	//卡方检验指标设置太低时更新坐标
	CString strUpdateCoordIfLowerBound;
	bool bUpdateCoordIfLowerBound=geoprj.bUpdateCoordIfLowerBound;
	if (bUpdateCoordIfLowerBound)
	{
		strUpdateCoordIfLowerBound=CombineKeyAndValue(UpdateCoordIfLowerBoundKey,_T("Yes"));
	} 
	else
	{
		strUpdateCoordIfLowerBound=CombineKeyAndValue(UpdateCoordIfLowerBoundKey,_T("No"));
	}
	vecStr.push_back(strUpdateCoordIfLowerBound);
	vecStr.push_back(separateStr);//分割符


	//点名管理
	m_vecNodes=geoprj.vecGeoNodes;
	CString strFixNode=_T("");
	CString strStationNode=_T("");
	CString strFloatNode=_T("");
	vector<CString>vecFixNodeNames;
	vector<CString>vecStationNodeNames;
	vector<CString>vecFloatNodeNames;
	for (vector<GeoNode>::iterator iter=m_vecNodes.begin();iter!=m_vecNodes.end();iter++)
	{
		GeoNode node=*iter;
		if (node.strNodeType==_T("已知点"))
		{
			CString strNodeName=node.strNodeName;
			vecFixNodeNames.push_back(strNodeName);
		}
		if (node.strNodeType==_T("测站点"))
		{
			CString strNodeName=node.strNodeName;
			vecStationNodeNames.push_back(strNodeName);
		}
		if (node.strNodeType==_T("浮动点"))
		{
			CString strNodeName=node.strNodeName;
			vecFloatNodeNames.push_back(strNodeName);
		}
	}
	for (vector<CString>::iterator iter=vecFixNodeNames.begin();iter!=vecFixNodeNames.end();iter++)
	{
		strFixNode+=*iter;
		if (iter!=vecFixNodeNames.end()-1)
		{
			strFixNode+=_T(",");
		}
	}
	strFixNode=CombineKeyAndValue(FixNodeKey,strFixNode);
	for (vector<CString>::iterator iter=vecStationNodeNames.begin();iter!=vecStationNodeNames.end();iter++)
	{
		strStationNode+=*iter;
		if (iter!=vecStationNodeNames.end()-1)
		{
			strStationNode+=_T(",");
		}
	}
	strStationNode=CombineKeyAndValue(StationNodeKey,strStationNode);
	for (vector<CString>::iterator iter=vecFloatNodeNames.begin();iter!=vecFloatNodeNames.end();iter++)
	{
		strFloatNode+=*iter;
		if (iter!=vecFloatNodeNames.end()-1)
		{
			strFloatNode+=_T(",");
		}
	}
	strFloatNode=CombineKeyAndValue(FloatNodeKey,strFloatNode);
	vecStr.push_back(strFixNode);
	vecStr.push_back(strStationNode);
	vecStr.push_back(strFloatNode);
	vecStr.push_back(separateStr);//分割符


	//观测方向
	CString strOrientation;
	m_vecOrientation=geoprj.vecGeoOrientations;
	for (vector<GeoOrientation>::iterator iter=m_vecOrientation.begin();iter!=m_vecOrientation.end();iter++)
	{
		strOrientation=iter->strStationPoint+_T(",")+iter->strTargetPoint;
		strOrientation=CombineKeyAndValue(MKey,strOrientation);
		vecStr.push_back(strOrientation);
	}
	vecStr.push_back(separateStr);//分割符


	//点位偏移
	m_vecOffset=geoprj.vecGeoOffsets;
	for (vector<GeoOffset>::iterator iter=m_vecOffset.begin();iter!=m_vecOffset.end();iter++)
	{
		CString strOffset=_T("");
		strOffset+=iter->strStartPoint;
		strOffset+=_T(",");
		strOffset+=iter->strEndPoint;
		strOffset+=_T(",");
		strOffset+=iter->strEastCoordError;
		strOffset+=_T(",");
		strOffset+=iter->strNorthCoordError;
		strOffset+=_T(",");
		strOffset+=iter->strElevationDiff;
		strOffset=CombineKeyAndValue(DKey,strOffset);
		vecStr.push_back(strOffset);
	}
	vecStr.push_back(separateStr);//分割符

	//开始时间
	CString strStartTime=_T("");
	strStartTime=CombineKeyAndValue(StartTimeKey,geoprj.strStartTime);
	vecStr.push_back(strStartTime);
	//处理时间
	CString strIntervalTime;
	strIntervalTime=CombineKeyAndValue(IntervalTimeKey,geoprj.strIntervalTime);
	vecStr.push_back(strIntervalTime);
	//结束时间
	CString strEndTime;
	strEndTime=CombineKeyAndValue(EndTimeKey,geoprj.strEndTime);
	vecStr.push_back(strEndTime);


	WriteToFile(vecStr,FilePath,MsgFlag);


}



void CDlgMonitor::EnableAllCtrlState( BOOL bState )
{
	GetDlgItem(IDC_editReportPath)->EnableWindow(bState);
	GetDlgItem(IDC_editDataPath)->EnableWindow(bState);
	GetDlgItem(IDC_editProjectPath)->EnableWindow(bState);
	GetDlgItem(IDC_editStarNetPath)->EnableWindow(bState);

	GetDlgItem(IDC_BtnReportPathSel)->EnableWindow(bState);
	GetDlgItem(IDC_BtnDataPathSel)->EnableWindow(bState);
	GetDlgItem(IDC_BtnProjectPathSel)->EnableWindow(bState);
	GetDlgItem(IDC_BtnStarNetPathSel)->EnableWindow(bState);

	GetDlgItem(IDC_editSeachTime)->EnableWindow(bState);

	GetDlgItem(IDC_CheckDifference)->EnableWindow(bState);
	GetDlgItem(IDC_CheckWriteDBFail)->EnableWindow(bState);
	GetDlgItem(IDC_CheckAxisLow)->EnableWindow(bState);

	GetDlgItem(IDC_BtnObserveCfg)->EnableWindow(bState);
	GetDlgItem(IDC_BtnSave)->EnableWindow(bState);

}

void CDlgMonitor::SetAllCtrlState()
{
	if (m_GridProjectSel.GetRowCount()>1&&m_nSelectedRowNum!=-1)
	{
		EnableAllCtrlState(TRUE);
	}
	else
	{
		EnableAllCtrlState(FALSE);
	}

	EnableStarNetDiff(m_bUsingStarNet);

}

void CDlgMonitor::EnableStarNetDiff( BOOL bState )
{
	GetDlgItem(IDC_editDataPath)->EnableWindow(bState);
	GetDlgItem(IDC_editProjectPath)->EnableWindow(bState);
	GetDlgItem(IDC_editStarNetPath)->EnableWindow(bState);
	GetDlgItem(IDC_BtnDataPathSel)->EnableWindow(bState);
	GetDlgItem(IDC_BtnProjectPathSel)->EnableWindow(bState);
	GetDlgItem(IDC_BtnStarNetPathSel)->EnableWindow(bState);

}

	//********* //设置某列单元为combobox的形式***************
// Method:    SetComboBoxCell
// FullName:  CObserveDlg::SetComboBoxCell
// Access:    public 
// Returns:   void
// Parameter: CGridCtrl & gridCtrl
// Parameter: int comboBoxColumn      //comboBox所在的列
// Parameter: vector<CString>vecComboOption  //combobox中的字符选项
	//************************************
void CDlgMonitor::SetComboBoxCell( CGridCtrl &gridCtrl,int comboBoxColumn,vector<CString>vecComboOption )
{
	for (int row = 0; row < gridCtrl.GetRowCount(); row++)
		for (int col = 0; col < gridCtrl.GetColumnCount(); col++)
		{
			if(row!=0)
			{
				if (!gridCtrl.SetCellType(row,comboBoxColumn, RUNTIME_CLASS(CGridCellCombo)))
					return;

				//gridCtrl.SetItemText(row,comboBoxColumn, _T("是"));//默认为是

				//下拉框选项文字
				CStringArray options;
				for (vector<CString>::iterator iter=vecComboOption.begin();iter!=vecComboOption.end();iter++)
				{
					options.Add(*iter);
				}

				CGridCellCombo *pCell = (CGridCellCombo*) gridCtrl.GetCell(row,comboBoxColumn);
				pCell->SetOptions(options);
				pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

			}

		}

}

void CDlgMonitor::SetReadOnlyCell( CGridCtrl &gridCtrl,int comboBoxColumn )
{
	for (int row=0;row<gridCtrl.GetRowCount();row++)
		for (int col=0;col<gridCtrl.GetColumnCount();col++)
		{
			if (row>0)
			{
				if (col==comboBoxColumn)
				{
					m_GridProjectSel.SetItemState(row,col,GVIS_READONLY);

				}
			}

		}

}

void CDlgMonitor::SetFirstColumnText(CGridCtrl &gridCtrl)
{
	for (int row=1;row<gridCtrl.GetRowCount();row++)
	{
		CString strPrjNum;
		strPrjNum.Format(_T("%d"),row);
		m_GridProjectSel.SetItemText(row,0,strPrjNum);
	}

}

void CDlgMonitor::SetRowSelected( CGridCtrl &gridCtrl,int row )
{
	ClearRowSelectdColor(gridCtrl);

	COLORREF crl=BLUE;//淡蓝色
	for (int col=1;col<6;col++)
	{
		gridCtrl.SetItemBkColour(row,col,crl);

	}
	gridCtrl.Refresh();
	

}

void CDlgMonitor::ClearRowSelectdColor( CGridCtrl &gridCtrl )
{
	for (int r=1;r<gridCtrl.GetRowCount();r++)
		for (int c=1;c<gridCtrl.GetColumnCount();c++)
		{
			CString geoPrjName=gridCtrl.GetItemText(r,2);
			bool bSaveSuc=m_mapGeoProject[geoPrjName].bSaveSuc;
			if (bSaveSuc)
			{
				gridCtrl.SetItemBkColour(r,c,GREEN);//保存颜色为绿色 
				
			}
			//else if (r==m_nSelectedRowNum)
			//{
			//	gridCtrl.SetItemBkColour(r,c,BLUE); //选中颜色
			//}
			else
			{
				gridCtrl.SetItemBkColour(r,c); //默认颜色
			}
			
		}
	gridCtrl.Refresh();

}

void CDlgMonitor::Refresh( const GeoPojectStruct &geo )
{
	m_strReportPath=geo.strReportPath;
	m_bUsingStarNet=geo.bUsingStarNet;
	m_strDataPath=geo.strDataPath;
	m_strPrjPath=geo.strPrjPath;
	m_strStarNetPath=geo.strStarNetPath;
	m_nBackTime=geo.nBackTime;
	m_bFailConvergaeWriteToDB=geo.bFailConvergaeWriteToDB;
	m_bUpdateCoordIfLowerBound=geo.bUpdateCoordIfLowerBound;
	m_vecNodes=geo.vecGeoNodes;
	m_vecOrientation=geo.vecGeoOrientations;
	m_vecOffset=geo.vecGeoOffsets;
	UpdateData(FALSE);

}

void CDlgMonitor::RowHasChangedState( CGridCtrl &gridCtrl,CString projectName,int selectedRow )
{
	m_mapGeoProject[projectName].bSaveSuc=false;
	ClearRowSelectdColor(gridCtrl);
	SetRowSelected(gridCtrl,selectedRow);

}

SaveFlag CDlgMonitor::GetSelectedGeoProject( int selectedRow ,GeoPojectStruct &geoPrj)
{
	m_GridProjectSel.Refresh();
	UpdateData(TRUE);
	m_saveflag=SaveSuc;

	CString selectState=m_GridProjectSel.GetItemText(selectedRow,1);//选用状态
	if (selectState==_T("是"))
	{
		geoPrj.bSelectedPrj=true;
	}
	else 
	{
		geoPrj.bSelectedPrj=false;
	}
	geoPrj.strPrjName=m_GridProjectSel.GetItemText(selectedRow,2);//项目名
	if (geoPrj.strPrjName==_T(""))
	{
		/*AfxMessageBox(_T("保存失败，项目名为空，请输入项目名!"));*/
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrPrjName;
		}
		
	} 
	
	geoPrj.strReportPath=m_strReportPath;//报告存放目录
	if (geoPrj.strReportPath==_T(""))
	{
		/*AfxMessageBox(_T("保存失败，报告目录为空，请设置"));*/
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrReport;
		}
		
	}
	geoPrj.bUsingStarNet=m_bUsingStarNet;//是否使用StarNet进行平差
	geoPrj.strDataPath=m_strDataPath;//data文件存放目录
	if (geoPrj.bUsingStarNet&&geoPrj.strDataPath==_T(""))//如果没有选择使用StarNet进行平差，不与检查路径是否为空
	{
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrDataFile;
		}
	}
	geoPrj.strPrjPath=m_strPrjPath;//工程文件选择
	if (geoPrj.bUsingStarNet&&geoPrj.strPrjPath==_T(""))//如果没有选择使用StarNet进行平差，不与检查路径是否为空
	{
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrPrjFile;
		}
	}
	geoPrj.strStarNetPath=m_strStarNetPath;//StarNet应用程序选择
	if (geoPrj.bUsingStarNet&&geoPrj.strStarNetPath==_T(""))//如果没有选择使用StarNet进行平差，不与检查路径是否为空
	{
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrStarNet;
		}
	}
	geoPrj.nBackTime=m_nBackTime;//回滚检索数据时间（分钟）
	if(geoPrj.nBackTime<=0)
	{
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrBackTime;
		}
	}

	//确保开始时间小于结束时间
	CString strStartTime=m_GridProjectSel.GetItemText(selectedRow,3);//开始时间
	COleDateTime starttime=ConvertTime(strStartTime);
	CString strEndTime=m_GridProjectSel.GetItemText(selectedRow,5);//结束时间
	COleDateTime endtime=ConvertTime(strEndTime);	
	if (endtime<starttime)
	{
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrTimeValue;
		}

	}

	//开始时间
	if (strStartTime==_T(""))
	{
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrStartTime;
		}
	}
	else
	{
		if (m_saveflag==SaveSuc)
		{
			strStartTime.Remove('-');//删除-
			strStartTime.Remove(':');//删除:
			strStartTime.Replace(_T(" "),_T(""));//删除空格
		}		
		geoPrj.strStartTime=strStartTime;

	}
	//处理时间
	CString strIntervalTime=m_GridProjectSel.GetItemText(selectedRow,4);//处理时间
	if (strIntervalTime==_T(""))
	{
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrIntervalTime;
		}
	}
	else
	{
		if(m_saveflag==SaveSuc)
		{
			strIntervalTime.Remove(':');
		}		
		geoPrj.strIntervalTime=strIntervalTime;

	}
	//结束时间
	if (strEndTime==_T(""))
	{
		if (m_saveflag==SaveSuc)
		{
			m_saveflag=ErrStopTime;
		}
		
	}
	else
	{
		if(m_saveflag==SaveSuc)
		{
			strEndTime.Remove('-');//删除-
			strEndTime.Remove(':');//删除:
			strEndTime.Replace(_T(" "),_T(""));//删除空格
		}		
		geoPrj.strEndTime=strEndTime;
	}
		
	geoPrj.bFailConvergaeWriteToDB=m_bFailConvergaeWriteToDB;//是否收敛失败仍将结果写入GeoMoS数据库
	geoPrj.bUpdateCoordIfLowerBound=m_bUpdateCoordIfLowerBound;//是否卡方检验指标设置太低时更新坐标

	geoPrj.vecGeoNodes=m_vecNodes;//点名管理
	geoPrj.vecGeoOrientations=m_vecOrientation; //观测方向
	geoPrj.vecGeoOffsets=m_vecOffset; //点位偏移

	switch(m_saveflag)
	{
	case ErrPrjName:
		MessageBox(MsgPrjNameErr);
		break;
	case ErrStartTime:
		MessageBox(MsgStartTimeErr);
		break;
	case ErrIntervalTime:
		MessageBox(MsgIntervalTimeErr);
		break;
	case ErrStopTime:
		MessageBox(MsgStopTimeErr);
		break;
	case ErrTimeValue:
		MessageBox(MsgTimeValueErr);
		break;
	case ErrReport:
		MessageBox(MsgReportErr);
		break;
	case ErrDataFile:
		MessageBox(MsgDataFileErr);
		break;
	case ErrPrjFile:
		MessageBox(MsgPrjFileErr);
		break;
	case ErrStarNet:
		MessageBox(MsgStarNetErr);
		break;
	case ErrBackTime:
		MessageBox(MsgBackTimeErr);
		break;
	}

	return m_saveflag;

}

void CDlgMonitor::SetDefaultProject(int selecedRow)
{
	int nProjectCount=m_GridProjectSel.GetRowCount();//项目数量
	int nColumn=m_GridProjectSel.GetColumnCount();//列数
	CString strItemText=_T("");
	COleDateTime oDateTime(2000,1,1,0,0,0);
	int newrow=nProjectCount-1;//最近增加的行
	GeoPojectStruct geoprj;
	for (int col=1;col<nColumn;col++)
	{
		switch(col)
		{
		case 1:
			strItemText=_T("是");
			geoprj.bSelectedPrj=true;
			break;
		case 2:
			strItemText.Format(_T("Project%d"),newrow);
			if (m_mapGeoProject.count(strItemText)==1)//已存在该名称
			{
				strItemText+=SUFFIX;//添加后缀名称
			}
			geoprj.strPrjName=strItemText;
			break;
		case 3:
			strItemText=oDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			geoprj.strStartTime=strItemText;
			break;
		case 5:				
			strItemText=oDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			geoprj.strEndTime=strItemText;
			break;
		case 4:
			CTimeSpan oTimeSpan(0,0,0,0);
			strItemText=oTimeSpan.Format(_T("%H:%M:%S"));
			geoprj.strIntervalTime=strItemText;
			break;
		}
		m_GridProjectSel.SetItemText(newrow,col,strItemText);

		if (col==3||col==4||col==5)
		{
			m_GridProjectSel.SetItemState(newrow,col,GVIS_READONLY);
		}
	}
	geoprj.bUsingStarNet=false;
	geoprj.nBackTime=60;
	geoprj.strReportPath=_T("");
	geoprj.strDataPath=_T("");
	geoprj.strPrjPath=_T("");
	if (selecedRow>0)
	{
		CString currentPrjName=m_GridProjectSel.GetItemText(selecedRow,2);
		geoprj.strStarNetPath=m_mapGeoProject[currentPrjName].strStarNetPath;
	}
	else
	{
		geoprj.strStarNetPath=_T("");
	}
	
	geoprj.bSaveSuc=false;
	m_mapGeoProject.insert(make_pair(geoprj.strPrjName,geoprj));
		
}

CString CDlgMonitor::ConvertDateTimeFormat( CString strNumberFormat )
{
	CString strItemText=strNumberFormat;
	CString strYear=strItemText.Left(4);
	strItemText=strItemText.Mid(4);
	CString strMonth=strItemText.Left(2);
	strItemText=strItemText.Mid(2);
	CString strDay=strItemText.Left(2);
	strItemText=strItemText.Mid(2);
	CString strHour=strItemText.Left(2);
	strItemText=strItemText.Mid(2);
	CString strMinute=strItemText.Left(2);
	strItemText=strItemText.Mid(2);
	CString strSecond=strItemText;
	CString strDateTime=strYear+_T("-")+strMonth+_T("-")+strDay+_T(" ")+strHour+_T(":")+strMinute+_T(":")+strSecond;
	return strDateTime;

}

CString CDlgMonitor::ConvertTimeFormat( CString strNumberFomat )
{
	CString strItemText=strNumberFomat;
	CString strHour=strItemText.Left(2);
	strItemText=strItemText.Mid(2);
	CString strMinute=strItemText.Left(2);
	strItemText=strItemText.Mid(2);
	CString strSecond=strItemText;
	CString timeStr=strHour+_T(":")+strMinute+_T(":")+strSecond;
	return timeStr;

}

COleDateTime CDlgMonitor::ConvertTime( CString strEpoch )
{
	//2014-8-8 8:08:08 --> 2014 8 8 8:8:8
	int nYear = 2000;
	int nMonth = 1;
	int nDay = 1;
	int nHour = 0;
	int nMin = 0;
	int nSec = 0;

	int pos = strEpoch.Find('-');
	if (pos != -1)
	{
		nYear = _wtoi(strEpoch.Left(pos));
		strEpoch = strEpoch.Mid(pos + 1);
		pos = strEpoch.Find('-');
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

	return COleDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);

}

COleDateTime CDlgMonitor::ConvertTimeSpan( CString strEpoch )
{
	int nYear = 2000;
	int nMonth = 1;
	int nDay = 1;
	int nHour = 0;
	int nMin = 0;
	int nSec = 0;

	int pos = strEpoch.Find(':');
	nHour = _wtoi(strEpoch.Left(pos));
	strEpoch = strEpoch.Mid(pos + 1);
	pos = strEpoch.Find(':');
	nMin = _wtoi(strEpoch.Left(pos));
	strEpoch = strEpoch.Mid(pos + 1);
	strEpoch.TrimRight();
	nSec = _wtoi(strEpoch);

	return COleDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);

}

BOOL CDlgMonitor::FindINIFiles()
{
	if(!m_vecINIFilePath.empty())//非空，清空数据
	{
		vector<CString>().swap(m_vecINIFilePath);

	}

	CFileFind finder;
	CString filePath=_T("INI\\*.ini");//和程序文件同级的INI目录
	BOOL bFindSuc = finder.FindFile(filePath);
	while (bFindSuc)
	{
		bFindSuc=finder.FindNextFile();
		CString fileName=finder.GetFilePath();
		if (VerifyINIFile(fileName,_T("GeoMoS Adjust INI FILE=")))
		{
			m_vecINIFilePath.push_back(fileName);
		}
	}
	if (!m_vecINIFilePath.empty())//非空
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}


}

CString CDlgMonitor::GetFileTitleFromFilePath( CString FileName )
{
	int Where;
	Where=FileName.ReverseFind('\\');
	if(Where==-1)
	{
		Where=FileName.ReverseFind('/');
	}
	CString FileTitle=FileName.Right(FileName.GetLength()-1-Where);//带扩展名的文件名
	int Which=FileTitle.ReverseFind('.');
	CString strExtendName=FileTitle.Right(FileTitle.GetLength()-Which-1);//扩展名
	if (Which!=-1)
	{
		FileTitle=FileTitle.Left(Which);
	}
	return FileTitle; //不带扩展名的文件名

}

BOOL CDlgMonitor::VerifyINIFile( CString filePath, CString fileHeader )
{
	CStdioFile mFile;
	if (mFile.Open(filePath, CFile::modeRead))
	{
		CString strTemp;
		BOOL bReadSucc = mFile.ReadString(strTemp);
		if (strTemp.Find(fileHeader) != -1)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}

	}
	return FALSE;

}

void CDlgMonitor::ReadINIFile( CString strINIFilePath,GeoPojectStruct &geoPrj )
{
		//设置语言为中文,否则在Unicode编码下读出中文字符为乱码
	char* old_locale=_strdup( setlocale(LC_CTYPE,NULL) ); 
	setlocale( LC_CTYPE,"chs");

	CStdioFile mFile;
	if (mFile.Open(strINIFilePath, CFile::modeRead))
	{
		CString strPrjName=GetFileTitleFromFilePath(strINIFilePath);
		geoPrj.strPrjName=strPrjName; //项目名

		CString strTemp;
		BOOL bReadSucc = mFile.ReadString(strTemp);
		CString strItemText;
		while (bReadSucc)
		{
			strTemp.TrimRight();//删除右边的空格

			if(strTemp.Find(_T("GeoMoS Adjust INI FILE=")) != -1)  //选用状态
			{
				strTemp.MakeUpper();//全部转换为大写
				strItemText=strTemp.Mid(strTemp.Find('=')  + 1);
				if (strItemText==_T("YES"))
				{
					geoPrj.bSelectedPrj=true;
				} 
				else
				{
					geoPrj.bSelectedPrj=false;
				}
			}
			
			if (strTemp.Find(_T("Report Folder=")) != -1)  //报告存放目录
			{
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				//创建目录
				/*if (!PathFileExists(strItemText)) {
					::CreateDirectory(strItemText, NULL);
				}*/
				geoPrj.strReportPath=strItemText;
			}

			if(strTemp.Find(_T("Using StarNet=")) != -1)  //选用状态
			{
				strTemp.MakeUpper();//全部转换为大写
				strItemText=strTemp.Mid(strTemp.Find('=')  + 1);
				if (strItemText==_T("YES"))
				{
					geoPrj.bUsingStarNet=true;
				} 
				else
				{
					geoPrj.bUsingStarNet=false;
				}
			}

			if (strTemp.Find(_T("Write File Folder=")) != -1) //dat文件存放路径
			{
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				geoPrj.strDataPath=strItemText;
			}

			if (strTemp.Find(_T("Write Project Folder=")) != -1) //工程文件存放路径
			{
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				geoPrj.strPrjPath=strItemText;
			}

			if (strTemp.Find(_T("StarNet Program Folder=")) != -1) //StarNet应用程序存放路径
			{
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				geoPrj.strStarNetPath=strItemText;
			}

			if (strTemp.Find(_T("BackTime to retrieve data(min)=")) != -1) //回滚检索数据时间
			{
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				geoPrj.nBackTime=_wtoi(strItemText);
			}

			if (strTemp.Find(_T("Fail Converage Still Write Points Into GeoMos DataBase=")) != -1) //是否收敛失败仍将结果写入GeoMoS数据库
			{
				strTemp.MakeUpper();//全部转换为大写
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				if (strItemText==_T("YES"))
				{
					geoPrj.bFailConvergaeWriteToDB=true;
				} 
				else
				{
					geoPrj.bFailConvergaeWriteToDB=false;
				}

			}

			if (strTemp.Find(_T("Update Coordinates if Chi-Square Test Lower Bound=")) != -1) //是否卡方检验指标设置太低时更新坐标
			{
				strTemp.MakeUpper();//全部转换为大写
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				if (strItemText==_T("YES"))
				{
					geoPrj.bUpdateCoordIfLowerBound=true;
				} 
				else
				{
					geoPrj.bUpdateCoordIfLowerBound=false;
				}

			}

			if (strTemp.Left(8) == _T("Fix Ref=")) //已知点
			{
				strTemp = strTemp.Mid(strTemp.Find('=') + 1);
				int pos = strTemp.Find(',');
				GeoNode node;
				while (pos != -1)
				{						
					node.strNodeName= strTemp.Left(pos);
					node.strNodeType=_T("已知点");
					geoPrj.vecGeoNodes.push_back(node);
					strTemp = strTemp.Mid(pos + 1);
					pos = strTemp.Find(',');
				}
				if(strTemp!=_T(""))//数据不为空
				{
					node.strNodeName=strTemp;
					node.strNodeType=_T("已知点");
					geoPrj.vecGeoNodes.push_back(node);
				}					
			}
			
			if (strTemp.Left(10) == _T("Float Stn=")) //测站点
			{
				strTemp = strTemp.Mid(strTemp.Find('=') + 1);
				int pos = strTemp.Find(',');
				GeoNode node;
				while (pos != -1)
				{						
					node.strNodeName= strTemp.Left(pos);
					node.strNodeType=_T("测站点");
					geoPrj.vecGeoNodes.push_back(node);
					strTemp = strTemp.Mid(pos + 1);
					pos = strTemp.Find(',');
				}
				if(strTemp!=_T(""))//数据不为空
				{
					node.strNodeName=strTemp;
					node.strNodeType=_T("测站点");
					geoPrj.vecGeoNodes.push_back(node);
				}					
			}

			if (strTemp.Left(12) == _T("Float Point=")) //浮动点
			{
				strTemp = strTemp.Mid(strTemp.Find('=') + 1);
				int pos = strTemp.Find(',');
				GeoNode node;
				while (pos != -1)
				{						
					node.strNodeName= strTemp.Left(pos);
					node.strNodeType=_T("浮动点");
					geoPrj.vecGeoNodes.push_back(node);
					strTemp = strTemp.Mid(pos + 1);
					pos = strTemp.Find(',');
				}
				if (strTemp!=_T(""))//数据不为空
				{
					node.strNodeName=strTemp;
					node.strNodeType=_T("浮动点");
					geoPrj.vecGeoNodes.push_back(node);

				}					
			}

			if (strTemp.Left(2) == _T("M=")) //观测方向
			{
				GeoOrientation orientation;
				strTemp = strTemp.Mid(strTemp.Find('=') + 1);
				int pos = strTemp.Find(',');
				orientation.strStationPoint=strTemp.Left(pos);
				orientation.strTargetPoint=strTemp.Mid(pos + 1);
				geoPrj.vecGeoOrientations.push_back(orientation);
			}

			if (strTemp.Left(2) == _T("D=")) //点位偏移
			{
				strTemp = strTemp.Mid(strTemp.Find('=') + 1);
				CString strSub[5];
				for (int i=0; i<5; i++)	
					strSub[i].Empty();
				int nComma = DivideString(strTemp, strSub, 4);
				if (nComma >= 4)
				{
					GeoOffset offset;
					offset.strStartPoint=strSub[0];
					offset.strEndPoint=strSub[1];
					offset.strEastCoordError=strSub[2];
					offset.strNorthCoordError=strSub[3];
					offset.strElevationDiff=strSub[4];
					geoPrj.vecGeoOffsets.push_back(offset);
				}
			}

			if (strTemp.Find(_T("Start Time=")) != -1) //开始时间
			{
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				CString strYear=strItemText.Left(4);
				strItemText=strItemText.Mid(4);
				CString strMonth=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strDay=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strHour=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strMinute=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strSecond=strItemText;
				geoPrj.strStartTime=strYear+_T("-")+strMonth+_T("-")+strDay+_T(" ")+strHour+_T(":")+strMinute+_T(":")+strSecond;
			}

			if (strTemp.Find(_T("End Time=")) != -1) //结束时间
			{
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				CString strYear=strItemText.Left(4);
				strItemText=strItemText.Mid(4);
				CString strMonth=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strDay=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strHour=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strMinute=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strSecond=strItemText;
				geoPrj.strEndTime=strYear+_T("-")+strMonth+_T("-")+strDay+_T(" ")+strHour+_T(":")+strMinute+_T(":")+strSecond;
			}
			if (strTemp.Find(_T("Interval Time=")) != -1) //处理时间
			{
				strItemText = strTemp.Mid(strTemp.Find('=')  + 1);
				CString strHour=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strMinute=strItemText.Left(2);
				strItemText=strItemText.Mid(2);
				CString strSecond=strItemText;
				geoPrj.strIntervalTime=strHour+_T(":")+strMinute+_T(":")+strSecond;
			}

			bReadSucc = mFile.ReadString(strTemp); //继续循环

		}

	}

	setlocale( LC_CTYPE, old_locale ); //还原语言区域的设置 
	free( old_locale );//还原区域设定


}

void CDlgMonitor::ReadINIFiles( vector<CString> vecstrINIFilePath,MAPGeoProject &mapGeoProject )
{		
	CString strINIFilePath;	
	for (vector<CString>::iterator iter=vecstrINIFilePath.begin();iter!=vecstrINIFilePath.end();iter++)
	{
		strINIFilePath=*iter;
		GeoPojectStruct geoPrj;
		ReadINIFile(strINIFilePath,geoPrj);
		geoPrj.bSaveSuc=true;
		mapGeoProject.insert(make_pair(geoPrj.strPrjName,geoPrj));
	}

}

short CDlgMonitor::DivideString( CString strText, CString *strArray,int nCommaCount )
{
	if (strText.IsEmpty())
		return 0;
	CString strPoint;
	int		nIndex;
	int     n = 0;
	while (strText.GetLength() != 0)
	{
		nIndex = strText.Find(',');
		if (nIndex == -1) 
			break;
		strPoint = strText.Mid(0,nIndex);
		strArray[n] = strPoint;
		strText  = strText.Mid(nIndex+1);
		n++;
		if (n > nCommaCount)
			break;
	}
	if (n <= nCommaCount)
		strArray[n] = strText;
	return n;

}




void CDlgMonitor::ClearNonexistentINIFiles()
{

	CFileFind finder;
	CString filePath=_T("INI\\*.ini");//和程序文件同级的INI目录
	BOOL bFindSuc = finder.FindFile(filePath);
	while (bFindSuc)
	{
		bFindSuc=finder.FindNextFile();
		CString filePath=finder.GetFilePath();
		CString fileName=finder.GetFileTitle();
		if (m_mapGeoProject.count(fileName)==0)
		{
			DeleteFile(filePath);

		}
		/*if (VerifyINIFile(fileName,_T("GeoMoS Adjust INI FILE=")))
		{
			m_vecINIFilePath.push_back(fileName);
		}*/
	}

}

void CDlgMonitor::OnGridDblClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if(pItem->iRow>0)
	{
		CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
		if (pItem->iColumn==3||pItem->iColumn==5)
		{
			CDateTimeDlg dlg;
			CString strItemText=m_GridProjectSel.GetItemText(pItem->iRow,pItem->iColumn);
			dlg.m_dateTimeSel=ConvertTime(strItemText);
			
			if (IDOK==dlg.DoModal())
			{
				CString strTime=dlg.m_dateTimeSel.Format(_T("%Y-%m-%d %H:%M:%S"));
				m_GridProjectSel.SetItemText(pItem->iRow,pItem->iColumn,strTime);
				m_GridProjectSel.Refresh();
				if (pItem->iColumn==3)
				{
					m_mapGeoProject[projectName].strStartTime=strTime;
				}
				else
				{
					m_mapGeoProject[projectName].strEndTime=strTime;
				}
				m_mapGeoProject[projectName].bSaveSuc=false;
			}

		}
		if (pItem->iColumn==4)
		{
			CTimeSpanDlg dlg;
			CString strItemText=m_GridProjectSel.GetItemText(pItem->iRow,pItem->iColumn);
			dlg.m_timeProcess=ConvertTimeSpan(strItemText);
			
			if (IDOK==dlg.DoModal())
			{
				CString strTime=dlg.m_timeProcess.Format(_T("%H:%M:%S"));
				m_GridProjectSel.SetItemText(pItem->iRow,pItem->iColumn,strTime);
				m_GridProjectSel.Refresh();
				m_mapGeoProject[projectName].strIntervalTime=strTime;
				m_mapGeoProject[projectName].bSaveSuc=false;
			}
		}

	}
	ClearRowSelectdColor(m_GridProjectSel);

}

void CDlgMonitor::OnGridClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	if (m_bDuplicateName)
	{
		CString msg;
		m_GridProjectSel.SetItemBkColour(m_nLastSelectedRowNum,2,RGB(255,0,0));
		m_GridProjectSel.Refresh();
		msg.Format(_T("第%d行输入的项目名称修改失败，失败原因:项目名称重复"),m_nLastSelectedRowNum);
		AfxMessageBox(msg);
		m_bDuplicateName=FALSE;
	}
	

	 NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	 if (pItem->iRow>0)
	 {
		  GeoPojectStruct geoproject;
		 SetRowSelected(m_GridProjectSel,pItem->iRow);		
		 m_nSelectedRowNum=pItem->iRow; //选择的行号，用于保存
		 CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);//项目名称
		 MAPGeoProject::iterator iter=m_mapGeoProject.find(projectName);
		 if (iter!=m_mapGeoProject.end())//读取元素，元素存在
		 {
			/* _T("INI\\*.ini");*/
			 CString strFilePath=_T("INI\\")+projectName+_T(".ini");
			 if (PathFileExists(strFilePath))//文件仍然存在,数据刷新
			 {
				/* GeoPojectStruct geoproject=m_mapGeoProject[projectName];*/				
				 ReadINIFile(strFilePath,geoproject);
				 m_bSelectedPrj=geoproject.bSelectedPrj;
				 m_strReportPath=geoproject.strReportPath;
				 m_bUsingStarNet=geoproject.bUsingStarNet;
				 m_strDataPath=geoproject.strDataPath;
				 m_strPrjPath=geoproject.strPrjPath;
				 m_strStarNetPath=geoproject.strStarNetPath;
				 m_nBackTime=geoproject.nBackTime;
				 m_bFailConvergaeWriteToDB=geoproject.bFailConvergaeWriteToDB;
				 m_bUpdateCoordIfLowerBound=geoproject.bUpdateCoordIfLowerBound;
				 m_vecNodes=geoproject.vecGeoNodes;
				 m_vecOrientation=geoproject.vecGeoOrientations;
				 m_vecOffset=geoproject.vecGeoOffsets;

				/* m_strStartTime=geoproject.strStartTime;
				 m_strIntervalTime=geoproject.strIntervalTime;
				 m_strEndTime=geoproject.strEndTime;

				 if (m_bSelectedPrj)
				 {
					 m_GridProjectSel.SetItemText(m_nSelectedRowNum,1,_T("是"));
				 }
				 else
				 {
					m_GridProjectSel.SetItemText(m_nSelectedRowNum,1,_T("否"));
				 }
				 m_GridProjectSel.SetItemText(m_nSelectedRowNum,3,m_strStartTime);
				 m_GridProjectSel.SetItemText(m_nSelectedRowNum,4,m_strIntervalTime);
				 m_GridProjectSel.SetItemText(m_nSelectedRowNum,5,m_strEndTime);*/

				 geoproject.bSaveSuc=m_mapGeoProject[projectName].bSaveSuc;
				
				 m_mapGeoProject[projectName]=geoproject;//更新数据
			 }
			 else //不存在，从原有数据取值
			 {
				 GeoPojectStruct geoproject=m_mapGeoProject[projectName];

				 m_strReportPath=geoproject.strReportPath;
				 m_bUsingStarNet=geoproject.bUsingStarNet;
				 m_strDataPath=geoproject.strDataPath;
				 m_strPrjPath=geoproject.strPrjPath;
				 m_strStarNetPath=geoproject.strStarNetPath;
				 m_nBackTime=geoproject.nBackTime;
				 m_bFailConvergaeWriteToDB=geoproject.bFailConvergaeWriteToDB;
				 m_bUpdateCoordIfLowerBound=geoproject.bUpdateCoordIfLowerBound;
				 m_vecNodes=geoproject.vecGeoNodes;
				 m_vecOrientation=geoproject.vecGeoOrientations;
				 m_vecOffset=geoproject.vecGeoOffsets;

				 m_strStartTime=geoproject.strStartTime;
				 m_strIntervalTime=geoproject.strIntervalTime;
				 m_strEndTime=geoproject.strEndTime;

				 m_GridProjectSel.SetItemText(m_nSelectedRowNum,3,m_strStartTime);
				 m_GridProjectSel.SetItemText(m_nSelectedRowNum,4,m_strIntervalTime);
				 m_GridProjectSel.SetItemText(m_nSelectedRowNum,5,m_strEndTime);				
			 }

			 m_GridProjectSel.Refresh();
			
		 }
		 
		 SetAllCtrlState();
		 UpdateData(FALSE);
	 }
	if (pItem->iRow==0)
	{
		ClearRowSelectdColor(m_GridProjectSel);
	}

}

void CDlgMonitor::OnGridStartEdit( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (pItem->iRow>0)
	{
		if (pItem->iColumn==1)
		{
			m_strlastSelectedState=m_GridProjectSel.GetItemText(pItem->iRow,1);
		}


		if(pItem->iColumn==2)
		{
			m_nLastSelectedRowNum=pItem->iRow;
			m_strLastSelectedProjectName=m_GridProjectSel.GetItemText(pItem->iRow,2);

		}

	}
	

}

void CDlgMonitor::OnGridEndEdit( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (pItem->iRow>0)
	{
		if (pItem->iColumn==1)
		{
			
			CString selectedState=m_GridProjectSel.GetItemText(pItem->iRow,1);
			if ((selectedState!=_T("是"))&&(selectedState!=_T("否")))
			{
				m_GridProjectSel.SetItemText(pItem->iRow,1,m_strlastSelectedState);
			}
			CString projectName=m_GridProjectSel.GetItemText(pItem->iRow,2);
			if (selectedState==_T("是"))
			{
				m_mapGeoProject[projectName].bSelectedPrj=true;
			} 
			else
			{
				m_mapGeoProject[projectName].bSelectedPrj=false;
			}

			m_mapGeoProject[projectName].bSaveSuc=false;

		}

		if (pItem->iColumn==2)
		{
			CString projectName=m_GridProjectSel.GetItemText(pItem->iRow,2);
			if (projectName!=_T(""))
			{
				if (m_mapGeoProject.count(projectName)==1)//存在该名称
				{
					if (projectName!=m_strLastSelectedProjectName)//不是原有名称
					{
						m_bDuplicateName=TRUE;
						m_GridProjectSel.SetItemText(pItem->iRow,2,m_strLastSelectedProjectName);//输入原有名称
						m_mapGeoProject[m_strLastSelectedProjectName].bSaveSuc=false;
					}
				}
				else
				{
					GeoPojectStruct oldProjectStruct=m_mapGeoProject[m_strLastSelectedProjectName];
					oldProjectStruct.strPrjName=projectName;
					m_mapGeoProject.erase(m_strLastSelectedProjectName);
					m_mapGeoProject.insert(make_pair(projectName,oldProjectStruct));
					m_mapGeoProject[projectName].bSaveSuc=false;

				}

			}
			else //名称为空，变为原有名称
			{
				m_GridProjectSel.SetItemText(pItem->iRow,2,m_strLastSelectedProjectName);//输入原有名称


			}
		}
		m_GridProjectSel.Refresh();
		ClearRowSelectdColor(m_GridProjectSel);
	}

	

}

void CDlgMonitor::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

void CDlgMonitor::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	/*CDialog::OnOK();*/
}

void CDlgMonitor::OnEnChangeeditreportpath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].strReportPath=m_strReportPath;

	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	UpdateData(FALSE);
	
}

void CDlgMonitor::OnEnChangeeditdatapath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].strDataPath=m_strDataPath;

	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	UpdateData(FALSE);

}

void CDlgMonitor::OnEnChangeeditprojectpath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].strPrjPath=m_strPrjPath;

	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	UpdateData(FALSE);
}

void CDlgMonitor::OnEnChangeeditstarnetpath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].strStarNetPath=m_strStarNetPath;

	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	UpdateData(FALSE);
}

void CDlgMonitor::OnEnChangeeditseachtime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString projectName=m_GridProjectSel.GetItemText(m_nSelectedRowNum,2);
	m_mapGeoProject[projectName].nBackTime=m_nBackTime;

	//状态变化
	RowHasChangedState(m_GridProjectSel,projectName,m_nSelectedRowNum);
	UpdateData(FALSE);
}





