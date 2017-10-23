#include "3DLineChart.h"
#include "OGLChart.h"
#include "MyRgn.h"
#include "3DLineChartResource.h"
#include "GDIUIElement.h"

void CLine::Measure(CDC* pDC)
{
	if(!m_bVisible)return;

	m_size.cx = m_lineLength + m_margin.left + m_margin.right;
	m_size.cy = m_margin.top + m_margin.bottom;
}

void CLine::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	HPEN hPen = e.DCPtr->SelectPen(*(m_getter()->GetPenPtr()));


	HBRUSH hBrush = e.DCPtr->SelectBrush(*(m_getter()->GetBrushPtr()));

	CRect rc(GetRect());
	rc.DeflateRect(m_margin);

	CPoint ptCenter(rc.CenterPoint());
	//Paint Line
	e.DCPtr->MoveToEx(rc.left, ptCenter.y);
	e.DCPtr->LineTo(rc.right, ptCenter.y);
	//Paint Mark
	//e.DCPtr->Ellipse(ptCenter.x - m_markRadius, ptCenter.y - m_markRadius,
	//				ptCenter.x + m_markRadius, ptCenter.y + m_markRadius);

	e.DCPtr->SelectPen(hPen);
	e.DCPtr->SelectBrush(hBrush);
}

C3DLineChart::C3DLineChart(std::shared_ptr<COGLChartProperty>& spChartProperty)
	:CPanel(nullptr),
	m_spChartProperty(spChartProperty),
	m_spRightPane(std::make_shared<CDockPanel>(this)),
	m_spLegend(std::make_shared<COGLChartLegend>(m_spRightPane.get())),
	m_spImage(std::make_shared<CImage<IDB_3DLINECHARTOPERATION>>(m_spRightPane.get())),
	m_spOGLChart(std::make_shared<COGLChart>(m_spRightPane.get(), spChartProperty)),
	m_spvspSeries(std::make_shared<std::vector<std::shared_ptr<COGLSeries>>>())
{
	m_spImage->SetMargin(CRect(0,10,0,10));
	m_spImage->SetHorizontalAlignment(HorizontalAlignment::Right);

	m_spRightPane->GetChildrenPtr()->push_back(m_spLegend);
	m_spRightPane->SetDock(m_spLegend, Dock::Top);
	m_spRightPane->GetChildrenPtr()->push_back(m_spImage);
	m_spRightPane->SetDock(m_spImage, Dock::Bottom);

	m_children->push_back(m_spOGLChart);
	m_children->push_back(m_spRightPane);

	SeriesChanged.connect(std::bind(&COGLChartLegend::OnContainerChanged,static_cast<COGLChartLegend*>(m_spLegend.get()),std::placeholders::_1));
	SeriesChanged.connect(std::bind(&COGLChart::OnContainerChanged,static_cast<COGLChart*>(m_spOGLChart.get()),std::placeholders::_1));
}

void C3DLineChart::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

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

	CSize sizeStackPanel = m_spRightPane->GetMeasuredSize();

	//Set Rect
	m_spRightPane->SetRect(CRect(rc.right-sizeStackPanel.cx, rc.top, rc.right, rc.bottom));
	m_spOGLChart->SetRect(CRect(rc.left, rc.top, rc.right - sizeStackPanel.cx, rc.bottom));
	
	//Paint All
	boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
		spElement->OnPaint(e);
	});

	e.DCPtr->SelectClipRgn(NULL);
}

void C3DLineChart::OnLButtonDown(MouseEventArgs& e)
{
	for(auto& child : *m_children){
		child->OnLButtonDown(e);
	}
}
void C3DLineChart:: OnLButtonUp(MouseEventArgs& e)
{
	for(auto& child : *m_children){
		child->OnLButtonUp(e);
	}
}
void C3DLineChart::OnMButtonDown(MouseEventArgs& e)
{
	for(auto& child : *m_children){
		child->OnMButtonDown(e);
	}
}
void C3DLineChart::OnMButtonUp(MouseEventArgs& e)
{
	for(auto& child : *m_children){
		child->OnMButtonUp(e);
	}
}
void C3DLineChart::OnMouseMove(MouseEventArgs& e)
{
	for(auto& child : *m_children){
		child->OnMouseMove(e);
	}
}
void C3DLineChart::OnMouseWheel(MouseWheelEventArgs& e)
{
	for(auto& child : *m_children){
		child->OnMouseWheel(e);
	}
}

