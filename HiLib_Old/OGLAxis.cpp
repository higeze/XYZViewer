#include "OGLAxis.h"
#include "OGLChart.h"

void COGLXAxis::PaintAxis(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetAxisProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetAxisProperty()->GetLineProperty()->GetColor() );

	::glBegin( GL_LINES );

	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(m_minimum,0,0))));
	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(m_maximum,0,0))));

	::glEnd();
}

void COGLYAxis::PaintAxis(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetAxisProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetAxisProperty()->GetLineProperty()->GetColor() );

	::glBegin( GL_LINES );

	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(0,m_minimum,0))));
	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(0,m_maximum,0))));

	::glEnd();
}

void COGLZAxis::PaintAxis(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetAxisProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetAxisProperty()->GetLineProperty()->GetColor() );

	::glBegin( GL_LINES );

	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(0,0,m_minimum))));
	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(0,0,m_maximum))));

	::glEnd();
}

void COGLXAxis::PaintTick(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetTickProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetTickProperty()->GetLineProperty()->GetColor() );

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		::glBegin( GL_LINES );
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, (pChart->GetYAxisPtr()->GetMaximum() - pChart->GetYAxisPtr()->GetMinimum())*0.02, 0.0f))));
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, -(pChart->GetYAxisPtr()->GetMaximum() - pChart->GetYAxisPtr()->GetMinimum())*0.02, 0.0f))));

			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, 0.0f, (pChart->GetZAxisPtr()->GetMaximum() - pChart->GetZAxisPtr()->GetMinimum())*0.02))));
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, 0.0f, -(pChart->GetZAxisPtr()->GetMaximum() - pChart->GetZAxisPtr()->GetMinimum())*0.02))));
		::glEnd();
	}
}

void COGLYAxis::PaintTick(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetTickProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetTickProperty()->GetLineProperty()->GetColor() );

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		::glBegin( GL_LINES );
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex((pChart->GetXAxisPtr()->GetMaximum() - pChart->GetXAxisPtr()->GetMinimum())*0.02, tick, 0.0f))));
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(-(pChart->GetXAxisPtr()->GetMaximum() - pChart->GetXAxisPtr()->GetMinimum())*0.02, tick, 0.0f))));

			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(0.0f, tick, (pChart->GetZAxisPtr()->GetMaximum() - pChart->GetZAxisPtr()->GetMinimum())*0.02))));
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(0.0f, tick, -(pChart->GetZAxisPtr()->GetMaximum() - pChart->GetZAxisPtr()->GetMinimum())*0.02))));
		::glEnd();
	}
}

void COGLZAxis::PaintTick(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetTickProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetTickProperty()->GetLineProperty()->GetColor() );

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		::glBegin( GL_LINES );
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex((pChart->GetXAxisPtr()->GetMaximum() - pChart->GetXAxisPtr()->GetMinimum())*0.02, 0.0f, tick))));
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(-(pChart->GetXAxisPtr()->GetMaximum() - pChart->GetXAxisPtr()->GetMinimum())*0.02, 0.0f, tick))));

			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(0.0f, (pChart->GetYAxisPtr()->GetMaximum() - pChart->GetYAxisPtr()->GetMinimum())*0.02, tick))));
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(0.0f, -(pChart->GetYAxisPtr()->GetMaximum() - pChart->GetYAxisPtr()->GetMinimum())*0.02, tick))));
		::glEnd();
	}
}

void COGLXAxis::PaintGrid(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetGridProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetGridProperty()->GetLineProperty()->GetColor() );

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		::glBegin( GL_LINES );
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, pChart->GetYAxisPtr()->GetMinimum(), 0.0f))));
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, pChart->GetYAxisPtr()->GetMaximum(), 0.0f))));

			//if(abs(pChart->GetYAxisPtr()->GetMaximum())>=abs(pChart->GetYAxisPtr()->GetMinimum())){
				::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, pChart->GetYAxisPtr()->GetMaximum(), pChart->GetZAxisPtr()->GetMinimum()))));
				::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, pChart->GetYAxisPtr()->GetMaximum(), pChart->GetZAxisPtr()->GetMaximum()))));
			//}else{
			//	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, pChart->GetYAxisPtr()->GetMinimum(), pChart->GetZAxisPtr()->GetMinimum()))));
			//	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(tick, pChart->GetYAxisPtr()->GetMinimum(), pChart->GetZAxisPtr()->GetMaximum()))));
			//}
		::glEnd();
	}
}

