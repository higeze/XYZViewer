#pragma once

#include "MyUniqueHandle.h"
#include "MyRect.h"
#include "MySize.h"
#include "MyDC.h"
#include "MyString.h"

#include <shellapi.h>
#include <commctrl.h>
#include <commdlg.h>
#include "MyPrerocessor.h"

#pragma comment(lib, "comctl32.lib")

using namespace std::placeholders;
namespace phs=std::placeholders;

#define NAMED_STRUCT_ARG_METHOD(cls,type,strct,prop) \
	cls& prop(type prop){strct.##prop=prop;return *this;}\
	type prop()const{return strct.##prop;}



/*******************/
/* GetOpenFileName */
/*******************/

struct GetOpenFileNameArg
{
	OPENFILENAME m_ofn;

	GetOpenFileNameArg(){
		::ZeroMemory(&m_ofn,sizeof(m_ofn));
		m_ofn.lStructSize=sizeof(m_ofn);
		m_ofn.nFilterIndex=1;
	}
	NAMED_STRUCT_ARG_METHOD(GetOpenFileNameArg,HWND,m_ofn,hwndOwner)
	NAMED_STRUCT_ARG_METHOD(GetOpenFileNameArg,DWORD,m_ofn,Flags);
	NAMED_STRUCT_ARG_METHOD(GetOpenFileNameArg,LPCTSTR,m_ofn,lpstrFilter)
	NAMED_STRUCT_ARG_METHOD(GetOpenFileNameArg,LPCTSTR,m_ofn,lpstrDefExt)
	NAMED_STRUCT_ARG_METHOD(GetOpenFileNameArg,LPCTSTR,m_ofn,lpstrTitle)
	NAMED_STRUCT_ARG_METHOD(GetOpenFileNameArg,LPCTSTR,m_ofn,lpstrInitialDir)
};

std::vector<std::basic_string<TCHAR>>
	GetOpenFileName(GetOpenFileNameArg arg);


std::wstring GetModuleFileName(HMODULE hModule=NULL);
std::wstring PathFindDirectory(std::wstring wstrPath);
std::wstring GetModuleDirectory(HMODULE hModule=NULL);

/********/
/* CWnd */
/********/
class CWnd;

struct RegisterClassExArg
{
private:
	WNDCLASSEX m_wcx;
	std::basic_string<TCHAR> m_strClassName;
	
public:
	RegisterClassExArg()
	{
		ZeroMemory(&m_wcx,sizeof(m_wcx));
		m_wcx.cbSize=sizeof(WNDCLASSEX);
		//Set my favorite default parameter
		m_wcx.hInstance=::GetModuleHandle(NULL);
	}
	RegisterClassExArg(const WNDCLASSEXW& wc):
		m_wcx(wc),m_strClassName(wc.lpszClassName)
	{	
		m_wcx.cbSize=sizeof(WNDCLASSEX);
		m_wcx.lpszClassName=&m_strClassName[0];
	}
	virtual ~RegisterClassExArg(){}

	operator const WNDCLASSEX*()const{return &m_wcx;}

	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,UINT,m_wcx,style)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,WNDPROC,m_wcx,lpfnWndProc)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,int,m_wcx,cbClsExtra)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,int,m_wcx,cbWndExtra)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,HINSTANCE,m_wcx,hInstance)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,HICON,m_wcx,hIcon)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,HCURSOR,m_wcx,hCursor)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,HBRUSH,m_wcx,hbrBackground)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,LPCTSTR,m_wcx,lpszMenuName)
	NAMED_STRUCT_ARG_METHOD(RegisterClassExArg,HICON,m_wcx,hIconSm)
	RegisterClassExArg& lpszClassName(LPCTSTR lpszClassName)
	{m_strClassName=lpszClassName;m_wcx.lpszClassName=&m_strClassName[0];return *this;}
	LPCTSTR lpszClassName()const{return m_wcx.lpszClassName;}

};

struct CreateWindowExStruct{
	DWORD dwExStyle; 
	LPTSTR lpszClassName; 
	LPTSTR lpszWindowName; 
	DWORD dwStyle; 
	int x;
	int y; 
	int nWidth; 
	int nHeight; 
	HWND hWndParent; 
	HMENU hMenu; 
	HINSTANCE hInstance; 
	LPVOID lpParam;
};

struct CreateWindowExArg{
private:
	CreateWindowExStruct m_cwx;
	std::basic_string<TCHAR> m_strClassName;
	std::basic_string<TCHAR> m_strWindowName;

public:
	CreateWindowExArg()
	{
		ZeroMemory(&m_cwx,sizeof(m_cwx));
		m_cwx.hInstance=::GetModuleHandle(NULL);
	}
	virtual ~CreateWindowExArg(){}
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,DWORD,m_cwx,dwExStyle)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,DWORD,m_cwx,dwStyle)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,int,m_cwx,x)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,int,m_cwx,y)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,int,m_cwx,nWidth)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,int,m_cwx,nHeight)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,HWND,m_cwx,hWndParent)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,HMENU,m_cwx,hMenu)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,HINSTANCE,m_cwx,hInstance)
	NAMED_STRUCT_ARG_METHOD(CreateWindowExArg,LPVOID,m_cwx,lpParam)
	CreateWindowExArg& lpszWindowName(LPCTSTR lpszWindowName)
	{m_strWindowName=lpszWindowName;m_cwx.lpszWindowName=&m_strWindowName[0];return *this;}
	LPCTSTR lpszWindowName()const{return m_cwx.lpszWindowName;}
	CreateWindowExArg& lpszClassName(LPCTSTR lpszClassName)
	{m_strClassName=lpszClassName;m_cwx.lpszClassName=&m_strClassName[0];return *this;}
	LPCTSTR lpszClassName()const{return m_cwx.lpszClassName;}
};

typedef std::function<LRESULT(UINT,LPARAM,WPARAM,BOOL&)> FunMsg;
typedef std::function<LRESULT(WORD,WORD,HWND,BOOL&)> FunCmd;
typedef std::function<LRESULT(int,LPNMHDR,BOOL&)> FunNtfy;

class CWnd
{
public:
	typedef std::multimap<UINT,FunMsg> MsgMap;

	typedef std::unordered_map<WORD,FunCmd> CmdIDMap;
	typedef std::unordered_map<WORD,FunCmd> CmdCdMap;

