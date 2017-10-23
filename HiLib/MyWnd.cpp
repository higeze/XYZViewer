#include "MyWnd.h"
#include "MyString.h"
#include <ShellAPI.h>
#include <Shlwapi.h>


#include "MyWin32.h"

#pragma comment( lib, "shlwapi.lib" )
/*
BOOL CWnd::AddNtfyCdHandler(UINT uCode,FunNtfy funNtfy)
{
	m_ntfyCdMap.insert(NtfyCdMap::value_type(uCode,funNtfy));
	return TRUE;
}
*/



std::vector<std::basic_string<TCHAR>>
	GetOpenFileName(GetOpenFileNameArg arg)
{
	TCHAR szBuff[4096]={0};
	arg.m_ofn.nMaxFile=4096;
	arg.m_ofn.lpstrFile=szBuff;

	std::vector<std::basic_string<TCHAR>> vstrRet;

	if(!::GetOpenFileName(&(arg.m_ofn))){
		return vstrRet;
	}else{
		vstrRet;

		TCHAR szFirstPath[MAX_PATH];
		TCHAR* lpszFirstPath=szFirstPath;
		TCHAR szFilePath[MAX_PATH];
		TCHAR* lpszFilePath=szFilePath;
		TCHAR* lpszBuff=szBuff;

		while(*lpszFirstPath++=*lpszBuff++);

		if(*lpszBuff==NULL){
			vstrRet.push_back(std::basic_string<TCHAR>(szFirstPath));
		}else{
			_tcscat_s(szFirstPath,TEXT("\\"));
			while(*lpszBuff!=NULL){
				_tcscpy_s(szFilePath,szFirstPath);
				_tcscat_s(szFilePath,lpszBuff); 
				vstrRet.push_back(std::basic_string<TCHAR>(szFilePath));
				while(*lpszBuff++);
			}
		}

		return vstrRet;
	}

}

std::wstring GetModuleFileName(HMODULE hModule)
{
	std::wstring wstrPath;
	::GetModuleFileName(hModule,GetBuffer(wstrPath,MAX_PATH),MAX_PATH);
	ReleaseBuffer(wstrPath);
	return wstrPath;
}

std::wstring PathFindDirectory(std::wstring wstrPath)
{
	::PathRemoveFileSpec(&wstrPath[0]);
	ReleaseBuffer(wstrPath);
	return wstrPath;
}

