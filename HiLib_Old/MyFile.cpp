#include "MyFile.h"
#include "MyWin32.h"

CFile::CFile():m_hFile(NULL),m_path(){}
CFile::CFile(HANDLE hFile):m_hFile(hFile),m_path(){}
CFile::CFile(
	LPCTSTR lpFileName,                         
	DWORD dwDesiredAccess,                      
	DWORD dwShareMode,                         
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
	DWORD dwCreationDisposition,                
	DWORD dwFlagsAndAttributes,                 
	HANDLE hTemplateFile)
	:m_hFile(NULL),m_path(lpFileName)
{
	m_hFile = ::CreateFile(
					lpFileName,
					dwDesiredAccess,
					dwShareMode,
					lpSecurityAttributes,
					dwCreationDisposition,
					dwFlagsAndAttributes,
					hTemplateFile);
	if(m_hFile == INVALID_HANDLE_VALUE){
		throw std::exception(("Error on CreateFile : " + GetLastErrorString()).c_str());
	}
}

CFile::~CFile()
{
	Close();	
}

BOOL CFile::Close()
{
	if(m_hFile && m_hFile!=INVALID_HANDLE_VALUE){
		::CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	return TRUE;
}

BOOL CFile::Remove()
{
	BOOL bRet = Close();
	return bRet && ::DeleteFile(m_path.c_str());
}

BOOL CFile::WriteFile(                   
	LPCVOID lpBuffer,                
	DWORD nNumberOfBytesToWrite,     
	LPDWORD lpNumberOfBytesWritten,  
	LPOVERLAPPED lpOverlapped)
{
	return ::WriteFile(
					m_hFile,
					lpBuffer,
					nNumberOfBytesToWrite,
					lpNumberOfBytesWritten,
					lpOverlapped);
}

BOOL CFile::FlushFileBuffers()
{
	return ::FlushFileBuffers(m_hFile);
}