	typedef std::function<LRESULT(int,LPNMHDR,BOOL&)> FunNtfy;
	typedef std::unordered_map<LONG,FunNtfy> NtfyMap;
//	typedef std::tr1::unordered_map<UINT,FunNtfy> NtfyCdMap;

private:
	//prohibit copy constructor and substitute
public:
	FunMsg m_allMsg;
	MsgMap m_msgMap;
	CmdIDMap m_cmdidMap;
	CmdCdMap m_cmdcdMap;
	NtfyMap m_ntfyMap;
//	NtfyCdMap m_ntfyCdMap;

	LRESULT CALLBACK DefWndProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam);

	LRESULT CALLBACK WndProc(UINT uiMsg,WPARAM wParam,LPARAM lParam);
protected:
	RegisterClassExArg m_rca;
	CreateWindowExArg m_cwa;

	WNDPROC m_pfnDefWndProc;

public:
	HWND m_hWnd;

	CWnd():m_hWnd(NULL),m_pfnDefWndProc(::DefWindowProc)
	{
		AddMsgHandler(WM_COMMAND,&CWnd::OnCommand,this);
		AddMsgHandler(WM_NOTIFY,&CWnd::OnNotify,this);
	}
	CWnd(HWND hWnd):m_hWnd(hWnd),m_pfnDefWndProc(::DefWindowProc)
	{
		AddMsgHandler(WM_COMMAND,&CWnd::OnCommand,this);
		AddMsgHandler(WM_NOTIFY,&CWnd::OnNotify,this);	
	}
	virtual ~CWnd(){}

	void SetCreateWindowExArgument();
	void SetRegisterClassExArgument();

	CreateWindowExArg& CreateWindowExArgument(){return m_cwa;}
	RegisterClassExArg& RegisterClassExArgument(){return m_rca;}
	virtual HWND Create(HWND hWndParent);
	virtual HWND Create(HWND hWndParent,RECT& rc);

	virtual void OnFinalMessage(HWND hWnd){}

	static LRESULT CALLBACK StartWndProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam);

	BOOL SubclassWindow(HWND hWnd);

	LRESULT OnCommand(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	LRESULT OnNotify(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

	BOOL AddAllMsgHandler(UINT uMsg, FunMsg funMsg)
	{
		m_allMsg = funMsg;
		return TRUE;
	}

	template<class U>
	BOOL AddAllMsgHandler(LRESULT(U::*memberfunc)(UINT, WPARAM, LPARAM, BOOL&), U* that)
	{
		m_allMsg = std::bind(memberfunc, that, phs::_1, phs::_2, phs::_3, phs::_4);
		return TRUE;
	}

	BOOL RemoveAllMsgHandler()
	{
		m_allMsg = nullptr;
		return TRUE;
	}

	BOOL AddMsgHandler(UINT uMsg,FunMsg funMsg)
	{	
		/*MsgMap::_Pairib pib=*/m_msgMap.insert(MsgMap::value_type(uMsg,funMsg));
		/*return pib.second;*/
		return TRUE;
	}

	template<class U>
	BOOL AddMsgHandler(UINT uMsg,LRESULT(U::*memberfunc)(UINT,WPARAM,LPARAM,BOOL&),U* that)
	{	
		/*MsgMap::_Pairib pib=*/m_msgMap.insert(MsgMap::value_type(uMsg,std::bind(memberfunc,that,phs::_1,phs::_2,phs::_3,phs::_4)));
		/*return pib.second;*/
		return TRUE;
	}

	template<class U>
	BOOL ReplaceMsgHandler(UINT uMsg,LRESULT(U::*memberfunc)(UINT,WPARAM,LPARAM,BOOL&),U* that)
	{	
		EraseMsgHandler(uMsg);
		return	AddMsgHandler(uMsg,memberfunc,that);
	}

	template<class U>
	BOOL AddCmdIDHandler(WORD wID,LRESULT(U::*memberfunc)(WORD,WORD,HWND,BOOL&),U* that)
	{
		CmdIDMap::_Pairib pib=m_cmdidMap.insert(CmdIDMap::value_type(wID,std::bind(memberfunc,that,phs::_1,phs::_2,phs::_3,phs::_4)));
		return pib.second;
	}
	BOOL AddCmdIDHandler(WORD wID,FunCmd cmd)
	{
		CmdIDMap::_Pairib pib=m_cmdidMap.insert(CmdIDMap::value_type(wID, cmd));
		return pib.second;
	}

	template<class U>
	BOOL ReplaceCmdIDHandler(WORD wID,LRESULT(U::*memberfunc)(WORD,WORD,HWND,BOOL&),U* that)
	{	
		EraseCmdIDHandler(wID);
		return	AddCmdIDHandler(wID,memberfunc,that);
	}

	BOOL ReplaceCmdIDHandler(WORD wID,FunCmd cmd)
	{	
		EraseCmdIDHandler(wID);
		return	AddCmdIDHandler(wID,cmd);
	}

	template<class U>
	BOOL AddCmdCdHandler(WORD wCd,LRESULT(U::*memberfunc)(WORD,WORD,HWND,BOOL&),U* that)
	{
		CmdIDMap::_Pairib pib=m_cmdcdMap.insert(CmdCdMap::value_type(wCd,std::bind(memberfunc,that,phs::_1,phs::_2,phs::_3,phs::_4)));
		return pib.second;
	}
	

	template<class U>
	BOOL AddNtfyHandler(UINT_PTR upID,UINT uCode,LRESULT(U::*memberfunc)(int,LPNMHDR,BOOL&),U* that)
	{
		NtfyMap::_Pairib pib=m_ntfyMap.insert(NtfyMap::value_type(MAKELONG(upID,uCode),std::bind(memberfunc,that,phs::_1,phs::_2,phs::_3)));	
		return pib.second;
	}
	BOOL AddNtfyHandler(UINT_PTR upID,UINT uCode,FunNtfy ntfy)
	{
		NtfyMap::_Pairib pib=m_ntfyMap.insert(NtfyMap::value_type(MAKELONG(upID,uCode),ntfy));	
		return pib.second;
	}

	BOOL ClearMsgHandler();

	BOOL ClearCmdIDHandler();

	BOOL EraseMsgHandler(UINT uMsg);

	BOOL EraseCmdIDHandler(WORD wID);

	BOOL EraseNtfyHandler(UINT_PTR upID,UINT uCode);
/*
	template<class U>
	BOOL AddNtfyCdHandler(UINT uCode,LRESULT(U::*memberfunc)(int,LPNMHDR,BOOL&),U* that)
	{
		return AddNtfyCdHandler(uCode,std::tr1::bind(memberfunc,that,_1,_2,_3));	
	}
*/
	BOOL SetWindowText(LPCTSTR lpszString){return ::SetWindowText(m_hWnd,lpszString);}
	BOOL InvalidateRect(const RECT *lpRect,BOOL bErase){return ::InvalidateRect(m_hWnd,lpRect,bErase);}
	BOOL ClientToScreen(LPPOINT lpPoint){return ::ClientToScreen(m_hWnd,lpPoint);}
	BOOL DestroyWindow(){return ::DestroyWindow(m_hWnd);}
	BOOL GetClientRect(LPRECT lpRect){return ::GetClientRect(m_hWnd,lpRect);}
	CRect GetClientRect()const
	{
		CRect rc;
		::GetClientRect(m_hWnd,&rc);
		return rc;
	}

	BOOL MoveWindow(int X,int Y,int nWidth,int nHeight,BOOL bRepaint){return ::MoveWindow(m_hWnd,X,Y,nWidth,nHeight,bRepaint);}
	BOOL MoveWindow(RECT& rc,BOOL bRepaint){return ::MoveWindow(m_hWnd,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,bRepaint);}
	HWND GetParent(){return ::GetParent(m_hWnd);}
	BOOL GetWindowRect(LPRECT lpRect){return ::GetWindowRect(m_hWnd,lpRect);}
	CRect GetWindowRect()
	{
		CRect rc;
		::GetWindowRect(m_hWnd,&rc);
		return rc;
	}

	BOOL CenterWindow(HWND hWnd=NULL)
	{
		CRect rcArea,rcWnd;
		::SystemParametersInfo(SPI_GETWORKAREA, NULL,&rcArea,NULL);
		GetWindowRect(&rcWnd);
		return MoveWindow((rcArea.left + rcArea.right)/2 - rcWnd.Width()/2,
				(rcArea.top + rcArea.bottom)/2 - rcWnd.Height()/2,
				rcWnd.Width(),rcWnd.Height(),FALSE);
		
	}
	HMENU GetMenu(){return ::GetMenu(m_hWnd);}
	BOOL ShowWindow(int nCmdShow){return ::ShowWindow(m_hWnd,nCmdShow);}
	BOOL UpdateWindow(){return ::UpdateWindow(m_hWnd);}
	int SetWindowRgn(HRGN hRgn,BOOL bRedraw){return ::SetWindowRgn(m_hWnd,hRgn,bRedraw);}
	HDC BeginPaint(LPPAINTSTRUCT lpPaint){return ::BeginPaint(m_hWnd,lpPaint);}
	BOOL EndPaint(const PAINTSTRUCT *lpPaint){return ::EndPaint(m_hWnd,lpPaint);}
	BOOL SetWindowPos(HWND hWndInsertAfter,int X,int Y,int cx,int cy,UINT uFlags){return ::SetWindowPos(m_hWnd,hWndInsertAfter,X,Y,cx,cy,uFlags);}
	int ReleaseDC(HDC hDC){return ::ReleaseDC(m_hWnd,hDC);}
	LONG_PTR SetWindowLongPtr(int nIndex,LONG_PTR dwNewLong){return ::SetWindowLongPtr(m_hWnd,nIndex,dwNewLong);}
	LONG GetWindowLong(int nIndex){return ::GetWindowLong(m_hWnd,nIndex);}
	LRESULT SendMessage(UINT Msg,WPARAM wParam,LPARAM lParam){return ::SendMessage(m_hWnd,Msg,wParam,lParam);}
	LRESULT PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam){return ::PostMessage(m_hWnd,Msg,wParam,lParam);}
	BOOL GetWindowPlacement(PWINDOWPLACEMENT pWp){return (BOOL)::GetWindowPlacement(m_hWnd,pWp);}
	int MessageBox(LPCWSTR lpText,LPCWSTR lpCaption,UINT uType){return (int)::MessageBox(m_hWnd,lpText,lpCaption,uType);}
	UHDC GetUHDC(){
		//release_dc releaser(m_hWnd);
		return UHDC(GetDC(),release_dc(m_hWnd));
	}
	//HDC GetDC(){return ::GetDC(m_hWnd);}
	CDC GetDC(){return ::GetDC(m_hWnd);}
	HFONT GetFont(){return (HFONT)::SendMessage(m_hWnd,WM_GETFONT,0,0);}
	BOOL IsWindowVisible(){return ::IsWindowVisible(m_hWnd);}
	BOOL IsWindow(){return ::IsWindow(m_hWnd);}
	BOOL ScreenToClient(LPPOINT lpPoint){return ::ScreenToClient(m_hWnd,lpPoint);}
	BOOL ScreenToClient(LPRECT lpRect)
	{
		if(!::ScreenToClient(m_hWnd,(LPPOINT)lpRect)){return FALSE;};
		return ::ScreenToClient(m_hWnd,((LPPOINT)lpRect)+1);
	}
	CRect GetScreenToClientRect(LPRECT lpRect)
	{
		if(!::ScreenToClient(m_hWnd,(LPPOINT)lpRect)){return CRect();};
		::ScreenToClient(m_hWnd,((LPPOINT)lpRect)+1);
		return CRect(*lpRect);
	}
	BOOL EnableWindow(BOOL bEnable){return ::EnableWindow(m_hWnd,bEnable);}
	BOOL IsWindowEnabled(){return ::IsWindowEnabled(m_hWnd);}
	HWND SetCapture(){return ::SetCapture(m_hWnd);}
	void SetFont(HFONT hFont,BOOL bRedraw=TRUE){::SendMessage(m_hWnd,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(bRedraw,0));}
	std::basic_string<TCHAR> GetWindowText()
	{
		std::basic_string<TCHAR> strText;
		::GetWindowText(m_hWnd,GetBuffer(strText,256),256);
		ReleaseBuffer(strText);
		return strText;
	}
	HWND SetFocus(){return ::SetFocus(m_hWnd);}
	BOOL SetForegroundWindow(){return ::SetForegroundWindow(m_hWnd);}
	void SetForceForegroundWindow()
	{
		if(!::IsWindow(m_hWnd)) return;
 
		//relation time of SetForegroundWindow lock
		DWORD lockTimeOut = 0;
		HWND  hCurrWnd = ::GetForegroundWindow();
		DWORD dwThisTID = ::GetCurrentThreadId(),
			  dwCurrTID = ::GetWindowThreadProcessId(hCurrWnd,0);
 
		//we need to bypass some limitations from Microsoft :)
		if(dwThisTID != dwCurrTID)
		{
			::AttachThreadInput(dwThisTID, dwCurrTID, TRUE);
 
			::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT,0,&lockTimeOut,0);
			::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,0,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
 
			::AllowSetForegroundWindow(ASFW_ANY);
		}
 
		::SetForegroundWindow(m_hWnd);
 
		if(dwThisTID != dwCurrTID)
		{
			::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,(PVOID)lockTimeOut,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
			::AttachThreadInput(dwThisTID, dwCurrTID, FALSE);
		}
	}


};
/*****************/
/* CDoubleBuffer */
/*****************/

