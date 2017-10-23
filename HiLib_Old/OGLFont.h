#pragma once
#include "MyFont.h"

struct CFloatSize
{
	CFloatSize(float x = 0.0f, float y = 0.0f):cx(x),cy(y){}
	float cx,cy;
};

class COGLFont
{
private:
	CFont m_font;
	GLYPHMETRICSFLOAT m_agmf[256];
	DWORD m_dwListNumber;
public:
	COGLFont(CFont font, DWORD dwListNumber):m_font(font),m_dwListNumber(dwListNumber)
	{
		//Update(hDC);
	}
	virtual ~COGLFont(){}
	void Update(HDC hDC)
	{
		//Delete lists
		::glDeleteLists(m_dwListNumber, 256);
		//Create lists
		auto hFontOld = (HFONT)::SelectObject(hDC, (HFONT)m_font);
		auto b=::wglUseFontOutlinesA(hDC, 0, 255, m_dwListNumber, 0.0f, 0.1f, WGL_FONT_POLYGONS, m_agmf);
		::SelectObject(hDC, hFontOld);	
	}
	DWORD GetListNumber()const{return m_dwListNumber;}
	CFloatSize MeasureString(const std::string& str)
	{
		CFloatSize size;
		boost::for_each(str,[&](const char& c)->void{
			size.cx += m_agmf[c].gmfCellIncX;
			size.cy = max(size.cy, m_agmf[c].gmfBlackBoxY);
		});
		return size;
	}

};