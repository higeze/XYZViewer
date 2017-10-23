#pragma once

#include "OGLRenderer.h"

class COGLWindowRenderer:public COGLRenderer
{
protected:
	CDC* m_pDC;
	//CRect m_rc;


public:
	COGLWindowRenderer():COGLRenderer(){}
	virtual ~COGLWindowRenderer(){}
	virtual PIXELFORMATDESCRIPTOR GetPixelFormatDescriptor()const override;
	//virtual CRect GetRect()const{return m_rc;}
	virtual void Update(CDC* pDC, const CRect& rc) override;
	virtual HDC GetHDC()const override {return m_pDC->GetHDC();}
	void Initialize(CDC* pDC, const CRect& rc);

};