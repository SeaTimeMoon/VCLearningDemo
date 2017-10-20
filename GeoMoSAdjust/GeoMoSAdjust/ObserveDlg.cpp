// ObserveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeoMoSAdjust.h"
#include "ObserveDlg.h"
#include "NewCellTypes/GridCellCombo.h"
#include "FindX.h"


// CObserveDlg dialog

IMPLEMENT_DYNAMIC(CObserveDlg, CDialog)

CObserveDlg::CObserveDlg(CWnd* pParent /*=NULL*/)
: CDialog(CObserveDlg::IDD, pParent)
	,m_bDuplicateNodeName(false)
	,m_bDuplicateOffsetName(false)
	,m_bDuplicateOrientation(false)
{
	InitGridTitles();

}

CObserveDlg::~CObserveDlg()
{
}

void CObserveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GridNodeManager,m_oNodeManager);
	DDX_Control(pDX,IDC_GridOrientation,m_oOrientationManager);
	DDX_Control(pDX,IDC_GridOffset,m_oOffsetManager);
}

// CObserveDlg message handlers

BOOL CObserveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//对话框标题设置
	m_strSelProjectName=_T("观测设置———")+m_strSelProjectName;
	SetWindowText(m_strSelProjectName);
		
	InitNodeGridCtrl(m_oNodeManager,m_vecNodes.size()+1,2,m_vecNodeTitles);
	InitOrientationGridCtrl(m_oOrientationManager,m_vecOrientation.size()+1,2,m_vecOrientationTitles);
	InitOffsetGridCtrl(m_oOffsetManager,m_vecOffset.size()+1,5,m_vecOffsetTitles);
	//自动排序
	m_oNodeManager.SetHeaderSort(TRUE);
	m_oOrientationManager.SetHeaderSort(TRUE);
	m_oOffsetManager.SetHeaderSort(TRUE);

	//数据校验部分
	for (vector<GeoNode>::iterator iter=m_vecNodes.begin();iter!=m_vecNodes.end();iter++)
	{
		CString nodeNames=iter->strNodeName;
		m_setNodeNames.insert(nodeNames);
	}
	
	for (vector<GeoOffset>::iterator iter=m_vecOffset.begin();iter!=m_vecOffset.end();iter++)
	{
		CString strStartPoint=iter->strStartPoint;
		m_setUsedPoint.insert(strStartPoint);
		CString strEndPoint=iter->strEndPoint;
		m_setUsedPoint.insert(strEndPoint);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CObserveDlg, CDialog)
	ON_BN_CLICKED(IDC_BtnInsertNode, &CObserveDlg::OnBnClickedBtninsertnode)
	ON_BN_CLICKED(IDC_BtnDeleteNode, &CObserveDlg::OnBnClickedBtndeletenode)
	ON_BN_CLICKED(IDC_BtnInsertOrientation, &CObserveDlg::OnBnClickedBtninsertorientation)
	ON_BN_CLICKED(IDC_BtnDeleteOrientation, &CObserveDlg::OnBnClickedBtndeleteorientation)
	ON_BN_CLICKED(IDC_BtnInsertOffset, &CObserveDlg::OnBnClickedBtninsertoffset)
	ON_BN_CLICKED(IDC_BtnDeleteOffset, &CObserveDlg::OnBnClickedBtndeleteoffset)
	ON_WM_PAINT()
	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GridNodeManager, OnGridNodeStartEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GridNodeManager, OnGridNodeEndEdit)
	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GridOrientation, OnGridOrientationStartEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GridOrientation, OnGridOrientationEndEdit)
	ON_NOTIFY(NM_CLICK, IDC_GridNodeManager, OnGridNodeClick)
	ON_NOTIFY(NM_CLICK, IDC_GridOrientation, OnGridOrientationClick)
	ON_NOTIFY(NM_CLICK, IDC_GridOffset, OnGridOffsetClick)
	ON_BN_CLICKED(IDOK, &CObserveDlg::OnBnClickedOk)
	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GridOffset, OnGridOffsetStartEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GridOffset, OnGridOffsetEndEdit)
END_MESSAGE_MAP()

void CObserveDlg::InitGridCtrl( CGridCtrl &gridCtrl )
{
	gridCtrl.SetEditable(true);
	gridCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xFF));//黄色背景
	gridCtrl.SetGridLineColor(RGB(0x00, 0x00, 0x00));

	gridCtrl.SetRowCount(8); //初始为10行
	gridCtrl.SetColumnCount(8); //初始化为11列
	gridCtrl.SetFixedRowCount(1); //表头为一行
	gridCtrl.SetFixedColumnCount(1); //表头为一列 

	for (int row = 0; row < gridCtrl.GetRowCount(); row++)
		for (int col = 0; col < gridCtrl.GetColumnCount(); col++)
		{ 
			//设置表格显示属性
			GV_ITEM Item; 
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			gridCtrl.SetRowHeight(row,25); //设置各行高          
			gridCtrl.SetColumnWidth(0,64); //设置0列宽 
			gridCtrl.SetColumnWidth(col,64); //设置各列宽

			if(row==0&&col==0) //第(0，0)格
			{
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("报表显示"),col);
			}
			else if (row < 1) //设置0行表头显示
			{        
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T(" 项目%d"),col);
			}
			else if (col < 1) //设置0列表头显示
			{
				if(row< gridCtrl.GetRowCount())
				{
					Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
					Item.strText.Format(_T("第%d次"),row);
				}
			}
			else
			{
				Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
				Item.strText.Format(_T(""),2);
			}
			gridCtrl.SetItem(&Item); 
		}



}

