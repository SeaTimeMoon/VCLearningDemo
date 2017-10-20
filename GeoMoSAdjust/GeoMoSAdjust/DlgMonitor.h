#pragma once

#include "GridCtrl.h"
#include "ObserveDlg.h"
#include "NewCellTypes/GridCellDateTime.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellCombo.h"
#include <iostream>
#include "DateTimeDlg.h"
#include "TimeSpanDlg.h"
#include "GeoAdjustStruct.h"
#include <algorithm>
#include <Shlwapi.h>


// CDlgMonitor dialog

class CDlgMonitor : public CDialog
{
	DECLARE_DYNAMIC(CDlgMonitor)

public:
	CDlgMonitor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMonitor();

// Dialog Data
	enum { IDD = IDD_DlgMonitor };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	void InitGridCtrl(); //初始化项目选择列表控件
	
	CString CombineKeyAndValue(CString key,CString strValue); //将键和键值组合为“key=value”的形式
	void FormatStr(CString &orginStr,CString mark); //将字符串加上标记
	void WriteToFile(vector<CString>vecStr,CString FilePath,bool MsgFlag); //将字符串集合写入指定路径的文件MsgFlag为是否显示“保存成功”的标志位,true为显示,false为不显示
	void WriteGeoPrjToFile(GeoPojectStruct geoprj,CString separateStr,bool MsgFlag); //将一个prj文件写入ini文件(配置文件),加分割符,MsgFlag为是否显示“保存成功”的标志位,true为显示,false为不显示

	void EnableAllCtrlState(BOOL bState); //设置除列表框以外的所有控件的状态，可用或不可用
	void SetAllCtrlState();//根据项目数量设置除列表框以外的所有控件的状态，可用或不可用

	void EnableStarNetDiff(BOOL bState);//是否使用StarNet进行平差

	void SetComboBoxCell(CGridCtrl &gridCtrl,int comboBoxColumn,vector<CString>vecComboOption);
	//********* //设置某列单元为ReadOnly***************
	void SetReadOnlyCell(CGridCtrl &gridCtrl,int comboBoxColumn);
	void SetFirstColumnText(CGridCtrl &gridCtrl); //设置第一列的编号
	void SetRowSelected(CGridCtrl &gridCtrl,int row);//选中某一行,某行颜色为淡蓝色
	void ClearRowSelectdColor(CGridCtrl &gridCtrl);//清除选中行的颜色
	void Refresh(const GeoPojectStruct &geo);//刷新数据格式、路径、平差设置等信息
	void RowHasChangedState(CGridCtrl &gridCtrl,CString projectName,int selectedRow);//行的状态改变，由绿色变为白色，如果选中仍为选中色

	SaveFlag GetSelectedGeoProject(int selectedRow,GeoPojectStruct &geoPrj); //得到选中的项目

	void SetDefaultProject(int selecedRow);//设置默认项目信息

	CString ConvertDateTimeFormat(CString strNumberFormat);//将数字时间形式20000101000000转换为2000-01-01 00:00:00
	CString ConvertTimeFormat(CString strNumberFomat);//将数字时间形式000000转换为00:00:00

	COleDateTime ConvertTime(CString strEpoch);//将字符串转换为时间，如：2014-8-8 8:08:08 --> 2014 8 8 8:8:8
	COleDateTime ConvertTimeSpan(CString strEpoch);//将字符串转换为时间，如：8:08:08 -->8 8 8
	
	BOOL FindINIFiles(); //找到所有INI配置文件
	CString GetFileTitleFromFilePath(CString FileName);//从路径获得文件名
	BOOL VerifyINIFile(CString filePath, CString fileHeader); //判断是否包含文件头标志
	void ReadINIFile(CString strINIFilePath,GeoPojectStruct &geoPrj);//读取单个INI文件
	void ReadINIFiles(vector<CString> vecstrINIFilePath,MAPGeoProject &mapGeoProject);//将INI文件集读入内存
	short DivideString(CString strText, CString *strArray,int nCommaCount); // Divide String by ','

	void ClearNonexistentINIFiles();//清空INI文件夹下的不在列表框中的project文件

	SaveFlag m_saveflag;

	CString m_strPrjName;//项目名
	bool m_bSelectedPrj;//是否选用
	CString m_strStartTime;//开始时间
	CString m_strIntervalTime;//处理时间
	CString m_strEndTime;//结束时间

	BOOL m_bUsingStarNet; //checkBox 使用StarNet进行平差
	CString m_strReportPath; //报告存放的目录
	CString m_strDataPath; //dat文件所在的路径
	CString m_strPrjPath; //工程文件
	CString m_strStarNetPath;//StartNet应用程序选择
	
	int m_nBackTime;//回滚检索数据时间（分钟）
	BOOL m_bFailConvergaeWriteToDB;//是否收敛失败仍将结果写入GeoMoS数据库
	BOOL m_bUpdateCoordIfLowerBound;//是否卡方检验指标设置太低时更新坐标

	vector<GeoNode>m_vecNodes;//点名
	vector<GeoOrientation>m_vecOrientation;//观测方向
	vector<GeoOffset>m_vecOffset;//点位偏移

	vector<CString>m_vecINIFilePath; //配置文件的路径集
	MAPGeoProject m_mapGeoProject; //Project文件的map容器

	//控件相关
	CGridCtrl m_GridProjectSel;//项目选择列表控件
	//CObserveDlg m_oObserveDlg;//观测设置对话框
	int m_nProjectCount;//列表的行数，初始化为1,也是列表中的项目数
	int m_nSelectedRowNum; //选中的行号,用于保存文件

	CString m_strlastSelectedState;//编辑前的选用状态
	CString m_strLastSelectedProjectName;//编辑前的项目名称
	int m_nLastSelectedRowNum;
	BOOL m_bDuplicateName;//是否出现Project Name重名的标志


protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnnewprj();
	afx_msg void OnBnClickedBtndeleteprj();
	afx_msg void OnBnClickedBtnobservecfg();
	afx_msg void OnBnClickedBtnreportpathsel();
	afx_msg void OnBnClickedBtndatapathsel();
	afx_msg void OnBnClickedBtnprojectpathsel();
	afx_msg void OnBnClickedBtnstarnetpathsel();
	afx_msg void OnBnClickedCheckdifference();
	afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedBtnsave(); //保存设置的数据未ini文件
	afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);//双击单元格
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);//单击单元格
	afx_msg void OnGridStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult);//单元格开始编辑时
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);//单元格编辑结束时
	afx_msg void OnClose();

	virtual void OnOK();

	afx_msg void OnEnChangeeditreportpath();
	afx_msg void OnEnChangeeditdatapath();
	afx_msg void OnEnChangeeditprojectpath();
	afx_msg void OnEnChangeeditstarnetpath();
	afx_msg void OnEnChangeeditseachtime();
	afx_msg void OnBnClickedBtnsaveall();
public:
	afx_msg void OnBnClickedCheckwritedbfail();
	afx_msg void OnBnClickedCheckaxislow();
};
