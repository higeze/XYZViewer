#pragma once
#include "OGLEventArgs.h"
#include "UIElement.h"

//COGLUIElement
class COGLUIElement:public virtual CUIElement
{
protected:
	COGLUIElement* m_pParent;
public:
	COGLUIElement(COGLUIElement* pParent = nullptr):CUIElement(),m_pParent(pParent){}
	virtual ~COGLUIElement(){}
	virtual void OnPaint(OGLPaintEventArgs& e){}
};



