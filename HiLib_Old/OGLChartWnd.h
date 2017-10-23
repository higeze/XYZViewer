#pragma once
#include "MyWnd.h"
#include "Vertex.h"
#include "OGLUIElement.h"
#include "OGLEventArgs.h"


#include "MyFont.h"
#include "MyPen.h"
#include "MyColor.h"
#include "MyFriendSerializer.h"

class C3DLineChart;
class COGLChartWnd:public CWnd
{
protected:
	//Chart
	std::shared_ptr<C3DLineChart> m_spChart;
	//wnd
	UPBufferDC m_upBuffDC;
	bool m_canDeleteThis;
	bool m_canPostQuit;
	CDC* m_pDC;
public:
	COGLChartWnd(std::shared_ptr<C3DLineChart> spOGLChart, bool canDeleteThis = false, bool canPostQuit = true);
	virtual ~COGLChartWnd(){}
	LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual void OnFinalMessage(HWND hWnd);
	LRESULT OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnEraseBkGnd(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnMButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnMButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnMouseWheel(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	LRESULT OnCommandReset(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
};