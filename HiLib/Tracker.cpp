#include "Tracker.h"
#include "Sheet.h"
#include "Row.h"
#include "Column.h"
#include "SheetState.h"
#include "SheetEventArgs.h"
#include "GridView.h"

CSheetState* CTracker::OnColumnHeaderBeginTrack(CSheet* pSheet, MouseEventArgs& e, size_type col)
{
	::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
	m_coTrackLeftVisib=col;	
	return CSheetState::Tracking();
}	

CSheetState* CTracker::OnColumnHeaderTrack(CSheet* pSheet, MouseEventArgs& e)
{
	::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
	auto pCol=pSheet->VisibleColumn(m_coTrackLeftVisib);
	pCol->SetWidthWithoutSignal(max(e.Point.x-pCol->GetLeft(),MIN_COLUMN_WIDTH));
	pSheet->ColumnHeaderTrack(CColumnEventArgs(pCol.get()));
	return CSheetState::Tracking();
}
CSheetState* CTracker::OnColumnHeaderEndTrack(CSheet* pSheet, MouseEventArgs& e)
{
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	auto pCol=pSheet->VisibleColumn(m_coTrackLeftVisib);
	pCol->SetWidthWithoutSignal(max(e.Point.x-pCol->GetLeft(),MIN_COLUMN_WIDTH));
	pSheet->ColumnHeaderEndTrack(CColumnEventArgs(pCol.get()));
	return CSheetState::Normal();
}

CSheetState* CTracker::OnColumnHeaderDividerDblClk(CSheet* pSheet, MouseEventArgs& e, size_type col)
{
	auto& colDictionary=pSheet->m_columnVisibleDictionary.get<IndexTag>();
	auto pColumn=pSheet->ColumnVisibleIndex2Pointer(col);
	pSheet->ColumnHeaderFitWidth(CColumnEventArgs(pColumn));	
	return CSheetState::Normal();
}


CSheetState* CTracker::OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();

	auto& colDictionary=pSheet->m_columnVisibleDictionary.get<IndexTag>();
	auto visibleIndexes = pSheet->Point2VisibleIndexes(e.Point);
	//If Header except Filter
	if(IsTrackable(pSheet, visibleIndexes)){
		auto maxColumn = boost::prior(colDictionary.end())->DataPtr->GetVisibleIndex();
		auto minColumn = colDictionary.begin()->DataPtr->GetVisibleIndex();
		if(visibleIndexes.second < minColumn){
			//Out of Left
			//Do Nothing	
		}else if(visibleIndexes.second > maxColumn){
			//Out of Right
			if(e.Point.x < boost::prior(colDictionary.end())->DataPtr->GetRight() + RESIZE_AREA_HARF_WIDTH){
				return OnColumnHeaderBeginTrack(pSheet, e, visibleIndexes.second -1);
			}
		}else if(e.Point.x < (pSheet->ColumnVisibleIndex2Pointer(visibleIndexes.second)->GetLeft() + RESIZE_AREA_HARF_WIDTH)){
			return OnColumnHeaderBeginTrack(pSheet, e, max(visibleIndexes.second - 1, minColumn));
		}else if((pSheet->ColumnVisibleIndex2Pointer(visibleIndexes.second)->GetRight() - RESIZE_AREA_HARF_WIDTH) < e.Point.x){
			return OnColumnHeaderBeginTrack(pSheet, e, min(visibleIndexes.second, maxColumn));
		}else{
			//Do Nothing
		}
	}
	return CSheetState::Normal();
}

CSheetState* CTracker::OnLButtonUp(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}

CSheetState* CTracker::OnRButtonDown(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}

CSheetState* CTracker::OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();

	auto& colDictionary=pSheet->m_columnVisibleDictionary.get<IndexTag>();

	//If ColumnmHeader
	auto visibleIndexes = pSheet->Point2VisibleIndexes(e.Point);
	//If Header except Filter
	if(IsTrackable(pSheet, visibleIndexes)){
		auto maxColumn = boost::prior(colDictionary.end())->DataPtr->GetVisibleIndex();
		auto minColumn = colDictionary.begin()->DataPtr->GetVisibleIndex();
		if(visibleIndexes.second < minColumn){
			//Out of Left
			//Do Nothing	
		}else if(visibleIndexes.second > maxColumn){
			//Out of Right
			if(e.Point.x < boost::prior(colDictionary.end())->DataPtr->GetRight() + RESIZE_AREA_HARF_WIDTH){
				return OnColumnHeaderDividerDblClk(pSheet, e, visibleIndexes.second -1);
			}
		}else if(e.Point.x < (pSheet->ColumnVisibleIndex2Pointer(visibleIndexes.second)->GetLeft() + RESIZE_AREA_HARF_WIDTH)){
			return OnColumnHeaderDividerDblClk(pSheet, e, max(visibleIndexes.second - 1, minColumn));
		}else if((pSheet->ColumnVisibleIndex2Pointer(visibleIndexes.second)->GetRight() - RESIZE_AREA_HARF_WIDTH) < e.Point.x){
			return OnColumnHeaderDividerDblClk(pSheet, e, min(visibleIndexes.second, maxColumn));
		}else{
			//Do Nothing
		}
	}
	return CSheetState::Normal();
}

