#pragma once
#include "ParentMapColumn.h"

class CCompareColumn:public CParentDefaultMapColumn
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::make_nvp("ParentDefaultMapColumn", boost::serialization::base_object<CParentDefaultMapColumn>(*this));
	}
public:
	CCompareColumn(CGridView* pGrid = nullptr);
	virtual ~CCompareColumn(){}
	virtual CColumn& ShallowCopy(const CColumn& column)override
	{
		CParentDefaultMapColumn::ShallowCopy(column);
		return *this;
	}
	virtual CCompareColumn* CloneRaw()const{return new CCompareColumn(*this);}
	std::shared_ptr<CCompareColumn> Clone()const{return std::shared_ptr<CCompareColumn>(CloneRaw());}

	virtual cell_type NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn);
};

BOOST_CLASS_EXPORT_KEY(CCompareColumn);