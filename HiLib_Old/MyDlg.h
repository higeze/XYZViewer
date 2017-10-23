#pragma once

#include <shellapi.h>
#include <commctrl.h>
#include <commdlg.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <utility>
#include <map>
#include "MyPrerocessor.h"

#pragma comment(lib, "comctl32.lib")
using namespace std::placeholders; 
namespace phs=std::placeholders;




/********/
/* CDlg */
/********/
template<WORD TIDD>
class CDlg
{
	typedef std::function<INT_PTR(UINT,LPARAM,WPARAM,BOOL&)> FunMsg;
	typedef std::unordered_map<UINT,FunMsg> MsgMap;
	typedef std::function<INT_PTR(WORD,WORD,HWND,BOOL&)> FunCmd;
	typedef std::unordered_map<WORD,FunCmd> CmdIDMap;
	typedef std::function<INT_PTR(int,LPNMHDR,BOOL&)> FunNtfy;
	typedef std::unordered_map<LONG,FunNtfy> NtfyMap;
	typedef std::unordered_map<UINT,FunNtfy> NtfyCdMap;

private:

	//prohibit copy constructor and substitute
	CDlg(const CDlg&){}
	CDlg& operator=(const CDlg&);

	MsgMap m_msgMap;
	CmdIDMap m_cmdidMap;
	NtfyMap m_ntfyMap;
	NtfyCdMap m_ntfyCdMap;


protected:
	virtual void OnFinalMessage(HWND hWnd){}
	static INT_PTR CALLBACK StartDlgProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam)
	{
		switch(uiMsg){
		case WM_INITDIALOG:
		{
			CDlg *pDlg=(CDlg*)lParam;
			if(pDlg){
				::SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG)pDlg);
				pDlg->m_hWnd=hWnd;
				return pDlg->DlgProc(uiMsg,wParam,lParam);
			}		
			break;
		}
		case WM_NCDESTROY:
		{
			CDlg *pDlg=(CDlg*)::GetWindowLongPtr(hWnd,GWLP_USERDATA);
			if(pDlg){
                LRESULT lRes = pDlg->DlgProc(uiMsg, wParam, lParam);
                pDlg->m_hWnd = NULL;
                pDlg->OnFinalMessage(hWnd);
				return lRes;
			}			
		}
		default:
		{
			CDlg *pDlg=(CDlg*)::GetWindowLongPtr(hWnd,GWLP_USERDATA);
			if(pDlg){
				return pDlg->DlgProc(uiMsg,wParam,lParam);
			}
			break;
		}
		}
		return FALSE;
	}

	INT_PTR CALLBACK DlgProc(UINT uiMsg,WPARAM wParam,LPARAM lParam)
	{
		if(m_msgMap.empty()){return FALSE;}
		MsgMap::iterator iter=m_msgMap.find(uiMsg);
		INT_PTR intPtr=0;

		if(iter!=m_msgMap.end()){
			BOOL bHandled=TRUE;
			intPtr=iter->second(uiMsg,wParam,lParam,bHandled);
			if(bHandled==TRUE){return intPtr;}
		}	
		return FALSE;//::DefWindowProc(m_hWnd,uiMsg,wParam,lParam);		
	}