void COGLYAxis::PaintGrid(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetGridProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetGridProperty()->GetLineProperty()->GetColor() );

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		::glBegin( GL_LINES );
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMinimum(), tick, 0.0f))));
			::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMaximum(), tick, 0.0f))));
			//if(abs(pChart->GetXAxisPtr()->GetMaximum())>=abs(pChart->GetXAxisPtr()->GetMinimum())){
				::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMaximum(), tick, pChart->GetZAxisPtr()->GetMinimum()))));
				::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMaximum(), tick, pChart->GetZAxisPtr()->GetMaximum()))));
			//}else{
			//	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMinimum(), tick, pChart->GetZAxisPtr()->GetMinimum()))));
			//	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMinimum(), tick, pChart->GetZAxisPtr()->GetMaximum()))));
			//}

		::glEnd();
	}
}

void COGLZAxis::PaintGrid(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	::glLineWidth( pChart->GetChartProperty()->GetGridProperty()->GetLineProperty()->GetWidth() );
	::glColor3ubv( pChart->GetChartProperty()->GetGridProperty()->GetLineProperty()->GetColor() );

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		::glBegin( GL_LINES );
			//if(abs(pChart->GetYAxisPtr()->GetMaximum())>=abs(pChart->GetYAxisPtr()->GetMinimum())){
				::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMinimum(), pChart->GetYAxisPtr()->GetMaximum(), tick))));
				::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMaximum(), pChart->GetYAxisPtr()->GetMaximum(), tick))));
			//}else{
			//	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMinimum(), pChart->GetYAxisPtr()->GetMinimum(), tick))));
			//	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMaximum(), pChart->GetYAxisPtr()->GetMinimum(), tick))));
			//}

			//if(abs(pChart->GetXAxisPtr()->GetMaximum())>=abs(pChart->GetXAxisPtr()->GetMinimum())){
				::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMaximum(), pChart->GetYAxisPtr()->GetMinimum(), tick))));
				::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMaximum(), pChart->GetYAxisPtr()->GetMaximum(), tick))));
			//}else{
			//	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMinimum(), pChart->GetYAxisPtr()->GetMinimum(), tick))));
			//	::glVertex3fv((float*)&(pChart->NormalizeVertex(CVertex(pChart->GetXAxisPtr()->GetMinimum(), pChart->GetYAxisPtr()->GetMaximum(), tick))));
			//}
		::glEnd();
	}
}

void COGLXAxis::PaintAxisLabel(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	auto spFont = e.RendererPtr->GetArialFont();
	auto spFontProp = pChart->GetChartProperty()->GetAxisProperty()->GetFontProperty(); 
	::glPushMatrix();
	::glColor3ubv( spFontProp->GetColor() );

	GLfloat fontDiffuse[3]={1.0f,1.0f,0.0f};
	::glMaterialfv(GL_FRONT,GL_DIFFUSE,fontDiffuse);
	::glEnable(GL_NORMALIZE);

	::glTranslatef(m_unit,pChart->NormalizeY(0),pChart->NormalizeZ(0));
	::glRotatef(90,1,0,0);
	::glScalef(spFontProp->GetScale(),spFontProp->GetScale(),spFontProp->GetScale());
	::glListBase(spFont->GetListNumber());
	::glCallLists((GLuint)m_strName.size(),GL_UNSIGNED_BYTE,m_strName.c_str());

	::glPopMatrix();
}

void COGLYAxis::PaintAxisLabel(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	auto spFont = e.RendererPtr->GetArialFont();
	auto spFontProp = pChart->GetChartProperty()->GetAxisProperty()->GetFontProperty(); 
	::glPushMatrix();
	::glColor3ubv( spFontProp->GetColor() );

	GLfloat fontDiffuse[3]={1.0f,1.0f,0.0f};
	::glMaterialfv(GL_FRONT,GL_DIFFUSE,fontDiffuse);
	::glEnable(GL_NORMALIZE);

	::glTranslatef(pChart->NormalizeX(0),m_unit + spFont->MeasureString(m_strName).cx*spFontProp->GetScale(),pChart->NormalizeZ(0));
	::glRotatef(-90,0,0,1);
	::glRotatef(90,1,0,0);
	::glScalef(spFontProp->GetScale(),spFontProp->GetScale(),spFontProp->GetScale());
	::glListBase(spFont->GetListNumber());
	::glCallLists((GLuint)m_strName.size(),GL_UNSIGNED_BYTE,m_strName.c_str());

	::glPopMatrix();
}

