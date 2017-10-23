#include "Series.h"
#include "Chart.h"
#include "ChartElements.h"

CLineSeries::CLineSeries(CChart* pChart):CGDIUIElement(pChart){}

void CLineSeries::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	//HPEN hPen = NULL;
	//if(GetState()==UIElementState::Hot){
	//	hPen= e.DCPtr->SelectPen(*(m_spSeriesProperty->GetHotPenPtr()));
	//}else{
	HPEN hPen= e.DCPtr->SelectPen(*(m_spSeriesProperty->GetPenPtr()));
//	}
	HBRUSH hBrush = e.DCPtr->SelectBrush(*(m_spSeriesProperty->GetBrushPtr()));

	CRect rc = GetRect();
	double xMin=static_cast<CLineChart*>(m_pParent)->GetXAxisPtr()->GetMinimum();
	double xMax=static_cast<CLineChart*>(m_pParent)->GetXAxisPtr()->GetMaximum();
	double yMin=static_cast<CLineChart*>(m_pParent)->GetYAxisPtr()->GetMinimum();
	double yMax=static_cast<CLineChart*>(m_pParent)->GetYAxisPtr()->GetMaximum();


	switch(m_dataPtr->GetExtrapolationType()){
	case ExtrapolationType::Constant:
		{
			int x=(int)(rc.left);
			int y=(int)(rc.bottom - (m_dataPtr->GetPoint(0).y-yMin)/(yMax-yMin)*rc.Height());
			e.DCPtr->MoveToEx(x,y);
			for(unsigned i=0;i<m_dataPtr->GetSize();i++){
				x = (int)((m_dataPtr->GetPoint(i).x-xMin)/(xMax-xMin)*rc.Width() + rc.left);
				y = (int)(rc.bottom - (m_dataPtr->GetPoint(i).y-yMin)/(yMax-yMin)*rc.Height());
				e.DCPtr->LineTo(x,y);
				e.DCPtr->Ellipse(x-m_markRadius,y-m_markRadius,x+m_markRadius,y+m_markRadius);
			}
			x = (int)(rc.right);
			y = (int)(rc.bottom - (m_dataPtr->GetPoint(m_dataPtr->GetSize()-1).y-yMin)/(yMax-yMin)*rc.Height());
			e.DCPtr->LineTo(x,y);
			break;
		}
	case ExtrapolationType::None:
		{
			for(unsigned i=0;i<m_dataPtr->GetSize();i++){
				auto x = (int)((m_dataPtr->GetPoint(i).x-xMin)/(xMax-xMin)*rc.Width() + rc.left);
				auto y = (int)(rc.bottom - (m_dataPtr->GetPoint(i).y-yMin)/(yMax-yMin)*rc.Height());
				if(i==0){
					e.DCPtr->MoveToEx(x,y);
				}else{
					e.DCPtr->LineTo(x,y);		
				}
				e.DCPtr->Ellipse(x-m_markRadius,y-m_markRadius,x+m_markRadius,y+m_markRadius);
			}
		}
	}

	e.DCPtr->SelectPen(hPen);
	e.DCPtr->SelectBrush(hBrush);
}