void CObserveDlg::InitGridCtrl( CGridCtrl &gridCtrl,int row,int column,vector<CString>vecColumnStr)
{
	gridCtrl.SetEditable(true);
	gridCtrl.SetTextBkColor(RGB(0xFF, 0xFF, 0xFF));//白色背景
	gridCtrl.SetGridLineColor(RGB(0xAA, 0xAA, 0xAA));

	gridCtrl.SetRowCount(row); //初始化行
	gridCtrl.SetColumnCount(column); //初始化列
	gridCtrl.SetFixedRowCount(1); //表头为一行

	for (int row = 0; row < gridCtrl.GetRowCount(); row++)
		for (int col = 0; col < gridCtrl.GetColumnCount(); col++)
		{ 
			//设置表格显示属性
			GV_ITEM Item; 
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			gridCtrl.SetRowHeight(row,25); //设置各行高          
			gridCtrl.SetColumnWidth(0,74); //设置0列宽 
			gridCtrl.SetColumnWidth(col,74); //设置各列宽

			if (row < 1) //设置第1行的表头
			{
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				if (col<(int)vecColumnStr.size())
				{
					Item.strText=vecColumnStr[col];
				}
				else
				{
					Item.strText.Format(_T(""));

				}
			}
			else
			{
				
				Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
				Item.strText.Format(_T(""),2);
			}
			gridCtrl.SetItem(&Item); 
		}

}

void CObserveDlg::InitNodeGridCtrl( CGridCtrl &gridCtrl, int row, int column, vector<CString>vecColumnStr )
{
	InitGridCtrl(gridCtrl,row,column,vecColumnStr);
	for (int r=1;r<row;r++)
		for(int c=0;c<column;c++)
		{
			GeoNode node=m_vecNodes[r-1];
			if (c==0)
			{
				gridCtrl.SetItemText(r,c,node.strNodeName);
			} 
			else
			{
				gridCtrl.SetItemText(r,c,node.strNodeType);
			}
		}
	vector<CString>vecOptionStr;
	vecOptionStr.push_back(_T("已知点"));
	vecOptionStr.push_back(_T("测站点"));
	vecOptionStr.push_back(_T("浮动点"));
	SetComboBoxCell(gridCtrl,1,vecOptionStr,-1);
	gridCtrl.Refresh();
}

void CObserveDlg::InitOrientationGridCtrl( CGridCtrl &gridCtrl, int row, int column, vector<CString>vecColumnStr )
{
	InitGridCtrl(gridCtrl,row,column,vecColumnStr);
	for (int r=1;r<row;r++)
		for(int c=0;c<column;c++)
		{
			GeoOrientation ori=m_vecOrientation[r-1];
			if (c==0)
			{
				gridCtrl.SetItemText(r,c,ori.strStationPoint);
			} 
			else
			{
				gridCtrl.SetItemText(r,c,ori.strTargetPoint);
			}
		}
	vector<CString>vecOrientationOption;
	GetNodeNames(m_oNodeManager,_T("测站点"),1,vecOrientationOption);
	SetComboBoxCell(gridCtrl,0,vecOrientationOption,-1);
	gridCtrl.Refresh();

}

void CObserveDlg::InitOffsetGridCtrl( CGridCtrl &gridCtrl, int row, int column, vector<CString>vecColumnStr )
{
	InitGridCtrl(gridCtrl,row,column,vecColumnStr);
	for (int r=1;r<row;r++)
		for(int c=0;c<column;c++)
		{
			GeoOffset offset=m_vecOffset[r-1];
			switch(c)
			{
			case 0:
				gridCtrl.SetItemText(r,c,offset.strStartPoint);
				break;
			case 1:
				gridCtrl.SetItemText(r,c,offset.strEndPoint);
				break;
			case 2:
				gridCtrl.SetItemText(r,c,offset.strEastCoordError);
				break;
			case 3:
				gridCtrl.SetItemText(r,c,offset.strNorthCoordError);
				break;
			case 4:
				gridCtrl.SetItemText(r,c,offset.strElevationDiff);
				break;
			}
		}
		gridCtrl.Refresh();

}

void CObserveDlg::InitGridTitles()
{
	
	m_vecNodeTitles.push_back(_T("点名"));
	m_vecNodeTitles.push_back(_T("点类型"));
	
	m_vecOrientationTitles.push_back(_T("观测点名"));
	m_vecOrientationTitles.push_back(_T("目标点名"));

	m_vecOffsetTitles.push_back(_T("起点"));
	m_vecOffsetTitles.push_back(_T("终点"));
	m_vecOffsetTitles.push_back(_T("东坐标差dE"));
	m_vecOffsetTitles.push_back(_T("北坐标差dN"));
	m_vecOffsetTitles.push_back(_T("高程差dH"));
	
}


