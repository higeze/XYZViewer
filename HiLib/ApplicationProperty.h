#pragma once

#include "MyRect.h"
#include "MyFriendSerializer.h"

class CApplicationProperty
{
public:
	bool m_bOnlyOneInstance;
	bool m_bCheckUpdateOnCreate;
	bool m_bDebug;

	CApplicationProperty():
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
    }
};