void CLineSeries::OnMouseMove(MouseEventArgs& e)
{
	if(!m_bVisible)return;

	CRect rc = GetRect();
	double xMin=static_cast<CLineChart*>(m_pParent)->GetXAxisPtr()->GetMinimum();
	double xMax=static_cast<CLineChart*>(m_pParent)->GetXAxisPtr()->GetMaximum();
	double yMin=static_cast<CLineChart*>(m_pParent)->GetYAxisPtr()->GetMinimum();
	double yMax=static_cast<CLineChart*>(m_pParent)->GetYAxisPtr()->GetMaximum();


	switch(m_dataPtr->GetExtrapolationType()){
	case ExtrapolationType::Constant:
		{
			int x = 0;
			int y = 0;
			int x_before=(int)(rc.left);
			int y_before=(int)(rc.bottom - (m_dataPtr->GetPoint(0).y-yMin)/(yMax-yMin)*rc.Height());

			for(unsigned i=0;i<m_dataPtr->GetSize();i++){
				x = (int)((m_dataPtr->GetPoint(i).x-xMin)/(xMax-xMin)*rc.Width() + rc.left);
				y = (int)(rc.bottom - (m_dataPtr->GetPoint(i).y-yMin)/(yMax-yMin)*rc.Height());
				if(x_before <= e.Point.x && e.Point.x <= x  && abs(double(y-y_before)/double(x-x_before)*double(e.Point.x-x_before)+double(y_before) - double(e.Point.y)) < 3.0f ) {
					SetState(UIElementState::Hot);
					return;
				}
				x_before = x;
				y_before = y;
			}
			x = (int)(rc.right);
			y = (int)(rc.bottom - (m_dataPtr->GetPoint(m_dataPtr->GetSize()-1).y-yMin)/(yMax-yMin)*rc.Height());
			if(x_before <= e.Point.x && e.Point.x <= x  && abs(double(y-y_before)/double(x-x_before)*double(e.Point.x-x_before)+double(y_before) - double(e.Point.y)) < 3.0f ) {
				SetState(UIElementState::Hot);
				return;
			}
			SetState(UIElementState::Normal);
			break;
		}
	case ExtrapolationType::None:
		{
			//for(unsigned i=0;i<m_dataPtr->GetSize();i++){
			//	auto x = (int)((m_dataPtr->GetPoint(i).x-xMin)/(xMax-xMin)*rc.Width() + rc.left);
			//	auto y = (int)(rc.bottom - (m_dataPtr->GetPoint(i).y-yMin)/(yMax-yMin)*rc.Height());
			//	if(i==0){
			//		e.DCPtr->MoveToEx(x,y);
			//	}else{
			//		e.DCPtr->LineTo(x,y);		
			//	}
			//	e.DCPtr->Ellipse(x-m_markRadius,y-m_markRadius,x+m_markRadius,y+m_markRadius);
			//}
		}
	}

}

CBarSeries::CBarSeries(CChart* pChart):CGDIUIElement(pChart){}
void CBarSeries::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	HPEN hPen = e.DCPtr->SelectPen(*(m_spSeriesProperty->GetPenPtr()));
	HBRUSH hBrush = e.DCPtr->SelectBrush(*(m_spSeriesProperty->GetBrushPtr()));
	HFONT hFont = e.DCPtr->SelectFont(*(m_spSeriesProperty->GetFontPtr()));
	COLORREF cr = e.DCPtr->SetTextColor(m_spSeriesProperty->GetColor());

	CRect rc = GetRect();

	double yMin=static_cast<CBarChart*>(m_pParent)->GetYAxisPtr()->GetMinimum();
	double yMax=static_cast<CBarChart*>(m_pParent)->GetYAxisPtr()->GetMaximum();

	int y=(int)(rc.bottom - (m_value-yMin)/(yMax-yMin)*rc.Height());
	int y0=(int)(rc.bottom - (0-yMin)/(yMax-yMin)*rc.Height());

	e.DCPtr->Rectangle(
		CRect(
			rc.left+rc.Width()/4,
			y,
			rc.right-rc.Width()/4,
			y0)
		);

	CRect rcValue(0, 0, 0, 0);
	UINT format = DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER;
	std::wstring wstrValue = roundtowstring(m_value, 8);
	e.DCPtr->DrawTextEx((LPTSTR)wstrValue.c_str(), -1, rcValue, DT_CALCRECT, NULL);
	rcValue.SetRect(rc.left+rc.Width()/4, y - rcValue.Height() - MARGIN_BAR_VALUE, rc.right-rc.Width()/4, y - MARGIN_BAR_VALUE);
	e.DCPtr->DrawTextEx((LPTSTR)wstrValue.c_str(), -1, rcValue, format, NULL);

	e.DCPtr->SelectPen(hPen);
	e.DCPtr->SelectBrush(hBrush);
	e.DCPtr->SelectFont(hFont);
	e.DCPtr->SetTextColor(cr);
}