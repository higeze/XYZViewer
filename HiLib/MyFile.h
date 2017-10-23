#pragma once

#include "MyString.h"

class CFile
{
private:
	HANDLE m_hFile;
	std::basic_string<TCHAR> m_path;
public:
	CFile();
	CFile(HANDLE hFile);
	CFile(
		LPCTSTR lpFileName,                         
		DWORD dwDesiredAccess,                      
		DWORD dwShareMode,                         
		LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
		DWORD dwCreationDisposition,                
		DWORD dwFlagsAndAttributes,                 
		HANDLE hTemplateFile);

	CFile(const CFile&) = delete;
	CFile& operator=(const CFile&) = delete;

	~CFile();

	BOOL Close();

	std::basic_string<TCHAR> GetPath()const{return m_path;}

	BOOL Remove();

	BOOL WriteFile(                   
		LPCVOID lpBuffer,                
		DWORD nNumberOfBytesToWrite,     
		LPDWORD lpNumberOfBytesWritten,  
		LPOVERLAPPED lpOverlapped);

	template<typename T>
	void WriteAllString(const std::basic_string<T>& str)
	{
		DWORD dwBytes=0;
		if(!::WriteFile(m_hFile,str.c_str(),(DWORD)str.length()*sizeof(T),&dwBytes,NULL)){
			throw std::exception("Error on WriteFile");
		}
		FlushFileBuffers();
	}

	template<typename T>
	std::basic_string<T> ReadAllString()
	{
		std::basic_string<T> str;
		DWORD dwBytes=0;
		DWORD dwSize=::GetFileSize(m_hFile,NULL);
		if(!::ReadFile(m_hFile,GetBuffer(str,dwSize/sizeof(T)),dwSize,&dwBytes,NULL)){
			throw std::exception("Error on ReadFile");
		}
		ReleaseBuffer(str);
		return str;
	}

	template<typename T>
	static void WriteAllString(const std::basic_string<TCHAR>& path, const std::basic_string<T>& str)
	{
		CFile file(
			path.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		file.WriteAllString<T>(str);
	}

	template<typename T>
	static std::basic_string<T> ReadAllString(const std::basic_string<TCHAR>& path)
	{
		CFile file(
			path.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		return file.ReadAllString<T>();
	
	}


	BOOL FlushFileBuffers();
	operator HANDLE() const{return m_hFile;}

};