std::wstring GetModuleDirectory(HMODULE hModule)
{
	return PathFindDirectory(GetModuleFileName(hModule));
}

	LRESULT CALLBACK CWnd::DefWndProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam)
	{
		return ::CallWindowProc(m_pfnDefWndProc,hWnd,uiMsg,wParam,lParam);
	}

	LRESULT CALLBACK CWnd::WndProc(UINT uiMsg,WPARAM wParam,LPARAM lParam)
	{
		try{
/*			auto iter=m_msgMap.find(uiMsg);
			LRESULT lResult=0;
			if(iter!=m_msgMap.end()){
				BOOL bHandled=TRUE;
				lResult=iter->second(uiMsg,wParam,lParam,bHandled);
				if(bHandled==TRUE){return lResult;}
			}*/	
			//if(uiMsg == WM_SETFOCUS){
			//	auto a =1.1;

			//}
			LRESULT lResult=0;
			BOOL bHandled = TRUE;
			if (m_allMsg) {
				lResult = m_allMsg(uiMsg, wParam, lParam, bHandled);
				if (bHandled == TRUE) { return lResult; }
			}

			auto ret = m_msgMap.equal_range(uiMsg);
			for(auto iter = ret.first;iter!=ret.second;++iter){
				bHandled=TRUE;
				lResult=iter->second(uiMsg,wParam,lParam,bHandled);
				if(bHandled==TRUE){return lResult;}			
			}

		}catch(std::exception& ex){
			::OutputDebugStringA(ex.what());
			std::string msg =( boost::format(
				"What:%1%\r\n"
				"Last Error:%2%\r\n"
				) % ex.what() % GetLastErrorString()).str();

			MessageBoxA(m_hWnd, msg.c_str(), "Exception in Message Loop", MB_ICONWARNING);
		}catch(...){
			std::string text;
			text+="MSG:" + boost::lexical_cast<std::string>(uiMsg) +"\r\n";
			text+="WPARAM:" + boost::lexical_cast<std::string>(wParam) +"\r\n";
			text+="LPARAM:" + boost::lexical_cast<std::string>(lParam);

			MessageBoxA(m_hWnd, text.c_str(), "Exception in Message Loop", MB_ICONWARNING);
		}
		return DefWndProc(m_hWnd,uiMsg,wParam,lParam);		
	}

	HWND CWnd::Create(HWND hWndParent)
	{
		//Register
		WNDCLASSEX wc={0};
		if(::GetClassInfoEx(::GetModuleHandle(NULL),m_rca.lpszClassName(),&wc)==false){
			m_rca.lpfnWndProc((WNDPROC)CWnd::StartWndProc);
			if (!(::RegisterClassEx(m_rca))){return NULL;}
		}

		HWND hWnd=::CreateWindowEx(
			m_cwa.dwExStyle(), 
			m_cwa.lpszClassName(), 
			m_cwa.lpszWindowName(), 
			m_cwa.dwStyle(), 
			m_cwa.x(),
			m_cwa.y(), 
			m_cwa.nWidth(), 
			m_cwa.nHeight(), 
			(hWndParent!=NULL)?hWndParent:m_cwa.hWndParent(), 
			m_cwa.hMenu(), 
			m_cwa.hInstance(),
			(LPVOID)this                 
		);
		m_hWnd=hWnd;
		if(!hWnd){
			MessageBoxA(m_hWnd, ::GetLastErrorString().c_str(), "Exception", MB_ICONWARNING);
			return NULL;
		}else{
			return m_hWnd;
		}
	}

	HWND CWnd::Create(HWND hWndParent,RECT& rc)
	{
		m_cwa.x(rc.left).y(rc.top).nWidth(rc.right-rc.left).nHeight(rc.bottom-rc.top);

		return CWnd::Create(hWndParent);
	}
	LRESULT CALLBACK CWnd::StartWndProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam)
	{

		switch(uiMsg){
			case WM_NCCREATE:
				{
					CWnd *pWnd=(CWnd*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
					if(pWnd){
						::SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)pWnd);
						pWnd->m_hWnd=hWnd;
						return pWnd->WndProc(uiMsg,wParam,lParam);
					}
					break;
				}
			case WM_NCDESTROY:
				{
					CWnd *pWnd=(CWnd*)::GetWindowLongPtr(hWnd,GWLP_USERDATA);
					if(pWnd){
						LONG_PTR pfnWndProc = ::GetWindowLongPtr(pWnd->m_hWnd, GWLP_WNDPROC);
                        LRESULT lRes = pWnd->WndProc(uiMsg, wParam, lParam);
                        if(pWnd->m_pfnDefWndProc != ::DefWindowProc && ::GetWindowLongPtr(pWnd->m_hWnd, GWLP_WNDPROC) == pfnWndProc){
                                ::SetWindowLongPtr(pWnd->m_hWnd, GWLP_WNDPROC, (LONG_PTR)pWnd->m_pfnDefWndProc);
						}
						::SetWindowLongPtr(hWnd,GWLP_WNDPROC,(LONG_PTR)::DefWindowProc);
                        pWnd->m_hWnd = NULL;
                        pWnd->OnFinalMessage(hWnd);
						return lRes;
					}
					break;
				}

			default:
				{
				CWnd *pWnd=(CWnd*)::GetWindowLongPtr(hWnd,GWLP_USERDATA);
				if(pWnd){
						return pWnd->WndProc(uiMsg,wParam,lParam);
				}
				break;
				}
		}
		return 0;
	}

	BOOL CWnd::SubclassWindow(HWND hWnd)
	{
		WNDPROC pfnWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)StartWndProc);
		if(pfnWndProc == NULL){return FALSE;}
		m_pfnDefWndProc = pfnWndProc;
		::SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)this);
		m_hWnd = hWnd;
		return TRUE;
	}

	LRESULT CWnd::OnCommand(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		auto iter=m_cmdidMap.find(LOWORD(wParam));
		LRESULT lResult=0;
		if(iter!=m_cmdidMap.end()){
			lResult=iter->second(HIWORD(wParam),LOWORD(wParam),(HWND)lParam,bHandled);
		}else{
			bHandled=FALSE;
		}
		
		if(bHandled==FALSE){
			CmdCdMap::iterator iter=m_cmdcdMap.find(HIWORD(wParam));
			LRESULT lResult=0;
			if(iter!=m_cmdcdMap.end()){
				lResult=iter->second(HIWORD(wParam),LOWORD(wParam),(HWND)lParam,bHandled);
			}else{
				bHandled=FALSE;
			}
		}
		return lResult;
	}

	LRESULT CWnd::OnNotify(UINT uiMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		auto iter=m_ntfyMap.find(MAKELONG(((LPNMHDR)lParam)->idFrom,((LPNMHDR)lParam)->code));
	//	NtfyCdMap::iterator iter=m_ntfyCdMap.find(((LPNMHDR)lParam)->code);
		LRESULT lResult=0;
		if(iter!=m_ntfyMap.end()){
			bHandled=TRUE;
			lResult=iter->second(int(wParam),(LPNMHDR)lParam,bHandled);
		}else{
			bHandled=FALSE;
		}
		//bHandled=FALSE;
		return lResult;
	}

	BOOL CWnd::ClearMsgHandler()
	{
		m_msgMap.clear();
		AddMsgHandler(WM_COMMAND,&CWnd::OnCommand,this);
		AddMsgHandler(WM_NOTIFY,&CWnd::OnNotify,this);
		return TRUE;
	}

	BOOL CWnd::ClearCmdIDHandler()
	{
		m_cmdidMap.clear();
		return TRUE;
	}

	BOOL CWnd::EraseMsgHandler(UINT uMsg)
	{
		m_msgMap.erase(uMsg);
		return TRUE;
	}

	BOOL CWnd::EraseCmdIDHandler(WORD wID)
	{
		m_cmdidMap.erase(wID);
		return TRUE;
	}

	BOOL CWnd::EraseNtfyHandler(UINT_PTR upID,UINT uCode)
	{
		m_ntfyMap.erase(MAKELONG(upID,uCode));
		return TRUE;
	}