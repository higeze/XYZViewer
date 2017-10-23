#pragma once

#include "MyGdiObj.h"

class CRgn:private boost::noncopyable,public CGdiObj<HRGN>
{
public:
	CRgn(){};
	virtual ~CRgn(){}
	HRGN CreateRectRgnIndirect(LPCRECT lpRect)
	{
		Attach(::CreateRectRgnIndirect(lpRect));
		return m_hGdiObj;
	}
	int CombineRgn(HRGN hRgnSrc, int nCombineMode)
	{
		return ::CombineRgn(m_hGdiObj, m_hGdiObj, hRgnSrc, nCombineMode);
	}
};