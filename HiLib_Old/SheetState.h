#pragma once
#include "RowColumn.h"
class CSheet;
struct MouseEventArgs;
struct SetCursorEventArgs;

class CSheetState
{
public:
	CSheetState(){}
	virtual ~CSheetState(){}

	static CSheetState* Normal();
	static CSheetState* Tracking();
	static CSheetState* Dragging();

	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)=0;

	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e)=0;

	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e)=0;

	virtual CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e)=0;
};

class CNormalState:public CSheetState
{
private:
	CRowColumn m_rocoMouse;
public:
	CNormalState():m_rocoMouse(){}
	virtual ~CNormalState(){}

	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e);
};

class CTrackingState:public CSheetState
{
public:
	CTrackingState(){}
	virtual ~CTrackingState(){}

	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e);

};

class CDraggingState:public CSheetState
{
public:
	CDraggingState(){}
	virtual ~CDraggingState(){}

	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e);

};


