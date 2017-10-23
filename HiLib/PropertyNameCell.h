#pragma once

#include "TextCell.h"


class CPropertyNameCell:public CStringCell
{
public:
	CPropertyNameCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty)
		:CStringCell(pSheet,pRow,pColumn,spProperty,L""){}
	virtual ~CPropertyNameCell(){}
	virtual bool IsComparable()const
	{
		return false;
	}
};