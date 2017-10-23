#pragma once
#include "MyGdiObj.h"
#include "MyColor.h"
#include "MyFriendSerializer.h"

class CBrush:/*private boost::noncopyable,*/public CGdiObj<HBRUSH>
{
private:
	CColor m_crColor;
public:
	CBrush(){}
	virtual ~CBrush(){}

	CBrush(const CBrush& br):m_crColor(br.m_crColor)
	{
		Create();
	}
	CBrush& operator=(const CBrush& br)
	{
		m_crColor = br.m_crColor;
		Create();
		return *this;
	}
	CBrush(COLORREF crColor):m_crColor(crColor)
	{
		Create();
	}
	CBrush(CColor crColor):m_crColor(crColor)
	{
		Create();
	}
	COLORREF GetColor()const{return m_crColor;}
	HBRUSH Create()
	{
		Attach(::CreateSolidBrush(m_crColor));
		return m_hGdiObj;
	}
	HBRUSH CreateSolidBrush(CColor cr)
	{
		m_crColor=cr;
		Attach(::CreateSolidBrush(m_crColor));
		return m_hGdiObj;
	}

	FRIEND_SERIALIZER
	template <class Archive>
    void save(Archive& ar)
    {
		ar("Color",m_crColor);
    }
	template <class Archive>
    void load(Archive& ar)
    {
		ar("Color",m_crColor);
		Create();
    }

};

typedef std::shared_ptr<CBrush> SPBrush;