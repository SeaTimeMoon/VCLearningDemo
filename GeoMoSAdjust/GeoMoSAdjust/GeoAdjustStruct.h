#pragma once

#include "stdafx.h"
#include <vector>
#include <map>
#include <set>


using namespace std;

//ini文件的关键字
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


//后缀符号
const CString SUFFIX=_T("XXX");

//表格行的颜色
const COLORREF BLUE=RGB(100,149,237);//蓝色
const COLORREF GREEN=RGB(188,238,104);//绿色


//保存失败信息
const CString MsgPrjNameErr=_T("保存失败，项目名为空，请设置!");
const CString MsgReportErr=_T("保存失败，报告目录为空，请设置!");
const CString MsgDataFileErr=_T("保存失败，dat文件存放目录为空，请设置!");
const CString MsgPrjFileErr=_T("保存失败，工程文件路径为空，请设置!");
const CString MsgStarNetErr=_T("保存失败，StarNet应用程序路径为空，请设置!");
const CString MsgBackTimeErr=_T("保存失败，回滚检索数据时间要大于0，请设置!");
const CString MsgStartTimeErr=_T("保存失败，开始时间为空，请设置!");
const CString MsgIntervalTimeErr=_T("保存失败，处理时间为空，请设置!");
const CString MsgStopTimeErr=_T("保存失败，结束时间为空，请设置!");
const CString MsgTimeValueErr=_T("保存失败，结束时间不能早于开始时间，请设置!");


//保存状态标志
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
}SaveFlag;//保存文件错误标志 

typedef struct GeoNode
{
	CString strNodeName; //点名
	CString strNodeType; //点类型
}GeoNode; //点名

typedef struct GeoOrientation
{
	CString strStationPoint;//测站点
	CString strTargetPoint;//目标点
	bool operator == (const GeoOrientation &geo) {return (this->strStationPoint == geo.strStationPoint)&&(this->strTargetPoint == geo.strTargetPoint);}//重载等号操作
}GeoOrientation;//观测方向

typedef struct GeoOffset
{
	CString strStartPoint; //起点
	CString strEndPoint;//终点
	CString strEastCoordError;//东坐标差
	CString strNorthCoordError;//北坐标差
	CString strElevationDiff;//高程差
}GeoOffset; //点位偏移

typedef struct GeoProject
{
	//项目选择
	CString strPrjName;//项目名
	bool bSelectedPrj;//是否选用
	//数据格式、路径///
	bool bUsingStarNet;//是否使用StarNet进行平差
	CString strReportPath;//报告存放目录
	CString strDataPath;//dat文件存放目录
	CString strPrjPath;//工程文件选择
	CString strStarNetPath;//StarNet应用程序选择
	//平差设置
	int nBackTime;//回滚检索数据时间（分钟）
	bool bFailConvergaeWriteToDB;//是否收敛失败仍将结果写入GeoMoS数据库
	bool bUpdateCoordIfLowerBound;//是否卡方检验指标设置太低时更新坐标

	//点名
	vector<GeoNode>vecGeoNodes;
	//观测方向
	vector<GeoOrientation>vecGeoOrientations;
	//点位偏移
	vector<GeoOffset>vecGeoOffsets;

	CString strStartTime;//开始时间
	CString strIntervalTime;//处理时间
	CString strEndTime;//结束时间

	bool bSaveSuc;//是否保存成功

}GeoPojectStruct;

typedef map<CString,GeoPojectStruct> MAPGeoProject;
typedef pair<CString,CString> NodeOrientation;



