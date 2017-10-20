// GeoMoSAdjustDlg.h : header file
//

#pragma once
#include "CustomListBox.h"
#include "BtnST.h"
#include "GeoAdjustStruct.h"
#include "Shlwapi.h"
#include <vector>
#include "Adjustment/Adjustment.h"
#include "Adjustment/Utilities.h"

using namespace std;

#define WM_SHOWTASK WM_USER+100 //自定义消息，点击托盘显示对话框
// CGeoMoSAdjustDlg dialog
class CGeoMoSAdjustDlg : public CDialog
{
// Construction
public:
	CGeoMoSAdjustDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GEOMOSADJUST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam); //消息映射函数，显示托盘
	DECLARE_MESSAGE_MAP()

public:
	void InitListBoxST();//初始化ListBox
	void InitButtonST();//初始化两个控制按钮

	enum DialogWindowsPos{ZTop,ZTopMost};
	void SetDialogWindowPostition(DialogWindowsPos windowspos );//设置对话框的位置

public:
	CCustomListBox m_ListBox;//列表控件
	CImageList m_ImageList;//列表图标

	CButtonST m_btnStart; //开始按钮
	CButtonST m_btnStop; //停止按钮

	afx_msg void OnBnClickedBtnstart();
	afx_msg void OnBnClickedBtnstop();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	void ZoomInTaskTray();//缩小到托盘
	void DeleteTray();//销毁托盘图标
protected:
	NOTIFYICONDATA m_nid;
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void StopAutoAdjustment(BOOL bAll = FALSE);
	void UpdateStatus(CString strStatus);
	CString m_strIpAddress;
	CString m_strDbName;
	CString m_strUserName;
	CString m_strPassword;
	_ConnectionPtr m_pConnection;
	_RecordsetPtr  m_pRecordset;
	CTime m_timeEndAll;

	_RecordsetPtr& GetRS(CString strSQL);
	void RefreshControls(BOOL bEnabled);
	BOOL StartAdjustment();
	BOOL GetCoordinates();
	BOOL GetCoordinates(POINT_3DList * pList, short type);
	BOOL GetMeasurementValid(CTime currTime);
	BOOL SetCoordinates();
	BOOL SetCoordinates234(POINT_3DList * pListResult, POINT_3DList * pList, short type);
	BOOL SetCoordinates(POINT_3DList * pListResult, POINT_3DList * pList, short type);
	//
	int GetIdByNameDesc(CString tableName, CString pointName);
	std::vector<int> m_veccTpsSetupId;
	CString ConvertTime(CTime currTime);
	void GetSetupIdByPointIdDesc(int pointId, short type, CTime currTime, int nRetrieveMin = 240);
	// Sql command string
	CString SQL_Select_IdByName(CString tableName, CString pointName);
	CString SQL_Select_SetupIdByPointId(int pointId, short type, CTime currTime, int nRetrieveMin);
	CString SQL_Select_Coordinates(int pointId, short type);
	CString SQL_Select_TpsMeasurents(int setupId, int pointId, short face);
	CString SQL_Update_Coordinates234(int id, short type, POINT_3D point);
	CString SQL_Update_Coordinates(int pointId, short type, POINT_3D point);
	//
	vector<CAdjustment> m_vecAdjustment;
	int m_nCurrAdjIndex;
	CAdjustment theAdjustment;
	short m_nFaceTypeMeas;
	short m_nCoordTypeRef;
	short m_nCoordTypeStn;
	short m_nCoordTypeMeas;
	short m_nCoordTypeScan;
	//int m_nAdjustTimes;
public:
	TpsMeasurement GetAverageMeasurement(TpsMeasurementList measList);
	CTime ConvertTime(CString strEpoch);
	__int64 CalcPassword();
	BOOL IsHzCorrected(double dHzTrue, double dHzMeas, double &dHzCorr);
	BOOL RegAsmTunnelDll();
	CString GetCurrentPath();
	BOOL WriteRegAsmBatFile(CString strPath, CString strFileName);
	void StartAutoAdjustment();
	double CalcDistance(POINT_3D oPoint1, POINT_3D oPoint2, int nSize);

private:
	BOOL GetAllValidIniFiles();
	BOOL IsValidIniFile(CString filePath, ProjectInfo &projectInfo);
	CTime ConvertToTime(CString strText);
	CTimeSpan ConvertToTimeSpan(CString strText);
};
