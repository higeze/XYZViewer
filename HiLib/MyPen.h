#pragma once

#include "MyGdiObj.h"
#include "MyColor.h"
#include "MyFriendSerializer.h"

class CPen:private boost::noncopyable,public CGdiObj<HPEN>
{
private:
	int m_nPenStyle;
	int m_nWidth;
	CColor m_crColor;
public:
	CPen(){}
	CPen(int nPenStyle,int nWidth,CColor crColor)
		:m_nPenStyle(nPenStyle),m_nWidth(nWidth),m_crColor(crColor)
	{Create();}
	virtual ~CPen(){}
	int GetPenStyle()const{return m_nPenStyle;}
	int GetWidth()const{return m_nWidth;}
	int GetLeftTopPenWidth()const;
	int GetRightBottomPenWidth()const;

	CColor GetColor()const{return m_crColor;}
	HPEN Create()
	{
		Attach(::CreatePen(m_nPenStyle, m_nWidth,m_crColor));
		return m_hGdiObj;
	}
	HPEN CreatePen(int nPenStyle,int nWidth,COLORREF crColor)
	{
		m_nPenStyle=nPenStyle;m_nWidth=nWidth,m_crColor=crColor;
		Create();
		return m_hGdiObj;
	}

	FRIEND_SERIALIZER
    template <class Archive>
    void save(Archive& ar)
    {
		ar("PenStyle", m_nPenStyle);
		ar("Width", m_nWidth);
		ar("Color", m_crColor);
    }
    template <class Archive>
    void load(Archive& ar)
    {
		ar("PenStyle", m_nPenStyle);
		ar("Width", m_nWidth);
		ar("Color", m_crColor);
		Create();
    }

};
typedef std::shared_ptr<CPen> SPPen;