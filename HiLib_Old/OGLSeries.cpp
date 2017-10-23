#include "OGLSeries.h"
#include "OGLChartWnd.h"
#include "OGLChart.h"


void COGLSeries::OnPaint(OGLPaintEventArgs& e)
{
	if(!m_bVisible)return;

	auto pChart = static_cast<COGLChart*>(m_pParent);
	auto spPen = m_spSeriesProperty->GetPenPtr();
	auto index = static_cast<COGLChart*>(m_pParent)->GetSeriesIndex(this);
	::glLoadName(index);
	::glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
	//Line
	::glLineWidth( spPen->GetWidth() );
	::glBegin(GL_LINE_STRIP);
	::glColor3ub(spPen->GetColor().GetR(), spPen->GetColor().GetG(), spPen->GetColor().GetB());
	for (auto& p : m_points) {
		::glVertex3fv((float*)&(pChart->NormalizeVertex(p)));	
	}
	::glEnd();

	//Plot
	::glPointSize(4);
	::glBegin( GL_POINTS );
	::glColor3ub(spPen->GetColor().GetR(),spPen->GetColor().GetG(), spPen->GetColor().GetB() );
	for(auto& p : m_points){
		::glVertex3fv((float*)&(pChart->NormalizeVertex(p)));
	}
	::glEnd();
	::glPopAttrib();
	::glLoadName(0);
}

void COGLSeries::SetState(const UIElementState::Type& state)
{

	if(GetState()!=UIElementState::Hot && state == UIElementState::Hot){
		auto spPen = m_spSeriesProperty->GetPenPtr();
		m_spSeriesProperty->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()*2, spPen->GetColor()));
	}else if(GetState()==UIElementState::Hot && state != UIElementState::Hot){
		auto spPen = m_spSeriesProperty->GetPenPtr();
		m_spSeriesProperty->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()/2, spPen->GetColor()));
	}

	CUIElement::SetState(state);
}
