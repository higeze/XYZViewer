#pragma once
#include "ChildHeaderCell.h"

class CChildColumnHeaderCell:public CChildHeaderCell
{
public:
	CChildColumnHeaderCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty)
		:CChildHeaderCell(pSheet,pRow, pColumn,spProperty){}
	virtual ~CChildColumnHeaderCell(){}
};