class CDoubleBuffer
{
private:
	UHBITMAP m_uhBitMapBuff;
	UHDC m_uhDCBuff;
public:
	CDoubleBuffer():m_uhBitMapBuff(nullptr),m_uhDCBuff(nullptr){}
	virtual ~CDoubleBuffer(){}

	void UpdateBuffer(CWnd* pWnd)//OnCreate,OnSize
	{
		UHDC uhDC=pWnd->GetUHDC();
		RECT rcClient={0};
		pWnd->GetClientRect(&rcClient);
		m_uhBitMapBuff=UHBITMAP(::CreateCompatibleBitmap(uhDC.get(),rcClient.right-rcClient.left,rcClient.bottom-rcClient.top));
		m_uhDCBuff=UHDC(::CreateCompatibleDC(uhDC.get()));
		::SelectObject(m_uhDCBuff.get(),m_uhBitMapBuff.get());
	}
	void UpdateBuffer(HDC hDC,CRect rc){
		m_uhBitMapBuff=UHBITMAP(::CreateCompatibleBitmap(hDC,rc.Width(),rc.Height()));
		m_uhDCBuff=UHDC(::CreateCompatibleDC(hDC));
		::SelectObject(m_uhDCBuff.get(),m_uhBitMapBuff.get());	
	}
	HDC GetBufferDC(){return m_uhDCBuff.get();}
};

