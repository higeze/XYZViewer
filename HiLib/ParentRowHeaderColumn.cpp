#include "ParentRowHeaderColumn.h"
#include "Sheet.h"
#include "ParentHeaderCell.h"
#include "FilterCell.h"
#include "Cell.h"
#include "ParentRowHeaderCell.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CParentRowHeaderColumn);

CParentRowHeaderColumn::cell_type CParentRowHeaderColumn::NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentRowHeaderCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CParentRowHeaderColumn::cell_type CParentRowHeaderColumn::HeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentRowHeaderCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CParentRowHeaderColumn::cell_type CParentRowHeaderColumn::HeaderHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentRowHeaderCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CParentRowHeaderColumn::cell_type CParentRowHeaderColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentRowHeaderCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CParentRowHeaderColumn::cell_type CParentRowHeaderColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentRowHeaderIndexCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}