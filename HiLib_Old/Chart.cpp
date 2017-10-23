#include "Chart.h"
#include "MyRect.h"
#include "MyRgn.h"
#include "MySize.h"
#include "ChartElements.h"
#include "Series.h"

CChart::CChart(std::shared_ptr<CChartProperty> spChartProperty,
			std::shared_ptr<CXAxis> spXAxis,
			std::shared_ptr<CYAxis> spYAxis,
			std::shared_ptr<CPlotArea> spPlotArea,
			std::shared_ptr<CChartTitle> spTitle,
			std::shared_ptr<CChartLegend> spLegend,
			std::shared_ptr<CLabel> spXLabel,
			std::shared_ptr<CLabel> spYLabel)
	:CPanel(nullptr),
	m_spChartProperty(spChartProperty),
	m_spXAxis(spXAxis),
	m_spYAxis(spYAxis),
	m_spPlotArea(spPlotArea),
	m_spTitle(spTitle),
	m_spLegend(spLegend),
	m_spXLabel(spXLabel),
	m_spYLabel(spYLabel)
{
	m_children->push_back(m_spTitle);
	m_children->push_back(m_spXAxis);
	m_children->push_back(m_spYAxis);
	m_children->push_back(m_spPlotArea);
	m_children->push_back(m_spLegend);
	m_children->push_back(m_spXLabel);
	m_children->push_back(m_spYLabel);


}

CLineChart::CLineChart(std::shared_ptr<CChartProperty> spChartProperty)
	:CChart(
		spChartProperty,
		std::make_shared<CXAxis>(this, spChartProperty->GetXAxisPropertyPtr()),
		std::make_shared<CYAxis>(this, spChartProperty->GetYAxisPropertyPtr()),
		std::make_shared<CPlotArea>(this, spChartProperty->GetPlotAreaPropertyPtr()),
		std::make_shared<CChartTitle>(this, spChartProperty->GetTitlePropertyPtr()),
		std::make_shared<CLineChartLegend>(this),
		std::make_shared<CLabel>(this,spChartProperty->GetXLabelPropertyPtr()),
		std::make_shared<CLabel>(this,spChartProperty->GetYLabelPropertyPtr())
	),
		m_spvspSeries(std::make_shared<std::vector<std::shared_ptr<CLineSeries>>>())
{
	SeriesChanged.connect(std::bind(&CLineChartLegend::OnContainerChanged,static_cast<CLineChartLegend*>(m_spLegend.get()),std::placeholders::_1));
}

void CLineChart::OnPaint(PaintEventArgs& e)
{
	CRect rc(GetRect());
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rc);
	e.DCPtr->SelectClipRgn(rgn);
	e.DCPtr->SetBkMode(TRANSPARENT);

	//Measure All
	boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
		spElement->Measure(e.DCPtr);
	});

	//Get Measured Size
	int widthYAxis = m_spYAxis->GetMeasuredSize().cx;
	int widthYLabel = m_spYLabel->GetMeasuredSize().cy;
	int heightXAxis = m_spXAxis->GetMeasuredSize().cy;
	int heightXLabel = m_spXLabel->GetMeasuredSize().cy;
	CSize sizeTitle = m_spTitle->GetMeasuredSize();
	CSize sizeLegend = m_spLegend->GetMeasuredSize();

	//Set Rect
	m_spTitle->SetRect(CRect(rc.left, rc.top, rc.right, rc.top + sizeTitle.cy));
	m_spYLabel->SetRect(CRect(rc.left, rc.top + sizeTitle.cy, rc.left + widthYLabel, rc.bottom - (heightXAxis + heightXLabel)));
	m_spYAxis->SetRect(CRect(rc.left + widthYLabel, rc.top + sizeTitle.cy, rc.left + (widthYLabel + widthYAxis),rc.bottom - (heightXAxis + heightXLabel)));
	m_spXAxis->SetRect(CRect(rc.left + (widthYLabel + widthYAxis), rc.bottom - (heightXAxis + heightXLabel), rc.right-sizeLegend.cx, rc.bottom - heightXLabel));
	m_spXLabel->SetRect(CRect(rc.left + (widthYLabel + widthYAxis), rc.bottom - heightXAxis, rc.right-sizeLegend.cx, rc.bottom));

	m_spLegend->SetRect(CRect(rc.right-sizeLegend.cx, rc.top + sizeTitle.cy, rc.right, rc.bottom));
	m_spPlotArea->SetRect(CRect(rc.left + (widthYAxis + widthYLabel), rc.top + sizeTitle.cy, rc.right - sizeLegend.cx, rc.bottom - (heightXAxis + heightXLabel)));
	
	boost::for_each(*m_spvspSeries,[&](std::shared_ptr<CLineSeries>& spSeries)->void
	  {
		  spSeries->SetRect(m_spPlotArea->GetRect());
	  });
	
	//Paint All
	boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
		spElement->OnPaint(e);
	});

	e.DCPtr->SelectClipRgn(NULL);
}

