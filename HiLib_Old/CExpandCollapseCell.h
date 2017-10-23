#pragma once
#include "Cell.h"

class CExpandCollapseCell:public CCell
{
private:
	bool m_isExpanded;
public:
	CExpandCollapseCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty)
		:CCell(pSheet, pRow, pColumn, spProperty), m_isExpanded(true){}
	virtual ~CExpandCollapseCell(){}
};