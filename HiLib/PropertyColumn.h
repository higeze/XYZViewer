#include "ParentMapColumn.h"
#include "ChildIndexColumn.h"

class CParentPropertyNameColumn:public CParentMapColumn
{
public:
	CParentPropertyNameColumn(CGridView* pGrid);
	virtual ~CParentPropertyNameColumn(){}
	virtual cell_type NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn);
};

class CChildPropertyNameColumn:public CChildIndexColumn
{
public:
	CChildPropertyNameColumn(CSheetCell* pSheetCell);
	virtual ~CChildPropertyNameColumn(){}
	virtual cell_type HeaderCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn);
};
#include "ChildRowHeaderColumn.h"
class CChildPropertyIndexColumn:public CChildRowHeaderColumn
{
public:
	CChildPropertyIndexColumn(CSheetCell* pSheetCell);
	virtual ~CChildPropertyIndexColumn(){}
	virtual cell_type HeaderCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn);
};


class CParentPropertyValueColumn:public CParentMapColumn
{
public:
	CParentPropertyValueColumn(CGridView* pGrid);
	virtual ~CParentPropertyValueColumn(){}
	virtual cell_type NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn);
};

class CChildPropertyValueColumn:public CChildIndexColumn
{
public:
	CChildPropertyValueColumn(CSheetCell* pSheetCell);
	virtual ~CChildPropertyValueColumn(){}
	virtual cell_type HeaderCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn);
	virtual cell_type CellTemplate(CRow* pRow, CColumn* pColumn);
};