void CLineChart::OnMouseMove(MouseEventArgs& e)
{
	//Paint All
	boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
		spElement->OnMouseMove(e);
	});

}


void CLineChart::UpdateAxis()
{
	boost::for_each(*m_spvspSeries,[&](std::shared_ptr<CLineSeries> spSeries)
	{
		double xMin,xMax,yMin,yMax;
		if(m_spvspSeries->empty()){
			xMin=spSeries->GetPoint(0).x;
			xMax=spSeries->GetPoint(0).x;
			yMin=spSeries->GetPoint(0).y;
			yMax=spSeries->GetPoint(0).y;
		}else{
			xMin=m_spXAxis->GetMinimumValue();
			xMax=m_spXAxis->GetMaximumValue();
			yMin=m_spYAxis->GetMinimumValue();
			yMax=m_spYAxis->GetMaximumValue();
		}
		//Add Series
		//m_spvspSeries->push_back(spSeries);

		//Calc Min Max Value
		for(auto i=0;i<spSeries->GetSize();i++)
		{
			xMax=max(xMax,spSeries->GetPoint(i).x);
			xMin=min(xMin,spSeries->GetPoint(i).x);
			yMax=max(yMax,spSeries->GetPoint(i).y);
			yMin=min(yMin,spSeries->GetPoint(i).y);
		};
		m_spXAxis->SetMinimumValue(xMin);
		m_spXAxis->SetMaximumValue(xMax);
		m_spYAxis->SetMinimumValue(yMin);
		m_spYAxis->SetMaximumValue(yMax);

		//Calc Tick and Set Tick, Min and Max
		//In case Tick==0 (Min=Max=0), Set 1,1,-1.
		if(xMin==xMax){
			m_spXAxis->SetTick(1);
			m_spXAxis->SetMinimum(xMin-1);
			m_spXAxis->SetMaximum(xMax+1);
		}else{
			double xTick=CAxis::CalcAutoTick(xMin,xMax,4,8);
			m_spXAxis->SetTick(xTick);
			m_spXAxis->SetMinimum(CAxis::CalcTickStart(xMin,xTick));
			m_spXAxis->SetMaximum(CAxis::CalcTickEnd(xMax,xTick));
		}

		if(yMin==yMax){
			m_spYAxis->SetTick(1);
			m_spYAxis->SetMinimum(yMin-1);
			m_spYAxis->SetMaximum(yMax+1);
		}else{

			double yTick=CAxis::CalcAutoTick(yMin,yMax,4,8);
			m_spYAxis->SetTick(yTick);
			m_spYAxis->SetMinimum(CAxis::CalcTickStart(yMin,yTick));
			m_spYAxis->SetMaximum(CAxis::CalcTickEnd(yMax,yTick));	
		}

	});
}

void CLineChart::ClearSeries()
{
	std::vector<std::shared_ptr<CLineSeries>> oldItems;
	boost::copy(*m_spvspSeries, back_inserter(oldItems));
	m_spvspSeries->clear();
	auto iter = boost::remove_if(*m_children,[&](std::shared_ptr<CGDIUIElement> spElement)->bool{
		if(auto p = std::dynamic_pointer_cast<CLineSeries>(spElement)){
			return true;
		}else{
			return false;
		}
	});
	m_children->erase(iter, m_children->end());
	SeriesChanged(NotifyContainerChangedEventArgs<std::shared_ptr<CLineSeries>>(NotifyContainerChangedAction::Clear, std::vector<std::shared_ptr<CLineSeries>>(), oldItems));	
}


