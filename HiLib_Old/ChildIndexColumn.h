#pragma once
#include "Column.h"

class CChildIndexColumn:public CChildColumn
{
private:
	std::unordered_map<CRow*,cell_type> m_mapCell;
public:
	CChildIndexColumn(CSheetCell* pSheet):CChildColumn(pSheet){}
	virtual ~CChildIndexColumn(){}
	virtual cell_type& Cell(CRow* pRow);
	virtual size_type GetDataSize()const;
	virtual cell_type HeaderCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn);
};