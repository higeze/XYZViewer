#pragma once
#include "TextCell.h"
#include "MyColor.h"

class CColor;

class CColorCell:public CTextCell
{
private:
	CColor m_color;
public:
	CColorCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty,CColor color);
	virtual ~CColorCell(){}
	CColor GetColor();
	void PaintBackground(CDC* pDC,CRect rcPaint);
	virtual void OnLButtonClk(MouseEventArgs& e);
	virtual std::basic_string<TCHAR> GetString()const;
	virtual bool IsComparable()const{return false;}
};
