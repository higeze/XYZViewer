#include "SheetState.h"
#include "Sheet.h"
#include "RowColumn.h"
#include "Column.h"
#include "Cell.h"
#include "IMouseObserver.h"
#include "Tracker.h"
#include "Dragger.h"
#include "GridView.h"

CSheetState* CSheetState::Normal()
{
	static CNormalState state;
	return &state;
}

CSheetState* CSheetState::Tracking()
{
	static CTrackingState state;
	return &state;
}

CSheetState* CSheetState::Dragging()
{
	static CDraggingState state;
	return &state;
}

CSheetState* CNormalState::OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();
	auto roco = pSheet->Point2RowColumn(e.Point);

	for(auto& observer : pSheet->m_mouseObservers){
		auto state = observer->OnLButtonDown(pSheet, e);
		if(state != CSheetState::Normal())
		{
			return state;
		}
	}
	
	if(!roco.IsInvalid()){
		roco.GetColumnPtr()->Cell(roco.GetRowPtr())->OnLButtonDown(e);
	}
	return CSheetState::Normal();
}

CSheetState* CNormalState::OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();
	auto roco = pSheet->Point2RowColumn(e.Point);

	for(auto& observer : pSheet->m_mouseObservers){
		auto state = observer->OnLButtonUp(pSheet, e);
		if(state != CSheetState::Normal())
		{
			return state;
		}
	}

	if(!roco.IsInvalid()){
		roco.GetColumnPtr()->Cell(roco.GetRowPtr())->OnLButtonUp(e);
	}
	return CSheetState::Normal();
}

CSheetState* CNormalState::OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();
	auto roco = pSheet->Point2RowColumn(e.Point);

	for(auto& observer : pSheet->m_mouseObservers){
		auto state = observer->OnLButtonDblClk(pSheet, e);
		if(state != CSheetState::Normal())
		{
			return state;
		}
	}

	if(!roco.IsInvalid()){
		roco.GetColumnPtr()->Cell(roco.GetRowPtr())->OnLButtonDblClk(e);
	}
	return CSheetState::Normal();
};

CSheetState* CNormalState::OnRButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();
	auto roco = pSheet->Point2RowColumn(e.Point);

	for(auto& observer : pSheet->m_mouseObservers){
		auto state = observer->OnRButtonDown(pSheet, e);
		if(state != CSheetState::Normal())
		{
			return state;
		}
	}
	//TODOTODO
	//if(!roco.IsInvalid()){
	//	roco.GetColumnPtr()->Cell(roco.GetRowPtr())->OnRButtonDown(e);
	//}
	return CSheetState::Normal();
}

CSheetState* CNormalState::OnMouseMove(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();
	auto roco = pSheet->Point2RowColumn(e.Point);

	for(auto& observer : pSheet->m_mouseObservers){
		auto state = observer->OnMouseMove(pSheet, e);
		if(state != CSheetState::Normal())
		{
			return state;
		}
	}

	CRowColumn rocoMouse = pSheet->GetMouseRowColumn();
	if(!rocoMouse.IsInvalid() && rocoMouse!=roco){
		rocoMouse.GetColumnPtr()->Cell(rocoMouse.GetRowPtr())->OnMouseLeave(e);
	}
	pSheet->SetMouseRowColumn(roco);

	if(!roco.IsInvalid()){
		roco.GetColumnPtr()->Cell(roco.GetRowPtr())->OnMouseMove(e);
	}
	return CSheetState::Normal();
};

