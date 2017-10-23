#pragma once
#include "RowColumn.h"
#include "MyPoint.h"
class CSheet;
struct MouseEventArgs;
struct SetCursorEventArgs;

class IMouseState
{
public:
	IMouseState(){}
	~IMouseState(){}

	virtual void Entry(CSheet* pSheet){};
	virtual void Exit(CSheet* pSheet){};
	virtual IMouseState* ChangeState(CSheet* pSheet, IMouseState* pMouseState);
	virtual IMouseState* KeepState();
	virtual IMouseState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual IMouseState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual IMouseState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual IMouseState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual IMouseState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e);
};

class CDefaultMouseState:public IMouseState
{
public:
	CDefaultMouseState(){}
	virtual ~CDefaultMouseState(){}
	static IMouseState* State();
	virtual IMouseState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e)override;
};

class CDownedMouseState:public IMouseState
{
private:
	static bool m_isDblClkTimeExceed;
public:
	CDownedMouseState(){}
	virtual ~CDownedMouseState(){}
	static IMouseState* State();
	virtual void Entry(CSheet* pSheet);
	virtual IMouseState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e)override;
};

class CUppedMouseState:public IMouseState
{
private:
	static CPoint m_ptUpped;
public:
	CUppedMouseState(){}
	virtual ~CUppedMouseState(){}
	static IMouseState* State(CPoint pt = CPoint());
	virtual IMouseState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e)override;
};

class CDblClkedMouseState:public IMouseState
{
public:
	CDblClkedMouseState(){}
	virtual ~CDblClkedMouseState(){}
	static IMouseState* State();
	virtual IMouseState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e)override;
	virtual IMouseState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e)override;
};

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
	virtual CSheetState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e)=0;

	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e)=0;
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e)=0;

	virtual CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e)=0;
};

class CNormalState:public CSheetState
{
private:
	CRowColumn m_rocoMouse;
	IMouseState *m_pMouseState;
public:
	CNormalState():m_rocoMouse(),m_pMouseState(CDefaultMouseState::State()){}
	virtual ~CNormalState(){}

	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e);
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
	virtual CSheetState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e);
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
	virtual CSheetState* OnLButtonDblClkTimeExceed(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e);

};