public:
	HWND m_hWnd;
	enum{IDD=TIDD};

	CDlg():m_hWnd(NULL)
	{
		AddMsgHandler(WM_COMMAND,&CDlg::OnCommand,this);
		AddMsgHandler(WM_NOTIFY,&CDlg::OnNotify,this);
	}
	virtual ~CDlg(){}

	INT_PTR OnCommand(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		CmdIDMap::iterator iter=m_cmdidMap.find(LOWORD(wParam));
		INT_PTR intPtr=0;
		if(iter!=m_cmdidMap.end()){
			intPtr=iter->second(HIWORD(wParam),LOWORD(wParam),(HWND)lParam,bHandled);
		}else{
			bHandled=FALSE;
		}
		return intPtr;
	}

	INT_PTR OnNotify(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		NtfyMap::iterator iter=m_ntfyMap.find(MAKELONG(((LPNMHDR)lParam)->idFrom,((LPNMHDR)lParam)->code));
		NtfyCdMap::iterator iterCd=m_ntfyCdMap.find(((LPNMHDR)lParam)->code);
		INT_PTR intPtr=0;
		if(iter!=m_ntfyMap.end()){
			bHandled=TRUE;
			intPtr=iter->second(int(wParam),(LPNMHDR)lParam,bHandled);
		}else if(iterCd!=m_ntfyCdMap.end()){
			bHandled=TRUE;
			intPtr=iterCd->second(int(wParam),(LPNMHDR)lParam,bHandled);
		}else{
			bHandled=FALSE;
		}
		//bHandled=FALSE;
		return intPtr;
	}

	//Modal
	virtual INT_PTR DoModal(HWND hWndParent=::GetActiveWindow())
	{
		return ::DialogBoxParam(NULL,MAKEINTRESOURCE(IDD),hWndParent, StartDlgProc, (LPARAM)this);
	}
	// modeless dialogs
	virtual HWND Create(HWND hWndParent=NULL)
	{
		return ::CreateDialogParam(NULL, MAKEINTRESOURCE(IDD),hWndParent, StartDlgProc, (LPARAM)this);
	}

	template<class U>
	BOOL AddMsgHandler(UINT uMsg,INT_PTR(U::*memberfunc)(UINT,WPARAM,LPARAM,BOOL&),U* that)
	{
		MsgMap::_Pairib pib=m_msgMap.insert(MsgMap::value_type(uMsg,std::bind(memberfunc,that,phs::_1,phs::_2,phs::_3,phs::_4)));
		return pib.second;
	}

	template<class U>
	BOOL AddCmdIDHandler(WORD wID,INT_PTR(U::*memberfunc)(WORD,WORD,HWND,BOOL&),U* that)
	{
		CmdIDMap::_Pairib pib=m_cmdidMap.insert(CmdIDMap::value_type(wID,std::bind(memberfunc,that,phs::_1,phs::_2,phs::_3,phs::_4)));
		return pib.second;
	}

	template<class U>
	BOOL AddNtfyHandler(UINT_PTR upID,UINT uCode,INT_PTR(U::*memberfunc)(int,LPNMHDR,BOOL&),U* that)
	{
		NtfyMap::_Pairib pib=m_ntfyMap.insert(NtfyMap::value_type(MAKELONG(upID,uCode),std::bind(memberfunc,that,phs::_1,phs::_2,phs::_3)));	
		return pib.second;
	}
	template<class U>
	BOOL AddNtfyCdHandler(UINT uCode,INT_PTR(U::*memberfunc)(int,LPNMHDR,BOOL&),U* that)
	{
		NtfyCdMap::_Pairib pib=m_ntfyCdMap.insert(NtfyCdMap::value_type(uCode,std::bind(memberfunc,that,phs::_1,phs::_2,phs::_3)));
		return pib.second;
	}

	BOOL EraseNtfyHandler(UINT_PTR upID,UINT uCode)
	{
		m_ntfyMap.erase(MAKELONG(upID,uCode));
		return TRUE;
	}
	BOOL EndDialog(INT_PTR nRetCode){return ::EndDialog(m_hWnd, nRetCode);}
	BOOL DestroyWindow(){return ::DestroyWindow(m_hWnd);}
	BOOL ShowWindow(int nCmdShow){return ::ShowWindow(m_hWnd,nCmdShow);}
	BOOL UpdateWindow(){return ::UpdateWindow(m_hWnd);}
	int MessageBox(LPCWSTR lpText,LPCWSTR lpCaption,UINT uType){return (int)::MessageBox(m_hWnd,lpText,lpCaption,uType);}
	HWND GetParent(){return ::GetParent(m_hWnd);}
	LRESULT SendMessage(UINT Msg,WPARAM wParam,LPARAM lParam){return ::SendMessage(m_hWnd,Msg,wParam,lParam);}
	LRESULT PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam){return ::PostMessage(m_hWnd,Msg,wParam,lParam);}
};
	
