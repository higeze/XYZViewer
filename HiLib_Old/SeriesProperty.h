#pragma once

#include "MyFont.h"
#include "MyBrush.h"
#include "MyPen.h"

class CSeriesProperty
{
private:

	int m_fontSize;
	std::wstring m_faceName;

	int m_lineStyle;
	int m_lineWidth;
	CColor m_color;

	std::shared_ptr<CPen> m_spPen;
	std::shared_ptr<CPen> m_spHotPen;
	std::shared_ptr<CFont> m_spFont;
	std::shared_ptr<CBrush> m_spBrush;

public:
	CSeriesProperty(int fontSize, std::wstring faceName,
					int lineStyle, int lineWidth, 
					CColor color)
		:m_fontSize(fontSize), m_faceName(faceName), m_lineStyle(lineStyle), m_lineWidth(lineWidth), m_color(color){}
	virtual ~CSeriesProperty(){}

	std::shared_ptr<CFont> GetFontPtr()
	{
		if(!m_spFont)m_spFont=std::make_shared<CFont>(m_fontSize, m_faceName);
		return m_spFont;
	}

	std::shared_ptr<CPen> GetPenPtr()
	{
		if(!m_spPen)m_spPen=std::make_shared<CPen>(m_lineStyle, m_lineWidth, m_color);
		return m_spPen;
	}

	void SetPenPtr(std::shared_ptr<CPen>& spPen)
	{
		m_spPen=spPen;
	}

	std::shared_ptr<CPen> GetHotPenPtr()
	{
		if(!m_spHotPen)m_spHotPen=std::make_shared<CPen>(m_lineStyle, m_lineWidth*2, m_color);
		return m_spHotPen;
	}

	std::shared_ptr<CBrush> GetBrushPtr()
	{
		if(!m_spBrush)m_spBrush = std::make_shared<CBrush>(m_color);
		return m_spBrush;
	}
	
	int GetLineWidth(){return m_lineWidth;}
	int GetLineStyle(){return m_lineStyle;}
	CColor GetColor(){return m_color;}

};


//
//class CSeriesProperty
//{
//private:
//	std::shared_ptr<CPen> m_spPen;
//	std::shared_ptr<CBrush> m_spBrush;
//	std::shared_ptr<CFont> m_spFont;
//	CColor m_color;
//public:
//	CSeriesProperty(int nPointSize,std::wstring strFaceName, int nPenStyle, int nWidth, CColor color)
//		:m_spFont(std::make_shared<CFont>(nPointSize, strFaceName)),m_spPen(std::make_shared<CPen>(nPenStyle, nWidth, color)),m_spBrush(std::make_shared<CBrush>(color)), m_color(color){}
//	std::shared_ptr<CPen> GetPenPtr(){return m_spPen;}
//	std::shared_ptr<CFont> GetFontPtr(){return m_spFont;}
//	std::shared_ptr<CBrush> GetBrushPtr(){return m_spBrush;}
//	CColor GetColor(){return m_color;}
//
//};
//
//class COGLSeriesProperty
//{
//public:
//	float m_lineWidth;
//	CColor m_color;
//
//	std::shared_ptr<CPen> m_spPen;
//	std::shared_ptr<CFont> m_spFont;
//
//	COGLSeriesProperty(int nPointSize,std::wstring strFaceName, int nPenStyle, int nWidth, CColor color)
//		:m_spFont(std::make_shared<CFont>(nPointSize, strFaceName)),m_spPen(std::make_shared<CPen>(nPenStyle, nWidth, color)),m_lineWidth(nWidth), m_color(color){}
//};