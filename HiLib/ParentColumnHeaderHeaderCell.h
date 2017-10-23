#pragma once

#include "ParentHeaderCell.h"

class CParentColumnHeaderHeaderCell:public CParentHeaderCell
{
public:
	CParentColumnHeaderHeaderCell(CSheet* pSheet,CRow* pRow,CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu = nullptr)
		:CParentHeaderCell(pSheet,pRow,pColumn,spProperty,pMenu){}
	virtual ~CParentColumnHeaderHeaderCell(){}
	virtual string_type GetString()const;
	virtual string_type GetSortString()const;
	virtual bool Filter(const string_type& strFilter)const;

};