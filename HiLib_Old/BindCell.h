#pragma once

#include "TextCell.h"
#include "BindRow.h"
#include "BindColumn.h"
#include "CellProperty.h"
#include "Column.h"
#include "Row.h"
#include "ParentRowHeaderCell.h"

template<typename T>
class CBindCell:public CTextCell
{
private:
public:
	CBindCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu=nullptr)
		:CTextCell(pSheet, pRow, pColumn, spProperty,pMenu){}
	virtual ~CBindCell(){}
	virtual bool IsComparable()const{return false;}

	virtual string_type GetString()const
	{
		auto pBindRow = static_cast<CBindRow<T>*>(m_pRow);
		auto pBindColumn = static_cast<CBindColumn<T>*>(m_pColumn);
		auto t = pBindRow->GetDataContext();
		return pBindColumn->GetGetFunction()(t);
	}

	virtual void SetStringCore(const string_type& str)
	{
		auto pBindRow = static_cast<CBindRow<T>*>(m_pRow);
		auto pBindColumn = static_cast<CBindColumn<T>*>(m_pColumn);
		auto t = pBindRow->GetDataContext();
		pBindColumn->GetSetFunction()(t, str);	
	}
};

template<typename T>
class CBindRowHeaderCell:public CParentRowHeaderCell
{
private:
public:
	CBindRowHeaderCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty)
		:CParentRowHeaderCell(pSheet, pRow, pColumn, spProperty){}

	virtual bool GetChecked()const
	{
		auto pBindRow = static_cast<CBindRow<T>*>(m_pRow);
		auto pBindColumn = static_cast<CBindRowHeaderColumn<T>*>(m_pColumn);
		auto t = pBindRow->GetDataContext();
		return pBindColumn->GetGetCheckedFunction()(t);		
	}


};