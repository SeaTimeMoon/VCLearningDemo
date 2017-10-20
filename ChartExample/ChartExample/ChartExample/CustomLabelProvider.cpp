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
	//ȡ�õ����Ϣ
	SChartXYPoint Point=pSerie->GetPoint(PointIndex);
	//������
	COleDateTime timeX=CChartCtrl::ValueToDate(Point.X);
	CString strtime=timeX.Format(_T("%Y-%m-%d %H:%M"));
	//������
	ssText<<_T("Y=")<<Point.Y<<_T("\n");
	//��ʽ����
	TChartString strText=ssText.str();
	strText+=_T(" X=")+strtime;

	return strText;

}