void CObserveDlg::SetDefaultNodeName()
{
	int nNodeCount=m_oNodeManager.GetRowCount();
	int newRowNum=nNodeCount-1;//新增的行
	CString strNodeName=_T("");	
	strNodeName.Format(_T("Node%d"),newRowNum);
	if(m_setNodeNames.count(strNodeName)==1)//已存在该名称
	{
		strNodeName+=SUFFIX;//添加后缀名称
	}
	m_oNodeManager.SetItemText(newRowNum,0,strNodeName);
	CString strNodeType=_T("测站点");
	m_oNodeManager.SetItemText(newRowNum,1,strNodeType);

	m_setNodeNames.insert(strNodeName);

	//不需增加，因为最终保存会清空后保存
	//增加新的信息
	/*GeoNode node;
	node.strNodeName=strNodeName;
	node.strNodeType=m_oNodeManager.GetItemText(newRowNum,1);
	m_vecNodes.push_back(node);*/

}

void CObserveDlg::SetDefaultOrientation()
{
	vector<CString>vecStr;
	GetNodeNames(m_oNodeManager,_T("测站点"),1,vecStr);
	if(!vecStr.empty())
	{
		int nTargetNodeCount=m_oOrientationManager.GetRowCount();
		int newRowNum=nTargetNodeCount-1;//新增的行

		CString defaultOptionStr=vecStr[0];
		m_oOrientationManager.SetItemText(newRowNum,0,defaultOptionStr);
		
		CString strTargetNodeName=_T("");
		strTargetNodeName.Format(_T("TargetND%d"),newRowNum);
		GeoOrientation ori;
		ori.strStationPoint=defaultOptionStr;
		ori.strTargetPoint=strTargetNodeName;
		//判断是否有重复的观测方向
		vector<GeoOrientation>::iterator iter=find(m_vecOrientation.begin(),m_vecOrientation.end(),ori);
		if (iter!=m_vecOrientation.end())
		{
			strTargetNodeName+=SUFFIX;
		}
		ori.strTargetPoint=strTargetNodeName;
		m_oOrientationManager.SetItemText(newRowNum,1,strTargetNodeName);
		m_oOrientationManager.Refresh();
		m_vecOrientation.push_back(ori);

		
	}
	else
	{
		AfxMessageBox(_T("测站点为空，请设置测站点"));
		return;

	}

	

}

void CObserveDlg::SetDefaultOffset()
{
	int nOffsetCount=m_oOffsetManager.GetRowCount();
	int newRow=nOffsetCount-1;//新增的行
	int nColumnCount=m_oOffsetManager.GetColumnCount();
	CString strStartPoint; //起点
	CString strEndPoint;//终点
	CString strEastCoordError;//东坐标差
	CString strNorthCoordError;//北坐标差
	CString strElevationDiff;//高程差

	CString strItemText;
	for (int col=0;col<nColumnCount;col++)
	{
		switch(col)
		{
		case 0:
			strItemText.Format(_T("S%d"),newRow);
			if (m_setUsedPoint.count(strItemText)==1)//已存在该名称
			{
				strItemText+=SUFFIX;
			}
			break;
		case 1:
			strItemText.Format(_T("S%db"),newRow);
			if (m_setUsedPoint.count(strItemText)==1)//已存在该名称
			{
				strItemText+=SUFFIX;
			}
			break;
		case 2:
		case 3:
		case 4:
			strItemText=_T("-0.0000");
			break;
		}

		m_oOffsetManager.SetItemText(newRow,col,strItemText);
		if (col==0||col==1)
		{
			m_setUsedPoint.insert(strItemText);
		}
	}
	m_oOffsetManager.Refresh();

	//不需增加，因为最终保存会清空后保存
	//增加新的信息
	//GeoOffset geooffset;
	//geooffset.strStartPoint=m_oOffsetManager.GetItemText(newRow,0);
	//geooffset.strEndPoint=m_oOffsetManager.GetItemText(newRow,1);
	//geooffset.strEastCoordError=m_oOffsetManager.GetItemText(newRow,2);
	//geooffset.strNorthCoordError=m_oOffsetManager.GetItemText(newRow,3);
	//geooffset.strElevationDiff=m_oOffsetManager.GetItemText(newRow,4);

	//m_vecOffset.push_back(geooffset);
		
	
}

void CObserveDlg::SaveNodes()
{
	int nNodesCount=m_oNodeManager.GetRowCount();
	GeoNode node;
	if (!m_vecNodes.empty())//如果非空，清空数据
	{
		vector<GeoNode>().swap(m_vecNodes);
	}
	for (int row=1;row<nNodesCount;row++)
	{
		node.strNodeName=m_oNodeManager.GetItemText(row,0);
		node.strNodeType=m_oNodeManager.GetItemText(row,1);
		m_vecNodes.push_back(node);
	}

}

