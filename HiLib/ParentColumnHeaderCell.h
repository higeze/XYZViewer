#pragma once

#include "ParentHeaderCell.h"

class CParentColumnHeaderCell:public CParentHeaderCell
{
public:
	static CMenu ContextMenu;
public:
	CParentColumnHeaderCell(CSheet* pSheet,CRow* pRow,CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu = &CParentColumnHeaderCell::ContextMenu)
		:CParentHeaderCell(pSheet,pRow,pColumn,spProperty,pMenu?pMenu:&CParentColumnHeaderCell::ContextMenu){}
	virtual ~CParentColumnHeaderCell(){}
	virtual string_type GetString()const;
	virtual string_type GetSortString()const;
	virtual bool Filter(const string_type& strFilter)const;

};

class CParentColumnHeaderIndexCell:public CParentColumnHeaderCell
{
public:
	CParentColumnHeaderIndexCell(CSheet* pSheet,CRow* pRow,CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu = nullptr)
		:CParentColumnHeaderCell(pSheet,pRow,pColumn,spProperty,pMenu){}
	virtual ~CParentColumnHeaderIndexCell(){}
	virtual string_type GetString()const;
};

class CParentDefaultColumnHeaderIndexCell:public CParentColumnHeaderIndexCell
{
public:
	static CMenu ContextMenu;
public:
	CParentDefaultColumnHeaderIndexCell(CSheet* pSheet,CRow* pRow,CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu = &CParentDefaultColumnHeaderIndexCell::ContextMenu)
		:CParentColumnHeaderIndexCell(pSheet,pRow,pColumn,spProperty,pMenu?pMenu:&CParentDefaultColumnHeaderIndexCell::ContextMenu){}
	virtual ~CParentDefaultColumnHeaderIndexCell(){}
};