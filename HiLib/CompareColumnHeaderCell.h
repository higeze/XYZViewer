#pragma once
#include "ParentColumnNameHeaderCell.h"

class CCompareColumnHeaderCell:public CParentColumnNameHeaderCell
{
//public:
//	static CMenu ContextMenu;
public:
	CCompareColumnHeaderCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu = nullptr)
		:CParentColumnNameHeaderCell(pSheet,pRow,pColumn,spProperty,pMenu){}
	virtual ~CCompareColumnHeaderCell(){}
	virtual string_type GetString()const{return L"Compare";}
};