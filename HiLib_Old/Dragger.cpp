#include "Dragger.h"
#include "Sheet.h"
#include "SheetState.h"
#include "Row.h"
#include "Column.h"

CSheetState* CDragger::OnColumnHeaderBeginDrag(CSheet* pSheet, MouseEventArgs& e)
{
	m_dragFromIndex = pSheet->ColumnPointer2AllIndex(pSheet->Point2RowColumn(e.Point).GetColumnPtr());
	m_dragToIndex=COLUMN_INDEX_INVALID;
	return CSheetState::Dragging();
}

CSheetState* CDragger::OnColumnHeaderDrag(CSheet* pSheet, MouseEventArgs& e)
{
	if(e.Flags==MK_LBUTTON){
		

		auto visibleIndex = pSheet->X2VisibleColumnIndex(e.Point.x);

		auto& visibleDictionary = pSheet->m_columnVisibleDictionary.get<IndexTag>();
		auto& allDictionary = pSheet->m_columnAllDictionary.get<IndexTag>();
		auto maxVisibleIndex = boost::prior(visibleDictionary.end())->DataPtr->GetVisibleIndex();
		auto minVisibleIndex = visibleDictionary.begin()->DataPtr->GetVisibleIndex();
		auto maxAllIndex = boost::prior(allDictionary.end())->DataPtr->GetAllIndex();
		auto minAllIndex = allDictionary.begin()->DataPtr->GetAllIndex();


		if(visibleIndex == COLUMN_INDEX_INVALID){
			m_dragToIndex = pSheet->ColumnVisibleIndex2AllIndex(visibleIndex);
		}else if(visibleIndex < minVisibleIndex){
			m_dragToIndex = pSheet->ColumnVisibleIndex2AllIndex(minVisibleIndex);
		}else if(visibleIndex > maxVisibleIndex){
			m_dragToIndex = pSheet->ColumnVisibleIndex2AllIndex(maxVisibleIndex) + 1;
		}else if(visibleIndex == minVisibleIndex){
			m_dragToIndex = pSheet->ColumnVisibleIndex2AllIndex(minVisibleIndex);
		}else if(visibleIndex == maxVisibleIndex){
			m_dragToIndex = pSheet->ColumnVisibleIndex2AllIndex(maxVisibleIndex) + 1;
		}else{
			auto allIndex = pSheet->ColumnVisibleIndex2AllIndex(visibleIndex);
			if(allIndex < m_dragFromIndex){
				m_dragToIndex = allIndex;
			}else if(allIndex > m_dragFromIndex){
				m_dragToIndex = pSheet->ColumnVisibleIndex2AllIndex(visibleIndex + 1);
			}else{
				m_dragToIndex = allIndex;		
			}
		}
		return CSheetState::Dragging();

	}else{
		return CSheetState::Normal();
	}
}

CSheetState* CDragger::OnColumnHeaderEndDrag(CSheet* pSheet, MouseEventArgs& e)
{
	if(m_dragToIndex == COLUMN_INDEX_INVALID){

	}else{
		auto& ptrDictionary = pSheet
			->m_columnAllDictionary.get<PointerTag>();
		auto spDragFromColumn=ptrDictionary.find(pSheet->ColumnAllIndex2Pointer(m_dragFromIndex))->DataPtr;
		//auto spDragToColumn=ptrDictionary.find(ColumnAllIndex2Pointer(m_dragToIndex))->DataPtr;
		if(m_dragToIndex!=spDragFromColumn->GetAllIndex()){
			pSheet->MoveColumn(m_dragToIndex, spDragFromColumn);
		}
	}
	m_dragFromIndex = COLUMN_INDEX_INVALID;
	m_dragToIndex = COLUMN_INDEX_INVALID;

	return CSheetState::Normal();
}
CSheetState* CDragger::OnLButtonDown(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}
CSheetState* CDragger::OnLButtonUp(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}
CSheetState* CDragger::OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}
CSheetState* CDragger::OnRButtonDown(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}
CSheetState* CDragger::OnMouseMove(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();

	auto visibleIndexes = pSheet->Point2VisibleIndexes(e.Point);
	//If Header except Filter
	if(IsDragable(pSheet, visibleIndexes)){
		if(e.Flags==MK_LBUTTON){
			return OnColumnHeaderBeginDrag(pSheet, e);
		}
	}
	return CSheetState::Normal();
}
CSheetState* CDragger::OnMouseLeave(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}
CSheetState* CDragger::OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e){return CSheetState::Normal();}

bool CDragger::IsDragable(CSheet* pSheet, std::pair<size_type, size_type> visibleIndexes)
{
	auto& rowDictionary = pSheet->m_rowVisibleDictionary.get<IndexTag>();
	auto& colDictionary = pSheet->m_columnVisibleDictionary.get<IndexTag>();

	auto maxRow = boost::prior(rowDictionary.end())->DataPtr->GetVisibleIndex();
	auto minRow = rowDictionary.begin()->DataPtr->GetVisibleIndex();
	auto maxCol = boost::prior(colDictionary.end())->DataPtr->GetVisibleIndex();
	auto minCol = colDictionary.begin()->DataPtr->GetVisibleIndex();

	auto spRow = pSheet->VisibleRow(visibleIndexes.first);
	if( visibleIndexes.first >= 0 || 
		visibleIndexes.first < minRow || visibleIndexes.first > maxRow || 
		visibleIndexes.second < minCol || visibleIndexes.second > maxCol){
		return false;
	}else if(spRow && !spRow->IsDragTrackable()){
		return false;
	}
	return true;
}

CSheetState* CDragger::OnDragLButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	return CSheetState::Normal();
}
CSheetState* CDragger::OnDragLButtonUp(CSheet* pSheet, MouseEventArgs& e)
{
	return OnColumnHeaderEndDrag(pSheet, e);		
}
CSheetState* CDragger::OnDragLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)
{
	return CSheetState::Normal();
}
CSheetState* CDragger::OnDragRButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	return CSheetState::Normal();
}
CSheetState* CDragger::OnDragMouseMove(CSheet* pSheet, MouseEventArgs& e)
{
	return OnColumnHeaderDrag(pSheet, e);
}
CSheetState* CDragger::OnDragMouseLeave(CSheet* pSheet, MouseEventArgs& e)
{
	return OnColumnHeaderDrag(pSheet, e);
}
CSheetState* CDragger::OnDragSetCursor(CSheet* pSheet, SetCursorEventArgs& e)
{
	return CSheetState::Dragging();
}

CSheetState* CSheetCellDragger::OnMouseMove(CSheet* pSheet, MouseEventArgs& e)
{
	return CSheetState::Normal();
}