void C3DLineChart::OnKeyDown(KeyEventArgs& e)
{
	for(auto& child : *m_children){
		child->OnKeyDown(e);
	}
}

void C3DLineChart::OnContextMenu(ContextMenuEventArgs& e)
{
	for(auto& child : *m_children){
		child->OnContextMenu(e);
	}
}

//void C3DLineChart::UpdateAxis()
//{
//	boost::for_each(*m_spvspSeries,[&](std::shared_ptr<CLineSeries> spSeries)
//	{
//		double xMin,xMax,yMin,yMax;
//		if(m_spvspSeries->empty()){
//			xMin=spSeries->GetPoint(0).x;
//			xMax=spSeries->GetPoint(0).x;
//			yMin=spSeries->GetPoint(0).y;
//			yMax=spSeries->GetPoint(0).y;
//		}else{
//			xMin=m_spXAxis->GetMinimumValue();
//			xMax=m_spXAxis->GetMaximumValue();
//			yMin=m_spYAxis->GetMinimumValue();
//			yMax=m_spYAxis->GetMaximumValue();
//		}
//		//Add Series
//		//m_spvspSeries->push_back(spSeries);
//
//		//Calc Min Max Value
//		for(auto i=0;i<spSeries->GetSize();i++)
//		{
//			xMax=max(xMax,spSeries->GetPoint(i).x);
//			xMin=min(xMin,spSeries->GetPoint(i).x);
//			yMax=max(yMax,spSeries->GetPoint(i).y);
//			yMin=min(yMin,spSeries->GetPoint(i).y);
//		};
//		m_spXAxis->SetMinimumValue(xMin);
//		m_spXAxis->SetMaximumValue(xMax);
//		m_spYAxis->SetMinimumValue(yMin);
//		m_spYAxis->SetMaximumValue(yMax);
//
//		//Calc Tick and Set Tick, Min and Max
//		//In case Tick==0 (Min=Max=0), Set 1,1,-1.
//		if(xMin==xMax){
//			m_spXAxis->SetTick(1);
//			m_spXAxis->SetMinimum(xMin-1);
//			m_spXAxis->SetMaximum(xMax+1);
//		}else{
//			double xTick=CAxis::CalcAutoTick(xMin,xMax,4,8);
//			m_spXAxis->SetTick(xTick);
//			m_spXAxis->SetMinimum(CAxis::CalcTickStart(xMin,xTick));
//			m_spXAxis->SetMaximum(CAxis::CalcTickEnd(xMax,xTick));
//		}
//
//		if(yMin==yMax){
//			m_spYAxis->SetTick(1);
//			m_spYAxis->SetMinimum(yMin-1);
//			m_spYAxis->SetMaximum(yMax+1);
//		}else{
//
//			double yTick=CAxis::CalcAutoTick(yMin,yMax,4,8);
//			m_spYAxis->SetTick(yTick);
//			m_spYAxis->SetMinimum(CAxis::CalcTickStart(yMin,yTick));
//			m_spYAxis->SetMaximum(CAxis::CalcTickEnd(yMax,yTick));	
//		}
//
//	});
//}

void C3DLineChart::ClearSeries()
{
	std::vector<std::shared_ptr<COGLSeries>> oldItems;
	boost::copy(*m_spvspSeries, back_inserter(oldItems));
	m_spvspSeries->clear();
	
	SeriesChanged(NotifyContainerChangedEventArgs<std::shared_ptr<COGLSeries>>(NotifyContainerChangedAction::Clear, std::vector<std::shared_ptr<COGLSeries>>(), oldItems));	
}

void C3DLineChart::AddSeries(std::shared_ptr<COGLSeries> spSeries)
{
	m_spvspSeries->push_back(spSeries);
	std::vector<std::shared_ptr<COGLSeries>> newItems;
	newItems.push_back(spSeries);
	SeriesChanged(NotifyContainerChangedEventArgs<std::shared_ptr<COGLSeries>>(NotifyContainerChangedAction::Add,newItems, std::vector<std::shared_ptr<COGLSeries>>()));
}