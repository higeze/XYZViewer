#pragma once
#include "ParentMapColumn.h"
#include "ParentColumnNameHeaderCell.h"
#include "FilterCell.h"
#include "BindPaintCell.h"
#include "Sheet.h"
#include "MySize.h"
#include "MyDC.h"

template<typename T>
class CBindPaintColumn:public CParentMapColumn
{
private:
	string_type m_header;
	std::function<void(T,CDC*,CRect)> m_paintFunction;
	std::function<CSize(T,CDC*)> m_measureFunction;
public:
	CBindPaintColumn(CGridView* pGrid, string_type header, std::function<void(T,CDC*,CRect)> paintFunction, std::function<CSize(T,CDC*)> measureFunction)
		:CParentMapColumn(pGrid),m_header(header), m_paintFunction(paintFunction),m_measureFunction(measureFunction){}
	virtual ~CBindPaintColumn(){}

	std::function<void(T,CDC*,CRect)> GetPaintFunction(){return m_paintFunction;}
	std::function<CSize(T,CDC*)> GetMeasureFunction(){return m_measureFunction;}

	cell_type NameHeaderCellTemplate(CRow* pRow, CColumn* pColumn)
	{
		return std::make_shared<CParentColumnHeaderStringCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty(),m_header);
	}

	cell_type FilterCellTemplate(CRow* pRow, CColumn* pColumn)
	{
		return std::make_shared<CFilterCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
	}

	cell_type CellTemplate(CRow* pRow, CColumn* pColumn)
	{
		return std::make_shared<CBindPaintCell<T>>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty());
	}
};