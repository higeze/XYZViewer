#pragma once
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

template<class char_type>
class CStopwatch
{
private:
	DWORD m_dwStartTime;
	DWORD m_dwElapsedTime;
public:
	CStopwatch():m_dwStartTime(0),m_dwElapsedTime(0){::timeBeginPeriod(1);}
	virtual ~CStopwatch(){::timeEndPeriod(1);}
	void Start()
	{
		m_dwStartTime=::timeGetTime();
		m_dwElapsedTime=0;
	};
	void Stop()
	{
		if(m_dwStartTime!=0){m_dwElapsedTime=::timeGetTime()-m_dwStartTime;}
	};
	std::basic_string<char_type> GetElapsedMillesecondsString()
	{
		std::basic_string<char_type> strElapsedTime;
		::wsprintf(GetBuffer(strElapsedTime,128),L"%d",m_dwElapsedTime);
		ReleaseBuffer(strElapsedTime);

		return strElapsedTime;
	};
};
