//�������������ϵ��¼�����������¼�
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
