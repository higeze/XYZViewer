#include "SingletonMalloc.h"

LPMALLOC CSingletonMalloc::GetInstance()
{
//Double checked locking pattern
	if(NULL==m_pMalloc){
		::InitializeCriticalSection(&m_cs);
		::EnterCriticalSection(&m_cs);
		if(NULL==m_pMalloc){
			SHGetMalloc(&m_pMalloc);
			atexit(&ReleaseInstance);
		}
		::LeaveCriticalSection(&m_cs);
	}
	return m_pMalloc;

}

void CSingletonMalloc::ReleaseInstance()
{
	if(m_pMalloc)m_pMalloc->Release();
	::DeleteCriticalSection(&m_cs);
}

CRITICAL_SECTION CSingletonMalloc::m_cs;
LPMALLOC CSingletonMalloc::m_pMalloc=NULL;