/***********/
/* CButton */
/***********/
class __declspec(novtable) CButton:public CWnd
{
public:
	CButton(){}
	CButton(HWND hWnd):CWnd(hWnd){}
	virtual ~CButton(){}
	CButton& operator=(HWND hWnd){m_hWnd=hWnd;return *this;}
	int GetCheck(){return (int)SendMessage(BM_GETCHECK,0,0L);}
	void SetCheck(int nCheck){SendMessage(BM_SETCHECK,(WPARAM)(int)(nCheck),0L);}
};


/************/
/* CTabCtrl */
/************/

class __declspec(novtable) CTabCtrl:public CWnd
{
public:
	CTabCtrl()
	{
		INITCOMMONCONTROLSEX ic = {0};
		ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
		ic.dwICC = ICC_TAB_CLASSES;
		InitCommonControlsEx(&ic);
		m_rca.lpszClassName(WC_TABCONTROL);
		m_cwa.lpszClassName(WC_TABCONTROL);	
	}
	virtual ~CTabCtrl(){}
	CTabCtrl& operator=(HWND hWnd){m_hWnd=hWnd;return *this;}

	void AdjustRect(BOOL bLarger,LPRECT lpRect){::SendMessage(m_hWnd,TCM_ADJUSTRECT,bLarger,(LPARAM)lpRect);}
	int GetCurSel()
	{
		return (int)::SendMessage(m_hWnd,TCM_GETCURSEL,0,0L);
	}
	int SetCurSel(int nItem)
	{
		return (int)::SendMessage(m_hWnd, TCM_SETCURSEL, nItem, 0L);
	}
	int InsertItem(int nItem,LPCTSTR lpszItem)
	{
		TCITEM tci = {0};
		tci.mask = TCIF_TEXT;
		tci.pszText = (LPTSTR)lpszItem;
		return (int)::SendMessage(m_hWnd,TCM_INSERTITEM,nItem,(LPARAM)&tci);
	}
	int InsertItem(int nItem, LPTCITEM pTabCtrlItem)
	{
		return (int)::SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM)pTabCtrlItem);
	}

	int InsertItem(int nItem, UINT mask, LPCTSTR lpszItem, int iImage, LPARAM lParam)
	{
		TCITEM tci = { 0 };
		tci.mask = mask;
		tci.pszText = (LPTSTR) lpszItem;
		tci.iImage = iImage;
		tci.lParam = lParam;
		return (int)::SendMessage(m_hWnd, TCM_INSERTITEM, nItem, (LPARAM)&tci);
	}
	int SetItem(int nItem,UINT mask,LPCTSTR lpszItem,DWORD dwState,DWORD dwStateMask,int iImage,LPARAM lParam)
	{
		TCITEM tci = {0} ;
		tci.mask = mask;
		tci.pszText = (LPTSTR) lpszItem;
		tci.dwState = dwState;
		tci.dwStateMask = dwStateMask;
		tci.iImage = iImage;
		tci.lParam = lParam;
		return (int)::SendMessage(m_hWnd,TCM_SETITEM,nItem,(LPARAM)&tci);
	}

	int SetItemText(int nItem, LPCTSTR lpszItem)
	{
		return SetItem(nItem,TCIF_TEXT,lpszItem,0,0,0,NULL);	
	}

	int SetCurItemText(LPCTSTR lpszItem)
	{
		return SetItem(GetCurSel(),TCIF_TEXT,lpszItem,0,0,0,NULL);
	}

	BOOL GetItem(int nItem, LPTCITEM pTabCtrlItem) const
	{
		return (BOOL)::SendMessage(m_hWnd, TCM_GETITEM, nItem, (LPARAM)pTabCtrlItem);
	}

	int GetItemCount() const
	{
		return (int)::SendMessage(m_hWnd, TCM_GETITEMCOUNT, 0, 0L);
	}
	LPARAM GetItemParam(int index)
	{
		TCITEM tcItem = {0};
		tcItem.mask = TCIF_PARAM;
		GetItem(index,&tcItem);
		return tcItem.lParam;
	}
	LPARAM GetCurItemParam()
	{
		TCITEM tcItem = {0};
		tcItem.mask = TCIF_PARAM;
		GetItem(GetCurSel(),&tcItem);
		return tcItem.lParam;
	}

	BOOL DeleteItem(int nItem)
	{
		return (BOOL)::SendMessage(m_hWnd, TCM_DELETEITEM, nItem, 0L);
	}

	int HitTest(TC_HITTESTINFO* pHitTestInfo) const
	{
		return (int)::SendMessage(m_hWnd, TCM_HITTEST, 0, (LPARAM)pHitTestInfo);
	}
};

/*************/
/* CListView */
/*************/

class __declspec(novtable) CListView:public CWnd
{
public:
	CListView(){}
	virtual ~CListView(){}

	CListView& operator=(HWND hWnd){m_hWnd=hWnd;return *this;}

