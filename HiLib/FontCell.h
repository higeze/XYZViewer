#pragma once
#include "TextCell.h"
#include "MyFont.h"

class CFontCell:public CTextCell
{
private:
	CFont m_font;
public:
	CFontCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty,CFont font);
	virtual ~CFontCell(){}
	CFont GetFont();

	void PaintContent(CDC* pDC,CRect rcPaint);

	virtual void OnLButtonClk(MouseEventArgs& e);

	virtual string_type GetString()const;

	virtual bool IsComparable()const{return false;}
};
