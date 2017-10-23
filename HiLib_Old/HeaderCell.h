#pragma once
#include "Cell.h"

class CHeaderCell:public CCell
{
public:
	CHeaderCell(CSheet* pSheet,std::shared_ptr<CCellProperty> spProperty, size_type rowData, size_type colData):CCell(pSheet,spProperty,rowData,colData){}
	virtual ~CHeaderCell(){}
};