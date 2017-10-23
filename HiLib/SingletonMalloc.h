#pragma once

class CSingletonMalloc
{
private:
	static LPMALLOC m_pMalloc;
	static CRITICAL_SECTION m_cs;

	CSingletonMalloc(){};
	CSingletonMalloc(const CSingletonMalloc&);
	CSingletonMalloc& operator=(const CSingletonMalloc&);

	static void ReleaseInstance();

public:
	static LPMALLOC GetInstance();

};
