#pragma once

#include "OGLRenderer.h"
#include "MyRect.h"

class COGLBitmapRenderer:public COGLRenderer
{
protected:
	CDC m_dc;
	CBitmap m_bitmap;
	HBITMAP m_hBitmapOld;

	boost::thread m_thread;


public:
	COGLBitmapRenderer();
	virtual ~COGLBitmapRenderer();
	virtual PIXELFORMATDESCRIPTOR GetPixelFormatDescriptor()const override;
	virtual void Update(CDC* pDC, const CRect& rc) override;
	virtual HDC GetHDC()const override {return m_dc.GetHDC();}
	virtual CDC* GetDCPtr()override{return &m_dc;}
	void Initialize(CDC* pDC, const CRect& rc);
	CSize GetBufferSize()const{return m_bitmap.GetSize();}
};