void CObserveDlg::SaveOrientations()
{
	int nOrientationCount=m_oOrientationManager.GetRowCount();
	GeoOrientation orientation;
	if (!m_vecOrientation.empty())//如果非空，清空数据
	{
		vector<GeoOrientation>().swap(m_vecOrientation);
	}
	for (int row=1;row<nOrientationCount;row++)
	{
		orientation.strStationPoint=m_oOrientationManager.GetItemText(row,0);
		orientation.strTargetPoint=m_oOrientationManager.GetItemText(row,1);
		m_vecOrientation.push_back(orientation);
	}

}

void CObserveDlg::SaveOffset()
{
	int nOffsetCount=m_oOffsetManager.GetRowCount();
	GeoOffset offset;
	if (!m_vecOffset.empty())//如果非空，清空数据
	{
		vector<GeoOffset>().swap(m_vecOffset);
	}
	for (int row=1;row<nOffsetCount;row++)
	{
		offset.strStartPoint=m_oOffsetManager.GetItemText(row,0);
		offset.strEndPoint=m_oOffsetManager.GetItemText(row,1);
		offset.strEastCoordError=m_oOffsetManager.GetItemText(row,2);
		offset.strNorthCoordError=m_oOffsetManager.GetItemText(row,3);
		offset.strElevationDiff=m_oOffsetManager.GetItemText(row,4);
		m_vecOffset.push_back(offset);
	}

}

bool CObserveDlg::IsNameExisited( const CGridCtrl &gridCtrl,CString searchName,int columnNum )
{
	vector<CString>vecStrName;
	CString strName;
	for (int row=1;row<gridCtrl.GetRowCount();row++)
	{
		strName=gridCtrl.GetItemText(row,columnNum);
		vecStrName.push_back(strName);		
	}
	vector<CString>::iterator iter=find(vecStrName.begin(),vecStrName.end(),searchName);
	if (iter==vecStrName.end())
	{
		return false;//没找到
	} 
	else
	{
		return true; //找到
	}

}

void CObserveDlg::OnBnClickedBtninsertnode()
{
	// TODO: Add your control notification handler code here
	InsertRow(m_oNodeManager);
	vector<CString>vecOptionStr;//下拉框字符串
	vecOptionStr.push_back(_T("已知点"));
	vecOptionStr.push_back(_T("测站点"));
	vecOptionStr.push_back(_T("浮动点"));
	SetComboBoxCell(m_oNodeManager,1,vecOptionStr,-1);//1列设置为combobox的形式

	SetDefaultNodeName();//设置默认的点名

	//对观测方向测站点名进行刷新
	vector<CString> vecStationNodeNames;
	GetNodeNames(m_oNodeManager,_T("测站点"),1,vecStationNodeNames);
	if (!vecStationNodeNames.empty())
	{
		SetComboBoxCell(m_oOrientationManager,0,vecStationNodeNames,-1);//0列设置为combobox的形式
	}
	
}

void CObserveDlg::OnBnClickedBtndeletenode()
{
	// TODO: Add your control notification handler code here
	CString nodeName=m_oNodeManager.GetItemText(m_nSelNodeRowNum,0);
	CString nodeType=m_oNodeManager.GetItemText(m_nSelNodeRowNum,1);
	m_setNodeNames.erase(nodeName);
	
	DeleteRow(m_oNodeManager,m_nSelNodeRowNum);
	
	vector<CString> vecStationNodeNames;
	GetNodeNames(m_oNodeManager,_T("测站点"),1,vecStationNodeNames);

	//对观测方向测站点名进行刷新
	if (!vecStationNodeNames.empty())
	{
		if(nodeType==_T("测站点"))//观测方向中删除对应的已不存在观测点,用已存在的观测点中的第1个代替
		{
			vector<GeoOrientation>::iterator iter=find_if(m_vecOrientation.begin(),m_vecOrientation.end(),CFindX(nodeName));
			while (iter!=m_vecOrientation.end())
			{
				GeoOrientation geoori;
				geoori.strStationPoint=vecStationNodeNames[0];
				geoori.strTargetPoint=iter->strTargetPoint;
				m_vecOrientation.erase(iter);
				m_vecOrientation.push_back(geoori);
				//已经删除了重复的GeoOrientation,所以m_vecOrientaition已经改变
				iter=find_if(m_vecOrientation.begin(),m_vecOrientation.end(),CFindX(nodeName));
			}

		}

		SetComboBoxCell(m_oOrientationManager,0,vecStationNodeNames,-2);//0列设置为combobox的形式
	}
	else
	{
		int OrientationRow=m_oOrientationManager.GetRowCount();
		for (int row=OrientationRow;row>0;row--)
		{
			DeleteRow(m_oOrientationManager,row);
		}

	}

	m_oOrientationManager.Refresh();
	
}

