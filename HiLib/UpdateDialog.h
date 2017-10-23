#pragma once
#include "MyDlg.h"
#include "MyString.h"
#include "MyFile.h"
#include "MyAutoUpdater.h"
#include "UpdateResource.h"

extern HWND g_hDlgModeless;
extern CAutoUpdater updater;

enum class UpdateState
{
	Start,
	DeleteOldBackup,
	MoveCurrentToBackup,
	PrepareForCopy,
	CopyUpdateToCurrent,
	Finish
};
const UINT WM_UPDATE = RegisterWindowMessage(L"WM_UPDATE");
class CUpdateDialog:public CDlg<IDD_UPDATE>
{

private:
	CAutoUpdater* m_pUpdater;

	bool m_canDeleteThis;
	void OnFinalMessage(HWND hWnd)override
	{
		if(m_canDeleteThis){
			g_hDlgModeless = NULL;
			delete this;
		}		
	}

	void RunUpdate()
	{
		//Updating...
		PostMessage(WM_UPDATE, (WPARAM)(UpdateState::Start), 0);
		//Delete old backup		
		PostMessage(WM_UPDATE, (WPARAM)(UpdateState::DeleteOldBackup), 0);
		std::wstring	wstrBakAppPath=std::regex_replace(m_pUpdater->GetCurrentPath(),std::wregex(REGEX_FILEEXT),std::wstring(L".bak"));
		::DeleteFile(wstrBakAppPath.c_str());

		//Move current to backup
		PostMessage(WM_UPDATE, (WPARAM)(UpdateState::MoveCurrentToBackup), 0);
		if(!::MoveFileEx(m_pUpdater->GetCurrentPath().c_str(),wstrBakAppPath.c_str(),MOVEFILE_COPY_ALLOWED|MOVEFILE_WRITE_THROUGH)){
			return;// false;			
		}
		//Copy update to current
		PostMessage(WM_UPDATE, (WPARAM)(UpdateState::PrepareForCopy), 0);

		if( ::InternetAttemptConnect( 0 ) != ERROR_SUCCESS ){
			throw std::exception("Error on InternetAttemptConnect");
		}

		// Initialization
		auto session = InternetPtr(::InternetOpen( L"Update Program",
									 INTERNET_OPEN_TYPE_PRECONFIG,
									 NULL,
									 NULL,
									 0 ));
		if( session.get() == NULL ){
			throw std::exception("Error on InternetOpen");
		}

		// Connect
		auto connect = InternetPtr(::InternetConnect( session.get(),
										m_pUpdater->GetServerName().c_str(),
										INTERNET_DEFAULT_HTTP_PORT,
										NULL,
										NULL,
										INTERNET_SERVICE_HTTP,
										0,
										NULL ));
		if( connect.get() == NULL ){
			throw std::exception("Error on InternetConnect");
		}

		// HTTP Open
		auto http = InternetPtr(::HttpOpenRequest( connect.get(),
									 L"GET",
									 m_pUpdater->GetObjectName().c_str(),
									 NULL,
									 NULL,
									 NULL,
									 INTERNET_FLAG_RELOAD | INTERNET_FLAG_KEEP_CONNECTION,
									 NULL ));
		if( http.get() == NULL ){
			throw std::exception("Error on HttpOpenRequest");
		}

		// HTTP Request
		if( !( ::HttpSendRequest( http.get(), NULL, 0, NULL, 0 ) ) ){
			throw std::exception("Error on HttpSendRequest");
		}

		DWORD			dwContentLen; //!< TotalLength
		DWORD			dwBufLen = sizeof(dwContentLen);					

		// 
		::HttpQueryInfo( http.get(),
						 HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
						 (LPVOID)&dwContentLen,
						 &dwBufLen,
						 NULL );

		//
		CFile file( m_pUpdater->GetCurrentPath().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS ,
								FILE_ATTRIBUTE_NORMAL, NULL );
		if( (HANDLE)file == INVALID_HANDLE_VALUE ){
			throw std::exception("Error on CreateFile");
		}

		int step = 5;
		DWORD dwRead = static_cast<DWORD>(std::floor((dwContentLen)*step/100));
		DWORD dwByte = 0;
		DWORD dwTransfered = 0;
		DWORD dwWritten = 0;
		std::vector<char> buffer(dwRead);

		PostMessage(WM_UPDATE, (WPARAM)(UpdateState::CopyUpdateToCurrent), 0);

		while( 1 )
		{
			::InternetReadFile( http.get(), &buffer[0], buffer.size(), &dwByte );
			if( dwByte == 0 ) break;

			// ÉtÉ@ÉCÉãÇÃèëÇ´çûÇ›
			file.WriteFile( &buffer[0], dwByte, &dwWritten, NULL );
			dwTransfered += dwByte;

			PostMessage(WM_UPDATE, (WPARAM)(UpdateState::CopyUpdateToCurrent), (LPARAM)(dwTransfered*100/dwContentLen));
		}

		file.FlushFileBuffers();
		PostMessage(WM_UPDATE, (WPARAM)(UpdateState::Finish), (LPARAM)(dwTransfered*100/dwContentLen));
	}
	INT_PTR OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		//Get Icon from parent and Set Icon
		HWND hParent = GetParent();

