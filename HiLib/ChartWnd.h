#pragma once
#include "MyWnd.h"
#include "Chart.h"
#include "Series.h"
#include "MyPen.h"

class CChartWnd:public CWnd
{
private:
	UPBufferDC m_upBuffDC;
	std::shared_ptr<CChart> m_spChart;
	bool m_canDeleteThis;
	bool m_canPostQuit;
public:
	CChartWnd(std::shared_ptr<CChart> spChart, bool canDeleteThis = false, bool canPostQuit = true)
		:CWnd(),m_spChart(spChart),m_canDeleteThis(canDeleteThis),m_canPostQuit(canPostQuit)
	{
		m_rca
			.lpszClassName(L"CChartWnd")
			.style(CS_VREDRAW | CS_HREDRAW)
			.hCursor(::LoadCursor(NULL, IDC_ARROW))
			.hbrBackground((HBRUSH)GetStockObject(WHITE_BRUSH));

		m_cwa
			.lpszWindowName(L"ChartWnd") 
			.lpszClassName(L"CChartWnd")
			.dwStyle(WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN); 


		AddMsgHandler(WM_CREATE,&CChartWnd::OnCreate,this);
		AddMsgHandler(WM_SIZE,&CChartWnd::OnSize,this);
		AddMsgHandler(WM_CLOSE,&CChartWnd::OnClose,this);
		AddMsgHandler(WM_DESTROY,&CChartWnd::OnDestroy,this);
		AddMsgHandler(WM_ERASEBKGND,&CChartWnd::OnEraseBkGnd,this);
		AddMsgHandler(WM_PAINT,&CChartWnd::OnPaint,this);
		
		AddMsgHandler(WM_LBUTTONDOWN,&CChartWnd::OnLButtonDown,this);
		AddMsgHandler(WM_LBUTTONUP,&CChartWnd::OnLButtonUp,this);
		AddMsgHandler(WM_MOUSEMOVE,&CChartWnd::OnMouseMove,this);
		AddMsgHandler(WM_MOUSELEAVE,&CChartWnd::OnMouseLeave,this);
	}
	virtual ~CChartWnd(){}

	std::shared_ptr<CChart> GetChartPtr(){return m_spChart;}
	void SetChartPtr(std::shared_ptr<CChart> spChart){m_spChart=spChart;}


	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		SetFocus();
		SetCapture();

		CPoint ptClient((short)LOWORD(lParam),(short)HIWORD(lParam));	
		m_spChart->OnLButtonDown(MouseEventArgs((UINT)wParam,ptClient));
		
		InvalidateRect(NULL, TRUE);
		return 0;
	}

	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		ReleaseCapture();

		CPoint ptClient((short)LOWORD(lParam),(short)HIWORD(lParam));	
		m_spChart->OnLButtonUp(MouseEventArgs((UINT)wParam,ptClient));
		
		InvalidateRect(NULL, TRUE);
		return 0;
	}

	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		//TrackMouseEvent
		TRACKMOUSEEVENT stTrackMouseEvent;
		stTrackMouseEvent.cbSize = sizeof(stTrackMouseEvent);
		stTrackMouseEvent.dwFlags = TME_LEAVE;
		stTrackMouseEvent.hwndTrack = m_hWnd;
		::TrackMouseEvent( &stTrackMouseEvent );

		CPoint ptClient((short)LOWORD(lParam),(short)HIWORD(lParam));	
		m_spChart->OnMouseMove(MouseEventArgs((UINT)wParam,ptClient));

		InvalidateRect(NULL, TRUE);
		return 0;
	}

	LRESULT OnMouseLeave(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		CPoint ptClient((short)LOWORD(lParam),(short)HIWORD(lParam));	
		m_spChart->OnMouseLeave(MouseEventArgs((UINT)wParam,ptClient));

		InvalidateRect(NULL, TRUE);
		return 0;
	}

	LRESULT OnEraseBkGnd(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		//For Back buffering
		bHandled=TRUE;
		return 1;
	}

	LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		CPaintDC dc(m_hWnd);
		CRect rcClient(GetClientRect());

		m_upBuffDC->FillRect(rcClient,((HBRUSH)GetStockObject(WHITE_BRUSH)));
			m_upBuffDC->SetBkMode(TRANSPARENT);


		m_spChart->OnPaint(PaintEventArgs(m_upBuffDC.get()));
		dc.BitBlt(
			rcClient.left, rcClient.top,
			rcClient.Width(), rcClient.Height(),
			*m_upBuffDC.get(),0,0,SRCCOPY);

		return 0;
	}

	LRESULT OnCreate(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnClose(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		DestroyWindow();
		return 0;
	}

	LRESULT OnDestroy(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		if(m_canPostQuit){
			::PostQuitMessage(0);
		}
		return 0;
	}

	void OnFinalMessage(HWND hWnd)
	{
		if(m_canDeleteThis){
			delete this;
		}
	}

	LRESULT OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		CRect rcClient = GetClientRect();
		if(m_upBuffDC.get()==nullptr || rcClient.Width()>m_upBuffDC->GetSize().cx || rcClient.Height()>m_upBuffDC->GetSize().cy){
			CClientDC dc(m_hWnd);
			m_upBuffDC.reset(new CBufferDC(dc,rcClient.Width(),rcClient.Height()));	
		}

		m_spChart->SetRect(GetClientRect());
		return 0;
	}

};