void CObserveDlg::OnBnClickedBtninsertorientation()
{
	// TODO: Add your control notification handler code here
	vector<CString> vecStationNodeNames;
	GetNodeNames(m_oNodeManager,_T("测站点"),1,vecStationNodeNames);
	if (vecStationNodeNames.empty())
	{
		AfxMessageBox(_T("测站点为空，请设置测站点"));
	}
	else
	{
		InsertRow(m_oOrientationManager);
		SetComboBoxCell(m_oOrientationManager,0,vecStationNodeNames,-1);//0列设置为combobox的形式
	}

	SetDefaultOrientation();//设置默认的目标点名
	
}



void CObserveDlg::OnBnClickedBtndeleteorientation()
{
	// TODO: Add your control notification handler code here
	GeoOrientation geoOri;
	geoOri.strStationPoint=m_oOrientationManager.GetItemText(m_nSelOrientationRowNum,0);
	geoOri.strTargetPoint=m_oOrientationManager.GetItemText(m_nSelOrientationRowNum,1);
	vector<GeoOrientation>::iterator iter=find(m_vecOrientation.begin(),m_vecOrientation.end(),geoOri);
	if (iter!=m_vecOrientation.end())
	{
		m_vecOrientation.erase(iter);
	}

	DeleteRow(m_oOrientationManager,m_nSelOrientationRowNum);
	
}

void CObserveDlg::OnBnClickedBtninsertoffset()
{
	// TODO: Add your control notification handler code here
	InsertRow(m_oOffsetManager);
	SetDefaultOffset();//设置默认的点位偏移参数
}

void CObserveDlg::OnBnClickedBtndeleteoffset()
{
	// TODO: Add your control notification handler code here
	CString startPoint=m_oOffsetManager.GetItemText(m_nSelOffsetRowNum,0);
	CString endPoint=m_oOffsetManager.GetItemText(m_nSelOffsetRowNum,1);
	m_setUsedPoint.erase(startPoint);
	m_setUsedPoint.erase(endPoint);
	DeleteRow(m_oOffsetManager,m_nSelOffsetRowNum);
	//不需减少，因为最终保存会清空后保存
	/*vector<GeoOffset>::iterator iter=m_vecOffset.begin();
	m_vecOffset.erase(iter+m_nSelOffsetRowNum-1);*/
}




void CObserveDlg::InsertRow( CGridCtrl &gridCtrl )
{
	gridCtrl.InsertRow(_T(""));
	gridCtrl.Refresh();

}

void CObserveDlg::DeleteRow( CGridCtrl &gridCtrl ,int rowNum)
{
	int rowCount=gridCtrl.GetRowCount();//行数
	int selRowNum=rowNum;//选中的行

	if (rowCount>1)
	{
		gridCtrl.DeleteRow(selRowNum);
	}
	else
	{
		AfxMessageBox(_T("没有数据删除！"));
	}

	gridCtrl.Refresh();

}


void CObserveDlg::SetComboBoxCell(CGridCtrl &gridCtrl, int comboBoxColumn )
{
	
	for (int row = 0; row < gridCtrl.GetRowCount(); row++)
		for (int col = 0; col < gridCtrl.GetColumnCount(); col++)
		{
			if(row!=0)
			{
				if (!gridCtrl.SetCellType(row,comboBoxColumn, RUNTIME_CLASS(CGridCellCombo)))
					return;

				/*gridCtrl.SetItemText(row,comboBoxColumn, _T(""));*/

				CStringArray options;
				options.Add(_T("Option 1"));
				options.Add(_T("Option 2"));
				options.Add(_T("Option 3"));

				CGridCellCombo *pCell = (CGridCellCombo*) gridCtrl.GetCell(row,comboBoxColumn);
				pCell->SetOptions(options);
				pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

			}
	
		}

}

//********* //设置某个单元为combobox的形式***************
	// Method:    SetComboBoxCell
	// FullName:  CObserveDlg::SetComboBoxCell
	// Access:    public 
	// Returns:   void
	// Parameter: CGridCtrl & gridCtrl
	// Parameter: int comboBoxColumn      //comboBox所在的列 
	// Parameter: vector<CString>vecComboOption  //combobox中的字符选项
	// Parameter: int nDefaultOptionIndex      //comboBox所在的列的默认选项//-1表示无需默认参数,-2用默认选项覆盖原有的没有的选择项
	//************************************