	HWND GetHeader(){return (HWND)::SendMessage(m_hWnd,LVM_GETHEADER,0,0);}
	int GetColumnCount(){return (int)::SendMessage(GetHeader(),HDM_GETITEMCOUNT,0,0);}
	//int GetItemCount(){return (int)::SendMessage(GetHeader(),HDM_GETITEMCOUNT,0,0);}

	int GetColumnWidth(int nColumn){return (int)::SendMessage(m_hWnd,LVM_GETCOLUMNWIDTH,nColumn,0);}
	BOOL SetColumnWidth(int nColumn,int cx){return (BOOL)::SendMessage(m_hWnd,LVM_SETCOLUMNWIDTH,nColumn,MAKELPARAM(cx, 0));}
	BOOL SetColumn(int nColumn,LVCOLUMN* pColumn){return(BOOL)::SendMessage(m_hWnd,LVM_SETCOLUMN,nColumn,(LPARAM)pColumn);}
	BOOL GetColumn(int nColumn,LVCOLUMN* pColumn){return(BOOL)::SendMessage(m_hWnd,LVM_GETCOLUMN,nColumn,(LPARAM)pColumn);}
	tstring GetColumnText(int nColumn)
	{
		tstring strText;
		LVCOLUMN lvc = {0};
		lvc.mask = LVCF_TEXT;
		lvc.cchTextMax=256;
		lvc.pszText = (LPTSTR)GetBuffer(strText,256);
		GetColumn(nColumn,&lvc);
		ReleaseBuffer(strText);
		return strText;
	}
	BOOL SetColumnText(int nColumn,LPCTSTR lpszText)
	{
		LVCOLUMN lvc = {0};
		lvc.mask = LVCF_TEXT;
		lvc.pszText = (LPTSTR)lpszText;
		return SetColumn(nColumn,&lvc);
	}
	BOOL SetColumn(int nColumn,LPCTSTR lpszColumnHeading,int nFormat, int nWidth, int nSubItem)
	{
		LVCOLUMN lvc = { 0 };
		lvc.mask = LVCF_TEXT|LVCF_FMT;
		lvc.pszText = (LPTSTR)lpszColumnHeading;
		lvc.fmt = nFormat;
		if (nWidth != -1)
		{
			lvc.mask |= LVCF_WIDTH;
			lvc.cx = nWidth;
		}

		if (nSubItem != -1)
		{
			lvc.mask |= LVCF_SUBITEM;
			lvc.iSubItem = nSubItem;
		}
		int nRet=SetColumn(nColumn, &lvc);
		return nRet;
	
	}
	BOOL SetAllColumnWidth(int cx)
	{
		std::vector<int> vnOrder=GetColumnOrderVector();
		BOOL bRet=TRUE;
		for each(const int& i in vnOrder){
			bRet*=SetColumnWidth(i,cx);	
		}
		return bRet;
	}
	int InsertColumnImpl(int nColumn,LVCOLUMN* pLvColumn){return (int)::SendMessage(m_hWnd,LVM_INSERTCOLUMN,nColumn,(LPARAM)pLvColumn);}
	int InsertColumnImpl(int nColumn, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
	{
		LVCOLUMN lvc = { 0 };
		lvc.mask = LVCF_TEXT|LVCF_FMT;
		lvc.pszText = (LPTSTR)lpszColumnHeading;
		lvc.fmt = nFormat;
		if (nWidth != -1)
		{
			lvc.mask |= LVCF_WIDTH;
			lvc.cx = nWidth;
		}

		if (nSubItem != -1)
		{
			lvc.mask |= LVCF_SUBITEM;
			lvc.iSubItem = nSubItem;
		}
		int nRet=InsertColumnImpl(nColumn, &lvc);
		return nRet;
	}

	DWORD SetExtendedListViewStyle(DWORD dwExStyle,DWORD dwExMask){return (DWORD)::SendMessage(m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,dwExMask,dwExStyle);}
	BOOL SetItemCountEx(int nItems,DWORD dwFlags){return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMCOUNT,nItems,dwFlags);}
	BOOL GetSubItemRect(int nItem,int nSubItem,int nFlag,LPRECT lpRect)
	{
		lpRect->top = nSubItem;
		lpRect->left = nFlag;
		return (BOOL)::SendMessage(m_hWnd,LVM_GETSUBITEMRECT,nItem,(LPARAM)lpRect);
	}
	BOOL GetItemRect(int nItem,LPRECT lpRect,UINT nCode) const
	{
		lpRect->left = nCode;
		return (BOOL)::SendMessage(m_hWnd,LVM_GETITEMRECT,(WPARAM)nItem,(LPARAM)lpRect);
	}
	int GetItemText(int nItem,int nSubItem,LPTSTR lpszText,int nLen)
	{
		LVITEM lvi={0};
		lvi.iSubItem=nSubItem;
		lvi.cchTextMax=nLen;
		lvi.pszText=lpszText;
		return (int)::SendMessage(m_hWnd,LVM_GETITEMTEXT,(WPARAM)nItem,(LPARAM)&lvi);
	}
	BOOL GetColumnOrderArray(int nCount, int* lpnArray){return (BOOL)::SendMessage(m_hWnd,LVM_GETCOLUMNORDERARRAY,nCount,(LPARAM)lpnArray);}
	BOOL SetColumnOrderArray(int nCount, int* lpnArray){return (BOOL)::SendMessage(m_hWnd,LVM_SETCOLUMNORDERARRAY,nCount,(LPARAM)lpnArray);}
	BOOL SetColumnOrderVector(const std::vector<int>& vnOrder)
	{
		if(!vnOrder.empty()){
			return SetColumnOrderArray(vnOrder.size(),(int*)&vnOrder[0]);
		}else{
			return FALSE;
		}
	}
	std::vector<int> GetColumnOrderVector()
	{
		std::vector<int> vnOrder;
		int nColumnCount=GetColumnCount();
		if(nColumnCount==0){return vnOrder;};
		vnOrder.resize(nColumnCount,0);
		GetColumnOrderArray(nColumnCount,&vnOrder[0]);
		return vnOrder;
	}

	int HitTest(LVHITTESTINFO* pHitTestInfo){return (int)::SendMessage(m_hWnd,LVM_HITTEST,0,(LPARAM)pHitTestInfo);}
	int SubItemHitTest(LVHITTESTINFO* pHitTestInfo){return (int)::SendMessage(m_hWnd,LVM_SUBITEMHITTEST,0,(LPARAM)pHitTestInfo);}
	std::pair<int,int> GetItemSubItemPairUnderPt(const POINT& pt)
	{
		LVHITTESTINFO lvhti={0};
		lvhti.pt=pt;
		SubItemHitTest(&lvhti);
		return std::make_pair(lvhti.iItem,lvhti.iSubItem);
	}
	HIMAGELIST GetImageList(int nImageListType){return (HIMAGELIST)::SendMessage(m_hWnd,LVM_GETIMAGELIST,(WPARAM)nImageListType,0L);}

