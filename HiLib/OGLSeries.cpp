#include "OGLSeries.h"
#include "OGLChartWnd.h"
#include "OGLChart.h"


void COGLSeries::OnPaint(OGLPaintEventArgs& e)
{
	if(!m_bVisible)return;

	auto pChart = static_cast<COGLChart*>(m_pParent);
	auto spLine = m_spSeriesProperty->GetLinePtr();
	auto spPoint = m_spSeriesProperty->GetPointPtr();
	auto index = static_cast<COGLChart*>(m_pParent)->GetSeriesIndex(this);
	::glLoadName(index);
	::glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
	//Line
	::glLineWidth( spLine->GetWidth() );
	::glBegin(GL_LINE_STRIP);
	::glColor3ub(spLine->GetColor().GetR(), spLine->GetColor().GetG(), spLine->GetColor().GetB());
	for (auto& p : m_points) {
		::glVertex3fv((float*)&(pChart->NormalizeVertex(p)));	
	}
	::glEnd();

	//Plot
	::glPointSize(spPoint->GetSize());
	::glBegin( GL_POINTS );
	::glColor3ub(spLine->GetColor().GetR(),spLine->GetColor().GetG(), spLine->GetColor().GetB() );
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
		auto spLine = m_spSeriesProperty->GetLinePtr();
		m_spSeriesProperty->SetLinePtr(std::make_shared<COGLLineProperty>(spLine->GetWidth()*2, spLine->GetColor()));// TODO Only change Width
	}else if(GetState()==UIElementState::Hot && state != UIElementState::Hot){
		auto spLine = m_spSeriesProperty->GetLinePtr();
		m_spSeriesProperty->SetLinePtr(std::make_shared<COGLLineProperty>(spLine->GetWidth()/2, spLine->GetColor()));
	}

	CUIElement::SetState(state);
}