void CObserveDlg::SetComboBoxCell( CGridCtrl &gridCtrl,int comboBoxColumn,const vector<CString> &vecComboOption,int nDefaultOptionIndex )
{
	//下拉框选项文字
	CStringArray options;
	for (vector<CString>::const_iterator iter=vecComboOption.begin();iter!=vecComboOption.end();iter++)
	{
		options.Add(*iter);
	}
	for (int row = 0; row < gridCtrl.GetRowCount(); row++)
	{
		if(row!=0)
		{
			if (!gridCtrl.SetCellType(row,comboBoxColumn, RUNTIME_CLASS(CGridCellCombo)))
				return;
			CString strDefaultContent;
			if (nDefaultOptionIndex>=0)//-1表示无需默认参数,-2用默认选项覆盖原有的没有的选择项
			{
				strDefaultContent=options[nDefaultOptionIndex];
				gridCtrl.SetItemText(row,comboBoxColumn, strDefaultContent);
			}
			if (nDefaultOptionIndex==-2)
			{
				CString ItemTextOption;
				ItemTextOption=gridCtrl.GetItemText(row,comboBoxColumn);
				vector<CString>::const_iterator iter=vecComboOption.begin();
				iter=find(vecComboOption.begin(),vecComboOption.end(),ItemTextOption);
				if (iter==vecComboOption.end())
				{
					strDefaultContent=options[0];
					gridCtrl.SetItemText(row,comboBoxColumn, strDefaultContent);
					
				}
			}
			
			CGridCellCombo *pCell = (CGridCellCombo*) gridCtrl.GetCell(row,comboBoxColumn);
			pCell->SetOptions(options);
			pCell->SetStyle(CBS_DROPDOWN); //CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE

		}

	}

}

//*******获得某类型的点名***************
// Method:    GetNodeNames
// FullName:  CObserveDlg::GetNodeNames
// Access:    public 
// Returns:   void
// Parameter: CGridCtrl & gridCtrl
// Parameter: CString nodeType 点类型
// Parameter: int TypeColumn   点类型所在的列
// Parameter: vector<CString> & vecNodeNames 返回的点名
//************************************
void CObserveDlg::GetNodeNames(const CGridCtrl &gridCtrl, CString nodeType,int typeColumn,vector<CString> &vecNodeNames )
{
	if (!vecNodeNames.empty())//清空非空容器
	{
		vector<CString>().swap(vecNodeNames);
	}

	//得到所有符合情况的点名
	for (int row = 1; row < gridCtrl.GetRowCount(); row++)
	{
		if (gridCtrl.GetItemText(row,typeColumn).Compare(nodeType)==0)
		{
			vecNodeNames.push_back(gridCtrl.GetItemText(row,0));
		}

	}

}

void CObserveDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CObserveDlg::OnGridNodeStartEdit( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nLastSelectedRowNum=pItem->iRow;		
	m_strLastSelectedNodeName=m_oNodeManager.GetItemText(pItem->iRow,0);	
	m_strLastNodeType=m_oNodeManager.GetItemText(pItem->iRow,1);
}

void CObserveDlg::OnGridNodeEndEdit( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	vector<CString> vecStationNodeNames;

	if(pItem->iColumn==0)
	{
		//防止输入重复的名称
		CString nodeName=m_oNodeManager.GetItemText(pItem->iRow,0);
		if (nodeName!=_T(""))
		{
			if (m_setNodeNames.count(nodeName)==1)//存在该名称
			{
				if (nodeName!=m_strLastSelectedNodeName)//不是原有名称
				{
					m_bDuplicateNodeName=TRUE;
					m_oNodeManager.SetItemText(pItem->iRow,0,m_strLastSelectedNodeName);//输入原有名称
					m_oNodeManager.Refresh();
				}
			}
			else
			{
				//CString lastSelNodeType=m_oNodeManager.GetItemText(pItem->iRow,1);//编辑前的点类型
				if(m_strLastNodeType==_T("测站点"))//观测方向中删除对应的已不存在观测点,用已存在的观测点中的第1个代替
				{
					vector<GeoOrientation>::iterator iter=find_if(m_vecOrientation.begin(),m_vecOrientation.end(),CFindX(m_strLastSelectedNodeName));
					GetNodeNames(m_oNodeManager,_T("测站点"),1,vecStationNodeNames);
					if (!vecStationNodeNames.empty())
					{
						while (iter!=m_vecOrientation.end())
						{
							GeoOrientation geoori;
							geoori.strStationPoint=vecStationNodeNames[0];
							geoori.strTargetPoint=iter->strTargetPoint;
							m_vecOrientation.erase(iter);
							m_vecOrientation.push_back(geoori);
							//已经删除了重复的GeoOrientation,所以m_vecOrientaition已经改变
							iter=find_if(m_vecOrientation.begin(),m_vecOrientation.end(),CFindX(m_strLastSelectedNodeName));
						}
					}
				}

				m_setNodeNames.erase(m_strLastSelectedNodeName);
				m_setNodeNames.insert(nodeName);
				
			}
		} 
		else
		{
			m_oNodeManager.SetItemText(pItem->iRow,0,m_strLastSelectedNodeName);//输入原有名称
			m_oNodeManager.Refresh();
		}

	}
	if (pItem->iColumn==1)
	{
		CString nodeType=m_oNodeManager.GetItemText(pItem->iRow,1);
		if ((nodeType!=_T("测站点"))&&(nodeType!=_T("已知点"))&&(nodeType!=_T("浮动点")))
		{
			m_oNodeManager.SetItemText(pItem->iRow,1,m_strLastNodeType);
		}
		else
		{
			if (nodeType!=m_strLastNodeType)
			{
				if ((nodeType!=_T("测站点"))&&(m_strLastNodeType==_T("测站点")))
				{					
					vector<GeoOrientation>::iterator iter=find_if(m_vecOrientation.begin(),m_vecOrientation.end(),CFindX(m_strLastSelectedNodeName));
					GetNodeNames(m_oNodeManager,_T("测站点"),1,vecStationNodeNames);
					if (!vecStationNodeNames.empty())
					{
						while (iter!=m_vecOrientation.end())
						{
							GeoOrientation geoori;
							geoori.strStationPoint=vecStationNodeNames[0];
							geoori.strTargetPoint=iter->strTargetPoint;
							m_vecOrientation.erase(iter);
							m_vecOrientation.push_back(geoori);
							//已经删除了重复的GeoOrientation,所以m_vecOrientaition已经改变
							iter=find_if(m_vecOrientation.begin(),m_vecOrientation.end(),CFindX(m_strLastSelectedNodeName));
						}
					}
					
				}
			}

		}		
	}



	GetNodeNames(m_oNodeManager,_T("测站点"),1,vecStationNodeNames);
	if (!vecStationNodeNames.empty())
	{
		SetComboBoxCell(m_oOrientationManager,0,vecStationNodeNames,-2);//0列设置为combobox的形式
		m_oOrientationManager.Refresh();
	}
	else
	{
		int OrientationRow=m_oOrientationManager.GetRowCount();
		for (int row=OrientationRow;row>0;row--)
		{
			DeleteRow(m_oOrientationManager,row);
		}
		m_oOrientationManager.Refresh();

	}

}

void CObserveDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	SaveNodes();
	SaveOrientations();
	SaveOffset();

	OnOK();
}



void CObserveDlg::OnGridNodeClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	if (m_bDuplicateNodeName)
	{
		CString msg;
		m_oNodeManager.SetItemBkColour(m_nLastSelectedRowNum,0,RGB(255,0,0));
		m_oNodeManager.Refresh();
		msg.Format(_T("第%d行输入的点名修改失败，失败原因:点名称重复"),m_nLastSelectedRowNum);
		AfxMessageBox(msg);
		m_bDuplicateNodeName=FALSE;
	}
	else
	{
		for (int row=1;row<m_oNodeManager.GetRowCount();row++)
		{
			m_oNodeManager.SetItemBkColour(row,0);
		}
		m_oNodeManager.Refresh();
	}

	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nSelNodeRowNum=pItem->iRow;

}


void CObserveDlg::OnGridOrientationStartEdit( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nLastSelectedOrientationRowNum=pItem->iRow;
	m_lastGeoOri.strStationPoint=m_oOrientationManager.GetItemText(pItem->iRow,0);
	m_lastGeoOri.strTargetPoint=m_oOrientationManager.GetItemText(pItem->iRow,1);



}

void CObserveDlg::OnGridOrientationEndEdit( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	GeoOrientation geoOri;
	CString stationName=m_oOrientationManager.GetItemText(pItem->iRow,0);
	CString targetName=m_oOrientationManager.GetItemText(pItem->iRow,1);

	//下拉框不能为非测站点名
	if (pItem->iColumn==0)
	{
		vector<CString>vecStationNames;
		GetNodeNames(m_oNodeManager,_T("测站点"),1,vecStationNames);
		vector<CString>::iterator iter=find(vecStationNames.begin(),vecStationNames.end(),stationName);
		if (iter==vecStationNames.end())
		{
			m_oOrientationManager.SetItemText(pItem->iRow,0,m_lastGeoOri.strStationPoint);
			m_oOrientationManager.SetItemText(pItem->iRow,1,m_lastGeoOri.strTargetPoint);
		}

		geoOri.strStationPoint=stationName;
		geoOri.strTargetPoint=targetName;
		vector<GeoOrientation>::iterator iter1=find(m_vecOrientation.begin(),m_vecOrientation.end(),geoOri);
		if ((iter1!=m_vecOrientation.end())||(targetName==stationName))//存在该观测方向重复或目标点与观测点相同
		{
			if (iter1!=m_vecOrientation.end())//观测方向重复
			{
				m_bDuplicateOrientation=TRUE;
			}				
			m_oOrientationManager.SetItemText(pItem->iRow,0,m_lastGeoOri.strStationPoint);
			m_oOrientationManager.SetItemText(pItem->iRow,1,m_lastGeoOri.strTargetPoint);
		}
		else
		{
			iter1=find(m_vecOrientation.begin(),m_vecOrientation.end(),m_lastGeoOri);
			m_vecOrientation.erase(iter1);
			m_vecOrientation.push_back(geoOri);
		}

	}
	

	if (pItem->iColumn==1)
	{
		if (targetName!=_T(""))
		{
			geoOri.strStationPoint=stationName;
			geoOri.strTargetPoint=targetName;
			vector<GeoOrientation>::iterator iter=find(m_vecOrientation.begin(),m_vecOrientation.end(),geoOri);
			if ((iter!=m_vecOrientation.end())||(targetName==stationName))//存在该观测方向重复或目标点与观测点相同
			{
				if (iter!=m_vecOrientation.end())//观测方向重复
				{
					m_bDuplicateOrientation=TRUE;
				}				
				m_oOrientationManager.SetItemText(pItem->iRow,0,m_lastGeoOri.strStationPoint);
				m_oOrientationManager.SetItemText(pItem->iRow,1,m_lastGeoOri.strTargetPoint);
			}
			else
			{
				vector<GeoOrientation>::iterator it=find(m_vecOrientation.begin(),m_vecOrientation.end(),m_lastGeoOri);
				m_vecOrientation.erase(it);
				m_vecOrientation.push_back(geoOri);
			}
		} 
		else
		{
			m_oOrientationManager.SetItemText(pItem->iRow,0,m_lastGeoOri.strStationPoint);
			m_oOrientationManager.SetItemText(pItem->iRow,1,m_lastGeoOri.strTargetPoint);
		}
	}
		
	m_oOrientationManager.Refresh();


}

