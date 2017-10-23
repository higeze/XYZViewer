#pragma once

#include "MySize.h"
#include "MyRect.h"
#include "MyFont.h"
#include "MyDC.h"
#include "MyPen.h"
#include "MyFont.h"
#include "MySize.h"

#include "GDIUIElement.h"
#include "ObservableContainer.h"
#include "ChartProperty.h"


class CLineSeries;
class CBarSeries;

std::wstring roundtowstring(const double& arg, int precision);


//Plot Area
class CPlotAreaProperty;
class CPlotArea:public CGDIUIElement
{
private:
	std::shared_ptr<CPlotAreaProperty> m_spProp;
public:
	CPlotArea(CGDIUIElement* pChart, std::shared_ptr<CPlotAreaProperty> spProp)
		:CGDIUIElement(pChart),
		m_spProp(spProp){}
	virtual ~CPlotArea(){}
	virtual void OnPaint(PaintEventArgs& e);
};

//Axis


class CAxis:public CGDIUIElement
{
protected:
	double m_maximum;
	double m_minimum;
	double m_maximumValue;
	double m_minimumValue;

	double m_tick;
	static const int m_maxTickNumber=10;
	static const int m_minTickNumber=5;
	static const int m_tickLength=5;

	std::shared_ptr<CAxisProperty> m_spProp;

public:
	CAxis(CGDIUIElement* pChart, std::shared_ptr<CAxisProperty> spProp)
		:CGDIUIElement(pChart),
		m_spProp(spProp),
		m_maximum(0),m_minimum(0),m_maximumValue(0),m_minimumValue(0),m_tick(1){}
	virtual ~CAxis(){}
	double GetMaximum()const{return m_maximum;}
	void SetMaximum(const double& maximum){m_maximum=maximum;}
	double GetMinimum()const{return m_minimum;}
	void SetMinimum(const double& minimum){m_minimum=minimum;}
	double GetMaximumValue()const{return m_maximumValue;}
	void SetMaximumValue(const double& maximumValue){m_maximumValue=maximumValue;}
	double GetMinimumValue()const{return m_minimumValue;}
	void SetMinimumValue(const double& minimumValue){m_minimumValue=minimumValue;}
	double GetTick()const{return m_tick;}
	void SetTick(const double tick){m_tick=tick;}
	//void SetLabel(const std::wstring& wstrLabel){m_wstrLabel = wstrLabel;}

	double static CalcAutoTick(double min,double max,int minNum,int maxNum);
	double static CalcTickStart(double min, double tick)
	{
		double ret = 0;
		//if (min <= 0)
		//{
		//	ret = (int)((min-tick*1.5) / tick)  * tick;
		//}
		//else
		//{
		//	ret = min(0,(int)((min-tick*1.5) / tick) * tick);
		//}
		if(min>=0){
			ret = 0;
		}else{
			ret = (int)((min-tick*1.5) / tick)  * tick;
		}
		return ret;
	}

	double static CalcTickEnd(double max,double tick)
	{
		double ret = 0;
		//if (max <= 0)
		//{
		//	ret = max(0,(int)((max+tick*1.5) / tick) * tick);
		//}
		//else
		//{
		//	ret = (int)((max+tick*1.5) / tick) * tick;
		//}
		if(max>=0){
			ret = (int)((max+tick*1.5) / tick) * tick;
		}else{
			ret = 0;
		}
		return ret;
	}
};

class CXAxis:public CAxis
{
public:
	CXAxis(CGDIUIElement* pChart, std::shared_ptr<CAxisProperty> spProp):CAxis(pChart, spProp){}
	virtual ~CXAxis(){}
	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);

};

class CYAxis:public CAxis
{
public:
	CYAxis(CGDIUIElement* pChart, std::shared_ptr<CAxisProperty> spProp):CAxis(pChart, spProp){}
	virtual ~CYAxis(){}
	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);
};

class CXBarAxis:public CXAxis
{
private:
public:
	CXBarAxis(CGDIUIElement* pChart, std::shared_ptr<CAxisProperty> spProp):CXAxis(pChart, spProp){}
	virtual ~CXBarAxis(){}
	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);
};

class CChartTitle:public CBindTextBlock
{
public:
	CChartTitle(CGDIUIElement* pChart, std::shared_ptr<CTitleProperty> spProp)
		:CBindTextBlock(pChart, spProp->GetFontPtr(), std::make_shared<UINT>(DT_CENTER)){}
	virtual ~CChartTitle(){}
};

class CLabel:public CBindTextBlock
{
public:
	CLabel(CGDIUIElement* pChart, std::shared_ptr<CLabelProperty> spProp)
		:CBindTextBlock(pChart, spProp->GetFontPtr(), std::make_shared<UINT>(DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER)){}
	virtual ~CLabel(){}
};


class CChartLegend:public CStackPanel
{
public:
	CChartLegend(CGDIUIElement* pChart):CStackPanel(pChart)
	{
		SetOrientation(Orientation::Vertical);
	}
	virtual ~CChartLegend(){}
};

class CLineChartLegend:public CChartLegend
{
public:
	CLineChartLegend(CGDIUIElement* pChart):CChartLegend(pChart){}
	virtual ~CLineChartLegend(){}
	void OnContainerChanged(NotifyContainerChangedEventArgs<std::shared_ptr<CLineSeries>>& e);

//	virtual CSize Measure(CDC* pDC)const;
//	virtual void OnPaint(PaintEventArgs& e);
};

class CBarChartLegend:public CChartLegend
{
public:
	CBarChartLegend(CGDIUIElement* pChart):CChartLegend(pChart){}
	virtual ~CBarChartLegend(){}
	void OnContainerChanged(NotifyContainerChangedEventArgs<std::shared_ptr<CBarSeries>>& e);
};
