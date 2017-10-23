#pragma once
#include "Band.h"

class CCell;
class Sheet;

class CRow:public CBand
{
protected:
	coordinates_type m_height;
	coordinates_type m_top;
public:
	CRow(CSheet* pSheet):CBand(pSheet),m_height(0),m_top(0){}
	virtual ~CRow(){}

	virtual coordinates_type GetHeight();
	virtual void SetHeight(const coordinates_type& height);
	virtual void SetHeightWithoutSignal(const coordinates_type& height){m_height=height;}

	virtual coordinates_type GetTop()const{return m_top+Offset();}
	//virtual coordinates_type GetTop()const{return m_top;}
	virtual void SetTop(const coordinates_type& top){m_top=top;}//TODO low setter
	virtual void SetTopWithoutSignal(const coordinates_type& top){m_top=top;}//TODO low setter

	virtual coordinates_type GetBottom()const{return m_top+m_height+Offset();}
	//virtual coordinates_type GetBottom()const{return m_top+m_height;}//TODO low setter

	virtual void SetVisible(const bool& bVisible, bool notify = true)override;
	virtual void SetSelected(const bool& bSelected);
	virtual size_type GetVisibleIndex()const;
	virtual size_type GetAllIndex()const;
};

class CGridView;

class CParentRow:public CRow
{
public:
	CParentRow(CGridView* pGrid);
	virtual ~CParentRow(){}
	virtual coordinates_type Offset()const;
};

class CParentHeaderRow:public CParentRow
{
public:
	CParentHeaderRow(CGridView* pGrid):CParentRow(pGrid){}
	virtual bool IsDragTrackable()const override{return true;}
};

class CSheetCell;

class CChildRow:public CRow
{
public:
	CChildRow(CSheetCell* pSheetCell);
	virtual ~CChildRow(){}
	virtual coordinates_type Offset()const;
};

class CChildHeaderRow:public CChildRow
{
public:
	CChildHeaderRow(CSheetCell* pGrid):CChildRow(pGrid){}
	virtual bool IsDragTrackable()const override{return true;}
};
