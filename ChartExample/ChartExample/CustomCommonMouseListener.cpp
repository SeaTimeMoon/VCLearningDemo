#include "StdAfx.h"
#include "CustomCommonMouseListener.h"

CCustomCommonMouseListener::CCustomCommonMouseListener(void)
{
}

CCustomCommonMouseListener::~CCustomCommonMouseListener(void)
{
}

void CCustomCommonMouseListener::OnMouseEventPlotArea( MouseEvent mouseEvent, CPoint point )
{
	if (mouseEvent==CChartMouseListener::LButtonDoubleClick)
	{
		AfxMessageBox(_T("You clicked"));

	}
	

}
