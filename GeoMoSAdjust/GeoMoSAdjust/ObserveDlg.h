#pragma once

#include "GridCtrl.h"
#include "GeoAdjustStruct.h"
#include <vector>
#include <algorithm>

// CObserveDlg dialog
using namespace std;


class CObserveDlg : public CDialog
{
	DECLARE_DYNAMIC(CObserveDlg)

public:
	CObserveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CObserveDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DlgObserveCfg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:

	void InitGridCtrl(CGridCtrl &gridCtrl); //初始化一个列表控件
	void InitGridCtrl(CGridCtrl &gridCtrl,
						int row, 
						int column, 
						vector<CString>vecColumnStr);
	void InitNodeGridCtrl(CGridCtrl &gridCtrl,
							int row, 
							int column, 
						vector<CString>vecColumnStr);
	void InitOrientationGridCtrl(CGridCtrl &gridCtrl,
		int row, 
		int column, 
		vector<CString>vecColumnStr);
	void InitOffsetGridCtrl(CGridCtrl &gridCtrl,
		int row, 
		int column, 
		vector<CString>vecColumnStr);
	void InitGridTitles();//读入各列表控件的标题

	void InsertRow(CGridCtrl &gridCtrl);//列表进行添加操作
	void DeleteRow(CGridCtrl &gridCtrl,int rowNum);//列表进行删除操作,rowNum表示要删除的行
	void SetComboBoxCell(CGridCtrl &gridCtrl,int comboBoxColumn); //设置某个单元为combobox的形式	
	void SetComboBoxCell(CGridCtrl &gridCtrl,int comboBoxColumn,const vector<CString> &vecComboOption,int nDefaultOptionIndex); 
	void GetNodeNames(const CGridCtrl &gridCtrl, CString nodeType,int typeColumn,vector<CString> &vecNodeNames);

	void SetDefaultNodeName();//设置默认的点名
	void SetDefaultOrientation();//设置默认的目标点名
	void SetDefaultOffset();//设置默认的点位移

	void SaveNodes();//保存点名信息
	void SaveOrientations();//保存观测方向信息
	void SaveOffset();//保存点位偏移信息

	bool IsNameExisited(const CGridCtrl &gridCtrl,CString searchName,int columnNum);//名字是否存在某一列

public:
	CString m_strSelProjectName;//所选的项目名称

	CGridCtrl m_oNodeManager;//点名管理
	vector<CString>m_vecNodeTitles;//点名标题字符串集
	vector<GeoNode>m_vecNodes;//点名内容
	set<CString>m_setNodeNames;//点名map集，防止点名重复
	int m_nSelNodeRowNum;//选中的行数
	CString m_strLastSelectedNodeName;//修改前的点名称
	CString m_strLastNodeType;//修改前的点类型
	int m_nLastSelectedRowNum;//上一次选中的行
	BOOL m_bDuplicateNodeName;//是否出现Node Name重名的标志

	CGridCtrl m_oOrientationManager;//观测方向
	vector<CString>m_vecOrientationTitles;//观测方向标题字符串集
	vector<GeoOrientation>m_vecOrientation;//观测方向内容
	int m_nSelOrientationRowNum;////选中的行数
	GeoOrientation m_lastGeoOri;//修改前的观测方向
	int m_nLastSelectedOrientationRowNum;//上一次选中的行
	BOOL m_bDuplicateOrientation;//是否出现Offset Name重名的标志
	

	CGridCtrl m_oOffsetManager;//点位偏移
	vector<CString>m_vecOffsetTitles;//点位偏移标题字符串集
	vector<GeoOffset>m_vecOffset;//点位偏移内容
	set<CString>m_setUsedPoint;//起点终点set集,防止点重复
	int m_nSelOffsetRowNum;////选中的行数
	CString m_strLastSelectedOffsetName;//修改前的点名称
	int m_nLastSelectedOffsetRowNum;//上一次选中的行
	int m_nLastSelectedOffsetColumnNum;//上一次选中的列
	BOOL m_bDuplicateOffsetName;//是否出现Offset Name重名的标志	
	
public:
	afx_msg void OnBnClickedBtninsertnode();//增加点名
	afx_msg void OnBnClickedBtndeletenode();//删除点名
	afx_msg void OnBnClickedBtninsertorientation();//增加观测方向
	afx_msg void OnBnClickedBtndeleteorientation();//删除观测方向
	afx_msg void OnBnClickedBtninsertoffset();//增加点位偏移
	afx_msg void OnBnClickedBtndeleteoffset();//删除点位偏移
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnGridNodeStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult); //点名管理单元格编辑开始时
	afx_msg void OnGridNodeEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult); //点名管理单元格编辑结束时
	afx_msg void OnGridNodeClick(NMHDR *pNotifyStruct, LRESULT* pResult);//点名管理单击单元格
	afx_msg void OnGridOrientationStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult); //观测方向单元格编辑开始时
	afx_msg void OnGridOrientationEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult); //观测方向单元格编辑结束时
	afx_msg void OnGridOrientationClick(NMHDR *pNotifyStruct, LRESULT* pResult);//观测方向单击单元格
	afx_msg void OnGridOffsetClick(NMHDR *pNotifyStruct, LRESULT* pResult);//点位偏移单击单元格
	afx_msg void OnGridOffsetStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult);//点位偏移单元格开始编辑时
	afx_msg void OnGridOffsetEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);//点位偏移单元格编辑结束时
};
