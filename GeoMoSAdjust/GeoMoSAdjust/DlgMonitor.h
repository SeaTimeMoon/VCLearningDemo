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

	void InitGridCtrl(); //��ʼ����Ŀѡ���б�ؼ�
	
	CString CombineKeyAndValue(CString key,CString strValue); //�����ͼ�ֵ���Ϊ��key=value������ʽ
	void FormatStr(CString &orginStr,CString mark); //���ַ������ϱ��
	void WriteToFile(vector<CString>vecStr,CString FilePath,bool MsgFlag); //���ַ�������д��ָ��·�����ļ�MsgFlagΪ�Ƿ���ʾ������ɹ����ı�־λ,trueΪ��ʾ,falseΪ����ʾ
	void WriteGeoPrjToFile(GeoPojectStruct geoprj,CString separateStr,bool MsgFlag); //��һ��prj�ļ�д��ini�ļ�(�����ļ�),�ӷָ��,MsgFlagΪ�Ƿ���ʾ������ɹ����ı�־λ,trueΪ��ʾ,falseΪ����ʾ

	void EnableAllCtrlState(BOOL bState); //���ó��б����������пؼ���״̬�����û򲻿���
	void SetAllCtrlState();//������Ŀ�������ó��б����������пؼ���״̬�����û򲻿���

	void EnableStarNetDiff(BOOL bState);//�Ƿ�ʹ��StarNet����ƽ��

	void SetComboBoxCell(CGridCtrl &gridCtrl,int comboBoxColumn,vector<CString>vecComboOption);
	//********* //����ĳ�е�ԪΪReadOnly***************
	void SetReadOnlyCell(CGridCtrl &gridCtrl,int comboBoxColumn);
	void SetFirstColumnText(CGridCtrl &gridCtrl); //���õ�һ�еı��
	void SetRowSelected(CGridCtrl &gridCtrl,int row);//ѡ��ĳһ��,ĳ����ɫΪ����ɫ
	void ClearRowSelectdColor(CGridCtrl &gridCtrl);//���ѡ���е���ɫ
	void Refresh(const GeoPojectStruct &geo);//ˢ�����ݸ�ʽ��·����ƽ�����õ���Ϣ
	void RowHasChangedState(CGridCtrl &gridCtrl,CString projectName,int selectedRow);//�е�״̬�ı䣬����ɫ��Ϊ��ɫ�����ѡ����Ϊѡ��ɫ

	SaveFlag GetSelectedGeoProject(int selectedRow,GeoPojectStruct &geoPrj); //�õ�ѡ�е���Ŀ

	void SetDefaultProject(int selecedRow);//����Ĭ����Ŀ��Ϣ

	CString ConvertDateTimeFormat(CString strNumberFormat);//������ʱ����ʽ20000101000000ת��Ϊ2000-01-01 00:00:00
	CString ConvertTimeFormat(CString strNumberFomat);//������ʱ����ʽ000000ת��Ϊ00:00:00

	COleDateTime ConvertTime(CString strEpoch);//���ַ���ת��Ϊʱ�䣬�磺2014-8-8 8:08:08 --> 2014 8 8 8:8:8
	COleDateTime ConvertTimeSpan(CString strEpoch);//���ַ���ת��Ϊʱ�䣬�磺8:08:08 -->8 8 8
	
	BOOL FindINIFiles(); //�ҵ�����INI�����ļ�
	CString GetFileTitleFromFilePath(CString FileName);//��·������ļ���
	BOOL VerifyINIFile(CString filePath, CString fileHeader); //�ж��Ƿ�����ļ�ͷ��־
	void ReadINIFile(CString strINIFilePath,GeoPojectStruct &geoPrj);//��ȡ����INI�ļ�
	void ReadINIFiles(vector<CString> vecstrINIFilePath,MAPGeoProject &mapGeoProject);//��INI�ļ��������ڴ�
	short DivideString(CString strText, CString *strArray,int nCommaCount); // Divide String by ','

	void ClearNonexistentINIFiles();//���INI�ļ����µĲ����б���е�project�ļ�

	SaveFlag m_saveflag;

	CString m_strPrjName;//��Ŀ��
	bool m_bSelectedPrj;//�Ƿ�ѡ��
	CString m_strStartTime;//��ʼʱ��
	CString m_strIntervalTime;//����ʱ��
	CString m_strEndTime;//����ʱ��

	BOOL m_bUsingStarNet; //checkBox ʹ��StarNet����ƽ��
	CString m_strReportPath; //�����ŵ�Ŀ¼
	CString m_strDataPath; //dat�ļ����ڵ�·��
	CString m_strPrjPath; //�����ļ�
	CString m_strStarNetPath;//StartNetӦ�ó���ѡ��
	
	int m_nBackTime;//�ع���������ʱ�䣨���ӣ�
	BOOL m_bFailConvergaeWriteToDB;//�Ƿ�����ʧ���Խ����д��GeoMoS���ݿ�
	BOOL m_bUpdateCoordIfLowerBound;//�Ƿ񿨷�����ָ������̫��ʱ��������

	vector<GeoNode>m_vecNodes;//����
	vector<GeoOrientation>m_vecOrientation;//�۲ⷽ��
	vector<GeoOffset>m_vecOffset;//��λƫ��

	vector<CString>m_vecINIFilePath; //�����ļ���·����
	MAPGeoProject m_mapGeoProject; //Project�ļ���map����

	//�ؼ����
	CGridCtrl m_GridProjectSel;//��Ŀѡ���б�ؼ�
	//CObserveDlg m_oObserveDlg;//�۲����öԻ���
	int m_nProjectCount;//�б����������ʼ��Ϊ1,Ҳ���б��е���Ŀ��
	int m_nSelectedRowNum; //ѡ�е��к�,���ڱ����ļ�

	CString m_strlastSelectedState;//�༭ǰ��ѡ��״̬
	CString m_strLastSelectedProjectName;//�༭ǰ����Ŀ����
	int m_nLastSelectedRowNum;
	BOOL m_bDuplicateName;//�Ƿ����Project Name�����ı�־


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
	afx_msg void OnBnClickedBtnsave(); //�������õ�����δini�ļ�
	afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);//˫����Ԫ��
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);//������Ԫ��
	afx_msg void OnGridStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult);//��Ԫ��ʼ�༭ʱ
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);//��Ԫ��༭����ʱ
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
