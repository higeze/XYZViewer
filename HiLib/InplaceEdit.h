#pragma once
#include "MyWnd.h"
class CDrawTextProperty;
class CRect;

//class IInplaceEdit
//{
//public:
//	typedef std::basic_string<TCHAR> string_type;
//public:
//
//	virtual HWND Create(HWND hWnd,CRect rcClient)=0;
//	virtual BOOL SetWindowText(LPCTSTR lpszText)=0;
//	virtual void SetFont(HFONT hFont,BOOL bRedraw=TRUE)=0;
//	virtual DWORD SetMargins(UINT uLeft, UINT uRight) = 0;
//	virtual HWND SetFocus()=0;
//	virtual void SetSel(int nStartChar,int nEndChar,BOOL bNoScroll = FALSE)=0;
//	virtual BOOL ShowWindow(int nCmdShow)=0;
//	virtual BOOL MoveWindow(CRect& rc,BOOL bRepaint)=0;
//	virtual CRect GetRect() = 0;
//	virtual void SetRect(const CRect& rc) = 0;
//
//static IInplaceEdit* CreateInstance(
//	std::function<string_type()> getter,
//	std::function<void(const string_type&)> setter,
//	std::function<void(const string_type&)> changed,
//	std::function<void()> final,
//	std::shared_ptr<CDrawTextProperty> spDrawTextProperty);
//};
//
//typedef std::shared_ptr<IInplaceEdit> InplaceEditPtr;

class CInplaceEdit:public CEdit
{
	typedef std::basic_string<TCHAR> string_type;
private:
	std::function<string_type()> m_getter;
	std::function<void(const string_type&)> m_setter;
	std::function<void(const string_type&)> m_changed;
	std::function<void()> m_final;
	UINT m_format;
	string_type m_strInit;
	bool m_bAlt;
public:
	CInplaceEdit(
		std::function<string_type()> getter,
		std::function<void(const string_type&)> setter,
		std::function<void(const string_type&)> changed,
		std::function<void()> final,
		UINT dwDTFormat);
	virtual ~CInplaceEdit(){}
private:
	LRESULT OnSysKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnMenuChar(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	LRESULT OnCmdEnChange(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled);
	void OnFinalMessage(HWND hWnd);
	void EndEdit();

	//HWND Create(HWND hWnd,CRect rcClient){return CEdit::Create(hWnd,rcClient);}
	//BOOL SetWindowText(LPCTSTR lpszText){return CEdit::SetWindowTextW(lpszText);}
	//void SetFont(HFONT hFont,BOOL bRedraw){return CEdit::SetFont(hFont,bRedraw);}
	//DWORD SetMargins(UINT uLeft, UINT uRight){return CEdit::SetMargins(uLeft, uRight);}
	//HWND SetFocus(){return CEdit::SetFocus();}
	//void SetSel(int nStartChar,int nEndChar,BOOL bNoScroll){return CEdit::SetSel(nStartChar,nEndChar,bNoScroll);}
	//BOOL ShowWindow(int nCmdShow){return CEdit::ShowWindow(nCmdShow);}
	//BOOL MoveWindow(CRect& rc,BOOL bRepaint){return CEdit::MoveWindow(rc,bRepaint);}
	//CRect GetRect(){return CEdit::GetRect();};
	//void SetRect(const CRect& rc){return CEdit::SetRect(rc);}

};
