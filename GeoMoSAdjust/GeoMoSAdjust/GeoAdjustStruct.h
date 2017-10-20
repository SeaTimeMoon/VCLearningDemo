#pragma once

#include "stdafx.h"
#include <vector>
#include <map>
#include <set>


using namespace std;

//ini�ļ��Ĺؼ���
const CString IsSelectedKey=_T("GeoMoS Adjust INI FILE");
const CString ReportPathKey=_T("Report Folder");
const CString UsingStarNetKey=_T("Using StarNet");
const CString DataFilePathKey=_T("Write File Folder");
const CString PrjPathKey=_T("Write Project Folder");
const CString StarNetPathKey=_T("StarNet Program Folder");
const CString BackTimeKey=_T("BackTime to retrieve data(min)");
const CString FailConvergaeWriteToDBKey=_T("Fail Converage Still Write Points Into GeoMos DataBase");
const CString UpdateCoordIfLowerBoundKey=_T("Update Coordinates if Chi-Square Test Lower Bound");
const CString FixNodeKey=_T("Fix Ref");
const CString StationNodeKey=_T("Float Stn");
const CString FloatNodeKey=_T("Float Point");
const CString StartTimeKey=_T("Start Time");
const CString IntervalTimeKey=_T("Interval Time");
const CString EndTimeKey=_T("End Time");
const CString MKey=_T("M");
const CString DKey=_T("D");


//��׺����
const CString SUFFIX=_T("XXX");

//����е���ɫ
const COLORREF BLUE=RGB(100,149,237);//��ɫ
const COLORREF GREEN=RGB(188,238,104);//��ɫ


//����ʧ����Ϣ
const CString MsgPrjNameErr=_T("����ʧ�ܣ���Ŀ��Ϊ�գ�������!");
const CString MsgReportErr=_T("����ʧ�ܣ�����Ŀ¼Ϊ�գ�������!");
const CString MsgDataFileErr=_T("����ʧ�ܣ�dat�ļ����Ŀ¼Ϊ�գ�������!");
const CString MsgPrjFileErr=_T("����ʧ�ܣ������ļ�·��Ϊ�գ�������!");
const CString MsgStarNetErr=_T("����ʧ�ܣ�StarNetӦ�ó���·��Ϊ�գ�������!");
const CString MsgBackTimeErr=_T("����ʧ�ܣ��ع���������ʱ��Ҫ����0��������!");
const CString MsgStartTimeErr=_T("����ʧ�ܣ���ʼʱ��Ϊ�գ�������!");
const CString MsgIntervalTimeErr=_T("����ʧ�ܣ�����ʱ��Ϊ�գ�������!");
const CString MsgStopTimeErr=_T("����ʧ�ܣ�����ʱ��Ϊ�գ�������!");
const CString MsgTimeValueErr=_T("����ʧ�ܣ�����ʱ�䲻�����ڿ�ʼʱ�䣬������!");


//����״̬��־
typedef enum SaveFlag{	
				ErrPrjName,
				ErrStartTime,
				ErrIntervalTime,
				ErrStopTime,
				ErrTimeValue,
				ErrReport,
				ErrDataFile,
				ErrPrjFile,
				ErrStarNet,
				ErrBackTime,
				SaveSuc 
}SaveFlag;//�����ļ������־ 

typedef struct GeoNode
{
	CString strNodeName; //����
	CString strNodeType; //������
}GeoNode; //����

typedef struct GeoOrientation
{
	CString strStationPoint;//��վ��
	CString strTargetPoint;//Ŀ���
	bool operator == (const GeoOrientation &geo) {return (this->strStationPoint == geo.strStationPoint)&&(this->strTargetPoint == geo.strTargetPoint);}//���صȺŲ���
}GeoOrientation;//�۲ⷽ��

typedef struct GeoOffset
{
	CString strStartPoint; //���
	CString strEndPoint;//�յ�
	CString strEastCoordError;//�������
	CString strNorthCoordError;//�������
	CString strElevationDiff;//�̲߳�
}GeoOffset; //��λƫ��

typedef struct GeoProject
{
	//��Ŀѡ��
	CString strPrjName;//��Ŀ��
	bool bSelectedPrj;//�Ƿ�ѡ��
	//���ݸ�ʽ��·��///
	bool bUsingStarNet;//�Ƿ�ʹ��StarNet����ƽ��
	CString strReportPath;//������Ŀ¼
	CString strDataPath;//dat�ļ����Ŀ¼
	CString strPrjPath;//�����ļ�ѡ��
	CString strStarNetPath;//StarNetӦ�ó���ѡ��
	//ƽ������
	int nBackTime;//�ع���������ʱ�䣨���ӣ�
	bool bFailConvergaeWriteToDB;//�Ƿ�����ʧ���Խ����д��GeoMoS���ݿ�
	bool bUpdateCoordIfLowerBound;//�Ƿ񿨷�����ָ������̫��ʱ��������

	//����
	vector<GeoNode>vecGeoNodes;
	//�۲ⷽ��
	vector<GeoOrientation>vecGeoOrientations;
	//��λƫ��
	vector<GeoOffset>vecGeoOffsets;

	CString strStartTime;//��ʼʱ��
	CString strIntervalTime;//����ʱ��
	CString strEndTime;//����ʱ��

	bool bSaveSuc;//�Ƿ񱣴�ɹ�

}GeoPojectStruct;

typedef map<CString,GeoPojectStruct> MAPGeoProject;
typedef pair<CString,CString> NodeOrientation;



