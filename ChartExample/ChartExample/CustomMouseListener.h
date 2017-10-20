
//自定义监听曲线上的事件的类
#pragma once
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartSeriesMouseListener.h"
#include "ChartCtrl/ChartSerieBase.h"
#include "ChartCtrl/ChartXYSerie.h"
#include "ChartCtrl/ChartAxis.h"
#include "CustomLabelProvider.h"

class CCustomMouseListener:public CChartSeriesMouseListener<SChartXYPoint>
{
public:
	CCustomMouseListener(void);
	~CCustomMouseListener(void);

	void OnMouseEventSeries(CChartMouseListener::MouseEvent mouseEvent, CPoint point,
		CChartSerieBase<SChartXYPoint>* pSeries, unsigned uPointIndex);

public:


private:
	CCustomLabelProvider* m_pLabelProvider;


};
