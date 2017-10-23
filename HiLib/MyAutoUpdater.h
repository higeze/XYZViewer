#pragma once

#include "MyWnd.h"
#include "MyString.h"

#pragma comment(lib, "version.lib")


class CAutoUpdater
{
private:
	std::wstring m_currentPath;
	std::wstring m_backupPath;

	std::wstring m_serverUrl;
	std::wstring m_versionUrl;
	std::wstring m_serverName;
	std::wstring m_objectName;

	std::wstring m_wstrMutexName;
	std::wstring m_wstrRestartArg;

	std::wstring m_wstrCaption;

	BOOL m_bRestartArg;
	BOOL m_bInitMutex;
	BOOL m_bTermMutex;

	HWND m_hWnd;

	std::function<void()> m_funAfterUpdate;
public:
	CAutoUpdater(){}
	virtual ~CAutoUpdater(){}

	std::wstring GetCurrentPath()
	{
		if(m_currentPath.empty()){
			::GetModuleFileName(NULL,GetBuffer(m_currentPath,MAX_PATH),MAX_PATH);
			ReleaseBuffer(m_currentPath);	
		}
		return m_currentPath;
	}

	std::wstring GetBackupPath()
	{
		if(m_backupPath.empty()){
			m_backupPath=std::regex_replace(GetCurrentPath(),std::wregex(REGEX_FILEEXT),std::wstring(L".bak"));
		}
		return m_backupPath;
	}

	HWND GetHWnd()const{return m_hWnd;}
	void SetHWnd(HWND hWnd){m_hWnd = hWnd;}

	void SetServerUrl(const std::wstring& serverUrl){m_serverUrl = serverUrl;}
	void SetVersionUrl(const std::wstring& versionUrl){m_versionUrl = versionUrl;}
	std::wstring GetServerUrl()const{return m_serverUrl;}

	void SetServerName(const std::wstring& serverName){m_serverName = serverName;}
	std::wstring GetServerName()const{return m_serverName;}

	void SetObjectName(const std::wstring& objectName){m_objectName = objectName;}
	std::wstring GetObjectName()const{return m_objectName;}

	void SetMutexName(const std::wstring& mutexName){ m_wstrMutexName = mutexName; }
	std::wstring GetMutexName()const{return m_wstrMutexName;}

	void SetRestartArgument(const std::wstring& restartArgument){ m_wstrRestartArg = restartArgument; }
	std::wstring GetRestartArgument()const{return m_wstrRestartArg;}

	void SetCaption(const std::wstring& caption){ m_wstrCaption = caption; }
	void SetAfterUpdateFunction(const std::function<void()>& fun){ m_funAfterUpdate = fun; }
	
	void Init();
	void Term();
	bool Update();
	bool CheckConnection();
	bool CheckUpdate();
	bool PromptUpdate();
	std::vector<UINT> GetProductVersion(const std::wstring& wstrAppPath);
	int CompareVersion(std::vector<UINT> vnLHS,std::vector<UINT> vnRHS);//LHS>RHS=1,LHS==RHS=0,LHS<RHS=-1
//	std::wstring GetProductVersion(const std::wstring& wstrAppPath);
//	int CompareVersion(std::wstring wstrLHS,std::wstring wstrRHS);//LHS>RHS=1,LHS==RHS=0,LHS<RHS=-1
};
