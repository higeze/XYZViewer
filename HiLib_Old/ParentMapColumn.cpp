#include "ParentMapColumn.h"
#include "Sheet.h"
#include "GridView.h"
#include "ParentColumnNameHeaderCell.h"
#include "ParentColumnHeaderCell.h"
#include "ParentColumnHeaderHeaderCell.h"

#include "FilterCell.h"
#include "Cell.h"
#include "Row.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CParentMapColumn);

CParentMapColumn::cell_type& CParentMapColumn::Cell(CRow* pRow )
{
	auto iter=m_mapCell.find(pRow);
	if(iter!=m_mapCell.end()){
		return iter->second;	
	}else{
		auto pGrid = static_cast<CGridView*>(m_pSheet);
		if(pRow==pGrid->GetHeaderRowPtr().get()){
			return m_mapCell.insert(std::make_pair(pRow,HeaderCellTemplate(pRow,this))).first->second;
		}else if(pRow==pGrid->GetNameHeaderRowPtr().get()){
			return m_mapCell.insert(std::make_pair(pRow,NameHeaderCellTemplate(pRow,this))).first->second;
		}else if(pRow==pGrid->GetHeaderHeaderRowPtr().get()){
			return m_mapCell.insert(std::make_pair(pRow,HeaderHeaderCellTemplate(pRow,this))).first->second;
		}else if(pRow==pGrid->GetFilterRowPtr().get()){
			return m_mapCell.insert(std::make_pair(pRow,FilterCellTemplate(pRow,this))).first->second;		
		}else{
			return m_mapCell.insert(std::make_pair(pRow,CellTemplate(pRow,this))).first->second;
		}
	}
}

CParentMapColumn::cell_type CParentMapColumn::HeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentColumnHeaderIndexCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CParentMapColumn::cell_type CParentMapColumn::HeaderHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentColumnHeaderHeaderCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CParentMapColumn::cell_type CParentMapColumn::NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentColumnNameHeaderCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CParentMapColumn::cell_type CParentMapColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
}

CParentMapColumn::cell_type CParentMapColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CTextCell>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty());
}

BOOST_CLASS_EXPORT_IMPLEMENT(CParentDefaultMapColumn);

CParentDefaultMapColumn::cell_type CParentDefaultMapColumn::HeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CParentDefaultColumnHeaderIndexCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}