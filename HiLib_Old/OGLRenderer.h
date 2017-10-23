#pragma once

#include "OGLHelper.h"
#include "OGLFont.h"
#include "MyDC.h"
#include "MyRect.h"

class COGLRenderer
{
protected:
	UHGLRC m_uhGLRC;
	CRect m_rc;
	std::vector<std::shared_ptr<COGLFont>> m_vspOGLFont;

	virtual bool SetupPixelFormat( HDC hDC);
public:
	//Constructor Destructor
	COGLRenderer();
	virtual ~COGLRenderer(){}
	//Getter Setter
	virtual HDC GetHDC()const = 0;//Pure virtual
	virtual CDC* GetDCPtr()= 0;
	HGLRC GetHGLRC()const{return m_uhGLRC.get();}

	std::shared_ptr<COGLFont> GetArialFont()const{return m_vspOGLFont[0];}

	//Method
	virtual PIXELFORMATDESCRIPTOR GetPixelFormatDescriptor()const = 0;//Pure virtual
	virtual CRect GetRect()const{return m_rc;}
	virtual void Update(CDC* pDC, const CRect& rc) = 0;//Pure virtual

};