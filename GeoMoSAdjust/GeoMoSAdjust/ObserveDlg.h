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

	void InitGridCtrl(CGridCtrl &gridCtrl); //��ʼ��һ���б�ؼ�
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
	void InitGridTitles();//������б�ؼ��ı���

	void InsertRow(CGridCtrl &gridCtrl);//�б������Ӳ���
	void DeleteRow(CGridCtrl &gridCtrl,int rowNum);//�б����ɾ������,rowNum��ʾҪɾ������
	void SetComboBoxCell(CGridCtrl &gridCtrl,int comboBoxColumn); //����ĳ����ԪΪcombobox����ʽ	
	void SetComboBoxCell(CGridCtrl &gridCtrl,int comboBoxColumn,const vector<CString> &vecComboOption,int nDefaultOptionIndex); 
	void GetNodeNames(const CGridCtrl &gridCtrl, CString nodeType,int typeColumn,vector<CString> &vecNodeNames);

	void SetDefaultNodeName();//����Ĭ�ϵĵ���
	void SetDefaultOrientation();//����Ĭ�ϵ�Ŀ�����
	void SetDefaultOffset();//����Ĭ�ϵĵ�λ��

	void SaveNodes();//���������Ϣ
	void SaveOrientations();//����۲ⷽ����Ϣ
	void SaveOffset();//�����λƫ����Ϣ

	bool IsNameExisited(const CGridCtrl &gridCtrl,CString searchName,int columnNum);//�����Ƿ����ĳһ��

public:
	CString m_strSelProjectName;//��ѡ����Ŀ����

	CGridCtrl m_oNodeManager;//��������
	vector<CString>m_vecNodeTitles;//���������ַ�����
	vector<GeoNode>m_vecNodes;//��������
	set<CString>m_setNodeNames;//����map������ֹ�����ظ�
	int m_nSelNodeRowNum;//ѡ�е�����
	CString m_strLastSelectedNodeName;//�޸�ǰ�ĵ�����
	CString m_strLastNodeType;//�޸�ǰ�ĵ�����
	int m_nLastSelectedRowNum;//��һ��ѡ�е���
	BOOL m_bDuplicateNodeName;//�Ƿ����Node Name�����ı�־

	CGridCtrl m_oOrientationManager;//�۲ⷽ��
	vector<CString>m_vecOrientationTitles;//�۲ⷽ������ַ�����
	vector<GeoOrientation>m_vecOrientation;//�۲ⷽ������
	int m_nSelOrientationRowNum;////ѡ�е�����
	GeoOrientation m_lastGeoOri;//�޸�ǰ�Ĺ۲ⷽ��
	int m_nLastSelectedOrientationRowNum;//��һ��ѡ�е���
	BOOL m_bDuplicateOrientation;//�Ƿ����Offset Name�����ı�־
	

	CGridCtrl m_oOffsetManager;//��λƫ��
	vector<CString>m_vecOffsetTitles;//��λƫ�Ʊ����ַ�����
	vector<GeoOffset>m_vecOffset;//��λƫ������
	set<CString>m_setUsedPoint;//����յ�set��,��ֹ���ظ�
	int m_nSelOffsetRowNum;////ѡ�е�����
	CString m_strLastSelectedOffsetName;//�޸�ǰ�ĵ�����
	int m_nLastSelectedOffsetRowNum;//��һ��ѡ�е���
	int m_nLastSelectedOffsetColumnNum;//��һ��ѡ�е���
	BOOL m_bDuplicateOffsetName;//�Ƿ����Offset Name�����ı�־	
	
public:
	afx_msg void OnBnClickedBtninsertnode();//���ӵ���
	afx_msg void OnBnClickedBtndeletenode();//ɾ������
	afx_msg void OnBnClickedBtninsertorientation();//���ӹ۲ⷽ��
	afx_msg void OnBnClickedBtndeleteorientation();//ɾ���۲ⷽ��
	afx_msg void OnBnClickedBtninsertoffset();//���ӵ�λƫ��
	afx_msg void OnBnClickedBtndeleteoffset();//ɾ����λƫ��
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnGridNodeStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult); //��������Ԫ��༭��ʼʱ
	afx_msg void OnGridNodeEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult); //��������Ԫ��༭����ʱ
	afx_msg void OnGridNodeClick(NMHDR *pNotifyStruct, LRESULT* pResult);//������������Ԫ��
	afx_msg void OnGridOrientationStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult); //�۲ⷽ��Ԫ��༭��ʼʱ
	afx_msg void OnGridOrientationEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult); //�۲ⷽ��Ԫ��༭����ʱ
	afx_msg void OnGridOrientationClick(NMHDR *pNotifyStruct, LRESULT* pResult);//�۲ⷽ�򵥻���Ԫ��
	afx_msg void OnGridOffsetClick(NMHDR *pNotifyStruct, LRESULT* pResult);//��λƫ�Ƶ�����Ԫ��
	afx_msg void OnGridOffsetStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult);//��λƫ�Ƶ�Ԫ��ʼ�༭ʱ
	afx_msg void OnGridOffsetEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);//��λƫ�Ƶ�Ԫ��༭����ʱ
};
