#pragma once

#include "ParentHeaderCell.h"
#include "SheetEnums.h"
#include "MyMenu.h"


class CParentColumnNameHeaderCell:public CParentHeaderCell
{
public:
	static CMenu ContextMenu;

private:

	static const int TRIANGLE_WIDTH=12;
	static const int TRIANGLE_HEIGHT=6;
	static const CRect TRIANGLE_MARGIN;
	static const int MIN_COLUMN_WIDTH = 16;

public:
	CParentColumnNameHeaderCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty, CMenu* pMenu = &CParentColumnNameHeaderCell::ContextMenu)
		:CParentHeaderCell(pSheet, pRow, pColumn, spProperty, pMenu?pMenu:&CParentColumnNameHeaderCell::ContextMenu){}
	virtual ~CParentColumnNameHeaderCell(){}

	Sorts GetSort()const;
	CSize GetSortSize()const;
	virtual CSize GetInitSize(CDC* pDC) override{return CSize(MIN_COLUMN_WIDTH,0);};
	virtual CSize MeasureContentSize(CDC* pDC);
	virtual CSize MeasureContentSizeWithFixedWidth(CDC* pDC);
	virtual void PaintContent(CDC* pDC,CRect rcPaint);



	virtual void OnLButtonClk(MouseEventArgs& e);


	void PaintSortMark(CDC* pDC,CRect rcPaint);
};

class CParentColumnHeaderStringCell:public CParentColumnNameHeaderCell
{
private:
	string_type m_strHeader;
public:
	CParentColumnHeaderStringCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,string_type str,CMenu* pMenu=nullptr)
		:CParentColumnNameHeaderCell(pSheet, pRow, pColumn, spProperty,pMenu),m_strHeader(str){}
	virtual ~CParentColumnHeaderStringCell(){}
	virtual string_type GetString()const{return m_strHeader;}
	virtual void SetStringCore(const string_type& str){m_strHeader = str;}


};