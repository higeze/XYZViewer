#pragma once

#include "CellProperty.h"

class CGridViewProperty
{
public:
	std::shared_ptr<CBackgroundProperty> m_spBackgroundProperty;
	std::shared_ptr<CHeaderProperty> m_spPropHeader; 
	std::shared_ptr<CCellProperty> m_spPropCell; 
	std::shared_ptr<int> m_spDeltaScroll;

	CGridViewProperty()
		:m_spPropHeader(std::make_shared<CHeaderProperty>()),
		m_spBackgroundProperty(std::make_shared<CBackgroundProperty>()),
		m_spPropCell(std::make_shared<CCellProperty>()),
		m_spDeltaScroll(std::make_shared<int>(30))
	{
	
	}

	FRIEND_SERIALIZER

    template <class Archive>
    void serialize(Archive& ar)
    {
		ar("BackgroundProperty",m_spBackgroundProperty);
		ar("HeadersProperty",m_spPropHeader);
		ar("CellsProperty",m_spPropCell);
		ar("DeltaScroll",m_spDeltaScroll);
    }
};

