#include "Column.h"
#include "GridView.h"
#include "SheetCell.h"
#include "CellProperty.h"
#include "SheetEventArgs.h"

/**
 *  CColumn
 */
BOOST_CLASS_EXPORT_IMPLEMENT(CColumn);

CColumn::coordinates_type CColumn::GetWidth()
{
	if(m_isInit){
		//if(!m_isSerialized){
			SetWidthWithoutSignal(m_pSheet->GetColumnInitWidth(this));
		//}
		m_bMeasureValid=true;	
		m_isInit = false;
	}else if(!m_bMeasureValid){
		SetWidthWithoutSignal(m_pSheet->GetColumnFitWidth(this));
		m_bMeasureValid=true;
	}
	return m_width;
}

void CColumn::SetWidth(const coordinates_type& width)
{
	if(m_width!=width){
		SetWidthWithoutSignal(width);
		//m_pSheet->ColumnWidthChanged(CColumnEventArgs(this));
	}
}
void CColumn::SetVisible(const bool& bVisible, bool notify)
{
	if(m_bVisible!=bVisible){
		m_bVisible=bVisible;
		if(notify){
			m_pSheet->ColumnVisibleChanged(CColumnEventArgs(this));
		}
	}
}
void CColumn::SetSelected(const bool& bSelected)
{
	if(m_bSelected!=bSelected){
		m_bSelected=bSelected;
		m_pSheet->ColumnPropertyChanged(L"selected");//TODO should be changed to ColumnSelectedChanged
	}
}
void CColumn::SetSort(const Sorts& sort)
{
	if(sort!=Sorts::None){
		m_pSheet->ResetColumnSort();
	}
	if(m_sort!=sort){
		m_sort=sort;
		m_pSheet->ColumnPropertyChanged(L"sort");//TODO should be changed to ColumnSortChanged
	}
}
CColumn::size_type CColumn::GetVisibleIndex()const
{
	return m_pSheet->ColumnPointer2VisibleIndex(this);
}

CColumn::size_type CColumn::GetAllIndex()const
{
	return m_pSheet->ColumnPointer2AllIndex(this);
}

//bool CColumn::SortPredicate(const CCell& lhs, const CCell& rhs)
//{
//
//}

/**
 *  CParentColumn
 */
BOOST_CLASS_EXPORT_IMPLEMENT(CParentColumn);

CParentColumn::CParentColumn(CGridView* pGrid)
	:CColumn(pGrid){} 


CParentColumn::coordinates_type CParentColumn::Offset()const
{
	coordinates_type offset(0);//m_pGrid->GetRect().left is not necessary
	//if(m_pSheet->Visible() && GetAllIndex()>=0){
	if(m_pSheet->Visible() && GetVisibleIndex()>=0){
		offset-=static_cast<CGridView*>(m_pSheet)->GetHorizontalScrollPos();
	}
	return offset;
}

/**
 *  CChildColumn
 */
CChildColumn::CChildColumn(CSheetCell* pSheetCell)
	:CColumn(pSheetCell){} 

CChildColumn::coordinates_type CChildColumn::Offset()const
{
	return static_cast<CSheetCell*>(m_pSheet)->GetLeft()
			+ (coordinates_type)floor(static_cast<CSheetCell*>(m_pSheet)->GetPropertyPtr()->GetPenPtr()->GetWidth()*0.5)
			+ (coordinates_type)floor(static_cast<CSheetCell*>(m_pSheet)->GetHeaderPropertyPtr()->GetPenPtr()->GetWidth()*0.5)
			+ static_cast<CSheetCell*>(m_pSheet)->GetPropertyPtr()->GetPadding().left;
}