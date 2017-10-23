#pragma once

#include "ParentHeaderCell.h"

class CParentRowHeaderCell:public CParentHeaderCell
{
public:
	static CMenu ContextMenu;
public:
	CParentRowHeaderCell(CSheet* pSheet,CRow* pRow,CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu = &CParentRowHeaderCell::ContextMenu)
		:CParentHeaderCell(pSheet,pRow,pColumn,spProperty,pMenu?pMenu:&CParentRowHeaderCell::ContextMenu){}
	virtual ~CParentRowHeaderCell(){}
	virtual string_type GetString()const;
	virtual string_type GetSortString()const;
	virtual bool Filter(const string_type& strFilter)const;
};

class CParentRowHeaderIndexCell:public CParentRowHeaderCell
{
public:
	CParentRowHeaderIndexCell(CSheet* pSheet,CRow* pRow,CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu = nullptr)
		:CParentRowHeaderCell(pSheet,pRow,pColumn,spProperty,pMenu){}
	virtual ~CParentRowHeaderIndexCell(){}
	virtual string_type GetString()const;
};