	HIMAGELIST SetImageList(HIMAGELIST hImageList,int nImageListType){return (HIMAGELIST)::SendMessage(m_hWnd,LVM_SETIMAGELIST,(WPARAM)nImageListType, (LPARAM)hImageList);}
	BOOL GetItem(LPLVITEM pItem){return (BOOL)::SendMessage(m_hWnd,LVM_GETITEM,0,(LPARAM)pItem);}
	BOOL DeleteColumn(int nSubItem){return (BOOL)::SendMessage(m_hWnd,LVM_DELETECOLUMN,nSubItem,0L);}
	BOOL SetItemState(int nItem,UINT uState,UINT uStateMask)
	{
		LVITEM lvi={0};
		lvi.state=uState;
		lvi.stateMask=uStateMask;
		return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMSTATE,nItem,(LPARAM)&lvi);
	}
	int GetNextItem(int nItem,int nFlags){return (int)::SendMessage(m_hWnd,LVM_GETNEXTITEM,nItem,MAKELPARAM(nFlags,0));}
	BOOL Scroll(LONG cx,LONG cy){return (BOOL)::SendMessage(m_hWnd,LVM_SCROLL,cx,cy);}
};
/*********/
/* CEdit */
/*********/

class __declspec(novtable) CEdit:public CWnd
{
public:
	CEdit()
	{
		m_rca.lpszClassName(WC_EDIT);
		m_cwa.lpszClassName(WC_EDIT);
	}

	CEdit(HWND hWnd):CWnd(hWnd){}

	virtual ~CEdit(){}

	BOOL GetModify(){return (BOOL)::SendMessage(m_hWnd,EM_GETMODIFY,0,0);}
	void SetSel(int nStartChar,int nEndChar,BOOL bNoScroll = FALSE)
	{
		SendMessage(EM_SETSEL,nStartChar,nEndChar);
		if(!bNoScroll){
			SendMessage(EM_SCROLLCARET,0,0L);
		}
	}
	CRect GetRect(){
		CRect rcRect;
		if((BOOL)SendMessage(EM_GETRECT,0,(LPARAM)&rcRect)){
			return rcRect;
		}else{
			return CRect();
		}
	}

	void SetRect(const CRect& rc)
	{
		SendMessage(EM_SETRECT,0,(LPARAM)(LPCRECT)rc);
	}

	DWORD SetMargins(UINT uLeft, UINT uRight)
	{
		return (DWORD)SendMessage(EM_SETMARGINS,EC_LEFTMARGIN|EC_RIGHTMARGIN,MAKELONG(uLeft, uRight));
	}
};

/***************/
/* CHeaderCtrl */
/***************/
const int HD_FILTERTEXTSIZE=128;


#ifndef HEADER_SORT_TYPE
#define HEADER_SORT_TYPE
enum class HeaderSorts{
	None,
	Up,
	Down
};
#endif

class __declspec(novtable) CHeaderCtrl:public CWnd
{
public:
	CHeaderCtrl()
	{
		m_rca.lpszClassName(WC_HEADER);
		m_cwa.lpszClassName(WC_HEADER);
	}
	virtual ~CHeaderCtrl(){}

	CHeaderCtrl& operator=(HWND hWnd){m_hWnd=hWnd;return *this;}
	int HitTest(LPHDHITTESTINFO pHitTestInfo){return (int)::SendMessage(m_hWnd,HDM_HITTEST,0,(LPARAM)pHitTestInfo);}
	BOOL SetItem(int nItem,	const LPHDITEM phdi){return (BOOL)SendMessage(HDM_SETITEM,(WPARAM)(nItem),(LPARAM)(phdi));}
	BOOL SetColumnWidth(int nItem,int nWidth)
	{
		HDITEM hdi={0};
		hdi.mask=HDI_WIDTH;
		hdi.cxy=nWidth;
		return (BOOL)SendMessage(HDM_SETITEM,(WPARAM)(nItem),(LPARAM)(&hdi));
	}

	std::basic_string<TCHAR> GetItemText(int nItem){
		HDITEM hdi={0};
		std::basic_string<TCHAR> strText;
		hdi.cchTextMax=256;
		hdi.mask=HDI_TEXT;
		hdi.pszText=GetBuffer(strText,256);
		SendMessage(HDM_GETITEM,(WPARAM)(nItem),(LPARAM)(&hdi));
		ReleaseBuffer(strText);
		return strText;
	}
	BOOL SetItemText(int nItem,	LPCTSTR lpszText){
		HDITEM hdi={0};
		hdi.cchTextMax=_tcslen(lpszText);
		hdi.mask=HDI_TEXT;
		hdi.pszText=(LPTSTR)lpszText;
		return (BOOL)SendMessage(HDM_SETITEM,(WPARAM)(nItem),(LPARAM)(&hdi));
	}


