//监听除了曲线上的事件以外的其他事件
#pragma once

#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartMouseListener.h"


class CCustomCommonMouseListener:public CChartMouseListener
{
public:
	CCustomCommonMouseListener(void);
	~CCustomCommonMouseListener(void);

public:
	void OnMouseEventPlotArea(MouseEvent mouseEvent, CPoint point);
};
