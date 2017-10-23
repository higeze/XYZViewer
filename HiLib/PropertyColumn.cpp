#include "PropertyColumn.h"
#include "Sheet.h"
#include "ParentColumnHeaderCell.h"
#include "ParentColumnNameHeaderCell.h"
#include "PropertyNameCell.h"
#include "FilterCell.h"
#include "Cell.h"
#include "TextCell.h"
#include "MyString.h"
#include "Row.h"
#include "GridView.h"


CParentPropertyNameColumn::CParentPropertyNameColumn(CGridView* pGrid)
	:CParentMapColumn(pGrid)
{}

CParentPropertyNameColumn::cell_type CParentPropertyNameColumn::NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentColumnHeaderStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty(),L"Property");
}

CParentPropertyNameColumn::cell_type CParentPropertyNameColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
}

CParentPropertyNameColumn::cell_type CParentPropertyNameColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CPropertyNameCell>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty());
}

CChildPropertyNameColumn::CChildPropertyNameColumn(CSheetCell* pSheetCell)
	:CChildIndexColumn(pSheetCell)
{}

CChildPropertyNameColumn::cell_type CChildPropertyNameColumn::HeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty(),L"Property");
}

CChildPropertyNameColumn::cell_type CChildPropertyNameColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
}

CChildPropertyNameColumn::cell_type CChildPropertyNameColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CPropertyNameCell>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty());
}

CChildPropertyIndexColumn::CChildPropertyIndexColumn(CSheetCell* pSheetCell)
	:CChildRowHeaderColumn(pSheetCell)
{}

CChildPropertyIndexColumn::cell_type CChildPropertyIndexColumn::HeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty(),L"X");
}

CChildPropertyIndexColumn::cell_type CChildPropertyIndexColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
}

CChildPropertyIndexColumn::cell_type CChildPropertyIndexColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty(),boost::lexical_cast<std::wstring>(pRow->GetAllIndex()));
}

CParentPropertyValueColumn::CParentPropertyValueColumn(CGridView* pGrid)
	:CParentMapColumn(pGrid)
{}

CParentPropertyValueColumn::cell_type CParentPropertyValueColumn::NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentColumnHeaderStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty(),L"Value");
}

CParentPropertyValueColumn::cell_type CParentPropertyValueColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
}

CParentPropertyValueColumn::cell_type CParentPropertyValueColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CEditableStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty(),L"");
}

CChildPropertyValueColumn::CChildPropertyValueColumn(CSheetCell* pSheetCell)
	:CChildIndexColumn(pSheetCell)
{}

CChildPropertyValueColumn::cell_type CChildPropertyValueColumn::HeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty(),L"Value");
}

CChildPropertyValueColumn::cell_type CChildPropertyValueColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
}

CChildPropertyValueColumn::cell_type CChildPropertyValueColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CEditableStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty(),L"44");
}