CSheetState* CTracker::OnMouseMove(CSheet* pSheet, MouseEventArgs& e)
{
	return CSheetState::Normal();
}

CSheetState* CTracker::OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e)
{
	std::function<CSheetState*()> arrowFunction = [&]()->CSheetState*{
		//e.Handled = FALSE;
		//::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		return CSheetState::Normal();
	};

	std::function<CSheetState*()> resizeFunction = [&]()->CSheetState*{
		e.Handled = TRUE;
		::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
		return CSheetState::Normal();
	};

	std::function<CSheetState*()> defaultFunction = [&]()->CSheetState*{
		//e.Handled = FALSE;
		return CSheetState::Normal();
	};

	if(!pSheet->Visible()){
		return arrowFunction();
	}

	//Cursor must be in Inplace edit
	if(e.HWnd!=pSheet->GetGridPtr()->m_hWnd){
		return defaultFunction();
	}

	CPoint pt;
	::GetCursorPos(&pt);
	::ScreenToClient(pSheet->GetGridPtr()->m_hWnd, &pt);
	auto visibleIndexes = pSheet->Point2VisibleIndexes(pt);
	//If Header except Filter
	if(IsTrackable(pSheet, visibleIndexes)){
		auto& colDictionary = pSheet->m_columnVisibleDictionary.get<IndexTag>();
		auto maxColumn = boost::prior(colDictionary.end())->DataPtr->GetVisibleIndex();
		auto minColumn = colDictionary.begin()->DataPtr->GetVisibleIndex();
		if(visibleIndexes.second < minColumn){
			//Out of Left
			return arrowFunction();
		}else if(visibleIndexes.second > maxColumn){
			//Out of Right
			if(pt.x < boost::prior(colDictionary.end())->DataPtr->GetRight() + RESIZE_AREA_HARF_WIDTH){
				return resizeFunction();				
			}else{
				return arrowFunction();
			}
		}else if(pt.x < (pSheet->ColumnVisibleIndex2Pointer(visibleIndexes.second)->GetLeft() + RESIZE_AREA_HARF_WIDTH)){
			return resizeFunction();					
		}else if((pSheet->ColumnVisibleIndex2Pointer(visibleIndexes.second)->GetRight() - RESIZE_AREA_HARF_WIDTH) < pt.x){
			return resizeFunction();						
		}else{
			return arrowFunction();
		}
	}else{
		return arrowFunction();
	}
}

CSheetState* CTracker::OnMouseLeave(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}

bool CTracker::IsTrackable(CSheet* pSheet, std::pair<size_type, size_type> visibleIndexes)
{
	auto& rowDictionary = pSheet->m_rowVisibleDictionary.get<IndexTag>();
	auto& colDictionary = pSheet->m_columnVisibleDictionary.get<IndexTag>();

	auto maxRow = boost::prior(rowDictionary.end())->DataPtr->GetVisibleIndex();
	auto minRow = rowDictionary.begin()->DataPtr->GetVisibleIndex();
	auto maxCol = boost::prior(colDictionary.end())->DataPtr->GetVisibleIndex();
	auto minCol = colDictionary.begin()->DataPtr->GetVisibleIndex();
	CPoint pt;
	::GetCursorPos(&pt);
	::ScreenToClient(pSheet->GetGridPtr()->m_hWnd, &pt);

	auto spRow = pSheet->VisibleRow(visibleIndexes.first);
	if( visibleIndexes.first >= 0 || 
		visibleIndexes.first < minRow || visibleIndexes.first > maxRow || 
		visibleIndexes.second < minCol || (visibleIndexes.second > maxCol && pt.x > boost::prior(colDictionary.end())->DataPtr->GetRight() + RESIZE_AREA_HARF_WIDTH )){
		return false;
	}else if(spRow && !spRow->IsDragTrackable()){
		return false;
	}
	return true;
}

CSheetState* CTracker::OnTrackLButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	return CSheetState::Normal();
}
CSheetState* CTracker::OnTrackLButtonUp(CSheet* pSheet, MouseEventArgs& e)
{
	return OnColumnHeaderEndTrack(pSheet, e);		
}
CSheetState* CTracker::OnTrackLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)
{
	return CSheetState::Normal();
}
CSheetState* CTracker::OnTrackRButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	return CSheetState::Normal();
}
CSheetState* CTracker::OnTrackMouseMove(CSheet* pSheet, MouseEventArgs& e)
{
	return OnColumnHeaderTrack(pSheet, e);
}
CSheetState* CTracker::OnTrackMouseLeave(CSheet* pSheet, MouseEventArgs& e)
{
	return OnColumnHeaderTrack(pSheet, e);
}
CSheetState* CTracker::OnTrackSetCursor(CSheet* pSheet, SetCursorEventArgs& e)
{
	return CSheetState::Tracking();
}