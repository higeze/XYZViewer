#pragma once

#include "ChildIndexColumn.h"

class CChildRowHeaderColumn:public CChildIndexColumn
{
public:
	CChildRowHeaderColumn(CSheetCell* pSheetCell):CChildIndexColumn(pSheetCell){}
	virtual ~CChildRowHeaderColumn(){}

	virtual cell_type HeaderCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn);
};