#pragma once

#include "Cell.h"
#include "BindRow.h"
#include "BindColumn.h"

template<typename T>
class CBindPaintCell:public CCell
{
private:
public:
	CBindPaintCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty)
		:CCell(pSheet, pRow, pColumn, spProperty){}
	virtual ~CBindPaintCell(){}
	virtual bool IsComparable()const{return false;}

	virtual void PaintContent(CDC* pDC, CRect rcPaint)
	{
		auto pBindRow = static_cast<CBindRow<T>*>(m_pRow);
		auto pBindColumn = static_cast<CBindPaintColumn<T>*>(m_pColumn);
		auto t = pBindRow->GetDataContext();
		pBindColumn->GetPaintFunction()(t, pDC, rcPaint);
	}

	virtual CRect CalcRect(CDC* pDC)
	{
		//Calc Content Rect
		//CSize size(GetSize());
		//CRect rcCenter(0,0,size.cx,size.cy);
		//CRect rcContent(InnerBorder2Content(CenterBorder2InnerBorder(rcCenter)));
		//Calc Content Rect
		auto pBindRow = static_cast<CBindRow<T>*>(m_pRow);
		auto pBindColumn = static_cast<CBindPaintColumn<T>*>(m_pColumn);
		auto t = pBindRow->GetDataContext();
		CSize size = pBindColumn->GetMeasureFunction()(t, pDC);
		CRect rcContent(0, 0, size.cx, size.cy);
		//Calc CenterBorder Rect
		CRect rcCenter(InnerBorder2CenterBorder(Content2InnerBorder(rcContent)));
		rcCenter.MoveToXY(0,0);
		return rcCenter;	
	}

	virtual CRect CalcFitRect(CDC* pDC){return CalcRect(pDC);}

};