class CPropertySheet
{
private:
	PROPSHEETHEADER m_psh;
	std::vector<HPROPSHEETPAGE> m_vHPsp;
public:
	CPropertySheet()
	{
		ZeroMemory(&m_psh,sizeof(m_psh));
		m_psh.dwSize=sizeof(PROPSHEETHEADER);
		m_psh.hInstance=::GetModuleHandle(NULL);

		//m_psh.dwFlags=PSH_NOAPPLYNOW;
	}
	virtual ~CPropertySheet(){}

	INT_PTR DoModal()
	{
		m_psh.phpage=&m_vHPsp[0];
		m_psh.nPages=m_vHPsp.size();
		return ::PropertySheet(&m_psh);
	}
	INT_PTR AddPage(LPCPROPSHEETPAGE pPage)
	{
		HPROPSHEETPAGE hPage=::CreatePropertySheetPage(pPage);
		//if(hPage==NULL){return FALSE;}
		m_vHPsp.push_back(hPage);
		return TRUE;
	}
	NAMED_STRUCT_ARG_METHOD(CPropertySheet,HWND,m_psh,hwndParent)
	NAMED_STRUCT_ARG_METHOD(CPropertySheet,LPCTSTR,m_psh,pszCaption)



};

template<WORD TIDD>
class CPropertyPage:public CDlg<TIDD>
{
private:

	//prohibit copy constructor and substitute
	CPropertyPage<TIDD>(const CPropertyPage<TIDD>&);
	CPropertyPage<TIDD>& operator=(const CPropertyPage<TIDD>&);

	PROPSHEETPAGE m_psp;

protected:
static INT_PTR CALLBACK StartPageProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam)
	{
		CPropertyPage *pPage=(CPropertyPage*)::GetWindowLongPtr(hWnd,GWLP_USERDATA);
		if(pPage){
				return pPage->DlgProc(uiMsg,wParam,lParam);
		}else if(uiMsg==WM_INITDIALOG){
			PROPSHEETPAGE *pPsp=(PROPSHEETPAGE*)lParam;
			CPropertyPage *pPage=(CPropertyPage*)pPsp->lParam;
			if(pPage){
				::SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG)pPage);
				pPage->m_hWnd=hWnd;
				return pPage->DlgProc(uiMsg,wParam,lParam);
			}
		}
		return FALSE;
	}


public:
	HWND m_hWnd;
	enum{IDD=TIDD};
	CPropertyPage()
	{
		ZeroMemory(&m_psp,sizeof(m_psp));
		m_psp.dwSize=sizeof(PROPSHEETPAGE);
		m_psp.hInstance=::GetModuleHandle(NULL);
		m_psp.dwFlags=PSP_DEFAULT;
		m_psp.pszTemplate=MAKEINTRESOURCE(IDD);
		m_psp.pfnDlgProc=(DLGPROC)StartPageProc;
		m_psp.lParam=(LPARAM)this;
	}
	virtual ~CPropertyPage(){}

	operator PROPSHEETPAGE*(){return &m_psp;}

	NAMED_STRUCT_ARG_METHOD(CPropertyPage,HINSTANCE,m_psp,hInstance)
	INT_PTR CheckRadioButton(int nIDFirst,int nIDLast,int nIDCheck){return ::CheckRadioButton(m_hWnd,nIDFirst,nIDLast,nIDCheck);}
	int MessageBox(LPCWSTR lpText,LPCWSTR lpCaption,UINT uType){return (int)::MessageBox(m_hWnd,lpText,lpCaption,uType);}
	HWND GetDlgItem(int nID){return  ::GetDlgItem(m_hWnd,nID);}
};
