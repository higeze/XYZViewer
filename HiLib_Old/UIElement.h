#pragma once
#include "MyRect.h"
#include "MySize.h"
#include "MyDC.h"
#include "MyFont.h"
#include "MyPen.h"
#include "MyWnd.h"

namespace UIElementState
{
	enum Type{
		None,
		Normal,
		Hot,
		Pressed,
		PressedLeave,
		Disabled
	};
}

struct EventArgs
{
public:
	CWnd* WindowPtr;
	EventArgs(CWnd* pWnd = nullptr):WindowPtr(pWnd){}
	virtual ~EventArgs(){}
};

struct KeyEventArgs:public EventArgs
{
public:
	UINT Char;
	UINT RepeatCount;
	UINT Flags;
public:
	KeyEventArgs(UINT ch,UINT uRepCnt,UINT uFlags)
		:Char(ch),RepeatCount(uRepCnt),Flags(uFlags){}
	virtual ~KeyEventArgs(){}
};

struct PaintEventArgs:public EventArgs
{
	CDC* DCPtr;
	PaintEventArgs(CWnd* pWnd, CDC* pDC)
		:EventArgs(pWnd),DCPtr(pDC){}
	PaintEventArgs(CDC* pDC)
		:EventArgs(),DCPtr(pDC){}
	virtual ~PaintEventArgs(){}
};

//struct OGLPaintEventArgs:public PaintEventArgs
//{
//	CDC* DCPtr;
//	COGLRenderer* OGLRendererPtr;
//	OGLPaintEventArgs(COGLRenderer* pOGLRenderer, CDC* pDC)
//		:PaintEventArgs(pDC),OGLRendererPtr(pOGLRenderer){}
//};

struct MouseEventArgs:public EventArgs
{
	UINT Flags;
	CPoint Point;
	MouseEventArgs(UINT uFlags,CPoint pt)
		:Flags(uFlags),Point(pt){}
	virtual ~MouseEventArgs(){}
};

struct MouseWheelEventArgs:public MouseEventArgs
{
	short Delta;
	MouseWheelEventArgs(UINT uFlags, short zDelta, CPoint pt)
		:MouseEventArgs(uFlags, pt),Delta(zDelta){}
	virtual ~MouseWheelEventArgs(){}
};

struct SetCursorEventArgs:public EventArgs
{
	HWND HWnd;
	UINT HitTest;
	BOOL& Handled;
	SetCursorEventArgs(HWND HWnd, UINT nHitTest, BOOL& Handled)
		:HWnd(HWnd), HitTest(nHitTest), Handled(Handled){}
	virtual ~SetCursorEventArgs(){}
};


struct ContextMenuEventArgs:public EventArgs
{
public:
	CPoint Point;
	ContextMenuEventArgs(CWnd* pWnd, CPoint pt)
		:EventArgs(pWnd),Point(pt){}
	virtual ~ContextMenuEventArgs(){}
};


class CUIElement
{
protected:
	UIElementState::Type m_state;

public:
	CUIElement()
		:m_state(UIElementState::Normal){}
	virtual ~CUIElement(){}

	virtual UIElementState::Type GetState()const{return m_state;}
	virtual void SetState(const UIElementState::Type& state)
	{
		if(m_state!=state){
			m_state=state;
		}
	}

	virtual void OnLButtonDown(MouseEventArgs& e);
	virtual void OnLButtonUp(MouseEventArgs& e);
	virtual void OnMButtonDown(MouseEventArgs& e){}
	virtual void OnMButtonUp(MouseEventArgs& e){}
	virtual void OnMouseMove(MouseEventArgs& e);
	virtual void OnMouseEnter(MouseEventArgs& e);
	virtual void OnMouseLeave(MouseEventArgs& e);
	virtual void OnMouseWheel(MouseWheelEventArgs& e){}
	virtual void OnLButtonClk(MouseEventArgs& e){}
	virtual void OnLButtonDblClk(MouseEventArgs& e){}
	virtual void OnKeyDown(KeyEventArgs& e){}
	virtual void OnContextMenu(ContextMenuEventArgs& e){}
};
