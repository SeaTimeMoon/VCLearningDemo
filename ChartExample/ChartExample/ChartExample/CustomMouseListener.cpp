#include "StdAfx.h"
#include "CustomMouseListener.h"

CCustomMouseListener::CCustomMouseListener(void):m_pLabelProvider(NULL)
{
	
}

CCustomMouseListener::~CCustomMouseListener(void)
{
	if (m_pLabelProvider!=NULL)
	{
		delete m_pLabelProvider;
		m_pLabelProvider=NULL;
	}

}

void CCustomMouseListener::OnMouseEventSeries( CChartMouseListener::MouseEvent mouseEvent, CPoint point, CChartSerieBase<SChartXYPoint>* pSeries, unsigned uPointIndex )
{
	CChartBalloonLabel<SChartXYPoint>* pLabel;
	if (mouseEvent==CChartMouseListener::MouseMove&&uPointIndex!=INVALID_POINT)
	{		
		
		//鼠标点的信息
		TChartStringStream ssText;
		SChartXYPoint Point=pSeries->GetPoint(uPointIndex);
		//横坐标时间转换为字符串
		COleDateTime timeX=CChartCtrl::ValueToDate(Point.X);

		//相对基准时间的位置
		COleDateTime starttime(2008,1,1,0,0,0);//基准时间
		COleDateTimeSpan timespan=timeX-starttime;
		int lowIndex=(int)timespan.GetTotalDays(); //两个时间之间的天数
		
		//删除点的显示信息
		pSeries->ClearAllBalloonLabels();

		m_pLabelProvider=new CCustomLabelProvider();
		pLabel=pSeries->CreateBalloonLabel(lowIndex,m_pLabelProvider);
		pLabel->SetLineColor(RGB(0,0,120));
		//设置BalloonLabel的样式
		CChartFont labelFont;
		labelFont.SetFont(_T("Microsoft Sans Serif"),100,false,true,false);
		pLabel->SetFont(labelFont);

		//Balloon label的显示时间为50ms，下次刷新就删除了
		//Sleep(50);
		pSeries->ClearAllBalloonLabels();

		//删除指针，减少内存泄漏
		delete pLabel;
		pLabel=NULL;
		delete m_pLabelProvider;
		m_pLabelProvider=NULL;
	
	}
		

}

