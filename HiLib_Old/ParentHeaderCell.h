#pragma once
#include "TextCell.h"

class CColor;
class IInplaceEdit;

class CParentHeaderCell:public CTextCell
{
public:
	CParentHeaderCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty, CMenu* pMenu = nullptr)
		:CTextCell(pSheet,pRow, pColumn,spProperty,pMenu){}
	virtual ~CParentHeaderCell(){}
	virtual void PaintBackground(CDC* pDC,CRect rc);
	virtual bool IsComparable()const{return false;}

private:
	void TwoColorGradientFill(CDC* pDC,CRect rc,CColor& cr1,CColor& cr2, ULONG ulMode);
};

