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
		
		//�������Ϣ
		TChartStringStream ssText;
		SChartXYPoint Point=pSeries->GetPoint(uPointIndex);
		//������ʱ��ת��Ϊ�ַ���
		COleDateTime timeX=CChartCtrl::ValueToDate(Point.X);

		//��Ի�׼ʱ���λ��
		COleDateTime starttime(2008,1,1,0,0,0);//��׼ʱ��
		COleDateTimeSpan timespan=timeX-starttime;
		int lowIndex=(int)timespan.GetTotalDays(); //����ʱ��֮�������
		
		//ɾ�������ʾ��Ϣ
		pSeries->ClearAllBalloonLabels();

		m_pLabelProvider=new CCustomLabelProvider();
		pLabel=pSeries->CreateBalloonLabel(lowIndex,m_pLabelProvider);
		pLabel->SetLineColor(RGB(0,0,120));
		//����BalloonLabel����ʽ
		CChartFont labelFont;
		labelFont.SetFont(_T("Microsoft Sans Serif"),100,false,true,false);
		pLabel->SetFont(labelFont);

		//Balloon label����ʾʱ��Ϊ50ms���´�ˢ�¾�ɾ����
		//Sleep(50);
		pSeries->ClearAllBalloonLabels();

		//ɾ��ָ�룬�����ڴ�й©
		delete pLabel;
		pLabel=NULL;
		delete m_pLabelProvider;
		m_pLabelProvider=NULL;
	
	}
		

}

