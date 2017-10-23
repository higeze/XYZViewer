#include "OGLWindowRenderer.h"
#include "OGLUIElement.h"
#include "MyFont.h"
#include <math.h>

#ifndef	M_PI
#define	M_PI	3.1415926535897932384626433832795
#endif

PIXELFORMATDESCRIPTOR COGLWindowRenderer::GetPixelFormatDescriptor()const
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	return pfd;
}

void COGLWindowRenderer:: Initialize(CDC* pDC, const CRect& rc)
{
	m_pDC = pDC;
	// OpenGL pixel format ‚ÌÝ’è
	if( !SetupPixelFormat(m_pDC->GetHDC()))
	{
		assert( !"SetupPixelFormat failed" );
		return;
	}

	// Describe pixel format
	PIXELFORMATDESCRIPTOR	pfd;
	int num = ::GetPixelFormat(pDC->GetHDC());
	::DescribePixelFormat(pDC->GetHDC(), num, sizeof(pfd), &pfd );

	//Create GLRC
	m_uhGLRC.reset(::wglCreateContext(m_pDC->GetHDC()));
	if(!m_uhGLRC)
	{
		assert( !"wglCreateContext failed" );
		return;
	}

	//Make current
	CWglMakeCurrenter currenter(m_pDC->GetHDC(), m_uhGLRC.get());

	::glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	::glClearDepth( 1.0f );
	::glEnable( GL_DEPTH_TEST );
	::glDepthFunc( GL_LEQUAL );

	std::vector<std::shared_ptr<COGLFont>> vspOGLFont;
	for(auto& spOGLFont : m_vspOGLFont){
		spOGLFont->Update(m_pDC->GetHDC());
	}
}


void COGLWindowRenderer::Update(CDC* pDC, const CRect& rc)
{
	m_rc = rc;
	if(!m_uhGLRC){
		Initialize(pDC, rc);
	}
}