void CLineChart::AddSeries(std::shared_ptr<CLineSeries> spSeries)
{
	//Initialize Min Max	
	double xMin,xMax,yMin,yMax;
	//if(m_spvspSeries->empty()){
	//	xMin=spSeries->m_vDPT[0].x;
	//	xMax=spSeries->m_vDPT[0].x;
	//	yMin=spSeries->m_vDPT[0].y;
	//	yMax=spSeries->m_vDPT[0].y;
	//}else{
		//xMin=min(0, m_spXAxis->GetMinimumValue());
		//xMax=max(0, m_spXAxis->GetMaximumValue());
		//yMin=min(0, m_spYAxis->GetMinimumValue());
		//yMax=max(0, m_spYAxis->GetMaximumValue());
	//}
		xMin=m_spXAxis->GetMinimumValue();
		xMax=m_spXAxis->GetMaximumValue();
		yMin=m_spYAxis->GetMinimumValue();
		yMax=m_spYAxis->GetMaximumValue();
	//Add Series
	m_spvspSeries->push_back(spSeries);
	m_children->push_back(spSeries);

	std::vector<std::shared_ptr<CLineSeries>> newItems;
	newItems.push_back(spSeries);
	SeriesChanged(NotifyContainerChangedEventArgs<std::shared_ptr<CLineSeries>>(NotifyContainerChangedAction::Add,newItems, std::vector<std::shared_ptr<CLineSeries>>()));


	//Calc Min Max Value
	for(auto i=0;i<spSeries->GetSize();i++)
	{
		xMax=max(xMax,spSeries->GetPoint(i).x);
		xMin=min(xMin,spSeries->GetPoint(i).x);
		yMax=max(yMax,spSeries->GetPoint(i).y);
		yMin=min(yMin,spSeries->GetPoint(i).y);
	};
	m_spXAxis->SetMinimumValue(xMin);
	m_spXAxis->SetMaximumValue(xMax);
	m_spYAxis->SetMinimumValue(yMin);
	m_spYAxis->SetMaximumValue(yMax);

	//Calc Tick and Set Tick, Min and Max
	//In case Tick==0 (Min=Max=0), Set 1,1,-1.
	if(xMin==xMax){
		m_spXAxis->SetTick(1);
		m_spXAxis->SetMinimum(xMin-1);
		m_spXAxis->SetMaximum(xMax+1);
	}else{
		double xTick=CAxis::CalcAutoTick(xMin,xMax,4,8);
		m_spXAxis->SetTick(xTick);
		m_spXAxis->SetMinimum(CAxis::CalcTickStart(xMin,xTick));
		m_spXAxis->SetMaximum(CAxis::CalcTickEnd(xMax,xTick));
	}

	if(yMin==yMax){
		m_spYAxis->SetTick(1);
		m_spYAxis->SetMinimum(yMin-1);
		m_spYAxis->SetMaximum(yMax+1);
	}else{

		double yTick=CAxis::CalcAutoTick(yMin,yMax,4,8);
		m_spYAxis->SetTick(yTick);
		m_spYAxis->SetMinimum(CAxis::CalcTickStart(yMin,yTick));
		m_spYAxis->SetMaximum(CAxis::CalcTickEnd(yMax,yTick));	
	}



}


//CBarChart
CBarChart::CBarChart(std::shared_ptr<CChartProperty> spChartProperty)
	:CChart(
		spChartProperty,
		std::make_shared<CXBarAxis>(this, spChartProperty->GetXAxisPropertyPtr()),
		std::make_shared<CYAxis>(this, spChartProperty->GetYAxisPropertyPtr()),
		std::make_shared<CPlotArea>(this, spChartProperty->GetPlotAreaPropertyPtr()),
		std::make_shared<CChartTitle>(this, spChartProperty->GetTitlePropertyPtr()),
		std::make_shared<CBarChartLegend>(this),
		std::make_shared<CLabel>(this,spChartProperty->GetXLabelPropertyPtr()),
		std::make_shared<CLabel>(this,spChartProperty->GetYLabelPropertyPtr())
	),
	m_spvspSeries(std::make_shared<std::vector<std::shared_ptr<CBarSeries>>>())
{
	SeriesChanged.connect(std::bind(&CBarChartLegend::OnContainerChanged,static_cast<CBarChartLegend*>(m_spLegend.get()),std::placeholders::_1));
}