	BOOL GetItem(int nItem, const LPHDITEM phdi){return (BOOL)SendMessage(HDM_GETITEM,(WPARAM)(nItem),(LPARAM)(phdi));}
	BOOL SetSortMark(int nItem,HeaderSorts sort){
		HDITEM hdi={0};
		hdi.mask = HDI_FORMAT;
		GetItem(nItem, &hdi);
		hdi.mask = HDI_FORMAT;
		hdi.fmt &= ~HDF_SORTDOWN & ~HDF_SORTUP;
		switch(sort){
		case HeaderSorts::None:
				//Do nothing
				break;
		case HeaderSorts::Up:
				hdi.fmt |= HDF_SORTUP|HDF_STRING;
				break;
		case HeaderSorts::Down:
				hdi.fmt |= HDF_SORTDOWN|HDF_STRING;
				break;
			default:
				break;
		}
		return SetItem(nItem, &hdi);	
	
	}
	int GetItemCount(){return (int)SendMessage(HDM_GETITEMCOUNT,0,0L);}	
	BOOL GetItemRect(int nItem,LPRECT lprc){return (BOOL)SendMessage(HDM_GETITEMRECT,(WPARAM)(nItem),(LPARAM)(lprc));}
	std::basic_string<TCHAR> GetFilterString(int nColumn)
	{
		HDITEM hdItem={0};
		HDTEXTFILTER hdTextFilter={0};
		tstring strFilter;
		hdItem.mask=HDI_FILTER;
		hdItem.type=HDFT_ISSTRING;
		hdItem.pvFilter=&hdTextFilter;
		hdTextFilter.pszText=GetBuffer(strFilter,HD_FILTERTEXTSIZE);
		hdTextFilter.cchTextMax=HD_FILTERTEXTSIZE;

		GetItem(nColumn,&hdItem);
		ReleaseBuffer(strFilter);

		return strFilter;
	}
	BOOL SetFilterString(int nColumn,tstring strFilter)
	{
		HDITEM hdItem={0};
		HDTEXTFILTER hdTextFilter={0};
		hdItem.mask=HDI_FILTER;
		hdItem.type=HDFT_ISSTRING;
		hdItem.pvFilter=&hdTextFilter;
		hdTextFilter.pszText=&strFilter[0];
		hdTextFilter.cchTextMax=strFilter.length();

		return SetItem(nColumn,&hdItem);
	}
	std::vector<std::basic_string<TCHAR>> GetFilterStringVector()
	{
		int nItemCount = GetItemCount();
		HDITEM hdItem={0};
		HDTEXTFILTER hdTextFilter={0};
		TCHAR szFilter[HD_FILTERTEXTSIZE]={0};
		hdItem.mask=HDI_FILTER;
		hdItem.type=HDFT_ISSTRING;
		hdItem.pvFilter=&hdTextFilter;
		hdTextFilter.pszText=szFilter;
		hdTextFilter.cchTextMax=HD_FILTERTEXTSIZE;

		std::vector<std::basic_string<TCHAR>> vstrFilter(nItemCount);
		for(int i=0;i<nItemCount;++i){
			::ZeroMemory(szFilter,sizeof(szFilter));
			GetItem(i,&hdItem);
			if(::_tcslen(szFilter)){
				vstrFilter[i]=std::wstring(szFilter);
			}else{
				vstrFilter[i]=std::wstring();			
			}
		}
		return vstrFilter;
	}
	int SetFilterChangeTimeout(DWORD dwTimeout){return (int)SendMessage(HDM_SETFILTERCHANGETIMEOUT,0,500);}
	BOOL Layout(HDLAYOUT* phdl){return (BOOL)::SendMessage(m_hWnd,HDM_LAYOUT,0,(LPARAM)phdl);}
	int InsertItem(int nIndex,LPHDITEM phdi){return (int)::SendMessage(m_hWnd,HDM_INSERTITEM,nIndex,(LPARAM)phdi);}
	BOOL DeleteItem(int nIndex){return (BOOL)::SendMessage(m_hWnd,HDM_DELETEITEM,nIndex,0L);}


};

/****************/
/* CToolTipCtrl */
/****************/
class __declspec(novtable) CToolTipCtrl:public CWnd
{
public:
	CToolTipCtrl::CToolTipCtrl()
	{
		m_rca
		.lpszClassName(TOOLTIPS_CLASS);

		m_cwa
		.lpszClassName(TOOLTIPS_CLASS)
		.lpszWindowName(TOOLTIPS_CLASS) 
		.dwStyle(TTS_ALWAYSTIP|TTS_NOPREFIX);
	}

	virtual ~CToolTipCtrl(){}

	BOOL AddTool(LPTOOLINFO lpTI){
		return (BOOL)SendMessage(TTM_ADDTOOL,0,(LPARAM)lpTI);	
	}
	BOOL DelTool(LPTOOLINFO lpTI){
		return (BOOL)SendMessage(TTM_DELTOOL,0,(LPARAM)lpTI);	
	}

	void SetDelayTime(DWORD dwType,int nTime){
		SendMessage(TTM_SETDELAYTIME,dwType,MAKELPARAM(nTime,0));	
	}

	int SetMaxTipWidth(int nWidth){
		return (int)SendMessage(TTM_SETMAXTIPWIDTH,0,(LPARAM)nWidth);
	}
	long GetToolCount(){
		return (long)SendMessage(TTM_GETTOOLCOUNT,0,0);
	}
};
typedef std::tr1::shared_ptr<CToolTipCtrl> ToolTipCtrlPtr;
typedef std::tr1::shared_ptr<CToolTipCtrl> ToolTipCtrlWeakPtr;


/************/
/* CListBox */
/************/
class __declspec(novtable) CListBox:public CWnd
{
public:
	CListBox(){}
	virtual ~CListBox(){}
	CListBox& operator=(HWND hWnd){m_hWnd=hWnd;return *this;}
	int GetHorizontalExtent(){return (int)::SendMessage(m_hWnd, LB_GETHORIZONTALEXTENT,0, 0);}
	void SetHorizontalExtent(int nExtent){::SendMessage(m_hWnd, LB_SETHORIZONTALEXTENT,(WPARAM)nExtent, 0);}
	int GetText(int nIndex, LPTSTR lpszBuffer){return (int)::SendMessage(m_hWnd,LB_GETTEXT,nIndex,(LPARAM)lpszBuffer);}
	int GetCount(){return (int)::SendMessage(m_hWnd,LB_GETCOUNT,0,0);}
	int GetCurSel(){return (int)::SendMessage(m_hWnd,LB_GETCURSEL,0,0);}
	int GetSel(int nIndex){return (int)::SendMessage(m_hWnd,LB_GETSEL,nIndex,0);}
	int SetCurSel(int nSelect){return (int)::SendMessage(m_hWnd,LB_SETCURSEL,nSelect,0);}
	int InsertString(int nIndex, LPCTSTR lpszItem){return (int)::SendMessage(m_hWnd,LB_INSERTSTRING,nIndex,(LPARAM)lpszItem);} 
	int AddString(LPCTSTR lpszItem){return (int)::SendMessage(m_hWnd,LB_ADDSTRING,0,(LPARAM)lpszItem);}
	int DeleteString(int nIndex){return (int)::SendMessage(m_hWnd,LB_DELETESTRING,nIndex,0);}
	void DeleteAllString(){
		while(GetCount()!=0){
			DeleteString(0);
		}
		SetHorizontalExtent(0);
	}
	BOOL AddStringVector(std::vector<tstring>& vstr)
	{
		 if(vstr.empty())return FALSE;
		HDC hDCList=GetDC();							   
		::SelectObject(hDCList,(HFONT)GetFont());
		int nMaxSize=GetHorizontalExtent();
		int nSize=0;
		SIZE size;

		for(std::vector<tstring>::iterator iter=vstr.begin();iter!=vstr.end();++iter){
			AddString(iter->c_str());
			::GetTextExtentPoint32(hDCList, iter->c_str(), iter->length()+1, &size);
			nSize=size.cx;
			nMaxSize=(nMaxSize>nSize)?nMaxSize:nSize;					
		}

		SetHorizontalExtent(nMaxSize+1);	
		ReleaseDC(hDCList);

		return TRUE;
	}

