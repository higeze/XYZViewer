#include "OGLBitmapRenderer.h"
#include "MyFont.h"
#include <math.h>

#ifndef	M_PI
#define	M_PI	3.1415926535897932384626433832795
#endif

COGLBitmapRenderer::COGLBitmapRenderer() : COGLRenderer(),m_thread(),m_bitmap(),m_dc(),m_hBitmapOld(NULL)
{
	m_thread = boost::thread(std::bind(&COGLBitmapRenderer::Initialize, this, nullptr, CRect(0, 0, 500, 500)));
}

COGLBitmapRenderer::~COGLBitmapRenderer(void)
{
	if(!m_dc.IsNull() && m_hBitmapOld!=NULL){
		::SelectObject(m_dc.GetHDC(), m_hBitmapOld);
	}
}

PIXELFORMATDESCRIPTOR COGLBitmapRenderer::GetPixelFormatDescriptor()const
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI,
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

void COGLBitmapRenderer:: Initialize(CDC* pDC, const CRect& rc)
{
	CClientDC dc(NULL);

	if(!m_dc.IsNull() && m_hBitmapOld!=NULL){
		::SelectObject(m_dc.GetHDC(), m_hBitmapOld);
	}

	m_dc.CreateCompatibleDC(dc.GetHDC());
	BITMAPINFO bmi;
	DWORD	dwByteSize = rc.Width() * rc.Height();

	int		iClrUsed = 0;
	int		iPalletSize = sizeof(RGBQUAD) * iClrUsed;
	UINT	uiBitmapInfoSize = sizeof(BITMAPINFOHEADER) + iPalletSize;

	::ZeroMemory( &bmi, uiBitmapInfoSize );

	bmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth			= rc.Width();
	#ifdef _TOP_DOWN_
	bmi.bmiHeader.biHeight			= -height;	// Top-down DIB 
	#else
	bmi.bmiHeader.biHeight			=  rc.Height();	// Bottom-up DIB 
	#endif
	bmi.bmiHeader.biPlanes			= 1;
	bmi.bmiHeader.biBitCount		= 24;
	bmi.bmiHeader.biCompression		= BI_RGB;		// An uncompressed format.
	bmi.bmiHeader.biSizeImage		= dwByteSize;
	bmi.bmiHeader.biXPelsPerMeter	= 0;
	bmi.bmiHeader.biYPelsPerMeter	= 0;
	bmi.bmiHeader.biClrUsed			= iClrUsed;
	bmi.bmiHeader.biClrImportant	= 0;

	m_bitmap.CreateDIBSection(m_dc.GetHDC(), &bmi, DIB_RGB_COLORS, NULL, NULL, 0);

	m_hBitmapOld=(HBITMAP)::SelectObject(m_dc.GetHDC(),m_bitmap);


	if( NULL == m_dc.GetHDC() )
	{
		assert( !"m_hDC is NULL" );
		return;
	}

	// OpenGL pixel format ÇÃê›íË
	if( !SetupPixelFormat(m_dc.GetHDC()) )
	{
		assert( !"SetupPixelFormat failed" );
		return;
	}

	// Describe pixel format
	PIXELFORMATDESCRIPTOR	pfd;
	int num = ::GetPixelFormat(m_dc.GetHDC());
	::DescribePixelFormat( m_dc.GetHDC(), num, sizeof(pfd), &pfd );

	//Create GLRC
	m_uhGLRC.reset(::wglCreateContext(m_dc.GetHDC()));
	if(!m_uhGLRC)
	{
		assert( !"wglCreateContext failed" );
		return;
	}

	//Make current
	CWglMakeCurrenter currenter(m_dc.GetHDC(), m_uhGLRC.get());

	::glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	::glClearDepth( 1.0f );
	::glEnable( GL_DEPTH_TEST );
	::glDepthFunc( GL_LEQUAL );
		
	//Update Font
	std::vector<std::shared_ptr<COGLFont>> vspOGLFont;
	for(auto& spOGLFont : m_vspOGLFont){
		spOGLFont->Update(m_dc.GetHDC());
	}
}

void COGLBitmapRenderer::Update(CDC* pDC, const CRect& rc)
{
	m_rc = rc;
	if(m_dc.IsNull() || rc.Width()>m_bitmap.GetSize().cx || rc.Height()>m_bitmap.GetSize().cy){

		m_thread = boost::thread(std::bind(&COGLBitmapRenderer::Initialize, this, pDC, rc));
	}

	if(m_thread.joinable()){
		m_thread.join();
	}

	return;
}





