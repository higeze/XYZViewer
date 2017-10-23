#include "MyAutoUpdater.h"
#include "MyString.h"
#include "MyInternet.h"
#include "UpdateDialog.h"

extern HWND g_hDlgModeless;

void CAutoUpdater::Init()
{
	//Check restart process or not
	std::wstring wstrCmdLine(::GetCommandLine());
	//Check Restrat argument
	m_bRestartArg=(wstrCmdLine.find(m_wstrRestartArg)!=std::wstring::npos);
	//Wait for previous process
	if(m_bRestartArg){
		HANDLE hMutex=::CreateMutex(0,FALSE,m_wstrMutexName.c_str());//Get Mutex without orner ship
		if(hMutex==NULL){
		}else if(::GetLastError()==ERROR_ALREADY_EXISTS){//Previous process have
			::WaitForSingleObject(hMutex,INFINITE);//Wait for Previous release mutex and Get mutex
			::ReleaseMutex(hMutex);//Release mutex
		}
		if(hMutex)::CloseHandle(hMutex);
		if(!m_funAfterUpdate){m_funAfterUpdate();};
	}
}

void CAutoUpdater::Term()
{
	//Release mutex if I have
	HANDLE hMutex=::CreateMutex(0,FALSE,m_wstrMutexName.c_str());//Get Mutex without orner ship
	if(hMutex==NULL){
	}else if(::GetLastError()==ERROR_ALREADY_EXISTS){
//		DWORD dwWait=::WaitForSingleObject(hMutex,0);//Try to wait mutex to check signal or not
//		if(dwWait==WAIT_TIMEOUT){//I have mutex
			::ReleaseMutex(hMutex);
//			::CloseHandle(hMutex);
//		}
	}
	if(hMutex)::CloseHandle(hMutex);
}

bool CAutoUpdater::CheckConnection()
{
	if( (::InternetAttemptConnect( 0 ) != ERROR_SUCCESS) ||
		(::InternetCheckConnection(GetServerUrl().c_str(),FLAG_ICC_FORCE_CONNECTION, 0) != TRUE)){
		return false;
	}else{
		return true;
	}
}

bool CAutoUpdater::CheckUpdate()
{
	//Get Current Path
	::GetModuleFileName(NULL,GetBuffer(m_currentPath,MAX_PATH),MAX_PATH);
	ReleaseBuffer(m_currentPath);

	//Get Version
	std::vector<UINT> vuCurVersion = GetProductVersion(m_currentPath);
	std::vector<UINT> vuUdtVersion = GetInternetVersion(m_versionUrl);

	//Compare version
	if(CompareVersion(vuCurVersion,vuUdtVersion)>=0){
		return false;
	}else{
		return true;
	}
}

bool CAutoUpdater::PromptUpdate()
{
	//Prompt to user
	std::wstring wstrText;
	std::vector<UINT> vuUdtVersion = GetInternetVersion(m_versionUrl);
	wsprintf(GetBuffer(wstrText,256),L"New version %d.%d.%d.%d is available. Do you like to Update?",
		vuUdtVersion[0],vuUdtVersion[1],vuUdtVersion[2],vuUdtVersion[3]);
	ReleaseBuffer(wstrText);
	if(IDNO==::MessageBox(m_hWnd,wstrText.c_str(),m_wstrCaption.c_str(),MB_YESNO)){
		return false;
	}else{
		return true;
	}
}

bool CAutoUpdater::Update()
{
	try{
	//Confirm if update app is necessary, possible or not.
		if(!CheckConnection())return false;
		if(!CheckUpdate())return false;
		if(!PromptUpdate())return false;

		CUpdateDialog* pDlg = new CUpdateDialog(this);//Sucide dialog
		g_hDlgModeless = pDlg->Create(m_hWnd);
		pDlg->ShowWindow(SW_SHOW);

		return true;
	}catch(...){
		return false;
	}
}

