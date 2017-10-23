#include "ChildIndexColumn.h"
#include "Row.h"
#include "Sheet.h"
#include "ChildColumnHeaderCell.h"
#include "TextCell.h"
#include "EmptyCell.h"

CChildIndexColumn::cell_type& CChildIndexColumn::Cell(CRow* pRow )
{
	auto iter=m_mapCell.find(pRow);
	if(iter!=m_mapCell.end()){
		return iter->second;	
	}else{
		if(pRow==m_pSheet->GetHeaderRowPtr().get()){
			return m_mapCell.insert(std::make_pair(pRow,HeaderCellTemplate(pRow,this))).first->second;
		}else{
			return m_mapCell.insert(std::make_pair(pRow,CellTemplate(pRow,this))).first->second;
		}
	}
}

CChildIndexColumn::size_type CChildIndexColumn::GetDataSize()const
{
	throw std::exception("CIndexColumn:NotImplemented");
}

CChildIndexColumn::cell_type CChildIndexColumn::HeaderCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CChildColumnHeaderCell>(m_pSheet,pRow,pColumn,m_pSheet->GetHeaderProperty());
}

CChildIndexColumn::cell_type CChildIndexColumn::FilterCellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CEmptyCell>(m_pSheet,pRow,pColumn,m_pSheet->GetFilterProperty());
}

CChildIndexColumn::cell_type CChildIndexColumn::CellTemplate(CRow* pRow, CColumn* pColumn)
{
	return std::make_shared<CTextCell>(m_pSheet,pRow,pColumn,m_pSheet->GetCellProperty());
}

//void CChildIndexColumn::UpdateData(size_type rowData)
//{
//	//m_pHeader=HeaderCellTemplate(m_pSheet->GetNameHeaderRowPtr().get(),this);
//	//m_vpCell.resize(rowData);
//	//for(auto iter=m_vpCell.begin(),end=m_vpCell.end();iter!=end;++iter){
//	//	if(iter->get()==nullptr){
//	//		(*iter)=CellTemplate(m_pSheet->VisibleRow(iter-m_vpCell.begin()).get(),this);
//	//	}
//	//}
//}


