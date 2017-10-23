#pragma once

class CSheet;
class CSheetState;
struct MouseEventArgs;
struct SetCursorEventArgs;

class IMouseObserver
{
public:
	IMouseObserver(void){}
	virtual ~IMouseObserver(void){}
	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)=0;

	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e)=0;

	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e)=0;

	virtual CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e)=0;

};

