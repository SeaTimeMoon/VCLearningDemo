/*
 *
 *	ChartDateTimeAxis.h
 *
 *	Written by C閐ric Moonen (cedric_moonen@hotmail.com)
 *
 *
 *
 *	This code may be used for any non-commercial and commercial purposes in a compiled form.
 *	The code may be redistributed as long as it remains unmodified and providing that the 
 *	author name and this disclaimer remain intact. The sources can be modified WITH the author 
 *	consent only.
 *	
 *	This code is provided without any garanties. I cannot be held responsible for the damage or
 *	the loss of time it causes. Use it at your own risks
 *
 *	An e-mail to notify me that you are using this code is appreciated also.
 *
 *
 */

#ifndef _CHARTDATETIMEAXIS_H_
#define _CHARTDATETIMEAXIS_H_

#include "ChartAxis.h"
#include "ChartString.h"


//! A specialization of the CChartAxis class for displaying date and time data.
class CChartDateTimeAxis : public CChartAxis
{
	friend CChartCtrl;

public:
	
	//! Sets the tick increment.
	/**
		The tick increment is the value between two adjacents
		ticks on the axis. In case of a date time axis, the interval
		is specified by a time period because this interval might not
		be constant (for instance, if a tick interval of one month is 
		specified, the distance between two adjacents ticks is not 
		constant: it depends on the number of days in the month).
		The full tick interval is made of a base interval (day, month,
		hour, ...) and a multiplier, that is applied to this base interval.
		So, for an interval of three months between two ticks, you have to
		specify tiMonth for the interval and 3 for the multiplier.
		@param bAuto
			Specifies if the tick increment is automatically calculated.
		@param Interval
			The base interval.
		@param Multiplier
			The multiplier applied to the base interval.
	**/
	virtual void SetTickIncrement(bool bAuto, TimeInterval Interval, int Multiplier);
	//! Sets the format of the tick labels.
	/**
		@param bAutomatic
			Specifies if the format is calculated automatically.
		@param strFormat
			The format to apply to the tick label if bAutomatic is false.
		<br>Check the documentation of the COleDateTime::Format function on 
		MSDN for more information about the format string. 
	**/
	virtual void SetTickLabelFormat(bool bAutomatic, const TChartString& strFormat);
	//! Sets the reference tick.
	/**
		The reference tick is a date/time which specifies a tick which should
		always be displayed on the axis. This is needed when the tick
		interval multiplier is not 1 (e.g. the interval between two ticks is 3 
		months). In that specific case, there is no way for the control to know 
		which ticks should be displayed (in our example, the chart doesn't know 
		if the first tick will be january, february or march). This is particularly
		annoying when the axis is panned (in that case, if we always take the first
		month on the axis as first tick, the ticks will always switch from one month
		to another). By having a refence tick, this forces the control to calculate
		all tick intervals based on this reference. It is set to January 1st 2000 by
		default.
	**/
	void SetReferenceTick(COleDateTime referenceTick);

	/////////////////////设置时间间隔基准单位（年、月、日、时、分、秒）///////////////////////////
	virtual void SetTimeInterval(TimeInterval timeinterval){m_BaseInterval=timeinterval;}

	//保存放大前的坐标信息
	virtual void SaveAxisInfoBeforeZoom();
	//时间轴时间单位切换临界值设定
	virtual void SetZoomDateSwitchMode(int days,int hours,int minutes);
	//获得时间轴时间单位切换临界值
	virtual void GetZoomDateSwitchMode(int &days,int &hours,int &minutes);

	//将坐标轴的信息添加进去
	virtual void PushTimeAxisPos(DateTimeAxisInfo timeAxisInfo){m_StackTimeAxisPos.push(timeAxisInfo);}
	//将坐标轴的信息删除
	virtual void PopTimeAxisPos(){m_StackTimeAxisPos.pop();}
	//判断坐标轴保存信息栈是否为空
	virtual bool IsTimeAxisStackEmpty();
	//获得栈顶元素
	virtual void TopTimeAxisStack(DateTimeAxisInfo &timeAxisInfo){timeAxisInfo=m_StackTimeAxisPos.top();}
	//获得栈的元素个数
	virtual int GetTimeAxisStackSize(){return m_StackTimeAxisPos.size();}

private:
	//! Default constructor
	CChartDateTimeAxis();
	//! Default destructor
	~CChartDateTimeAxis();

	double GetFirstTickValue() const;
	bool GetNextTickValue(double dCurrentTick, double& dNextTick) const;
	TChartString GetTickLabel(double TickValue) const;
	long ValueToScreenDiscrete(double Value) const;
	long GetTickPos(double TickVal) const;

	void RefreshTickIncrement();
	void RefreshFirstTick();
	//! Forces a refresh of the date/time tick label format
	void RefreshDTTickFormat();

	//! Add a number of months to a date.
	/**
		The function takes care of 'overflow' (total number
		of months higher than 12) error when adding the months.
		@param Date
			The date to which months will be added.
		@param iMonthsToAdd
			The number of months to add to the date.
		@return the resulting date.
	**/
	COleDateTime AddMonthToDate(const COleDateTime& Date, 
								int iMonthsToAdd) const;

	double GetTickBeforeVal(double dValue) const;

	//! Format of the date/time tick labels
	TChartString m_strDTTickFormat;		
	//! Specifies if the tick labels format is automatic
	bool m_bAutoTickFormat;
	//! Specifies the base time interval for ticks
	/**
		This specifies an base interval in sec, min, hour,
		day, month or year. The total tick increment is 
		a mutliple of this base interval (specified by 
		m_iDTTickIntervalMult). E.g: 2 days
	**/
	TimeInterval m_BaseInterval;
	//! Specifies the multiplicator for the base interval
	/**
		This multiplies the base interval for the ticks, resulting
		in something like 3 minutes (a multiplicator of 1 can also
		be specified).
	**/
	int m_iDTTickIntervalMult;

	//! Caches the value of the first tick.
	double m_dFirstTickValue;

	//! The reference tick. See the SetReferenceTick function for details.
	COleDateTime m_ReferenceTick;

	//时间坐标轴的位置与模式
	typedef std::stack<DateTimeAxisInfo> StackDateTimeAxisInfo;
	StackDateTimeAxisInfo m_StackTimeAxisPos; //横轴（时间轴、底轴）的坐标轴位置范围和设置模式

	//放大时，时间单位模式切换临界值
	int m_zoomMModeOverDays; // 超过此时间（天数），时间轴显示为“月”模式
	int m_zoomDModeOverHours; //超过此时间（小时数），时间轴显示为“天”模式
	int m_zoomHModeOverMinutes; //超过此时间（分钟数），时间轴显示为“时”模式

};

#endif  // _CHARTDATETIMEAXIS_H_