/*
int CAutoUpdater::CompareVersion(std::wstring wstrLHS,std::wstring wstrRHS)//LHS>RHS=1,LHS==RHS=0,LHS<RHS=-1
{
	std::vector<std::wstring> vwstrLHS=wstrLHS|split(L"/\\s*\\.\\s*\/");
	std::vector<std::wstring> vwstrRHS=wstrRHS|split(L"/\\s*\\.\\s*\/");

	for(UINT i=0;i<max(vwstrLHS.size(),vwstrRHS.size());++i){
		UINT uLHS=0;
		UINT uRHS=0;
		if(i<vwstrLHS.size()){
			uLHS=_wtol(vwstrLHS[i].c_str());
		}
		if(i<vwstrRHS.size()){
			uRHS=_wtol(vwstrRHS[i].c_str());
		}
		if(uLHS==uRHS){
			continue;
		}else if(uLHS>uRHS){
			return 1;
		}else{
			return -1;
		}
	}
	return 0;
}
*/
int CAutoUpdater::CompareVersion(std::vector<UINT> vuLHS,std::vector<UINT> vuRHS)//LHS>RHS=1,LHS==RHS=0,LHS<RHS=-1
{
	for(UINT i=0;i<max(vuLHS.size(),vuRHS.size());++i){
		UINT uLHS=0;
		UINT uRHS=0;
		if(i<vuLHS.size()){
			uLHS=vuLHS[i];
		}
		if(i<vuRHS.size()){
			uRHS=vuRHS[i];
		}
		if(uLHS==uRHS){
			continue;
		}else if(uLHS>uRHS){
			return 1;
		}else{
			return -1;
		}
	}
	return 0;
}
std::vector<UINT> CAutoUpdater::GetProductVersion(const std::wstring& wstrAppPath)
{
	std::vector<UINT> vuRet;
	DWORD dwHandle = 0;
	DWORD dwVerInfoSize = ::GetFileVersionInfoSize(wstrAppPath.c_str(), &dwHandle);
	if(dwVerInfoSize == 0){
		return vuRet;
	}
	std::vector<BYTE> vbyteBlock(dwVerInfoSize);
	::GetFileVersionInfo(wstrAppPath.c_str(), dwHandle, dwVerInfoSize, &vbyteBlock[0]);

	//unsigned char *pBlock;
	//pBlock = new unsigned char[dwVerInfoSize];
	//if(pBlock==NULL){
	//	return vnRet;
	//}
	//Get Version
	VS_FIXEDFILEINFO *ffi={0};
	UINT uVersionLen;
	::VerQueryValue((LPCVOID)&vbyteBlock[0],L"\\",(LPVOID*) &ffi,&uVersionLen);
	if(uVersionLen == 0){
		return vuRet;
	}

	vuRet.push_back(HIWORD(ffi->dwProductVersionMS));
	vuRet.push_back(LOWORD(ffi->dwProductVersionMS));
	vuRet.push_back(HIWORD(ffi->dwProductVersionLS));
	vuRet.push_back(LOWORD(ffi->dwProductVersionLS));
	return vuRet;
}
/*
std::wstring CAutoUpdater::GetProductVersion(const std::wstring& wstrAppPath)
{
	DWORD dwHandle = 0;
	DWORD dwVerInfoSize = ::GetFileVersionInfoSize(wstrAppPath.c_str(), &dwHandle);
	if(dwVerInfoSize == 0){
		return wstring();
	}

	unsigned char *pBlock;
	pBlock = new unsigned char[dwVerInfoSize];
	if(pBlock==NULL){
		return wstring();
	}
	::GetFileVersionInfo(wstrAppPath.c_str(), dwHandle, dwVerInfoSize, pBlock);
	
	void *pvVersion;
	UINT VersionLen;
	//ProductVersion
	if(::VerQueryValue(pBlock, _T("\\StringFileInfo\\040904b0\\ProductVersion"), &pvVersion, &VersionLen)){
		return wstring((wchar_t*)pvVersion);
	}
	//FileVersion
//	if(VerQueryValue(pBlock, TEXT("\\StringFileInfo\\041104b0\\FileVersion"), &pvVersion, &VersionLen)){
//		printf("FileVersion:%d:%s\n", VersionLen, pvVersion);
//	}
	return wstring();

}
*/