void COGLZAxis::PaintAxisLabel(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	auto spFont = e.RendererPtr->GetArialFont();
	auto spFontProp = pChart->GetChartProperty()->GetAxisProperty()->GetFontProperty(); 
	::glPushMatrix();
	::glColor3ubv( spFontProp->GetColor() );

	GLfloat fontDiffuse[3]={1.0f,1.0f,0.0f};
	::glMaterialfv(GL_FRONT,GL_DIFFUSE,fontDiffuse);
	::glEnable(GL_NORMALIZE);
	::glTranslatef(pChart->NormalizeX(0)-spFont->MeasureString(m_strName).cx*spFontProp->GetScale() ,pChart->NormalizeY(0),m_unit);
	::glRotatef(90,1,0,0);
	::glScalef(spFontProp->GetScale(),spFontProp->GetScale(),spFontProp->GetScale());
	::glListBase(spFont->GetListNumber());
	::glCallLists((GLuint)m_strName.size(),GL_UNSIGNED_BYTE,m_strName.c_str());

	::glPopMatrix();
}

void COGLXAxis::PaintTickLabel(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	auto spFont = e.RendererPtr->GetArialFont();
	auto spFontProp = pChart->GetChartProperty()->GetTickProperty()->GetFontProperty();

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		::glPushMatrix();
		::glColor3ubv( spFontProp->GetColor() );

		GLfloat fontDiffuse[3]={1.0f,1.0f,0.0f};
		::glMaterialfv(GL_FRONT,GL_DIFFUSE,fontDiffuse);
		::glEnable(GL_NORMALIZE);
		std::string strTick = roundtostring<char>(tick,6);
		::glTranslatef(
			pChart->NormalizeX(tick)-spFont->MeasureString(strTick).cx/2*spFontProp->GetScale(),
			pChart->NormalizeY(0),
			pChart->NormalizeZ(0-(pChart->GetZAxisPtr()->GetMaximum() - pChart->GetZAxisPtr()->GetMinimum())*0.03)-spFont->MeasureString(strTick).cy*spFontProp->GetScale());
		::glRotatef(90,1,0,0);
		::glScalef(spFontProp->GetScale(),spFontProp->GetScale(),spFontProp->GetScale());
		::glListBase(spFont->GetListNumber());
		::glCallLists((GLuint)strTick.size(),GL_UNSIGNED_BYTE,strTick.c_str());

		::glPopMatrix();
	}
}

void COGLYAxis::PaintTickLabel(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	auto spFont = e.RendererPtr->GetArialFont();
	auto spFontProp = pChart->GetChartProperty()->GetTickProperty()->GetFontProperty();

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		::glPushMatrix();
		::glColor3ubv( spFontProp->GetColor() );

		GLfloat fontDiffuse[3]={1.0f,1.0f,0.0f};
		::glMaterialfv(GL_FRONT,GL_DIFFUSE,fontDiffuse);
		::glEnable(GL_NORMALIZE);
		std::string strTick = roundtostring<char>(tick,6);
		::glTranslatef(
			pChart->NormalizeX(0),
			pChart->NormalizeY(tick)+spFont->MeasureString(strTick).cx/2*spFontProp->GetScale(),
			pChart->NormalizeZ(0-(pChart->GetZAxisPtr()->GetMaximum() - pChart->GetZAxisPtr()->GetMinimum())*0.03)-spFont->MeasureString(strTick).cy*spFontProp->GetScale());
		::glRotatef(-90,0,0,1);
		::glRotatef(90,1,0,0);
		::glScalef(spFontProp->GetScale(),spFontProp->GetScale(),spFontProp->GetScale());
		::glListBase(spFont->GetListNumber());
		::glCallLists((GLuint)strTick.size(),GL_UNSIGNED_BYTE,strTick.c_str());

		::glPopMatrix();
	}
}

