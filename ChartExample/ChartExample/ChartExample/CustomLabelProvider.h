#pragma once
#include "ChartCtrl/ChartString.h"
#include "ChartCtrl/ChartXYSerie.h"

class CCustomLabelProvider:public CChartLabelProvider<SChartXYPoint>
{
public:
	CCustomLabelProvider(void);
	~CCustomLabelProvider(void);

	TChartString GetText(CChartSerieBase<SChartXYPoint>* pSerie, unsigned PointIndex);
};
