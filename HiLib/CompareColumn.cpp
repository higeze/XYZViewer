#include "CompareColumn.h"
#include "CompareColumnHeaderCell.h"
#include "CompareCell.h"
#include "FilterCell.h"
#include "Sheet.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CCompareColumn);

CCompareColumn::CCompareColumn(CGridView* pGrid)
	:CParentDefaultMapColumn(pGrid){}

CCompareColumn::cell_type CCompareColumn::NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CCompareColumnHeaderCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CCompareColumn::cell_type CCompareColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
}

CCompareColumn::cell_type CCompareColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CCompareCell>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty());
}