void COGLZAxis::PaintTickLabel(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);
	auto spFont = e.RendererPtr->GetArialFont();
	auto spFontProp = pChart->GetChartProperty()->GetTickProperty()->GetFontProperty();

	for(float tick = GetMinimum();tick<=GetMaximum()+GetTick()/2;tick+=GetTick()){
		{
		::glPushMatrix();
		::glColor3ubv( spFontProp->GetColor() );

		GLfloat fontDiffuse[3]={1.0f,1.0f,0.0f};
		::glMaterialfv(GL_FRONT,GL_DIFFUSE,fontDiffuse);
		::glEnable(GL_NORMALIZE);
		std::string strTick = roundtostring<char>(tick,6);
		//if(abs(pChart->GetXAxisPtr()->GetMaximum())>=abs(pChart->GetXAxisPtr()->GetMinimum())){
		::glTranslatef(
			pChart->NormalizeX(pChart->GetXAxisPtr()->GetMaximum()),
			pChart->NormalizeY(0-(pChart->GetYAxisPtr()->GetMaximum() - pChart->GetYAxisPtr()->GetMinimum())*0.03),
			pChart->NormalizeZ(tick)-spFont->MeasureString(strTick).cy/2*spFontProp->GetScale());
		::glRotatef(-90,0,0,1);
		::glRotatef(90,1,0,0);
		//}else{
		//::glTranslatef(pChart->NormalizeX(pChart->GetXAxisPtr()->GetMinimum()) ,pChart->NormalizeY(0)-spFont->MeasureString(strTick).cx*spFontProp->GetScale(), pChart->NormalizeZ(tick)-spFont->MeasureString(strTick).cy/2*spFontProp->GetScale());
		//::glRotatef(90,0,0,1);
		//::glRotatef(90,1,0,0);
		//}
		::glScalef(spFontProp->GetScale(),spFontProp->GetScale(),spFontProp->GetScale());
		::glListBase(spFont->GetListNumber());
		::glCallLists((GLuint)strTick.size(),GL_UNSIGNED_BYTE,strTick.c_str());

		::glPopMatrix();
		}

		{
		::glPushMatrix();
		::glColor3ubv( spFontProp->GetColor() );
		
		GLfloat fontDiffuse[3]={1.0f,1.0f,0.0f};
		::glMaterialfv(GL_FRONT,GL_DIFFUSE,fontDiffuse);
		::glEnable(GL_NORMALIZE);
		std::string strTick = roundtostring<char>(tick,6);
		//if(abs(pChart->GetXAxisPtr()->GetMaximum())>=abs(pChart->GetXAxisPtr()->GetMinimum())){
		::glTranslatef(
			pChart->NormalizeX(0-(pChart->GetXAxisPtr()->GetMaximum() - pChart->GetXAxisPtr()->GetMinimum())*0.03)-spFont->MeasureString(strTick).cx*spFontProp->GetScale(),
			pChart->NormalizeY(pChart->GetYAxisPtr()->GetMaximum()),
			pChart->NormalizeZ(tick)-spFont->MeasureString(strTick).cy/2*spFontProp->GetScale());
		::glRotatef(90,1,0,0);
		//}else{
		//::glTranslatef(pChart->NormalizeX(pChart->GetXAxisPtr()->GetMinimum()) ,pChart->NormalizeY(0)-spFont->MeasureString(strTick).cx*spFontProp->GetScale(), pChart->NormalizeZ(tick)-spFont->MeasureString(strTick).cy/2*spFontProp->GetScale());
		//::glRotatef(90,0,0,1);
		//::glRotatef(90,1,0,0);
		//}
		::glScalef(spFontProp->GetScale(),spFontProp->GetScale(),spFontProp->GetScale());
		::glListBase(spFont->GetListNumber());
		::glCallLists((GLuint)strTick.size(),GL_UNSIGNED_BYTE,strTick.c_str());

		::glPopMatrix();
		}
	}
}

void COGLAxis::OnPaint(OGLPaintEventArgs& e)
{
	auto pChart = static_cast<COGLChart*>(m_pParent);

	::glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );

	PaintAxis(e);
	PaintTick(e);
	PaintGrid(e);
	PaintAxisLabel(e);
	PaintTickLabel(e);

	::glPopAttrib();
}

float COGLAxis::CalcAutoTick(float min,float max,int minNum,int maxNum)
{
	float range = max - min;
	float place = floor(log10(exp(1.0f)) * log(range));
	float interval = pow(10, place - 1);
	float da[] = { 10, 5, 2 };

	for(int i=0;i<3;i++)
	{
		if (range / interval > maxNum)
		{
			interval *= da[i];
		}
		else if (range / interval < minNum)
		{
			interval /= da[i];
		}
	}
	return interval;
}

float COGLAxis::CalcTickStart(float min, float tick)
{
	float ret = 0;
	if (min <= 0)
	{
		ret = (int)((min-tick*1.5) / tick)  * tick;
	}
	else
	{
		ret = min(0,(int)((min-tick*1.5) / tick) * tick);
	}
	return ret;
};

float COGLAxis::CalcTickEnd(float max,float tick)
{
	float ret = 0;
	if (max <= 0)
	{
		ret = max(0,(int)((max+tick*1.5) / tick) * tick);
	}
	else
	{
		ret = (int)((max+tick*1.5) / tick) * tick;
	}
	return ret;
};