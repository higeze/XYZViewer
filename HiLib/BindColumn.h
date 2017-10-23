#pragma once
#include "ParentMapColumn.h"
#include "ParentRowHeaderColumn.h"
#include "ParentColumnNameHeaderCell.h"
#include "FilterCell.h"
#include "BindCell.h"
#include "Sheet.h"


template<typename T>
class CBindColumn:public CParentDefaultMapColumn
{
private:
	string_type m_header;
	std::function<string_type(T&)> m_getFunction;
	std::function<void(T&,string_type)> m_setFunction;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, unsigned int version)
    {
		ar & boost::serialization::make_nvp("ParentDefaultMapColumn", boost::serialization::base_object<CParentDefaultMapColumn>(*this));
		ar & boost::serialization::make_nvp("header",m_header);
	}
public:
	CBindColumn():CParentDefaultMapColumn(){}
	CBindColumn(CGridView* pGrid, string_type header, std::function<string_type(T&)> getFunction, std::function<void(T&,string_type)> setFunction)
		:CParentDefaultMapColumn(pGrid),m_header(header), m_getFunction(getFunction),m_setFunction(setFunction){}
	virtual ~CBindColumn(){}
	virtual CColumn& ShallowCopy(const CColumn& column)override
	{
		CParentDefaultMapColumn::ShallowCopy(column);
		auto c = dynamic_cast<const CBindColumn<T>&>(column);
		m_header = c.m_header;
		m_getFunction = c.m_getFunction;
		m_setFunction = c.m_setFunction;
		return *this;
	}

	virtual CBindColumn* CloneRaw()const{return new CBindColumn<T>(*this);}
	std::shared_ptr<CBindColumn> Clone()const{return std::shared_ptr<CBindColumn<T>>(CloneRaw());}

	std::function<string_type(T&)> GetGetFunction(){return m_getFunction;}
	std::function<void(T&,string_type)> GetSetFunction(){return m_setFunction;}

	cell_type NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
	{
		return std::make_shared<CParentColumnHeaderStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty(),m_header);
	}

	cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn)
	{
		return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
	}

	cell_type CellTemplate(CRow* pRow, CColumn* pColumn)
	{
		return std::make_shared<CBindCell<T>>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty());
	}
};

template<typename T>
class CBindRowHeaderColumn:public CParentRowHeaderColumn
{
private:
	std::function<bool(T&)> m_getCheckedFunction;
public:
	CBindRowHeaderColumn(CGridView* pGrid, std::function<bool(T&)> getCheckedFunction)
		:CParentRowHeaderColumn(pGrid),m_getCheckedFunction(getCheckedFunction){}
	virtual ~CBindRowHeaderColumn(){}

	std::function<bool(T&)> GetGetCheckedFunction(){return m_getCheckedFunction;}

	cell_type CellTemplate(CRow* pRow, CColumn* pColumn)
	{
		return std::make_shared<CBindRowHeaderCell<T>>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
	}
};