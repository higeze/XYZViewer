
#include "InplaceEdit.h"
#include "CellProperty.h"

#include "GridViewResource.h"

CInplaceEdit::CInplaceEdit(
	std::function<string_type()> getter,
	std::function<void(const string_type&)> setter,
	std::function<void(const string_type&)> changed,
	std::function<void()> final,
	UINT dwDTFormat)
	:m_getter(getter),
	m_setter(setter),
	m_changed(changed),
	m_final(final),
	m_format(dwDTFormat),
	m_bAlt(false),
	m_strInit(getter())

{
	WNDCLASSEX wc;
	::GetClassInfoEx(::GetModuleHandle(NULL),WC_EDIT,&wc);

	m_rca=RegisterClassExArg(wc);
	m_rca	
	.lpszClassName(L"CInplaceEdit");

	m_pfnDefWndProc=wc.lpfnWndProc;
	
	m_cwa.lpszWindowName(L"InplaceEdit")
	.lpszClassName(L"CInplaceEdit")
	.dwStyle(WS_CHILD|/*WS_BORDER|*//*WS_VSCROLL|WS_HSCROLL|*/ES_LEFT|ES_MULTILINE/*|ES_AUTOHSCROLL*/|ES_AUTOVSCROLL)//Use ES_AUTOHSCROLL avoid word break during typing
	.hMenu((HMENU)IDC_INPLACEEDIT);//TODO specify EditControl ID

	AddMsgHandler(WM_KEYDOWN,&CInplaceEdit::OnKeyDown,this);
	AddMsgHandler(WM_SYSKEYDOWN,&CInplaceEdit::OnSysKeyDown,this);
	AddMsgHandler(WM_MENUCHAR,&CInplaceEdit::OnMenuChar,this);

	//AddMsgHandler(WM_CHAR,&CInplaceEdit::OnChar,this);
	AddMsgHandler(WM_KILLFOCUS,&CInplaceEdit::OnKillFocus,this);
	AddCmdCdHandler(EN_CHANGE,&CInplaceEdit::OnCmdEnChange,this);	
}

LRESULT CInplaceEdit::OnSysKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	switch(wParam){
	case VK_RETURN:
		if(::GetKeyState(VK_MENU) & 0x80){
			m_bAlt=true;
			SendMessage(WM_CHAR,VK_RETURN,lParam);
			bHandled=TRUE;
		}
		break;
	default:
		bHandled=FALSE;
		break;
	}
	return 0;
}

LRESULT CInplaceEdit::OnMenuChar(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	return MNC_CLOSE << 16;
}

LRESULT CInplaceEdit::OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	switch(wParam){
	case VK_RETURN:
		::SetFocus(::GetParent(m_hWnd));//This function delete this as result
		bHandled=TRUE;		
		break;
	case VK_TAB:
	//case VK_PRIOR:
	//case VK_NEXT:
	//case VK_DOWN:
	//case VK_UP:
	//case VK_RIGHT:
	//case VK_LEFT:
		::SetFocus(::GetParent(m_hWnd));//This function delete this as result
		bHandled=TRUE;
		break;
	case VK_ESCAPE:
		CEdit::SetWindowText(m_strInit.c_str());
		::SetFocus(::GetParent(m_hWnd));//This function delete this as result
		bHandled=TRUE;
		break;
	default:
		bHandled=FALSE;
		break;
	}

	return 0;
}

LRESULT CInplaceEdit::OnKillFocus(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	EndEdit();
	return 0;
}

void CInplaceEdit::OnFinalMessage(HWND hWnd)
{
	if(m_final){m_final();}
	delete this;
}

LRESULT CInplaceEdit::OnCmdEnChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
{
//	if(!m_setter._Empty()){m_setter(GetWindowText());}
	if(m_changed){m_changed(GetWindowText());}

	CClientDC dc(m_hWnd);
	dc.SelectFont(GetFont());

	CRect rcRect(CEdit::GetRect());
	CRect rcWindow(GetWindowRect());
	CRect rcCalc(0,0,rcRect.Width(),0);
	std::basic_string<TCHAR> strText(GetWindowText());
	if(strText.size()>=2 && strText[strText.size()-2]==L'\r' && strText[strText.size()-1]==L'\n'){
		strText+=L" ";//In orcder to break final \r\n
	}

	dc.DrawTextEx(const_cast<LPTSTR>(strText.c_str()),-1,&rcCalc ,m_format | DT_CALCRECT,NULL);
	rcWindow.bottom+=max((rcCalc.Height()-rcRect.Height()),0);
	//BOOL b=SetWindowPos(NULL,rcWindow.left,rcWindow.top,rcWindow.Width(),rcWindow.Height(),SW_SHOW);
	CPoint ptTopLeft(rcWindow.TopLeft());
	::ScreenToClient(GetParent(),ptTopLeft);
	rcWindow.MoveToXY(ptTopLeft);
	CEdit::MoveWindow(rcWindow,FALSE);
	return 0;
}

void CInplaceEdit::EndEdit()
{
	if(m_setter){m_setter(GetWindowText());}
	SendMessage(WM_CLOSE,NULL,NULL);	
}