void CBarChart::OnPaint(PaintEventArgs& e)
{
	CRect rc(GetRect());
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rc);
	e.DCPtr->SelectClipRgn(rgn);
	e.DCPtr->SetBkMode(TRANSPARENT);

	//Measure All
	boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
		spElement->Measure(e.DCPtr);
	});

	//Get Measured Size
	//int w = m_spYAxis->GetMeasuredSize().cx;
	//int h = m_spXAxis->GetMeasuredSize().cy;
	//CSize sizeTitle = m_spTitle->GetMeasuredSize();
	//CSize sizeLegend = m_spLegend->GetMeasuredSize();

	////Set Rect
	//m_spTitle->SetRect(CRect(rc.left, rc.top, rc.right, rc.top + sizeTitle.cy));
	//m_spYAxis->SetRect(CRect(rc.left, rc.top + sizeTitle.cy, rc.left + w,rc.bottom-h));
	//m_spXAxis->SetRect(CRect(rc.left+w,rc.bottom-h,rc.right-sizeLegend.cx, rc.bottom));
	//m_spLegend->SetRect(CRect(rc.right-sizeLegend.cx, rc.top + sizeTitle.cy, rc.right, rc.bottom-h));
	//m_spPlotArea->SetRect(CRect(rc.left+w,rc.top + sizeTitle.cy,rc.right - sizeLegend.cx, rc.bottom-h));

	//Get Measured Size
	int widthYAxis = m_spYAxis->GetMeasuredSize().cx;
	int widthYLabel = m_spYLabel->GetMeasuredSize().cy;
	int heightXAxis = m_spXAxis->GetMeasuredSize().cy;
	int heightXLabel = m_spXLabel->GetMeasuredSize().cy;
	CSize sizeTitle = m_spTitle->GetMeasuredSize();
	CSize sizeLegend = m_spLegend->GetMeasuredSize();

	//Set Rect
	m_spTitle->SetRect(CRect(rc.left, rc.top, rc.right, rc.top + sizeTitle.cy));
	m_spYLabel->SetRect(CRect(rc.left, rc.top + sizeTitle.cy, rc.left + widthYLabel, rc.bottom - (heightXAxis + heightXLabel)));
	m_spYAxis->SetRect(CRect(rc.left + widthYLabel, rc.top + sizeTitle.cy, rc.left + (widthYLabel + widthYAxis),rc.bottom - (heightXAxis + heightXLabel)));
	m_spXAxis->SetRect(CRect(rc.left + (widthYLabel + widthYAxis), rc.bottom - (heightXAxis + heightXLabel), rc.right-sizeLegend.cx, rc.bottom - heightXLabel));
	m_spXLabel->SetRect(CRect(rc.left + (widthYLabel + widthYAxis), rc.bottom - heightXAxis, rc.right-sizeLegend.cx, rc.bottom));

	m_spLegend->SetRect(CRect(rc.right-sizeLegend.cx, rc.top + sizeTitle.cy, rc.right, rc.bottom));
	m_spPlotArea->SetRect(CRect(rc.left + (widthYAxis + widthYLabel), rc.top + sizeTitle.cy, rc.right - sizeLegend.cx, rc.bottom - (heightXAxis + heightXLabel)));
	
	//Set Rect only for Visible Series
	CRect rcPlot = m_spPlotArea->GetRect();
	std::vector<std::shared_ptr<CBarSeries>> vspVisibleSeries;
	std::copy_if(m_spvspSeries->begin(),m_spvspSeries->end(),std::back_inserter(vspVisibleSeries),
		[&](const std::shared_ptr<CBarSeries>& spSeries)->bool{
			return spSeries->GetVisible();
		});
	if(!vspVisibleSeries.empty()){
		int rCLineSeriesWidth = rcPlot.Width()/vspVisibleSeries.size();
		for(unsigned n=0;n<vspVisibleSeries.size();n++)
		{
			auto spSeries=vspVisibleSeries[n];
			if(spSeries->GetVisible()){
				spSeries->SetRect(
					CRect(
						rcPlot.left+rCLineSeriesWidth*n,
						rcPlot.top,
						rcPlot.left+rCLineSeriesWidth*(n+1),
						rcPlot.bottom)
				);
			}
		}
	}
	
	//Paint All
	boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
		spElement->OnPaint(e);
	});

	e.DCPtr->SelectClipRgn(NULL);

}