void CObserveDlg::OnGridOrientationClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	if (m_bDuplicateOrientation)
	{
		CString msg;
		m_oOrientationManager.SetItemBkColour(m_nLastSelectedOrientationRowNum,0,RGB(255,0,0));
		m_oOrientationManager.SetItemBkColour(m_nLastSelectedOrientationRowNum,1,RGB(255,0,0));
		m_oOrientationManager.Refresh();
		msg.Format(_T("第%d行设置的观测方向修改失败，失败原因:已存在该观测方向"),m_nLastSelectedOrientationRowNum);
		AfxMessageBox(msg);
		m_bDuplicateOrientation=FALSE;
	}
	else
	{
		for (int row=1;row<m_oOrientationManager.GetRowCount();row++)
		{
			m_oOrientationManager.SetItemBkColour(row,0);
			m_oOrientationManager.SetItemBkColour(row,1);
		}
		m_oOrientationManager.Refresh();
	}

	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nSelOrientationRowNum=pItem->iRow;


}

void CObserveDlg::OnGridOffsetClick( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (m_bDuplicateOffsetName)
	{
		CString msg;
		m_oOffsetManager.SetItemBkColour(m_nLastSelectedRowNum,m_nLastSelectedOffsetColumnNum,RGB(255,0,0));
		m_oOffsetManager.Refresh();
		msg.Format(_T("第%d行%d列输入的点名称修改失败，失败原因:该点名已被使用"),m_nLastSelectedRowNum,m_nLastSelectedOffsetColumnNum);
		AfxMessageBox(msg);
		m_bDuplicateOffsetName=FALSE;
	}
	else
	{
		for (int row=1;row<m_oOffsetManager.GetRowCount();row++)
		{
			m_oOffsetManager.SetItemBkColour(row,0);
			m_oOffsetManager.SetItemBkColour(row,1);
		}
		m_oOffsetManager.Refresh();
	}

	m_nSelOffsetRowNum=pItem->iRow;
}

void CObserveDlg::OnGridOffsetStartEdit( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if(pItem->iColumn==0||pItem->iColumn==1)
	{
		m_nLastSelectedRowNum=pItem->iRow;
		m_nLastSelectedOffsetColumnNum=pItem->iColumn;
		m_strLastSelectedOffsetName=m_oOffsetManager.GetItemText(pItem->iRow,pItem->iColumn);

	}

}

void CObserveDlg::OnGridOffsetEndEdit( NMHDR *pNotifyStruct, LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	
	if (pItem->iRow>0)
	{
		//对1,2列进行输入数检查
		CString pointName=m_oOffsetManager.GetItemText(pItem->iRow,pItem->iColumn);
		if (pointName!=_T(""))
		{
			if (m_setUsedPoint.count(pointName)==1)//存在该名称
			{
				if (pointName!=m_strLastSelectedOffsetName)//不是原有名称
				{
					m_bDuplicateOffsetName=TRUE;
					m_oOffsetManager.SetItemText(pItem->iRow,pItem->iColumn,m_strLastSelectedOffsetName);//输入原有名称
					m_oOffsetManager.Refresh();
				}
			}
			else
			{
				m_setUsedPoint.erase(m_strLastSelectedOffsetName);
				m_setUsedPoint.insert(pointName);

			}
		} 
		else
		{
			m_oOffsetManager.SetItemText(pItem->iRow,pItem->iColumn,m_strLastSelectedOffsetName);//输入原有名称
			m_oOffsetManager.Refresh();

		}
		

		//对3、4、5列进行输入检查，防止输入浮点数以外的字符
		if (pItem->iColumn>=2&&pItem->iColumn<=4)
		{
			CString ItemText;
			ItemText=m_oOffsetManager.GetItemText(pItem->iRow,pItem->iColumn);
			double fVal;
			fVal=_wtof(ItemText);
			CString strVal;
			strVal.Format(_T("%.4f"),fVal);

			if (fVal==0.0)//转换不成功
			{
				m_oOffsetManager.SetItemText(pItem->iRow,pItem->iColumn,_T("-0.0000"));

			}
			else //转换成功，输入4位浮点数
			{
				m_oOffsetManager.SetItemText(pItem->iRow,pItem->iColumn,strVal);

			}
		}
	}
	
	


}
