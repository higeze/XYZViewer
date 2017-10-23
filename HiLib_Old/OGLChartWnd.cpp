#include "OGLChartWnd.h"
#include "3DLineChart.h"
#include "OGLAxis.h"
#include "OGLSeries.h"
#include "MyFont.h"
#include <math.h>
#include "OGLChart.h"
#include "3DLineChartResource.h"

#ifndef	M_PI
#define	M_PI	3.1415926535897932384626433832795
#endif

COGLChartWnd::COGLChartWnd(std::shared_ptr<C3DLineChart> spChart, bool canDeleteThis, bool canPostQuit) :
	CWnd(),m_spChart(spChart),
	m_canDeleteThis(canDeleteThis),
	m_canPostQuit(canPostQuit)
{
	m_rca
	.lpszClassName(L"OGLChartWnd")
	.style(CS_VREDRAW | CS_HREDRAW)
	.hCursor(::LoadCursor(NULL, IDC_ARROW));

	m_cwa
	.lpszWindowName(L"OGLChartWnd") 
	.lpszClassName(L"OGLChartWnd")
	.dwStyle(WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS) 
	.x(CW_USEDEFAULT)
	.y(CW_USEDEFAULT) 
	.nWidth(CW_USEDEFAULT) 
	.nHeight(CW_USEDEFAULT); 

	AddMsgHandler(WM_CREATE,&COGLChartWnd::OnCreate,this);
	AddMsgHandler(WM_CLOSE,&COGLChartWnd::OnClose,this);
	AddMsgHandler(WM_DESTROY,&COGLChartWnd::OnDestroy,this);
	AddMsgHandler(WM_SIZE,&COGLChartWnd::OnSize,this);
	AddMsgHandler(WM_PAINT,&COGLChartWnd::OnPaint,this);	
	AddMsgHandler(WM_ERASEBKGND,&COGLChartWnd::OnEraseBkGnd,this);
	AddMsgHandler(WM_LBUTTONDOWN,&COGLChartWnd::OnLButtonDown,this);
	AddMsgHandler(WM_LBUTTONUP,&COGLChartWnd::OnLButtonUp,this);
	AddMsgHandler(WM_MBUTTONDOWN,&COGLChartWnd::OnMButtonDown,this);
	AddMsgHandler(WM_MBUTTONUP,&COGLChartWnd::OnMButtonUp,this);
	AddMsgHandler(WM_MOUSEMOVE,&COGLChartWnd::OnMouseMove,this);
	AddMsgHandler(WM_MOUSEWHEEL,&COGLChartWnd::OnMouseWheel,this);
	AddMsgHandler(WM_MOUSELEAVE,&COGLChartWnd::OnMouseLeave,this);
	AddMsgHandler(WM_KEYDOWN,&COGLChartWnd::OnKeyDown,this);
	AddMsgHandler(WM_CONTEXTMENU,&COGLChartWnd::OnContextMenu,this);

	AddCmdIDHandler(IDM_3DLINECHART_RESET, &COGLChartWnd::OnCommandReset, this);
}

LRESULT COGLChartWnd::OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	return 0;
}
LRESULT COGLChartWnd::OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	::DestroyWindow(m_hWnd);
	return 0;
}
LRESULT COGLChartWnd::OnDestroy(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	if(m_canPostQuit){
		::PostQuitMessage(0);
	}
	return 0;
}

void COGLChartWnd::OnFinalMessage(HWND hWnd)
{
	if(m_canDeleteThis){
		delete this;
	}
}

