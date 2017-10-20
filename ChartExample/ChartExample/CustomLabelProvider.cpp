#include "StdAfx.h"
#include "CustomLabelProvider.h"

CCustomLabelProvider::CCustomLabelProvider(void)
{
}

CCustomLabelProvider::~CCustomLabelProvider(void)
{
}

TChartString CCustomLabelProvider::GetText( CChartSerieBase<SChartXYPoint>* pSerie, unsigned PointIndex )
{
	TChartStringStream ssText;
	//取得点的信息
	SChartXYPoint Point=pSerie->GetPoint(PointIndex);
	//横坐标
	COleDateTime timeX=CChartCtrl::ValueToDate(Point.X);
	CString strtime=timeX.Format(_T("%Y-%m-%d %H:%M"));
	//纵坐标
	ssText<<_T("Y=")<<Point.Y<<_T("\n");
	//格式处理
	TChartString strText=ssText.str();
	strText+=_T(" X=")+strtime;

	return strText;

}
