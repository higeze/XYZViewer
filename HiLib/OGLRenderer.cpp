#include "OGLRenderer.h"

COGLRenderer::COGLRenderer():m_uhGLRC(nullptr),m_rc(),m_vspOGLFont()
{
	m_vspOGLFont.push_back(std::make_shared<COGLFont>(CFont(16,L"Arial"), 1000));	
}

bool COGLRenderer::SetupPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd = GetPixelFormatDescriptor();

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( hDC, &pfd ) ) )
	{
		assert( !"ChoosePixelFormat is failed" );
		return false;
	}

	if( TRUE != ::SetPixelFormat( hDC, iPixelFormat, &pfd ) )
	{
		assert( !"SetPixelFormat is failed" );
		return false;
	}

    return true;
}

