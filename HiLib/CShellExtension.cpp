#include "CShellExtension.h"

CShellExtension::CShellExtension()
{
	m_pMalloc=CSingletonMalloc::GetInstance();
	::SHGetDesktopFolder(&m_pshfDesktop);
}
CShellExtension::~CShellExtension()
{
	m_pshfDesktop->Release();
}