LRESULT COGLChartWnd::OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	CRect rcClient = GetClientRect();
	if(m_upBuffDC.get()==nullptr || rcClient.Width()>m_upBuffDC->GetSize().cx || rcClient.Height()>m_upBuffDC->GetSize().cy){
		CClientDC dc(m_hWnd);
		m_upBuffDC.reset(new CBufferDC(dc,rcClient.Width(),rcClient.Height()));	
	}
	return 0;
}
LRESULT COGLChartWnd::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	CPaintDC paintDC(m_hWnd);//Have to call BeginDC
	CRect rcClient(GetClientRect());

	m_upBuffDC->FillRect(rcClient,((HBRUSH)GetStockObject(WHITE_BRUSH)));
	m_upBuffDC->SetBkMode(TRANSPARENT);

	m_spChart->GetRightPanePtr()->SetVisible(true);
	m_spChart->SetRect(rcClient);
	m_spChart->OnPaint(PaintEventArgs(m_upBuffDC.get()));
	paintDC.BitBlt(
		rcClient.left, rcClient.top,
		rcClient.Width(), rcClient.Height(),
		*m_upBuffDC.get(),0,0,SRCCOPY);

	return 0;
}
LRESULT COGLChartWnd::OnEraseBkGnd(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	//Double buffering
	bHandled = TRUE;
	return 1;
}
LRESULT COGLChartWnd::OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{	
	SetFocus();
	SetCapture();
	m_spChart->OnLButtonDown(MouseEventArgs((UINT)wParam, CPoint((short)LOWORD(lParam),(short)HIWORD(lParam))));
	InvalidateRect(NULL, TRUE);
	return 0;
}
LRESULT COGLChartWnd::OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	ReleaseCapture();
	m_spChart->OnLButtonUp(MouseEventArgs((UINT)wParam, CPoint((short)LOWORD(lParam),(short)HIWORD(lParam))));
	InvalidateRect(NULL, TRUE);
	return 0;
}
LRESULT COGLChartWnd::OnMButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	SetFocus();
	SetCapture();
	m_spChart->OnMButtonDown(MouseEventArgs((UINT)wParam, CPoint((short)LOWORD(lParam),(short)HIWORD(lParam))));
	InvalidateRect(NULL, TRUE);
	return 0;
}
LRESULT COGLChartWnd::OnMButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	ReleaseCapture();
	m_spChart->OnMButtonUp(MouseEventArgs((UINT)wParam, CPoint((short)LOWORD(lParam),(short)HIWORD(lParam))));
	InvalidateRect(NULL, TRUE);
	return 0;
}
LRESULT COGLChartWnd::OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	//TrackMouseEvent
	TRACKMOUSEEVENT stTrackMouseEvent;
	stTrackMouseEvent.cbSize = sizeof(stTrackMouseEvent);
	stTrackMouseEvent.dwFlags = TME_LEAVE;
	stTrackMouseEvent.hwndTrack = m_hWnd;
	::TrackMouseEvent( &stTrackMouseEvent );

	m_spChart->OnMouseMove(MouseEventArgs((UINT)wParam, CPoint((short)LOWORD(lParam),(short)HIWORD(lParam))));
	InvalidateRect(NULL, TRUE );
	return 0;
}

LRESULT COGLChartWnd::OnMouseLeave(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	CPoint ptClient((short)LOWORD(lParam),(short)HIWORD(lParam));	
	m_spChart->OnMouseLeave(MouseEventArgs((UINT)wParam,ptClient));

	InvalidateRect(NULL, TRUE);
	return 0;
}

LRESULT COGLChartWnd::OnMouseWheel(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	m_spChart->OnMouseWheel(MouseWheelEventArgs((UINT)wParam, GET_WHEEL_DELTA_WPARAM(wParam), CPoint((short)LOWORD(lParam),(short)HIWORD(lParam))));
	InvalidateRect(NULL, TRUE );
	return 0;
}

LRESULT COGLChartWnd::OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	m_spChart->OnKeyDown(KeyEventArgs(wParam, lParam & 0xFF, lParam>>16 & 0xFF));
	InvalidateRect(NULL, TRUE);
	return 0;
}

LRESULT COGLChartWnd::OnContextMenu(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	CPoint ptClient((short)LOWORD(lParam),(short)HIWORD(lParam));	
	ScreenToClient(ptClient);//Necessary to convert Client
	m_spChart->OnContextMenu(ContextMenuEventArgs(this, ptClient));
	InvalidateRect(NULL, TRUE);
	return 0;
}

LRESULT COGLChartWnd::OnCommandReset(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
{
	m_spChart->OnKeyDown(KeyEventArgs(VK_SPACE, 1, 0));
	InvalidateRect(NULL, TRUE);
	return 0;
}