	BOOL AddSingleString(LPCTSTR lpszItem)
	{
		std::vector<tstring> vstr;
		vstr.push_back(tstring(lpszItem));
		return AddStringVector(vstr);
	}

	BOOL AddMultiFileName(LPCTSTR lpszTitle,LPCTSTR lpszFilter,LPCTSTR lpszDefExt)
	{ 
		tstring strTmp;
		GetText(0,GetBuffer(strTmp,MAX_PATH));
		ReleaseBuffer(strTmp);
		
		std::vector<tstring> vstrFilePath=GetOpenFileName(
			GetOpenFileNameArg()
			.Flags(OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_HIDEREADONLY)
			.hwndOwner(m_hWnd)
			.lpstrTitle(lpszTitle)
			.lpstrFilter(lpszFilter)
			.lpstrDefExt(lpszDefExt));

		if(vstrFilePath.empty()){return FALSE;}
		AddStringVector(vstrFilePath);

		return TRUE;
	}

	BOOL AddSingleFileName(LPCTSTR lpszTitle,LPCTSTR lpszFilter,LPCTSTR lpszDefExt)
	{ 
		tstring strTmp;
		GetText(0,GetBuffer(strTmp,MAX_PATH));
		ReleaseBuffer(strTmp);
		
		std::vector<tstring> vstrFilePath=GetOpenFileName(GetOpenFileNameArg()
			.Flags(/*OFN_ALLOWMULTISELECT|*/OFN_EXPLORER|OFN_HIDEREADONLY)
			.hwndOwner(m_hWnd)
			.lpstrTitle(lpszTitle)
			.lpstrFilter(lpszFilter)
			.lpstrDefExt(lpszDefExt));

		if(vstrFilePath.empty() || vstrFilePath.size()!=1){return FALSE;}
		DeleteAllString();
		return AddStringVector(vstrFilePath);
	}
/*
	BOOL AddFolderName(LPCTSTR lpszTitle=NULL)
	{
		String strFolder=GetOpenFolderName(m_hWnd,lpszTitle);
		
		if(strFolder.empty()){return FALSE;}
		DeleteAllString();
		return AddSingleString(strFolder.c_str());	
	}
*/
	std::vector<tstring> GetStringVector()
	{
		int nCount=GetCount();
		std::vector<tstring> vstrRet;

		for(int i=0;i<nCount;i++){
			tstring strTmp;
			GetText(i,GetBuffer(strTmp,MAX_PATH));
			ReleaseBuffer(strTmp);
			vstrRet.push_back(strTmp);
		}
		return vstrRet;
	}
};


/**************/
/* CScrollBar */
/**************/
class __declspec(novtable) CScrollBar:public CWnd
{
public:
	CScrollBar()
	{
		m_rca.lpszClassName(_T("SCROLLBAR"));
		m_cwa.lpszClassName(_T("SCROLLBAR"));
	}
	virtual ~CScrollBar(){}

	int SetScrollPos(int nPos,BOOL bRedraw=TRUE){return ::SetScrollPos(m_hWnd,SB_CTL,nPos,bRedraw);}
	int GetScrollPos()const{return ::GetScrollPos(m_hWnd,SB_CTL);}
	int SetScrollInfo(LPSCROLLINFO lpScrollInfo,BOOL bRedraw=TRUE){return ::SetScrollInfo(m_hWnd,SB_CTL,lpScrollInfo, bRedraw);}
	BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo)const{return ::GetScrollInfo(m_hWnd,SB_CTL,lpScrollInfo);}
	int SetScrollPage(int nPage,BOOL bRedraw=TRUE)
	{
		SCROLLINFO si={0};
		si.cbSize=sizeof(SCROLLINFO);
		si.fMask=SIF_PAGE;
		si.nPage=nPage;
		return SetScrollInfo(&si,bRedraw);		
	}
	BOOL SetScrollRange(int nMinPos,int nMaxPos,BOOL bRedraw=TRUE){return ::SetScrollRange(m_hWnd,SB_CTL,nMinPos,nMaxPos,bRedraw);}
};

/****************/
/* CProgressBar */
/****************/
class __declspec(novtable) CProgressBar:public CWnd
{
public:
	CProgressBar()
	{
		m_rca.lpszClassName(PROGRESS_CLASS);
		m_cwa.lpszClassName(PROGRESS_CLASS);
	}
	CProgressBar(HWND hWnd):CWnd(hWnd){}
	virtual ~CProgressBar(){}

	DWORD SetRange(int nLower, int nUpper)
	{
		return (DWORD)::SendMessage(m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(nLower, nUpper));
	}

	std::pair<int, int> GetRange(int& nLower, int& nUpper) const
	{
		PBRANGE range = { 0 };
		::SendMessage(m_hWnd, PBM_GETRANGE, TRUE, (LPARAM)&range);
		return std::make_pair(range.iLow, range.iHigh);
	}

	int SetPos(int nPos)
	{
		return (int)(short)LOWORD(::SendMessage(m_hWnd, PBM_SETPOS, nPos, 0L));
	}

	int OffsetPos(int nPos)
	{
		return (int)(short)LOWORD(::SendMessage(m_hWnd, PBM_DELTAPOS, nPos, 0L));
	}

	int SetStep(int nStep)
	{
		return (int)(short)LOWORD(::SendMessage(m_hWnd, PBM_SETSTEP, nStep, 0L));
	}

	UINT GetPos() const
	{
		return (UINT)::SendMessage(m_hWnd, PBM_GETPOS, 0, 0L);
	}

	int GetStep() const
	{
		return (int)::SendMessage(m_hWnd, PBM_GETSTEP, 0, 0L);
	}

	int GetState() const
	{
		return (int)::SendMessage(m_hWnd, PBM_GETSTATE, 0, 0L);
	}

	int SetState(int nState)
	{
		return (int)::SendMessage(m_hWnd, PBM_SETSTATE, nState, 0L);
	}

	int StepIt()
	{
		return (int)(short)LOWORD(::SendMessage(m_hWnd, PBM_STEPIT, 0, 0L));
	}
};

/****************/
/* CToolBarCtrl */
/****************/
class __declspec(novtable) CToolBarCtrl:public CWnd
{
public:
	CToolBarCtrl()
	{
		m_rca.lpszClassName(TOOLBARCLASSNAME);
		m_cwa.lpszClassName(TOOLBARCLASSNAME);
	}
	CToolBarCtrl(HWND hWnd):CWnd(hWnd){}
	virtual ~CToolBarCtrl(){}
};