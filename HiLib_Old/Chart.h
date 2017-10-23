#pragma once
#include "MySize.h"
#include "MyRect.h"
#include "MyColor.h"
#include "MyPen.h"
#include "MyFont.h"
#include "SeriesProperty.h"
#include "GDIUIElement.h"
#include "ChartElements.h"

class CXAxis;
class CYAxis;
class CPlotArea;
class CLineSeries;
class CChartTitle;
class CChartLegend;

#include "MyFriendSerializer.h"

#include "ObservableContainer.h"
#include "ChartProperty.h"

class CChart:public CPanel
{
protected:
	std::vector<std::shared_ptr<CGDIUIElement>> m_vspElement;
	std::shared_ptr<CChartProperty> m_spChartProperty;

	std::shared_ptr<CXAxis> m_spXAxis;
	std::shared_ptr<CYAxis> m_spYAxis;
	std::shared_ptr<CPlotArea> m_spPlotArea;
	std::shared_ptr<CChartTitle> m_spTitle;
	std::shared_ptr<CChartLegend> m_spLegend;

	std::shared_ptr<CLabel> m_spXLabel;
	std::shared_ptr<CLabel> m_spYLabel;

public:
	CChart(std::shared_ptr<CChartProperty> spChartProperty,
			std::shared_ptr<CXAxis> m_spXAxis,
			std::shared_ptr<CYAxis> m_spYAxis,
			std::shared_ptr<CPlotArea> m_spPlotArea,
			std::shared_ptr<CChartTitle> m_spTitle,
			std::shared_ptr<CChartLegend> m_spLegend,
			std::shared_ptr<CLabel> m_spXLabel,
			std::shared_ptr<CLabel> m_spYLabel);
	virtual ~CChart(){}

	std::shared_ptr<CXAxis> GetXAxisPtr(){return m_spXAxis;}
	std::shared_ptr<CYAxis> GetYAxisPtr(){return m_spYAxis;}
	std::shared_ptr<CLabel> GetXLabelPtr(){return m_spXLabel;}
	std::shared_ptr<CLabel> GetYLabelPtr(){return m_spYLabel;}
	std::shared_ptr<CPlotArea> GetPlotAreaPtr(){return m_spPlotArea;}
	std::shared_ptr<CChartTitle> GetTitlePtr(){return m_spTitle;}
	std::shared_ptr<CChartLegend> GetLegendPtr(){return m_spLegend;}
	//std::shared_ptr<CSeriesProperty> GetSeriesPropertyPtr(int i)
	//{
	//	auto spProperties = m_spChartProperty->GetSeriesPropertiesPtr();
	//	return spProperties->operator[](i%spProperties->size());
	//}
};

class CLineChart:public CChart
{
private:
	std::shared_ptr<std::vector<std::shared_ptr<CLineSeries>>> m_spvspSeries;

#pragma warning(disable: 4996)
	boost::signals2::signal<void(NotifyContainerChangedEventArgs<std::shared_ptr<CLineSeries>>&)> SeriesChanged;
#pragma warning(default: 4996)

public:
	CLineChart(std::shared_ptr<CChartProperty> spChartProperty);
	void OnPaint(PaintEventArgs& e);
	void OnMouseMove(MouseEventArgs& e);

	std::shared_ptr<std::vector<std::shared_ptr<CLineSeries>>> GetSeriesPtr(){return m_spvspSeries;}
	void SetSeriesPtr(std::shared_ptr<std::vector<std::shared_ptr<CLineSeries>>> spvsp){m_spvspSeries=spvsp;UpdateAxis();}
	void AddSeries(std::shared_ptr<CLineSeries> spSeries);
	void ClearSeries();
	void UpdateAxis();
	
	//CColor static Colors(int i)
	//{
	//	switch(i){
	//		case 0:return RGB(255,0,0);//Red
	//		case 1:return RGB(0,255,0);//Green
	//		case 2:return RGB(0,0,255);//Blue
	//		case 3:return RGB(255,255,0);//Yellow
	//		case 4:return RGB(255,0,255);//Purple
	//		case 5:return RGB(0,255,255);//SkyBlue
	//		default:return RGB(0,0,0);//Black
	//	}
	//}
};


class CBarSeries;
class CXBarAxis;
class CBarChartLegend;

class CBarChart:public CChart
{
private:
	std::shared_ptr<std::vector<std::shared_ptr<CBarSeries>>> m_spvspSeries;

#pragma warning(disable: 4996)
	boost::signals2::signal<void(NotifyContainerChangedEventArgs<std::shared_ptr<CBarSeries>>&)> SeriesChanged;
#pragma warning(default: 4996)

public:
	CBarChart(std::shared_ptr<CChartProperty> spChartProperty);
	void OnPaint(PaintEventArgs& e);

	std::shared_ptr<std::vector<std::shared_ptr<CBarSeries>>> GetSeriesPtr(){return m_spvspSeries;}
	void SetSeriesPtr(std::shared_ptr<std::vector<std::shared_ptr<CBarSeries>>> spvsp){m_spvspSeries=spvsp;UpdateAxis();}
	void AddSeries(std::shared_ptr<CBarSeries> spSeries);
	void ClearSeries();
	void UpdateAxis();
};