void CBarChart::UpdateAxis()
{
	double yMin=0,yMax=0;
	boost::for_each(*m_spvspSeries,[&](std::shared_ptr<CBarSeries> spSeries)
	{
		//Calc Min Max Value
		yMax=max(yMax,spSeries->GetValue());
		yMin=min(yMin,spSeries->GetValue());
	});

	//Set Min Max value
	m_spYAxis->SetMinimumValue(yMin);
	m_spYAxis->SetMaximumValue(yMax);

	//Calc Tick and Set Tick, Min and Max
	//In case Tick==0 (Min=Max=0), Set 1,1,-1.

	if(yMin==yMax){
		m_spYAxis->SetTick(1);
		m_spYAxis->SetMinimum(yMin-1);
		m_spYAxis->SetMaximum(yMax+1);
	}else{

		double yTick=CAxis::CalcAutoTick(yMin,yMax,4,8);
		m_spYAxis->SetTick(yTick);
		m_spYAxis->SetMinimum(CAxis::CalcTickStart(yMin,yTick));
		m_spYAxis->SetMaximum(CAxis::CalcTickEnd(yMax,yTick));	
	}

	//Left and Right
	m_spXAxis->SetTick(100);
	m_spXAxis->SetMinimum(0);
	m_spXAxis->SetMaximum(100);	
}

void CBarChart::ClearSeries()
{
	std::vector<std::shared_ptr<CBarSeries>> oldItems;
	boost::copy(*m_spvspSeries, back_inserter(oldItems));
	m_spvspSeries->clear();
	auto iter = boost::remove_if(*m_children,[&](std::shared_ptr<CGDIUIElement> spElement)->bool{
		if(auto p = std::dynamic_pointer_cast<CBarSeries>(spElement)){
			return true;
		}else{
			return false;
		}
	});
	m_children->erase(iter, m_children->end());
	SeriesChanged(NotifyContainerChangedEventArgs<std::shared_ptr<CBarSeries>>(NotifyContainerChangedAction::Clear, std::vector<std::shared_ptr<CBarSeries>>(), oldItems));	
}


void CBarChart::AddSeries(std::shared_ptr<CBarSeries> spSeries)
{
	//Initialize Min Max	
	double yMin=min(0, m_spYAxis->GetMinimumValue());
	double yMax=max(0, m_spYAxis->GetMaximumValue());

	//Add Series
	m_spvspSeries->push_back(spSeries);
	m_children->push_back(spSeries);

	std::vector<std::shared_ptr<CBarSeries>> newItems;
	newItems.push_back(spSeries);
	SeriesChanged(NotifyContainerChangedEventArgs<std::shared_ptr<CBarSeries>>(NotifyContainerChangedAction::Add,newItems, std::vector<std::shared_ptr<CBarSeries>>()));

	//Calc Min Max Value
	yMin=min(yMin,spSeries->GetValue());
	yMax=max(yMax,spSeries->GetValue());

	m_spYAxis->SetMinimumValue(yMin);
	m_spYAxis->SetMaximumValue(yMax);

	//Calc Tick and Set Tick, Min and Max
	//In case Tick==0 (Min=Max=0), Set 1,1,-1.

	if(yMin==yMax){
		m_spYAxis->SetTick(1);
		m_spYAxis->SetMinimum(yMin-1);
		m_spYAxis->SetMaximum(yMax+1);
	}else{

		double yTick=CAxis::CalcAutoTick(yMin,yMax,4,8);
		m_spYAxis->SetTick(yTick);
		m_spYAxis->SetMinimum(CAxis::CalcTickStart(yMin,yTick));
		m_spYAxis->SetMaximum(CAxis::CalcTickEnd(yMax,yTick));	
	}

	//Left and Right
	m_spXAxis->SetTick(100);
	m_spXAxis->SetMinimum(0);
	m_spXAxis->SetMaximum(100);	
}



