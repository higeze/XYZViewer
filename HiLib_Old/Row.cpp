#include "Row.h"
#include "GridView.h"
#include "SheetCell.h"
#include "CellProperty.h"
#include "SheetEventArgs.h"

CBand::coordinates_type CRow::GetHeight()
{
	if(!m_bMeasureValid){
		m_height = m_pSheet->GetRowHeight(this);
		m_bMeasureValid=true;
	}
	return m_height;
}

void CRow::SetHeight(const coordinates_type& height)
{
	if(m_height!=height){
		m_height=height;
		m_pSheet->RowPropertyChanged(L"height");
	}
}
void CRow::SetVisible(const bool& bVisible, bool notify)
{
	if(m_bVisible!=bVisible){
		m_bVisible=bVisible;
		if(notify){
			m_pSheet->RowVisibleChanged(CRowEventArgs(this));
		}
	}
}
void CRow::SetSelected(const bool& bSelected)
{
	if(m_bSelected!=bSelected){
		m_bSelected=bSelected;
		m_pSheet->RowPropertyChanged(L"selected");
	}
}

CRow::size_type CRow::GetVisibleIndex()const
{
	return m_pSheet->RowPointer2VisibleIndex(this);
}
CRow::size_type CRow::GetAllIndex()const
{
	return m_pSheet->RowPointer2AllIndex(this);
}

CParentRow::CParentRow(CGridView* pGrid):CRow(pGrid){} 

CParentRow::coordinates_type CParentRow::Offset()const
{
	coordinates_type offset(static_cast<CGridView*>(m_pSheet)->GetPaintRect().top);
	//if(m_pSheet->Visible() && GetVisible() && GetAllIndex()>=0){
	if(m_pSheet->Visible() && GetVisible() && GetVisibleIndex()>=0){
		offset-=static_cast<CGridView*>(m_pSheet)->GetVerticalScrollPos();
	}
	return offset;
}

CChildRow::CChildRow(CSheetCell* pSheetCell):CRow(pSheetCell){} 

CChildRow::coordinates_type CChildRow::Offset()const
{
	return static_cast<CSheetCell*>(m_pSheet)->GetTop()
			+ (coordinates_type)floor(static_cast<CSheetCell*>(m_pSheet)->GetPropertyPtr()->GetPenPtr()->GetWidth()*0.5)
			+ (coordinates_type)floor(static_cast<CSheetCell*>(m_pSheet)->GetHeaderPropertyPtr()->GetPenPtr()->GetWidth()*0.5)
			+ static_cast<CSheetCell*>(m_pSheet)->GetPropertyPtr()->GetPadding().top;
}