		HICON hIconBig = (HICON)::SendMessage(hParent, WM_GETICON, ICON_BIG, NULL);
		HICON hIconSmall = (HICON)::SendMessage(hParent, WM_GETICON, ICON_SMALL, NULL);

		SendMessage(WM_SETICON, ICON_BIG, (LPARAM)hIconBig);
		SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);

		CProgressBar progress(::GetDlgItem(m_hWnd, IDC_UPDATE_PROGRESS_BAR));
		progress.SetRange(0, 100);

		//Get Button
		CButton button(::GetDlgItem(m_hWnd, IDOK));
		button.EnableWindow(FALSE);

		boost::thread th(std::bind(&CUpdateDialog::RunUpdate, this));
		return TRUE;
	}

	INT_PTR OnUpdate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		UpdateState state = (UpdateState)wParam;
		int pos = (int)lParam;

		CProgressBar progress(::GetDlgItem(m_hWnd, IDC_UPDATE_PROGRESS_BAR));
		progress.SetPos(pos);

		HWND hPercnetage = ::GetDlgItem(m_hWnd, IDC_UPDATE_PROGRESS_PERCENTAGE);
		::SetWindowText(hPercnetage, (boost::lexical_cast<std::wstring>(pos) + L"%").c_str());

		HWND hComment = ::GetDlgItem(m_hWnd, IDC_UPDATE_PROGRESS_COMMENT);
		std::wstring comment;
		switch(state)
		{
		case UpdateState::Start:
			comment = L"Update start";
			break;
		case UpdateState::DeleteOldBackup:
			comment = L"Deleting old backup binary";
			break;
		case UpdateState::MoveCurrentToBackup:
			comment = L"Moving current binary to backup one";
			break;
		case UpdateState::PrepareForCopy:
			comment = L"Prepare for downloading";
		case UpdateState::CopyUpdateToCurrent:
			comment = L"Downloading update binary to current one";
			break;
		case UpdateState::Finish:
			comment = L"Update finished!\r\n"
					  L"For update, This application will be restarted after you press OK.";
			{
				CButton button(::GetDlgItem(m_hWnd, IDOK));
				button.EnableWindow(TRUE);
			}
			break;
		default:
			break;
		}
		::SetWindowText(hComment, comment.c_str());

		return TRUE;
	}

	INT_PTR OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		DestroyWindow();
		return TRUE;

		//int nRetCode(0);
		//EndDialog(nRetCode);
		//return 0;
	}

	INT_PTR OnCommandOK(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
	{
		HANDLE hMutex=::CreateMutex(0,true,m_pUpdater->GetMutexName().c_str());//Get mutex with owner ship
		if(hMutex==0){
		}else if(::GetLastError()==ERROR_ALREADY_EXISTS){//Already in Restart process
			::WaitForSingleObject(hMutex,INFINITE);//Wait for Previous release mutex and Get mutex	
			::ReleaseMutex(hMutex);//Release mutex
			::CloseHandle(hMutex);
			return 0;
		}

		STARTUPINFO	si = {0};
		PROCESS_INFORMATION	pi = {0};
		si.cb = sizeof(STARTUPINFO);
		std::wstring wstrCmdLine = L"\"" + m_pUpdater->GetCurrentPath() + L"\" " + m_pUpdater->GetRestartArgument();	
		// Create another copy of process
		::CreateProcess(NULL, (LPTSTR)wstrCmdLine.c_str(), NULL, NULL, false, 0, NULL, NULL, &si, &pi);
		// Close
		::PostMessage(m_pUpdater->GetHWnd(),WM_CLOSE,NULL,NULL);
		return OnClose(WM_CLOSE,NULL,NULL,bHandled);
	}

public:
	CUpdateDialog(CAutoUpdater* pUpdater):CDlg<IDD_UPDATE>(),m_pUpdater(pUpdater),m_canDeleteThis(true)
	{
		AddMsgHandler(WM_INITDIALOG,&CUpdateDialog::OnInitDialog,this);
		AddMsgHandler(WM_UPDATE,&CUpdateDialog::OnUpdate,this);
		AddMsgHandler(WM_CLOSE,&CUpdateDialog::OnClose,this);
		AddCmdIDHandler(IDOK,&CUpdateDialog::OnCommandOK,this);
	}

	virtual ~CUpdateDialog(){}

};