CSheetState* CNormalState::OnMouseLeave(CSheet* pSheet, MouseEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();
	auto roco = pSheet->Point2RowColumn(e.Point);

	for(auto& observer : pSheet->m_mouseObservers){
		auto state = observer->OnMouseLeave(pSheet, e);
		if(state != CSheetState::Normal())
		{
			return state;
		}
	}

	CRowColumn rocoMouse = pSheet->GetMouseRowColumn();
	if(!rocoMouse.IsInvalid()){
		pSheet->UnhotAll();
		//CSheet::Cell(m_rocoMouse.GetRowPtr(), m_rocoMouse.GetColumnPtr())->SetState(UIElementState::Normal);
		CSheet::Cell(rocoMouse.GetRowPtr(), rocoMouse.GetColumnPtr())->OnMouseLeave(e);
	}
	pSheet->SetMouseRowColumn(roco);
	return CSheetState::Normal();
}

CSheetState* CNormalState::OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e)
{
	if(!pSheet->Visible())return CSheetState::Normal();

	for(auto& observer : pSheet->m_mouseObservers){
		auto state = observer->OnSetCursor(pSheet, e);
		if(state != CSheetState::Normal())
		{
			return state;
		}
	}
	CPoint pt;
	::GetCursorPos(&pt);
	::ScreenToClient(pSheet->GetGridPtr()->m_hWnd, &pt);
	auto roco = pSheet->Point2RowColumn(pt);
	if(!roco.IsInvalid()){
		roco.GetColumnPtr()->Cell(roco.GetRowPtr())->OnSetCursor(e);
	}
	return CSheetState::Normal();
}



CSheetState* CTrackingState::OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spTracker->OnTrackLButtonDown(pSheet, e);
}
CSheetState* CTrackingState::OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spTracker->OnTrackLButtonUp(pSheet, e);		
}
CSheetState* CTrackingState::OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spTracker->OnTrackLButtonDblClk(pSheet, e);		
}
CSheetState* CTrackingState::OnRButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spTracker->OnTrackRButtonDown(pSheet, e);		
}
CSheetState* CTrackingState::OnMouseMove(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spTracker->OnTrackMouseMove(pSheet, e);		
}
CSheetState* CTrackingState::OnMouseLeave(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spTracker->OnTrackMouseLeave(pSheet, e);		
}
CSheetState* CTrackingState::OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e)
{
	return pSheet->m_spTracker->OnTrackSetCursor(pSheet, e);		
}


CSheetState* CDraggingState::OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spDragger->OnDragLButtonDown(pSheet, e);
}
CSheetState* CDraggingState::OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spDragger->OnDragLButtonUp(pSheet, e);		
}
CSheetState* CDraggingState::OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spDragger->OnDragLButtonDblClk(pSheet, e);		
}
CSheetState* CDraggingState::OnRButtonDown(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spDragger->OnDragRButtonDown(pSheet, e);		
}
CSheetState* CDraggingState::OnMouseMove(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spDragger->OnDragMouseMove(pSheet, e);		
}
CSheetState* CDraggingState::OnMouseLeave(CSheet* pSheet, MouseEventArgs& e)
{
	return pSheet->m_spDragger->OnDragMouseLeave(pSheet, e);		
}
CSheetState* CDraggingState::OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e)
{
	return pSheet->m_spDragger->OnDragSetCursor(pSheet, e);		
}



//CSheetState* CDraggingState::OnLButtonDown(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();};
//CSheetState* CDraggingState::OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)
//{
//	return pSheet->m_spDragger->OnColumnHeaderEndDrag(pSheet, e);		
//}
//CSheetState* CDraggingState::OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();};
//CSheetState* CDraggingState::OnRButtonDown(CSheet* pSheet, MouseEventArgs& e){return CSheetState::Normal();}
//CSheetState* CDraggingState::OnMouseMove(CSheet* pSheet, MouseEventArgs& e)
//{
//	return pSheet->m_spDragger->OnColumnHeaderDrag(pSheet, e);
//}
//CSheetState* CDraggingState::OnMouseLeave(CSheet* pSheet, MouseEventArgs& e)
//{
//	return pSheet->m_spDragger->OnColumnHeaderDrag(pSheet, e);
//}
//CSheetState* CDraggingState::OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e)
//{
//	return CSheetState::Dragging();
//}
