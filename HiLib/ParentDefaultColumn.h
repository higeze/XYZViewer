#pragma once
#include "ParentMapColumn.h"

class CParentDefaultColumn:public CParentMapColumn
{
protected:
	std::shared_ptr<bool> m_spVisible;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("ParentMapColumn", boost::serialization::base_object<CParentMapColumn>(*this));
	}
public:
	CParentDefaultColumn(CGridView* pGrid = nullptr, std::shared_ptr<bool> spVisible = nullptr):
		CParentMapColumn(pGrid), m_spVisible(spVisible){}
	virtual ~CParentDefaultColumn(){}

	bool GetVisible()const override
	{
		return *m_spVisible;
	}
	void SetVisible(const bool& bVisible, bool notify = true)override;

};

BOOST_CLASS_EXPORT_KEY(CParentDefaultColumn);