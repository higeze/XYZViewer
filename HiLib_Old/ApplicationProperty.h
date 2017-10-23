#pragma once

#include "MyRect.h"
#include "MyFriendSerializer.h"

class CApplicationProperty
{
public:
	CRect m_rcWnd;
	bool m_bOnlyOneInstance;
	bool m_bCheckUpdateOnCreate;
	bool m_bDebug;

	CApplicationProperty()
		:m_rcWnd(0,0,300,500),
		m_bOnlyOneInstance(false),
		m_bCheckUpdateOnCreate(true),
		m_bDebug(false){}
	virtual ~CApplicationProperty(){}

	FRIEND_SERIALIZER
    template <class Archive>
    void serialize(Archive& ar)
    {
		ar("OnlyOneInstance",m_bOnlyOneInstance);
		ar("CheckUpdateOnCreate",m_bCheckUpdateOnCreate);
		ar("Debug",m_bDebug);
        const type_info& info = typeid(ar);
		if(info == typeid(CSerializer&) || info == typeid(CDeserializer&)){ 
			ar("WindowRectangle", m_rcWnd);
		}
    }
};