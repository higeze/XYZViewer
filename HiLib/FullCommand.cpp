#include "FullCommand.h"
#include "GridView.h"
#include "Cell.h"
#include "Row.h"
#include "Column.h"
#include "Cursorer.h"

CDoCommandBase::CDoCommandBase(CGridView* pGrid):m_pGrid(pGrid){}

void CDoCommandBase::Do()//Do have to call first
{
	auto cursorer = m_pGrid->GetCursorerPtr();
	m_prevFocusedRCs = cursorer->GetFocusedRCs(m_pGrid);
	m_prevSelectedRCs = cursorer->GetSelectedRCs(m_pGrid);
	m_prevSelectedRows = cursorer->GetSelectedRows(m_pGrid);
	m_prevSelectedColumns = cursorer->GetSelectedColumns(m_pGrid);
}
void CDoCommandBase::UnDo()//UnDo have to call last
{
	auto cursorer = m_pGrid->GetCursorerPtr();
	cursorer->OnCursorClear(m_pGrid, EventArgs());
	cursorer->SetFocusedRCs(m_pGrid, m_prevFocusedRCs);
	cursorer->SetSelectedRCs(m_pGrid, m_prevSelectedRCs);
	cursorer->SetSelectedRows(m_pGrid, m_prevSelectedRows);
	cursorer->SetSelectedColumns(m_pGrid, m_prevSelectedColumns);
	if(!m_prevFocusedRCs.empty() && m_prevFocusedRCs.size()>0){
		m_pGrid->EnsureVisibleCell(m_pGrid->AllCell(m_prevFocusedRCs[0].Row, m_prevFocusedRCs[0].Column));
	}
}
void CDoCommandBase::ReDo()//UnDo have to call last
{
	auto cursorer = m_pGrid->GetCursorerPtr();
	cursorer->OnCursorClear(m_pGrid, EventArgs());
	cursorer->SetFocusedRCs(m_pGrid, m_prevFocusedRCs);
	cursorer->SetSelectedRCs(m_pGrid, m_prevSelectedRCs);
	cursorer->SetSelectedRows(m_pGrid, m_prevSelectedRows);
	cursorer->SetSelectedColumns(m_pGrid, m_prevSelectedColumns);
	if(!m_prevFocusedRCs.empty() && m_prevFocusedRCs.size()>0){
		m_pGrid->EnsureVisibleCell(m_pGrid->AllCell(m_prevFocusedRCs[0].Row, m_prevFocusedRCs[0].Column));
	}
}

//CFullCommand::CFullCommand(CGridView* pGrid, std::function<void()> action)
//	:CDoCommandBase(pGrid), m_action(action){}
//
//void CFullCommand::Do()
//{
//	m_prevString = m_pGrid->FullXMLSave();
//
//	m_action();
//}
//void CFullCommand::UnDo()
//{
//	m_pGrid->FullXMLLoad(m_prevString);
//}
//void CFullCommand::ReDo()
//{
//	m_action();
//}
//
//CColumnCommand::CColumnCommand(CGridView* pGrid,CColumn* pColumn, std::function<void()> action)
//	:m_pColumn(pColumn),m_pGrid(pGrid), m_action(action){}
//
//void CColumnCommand::Do()
//{
//	m_prevColumn = m_pColumn->Clone();
//
//	m_action();
//
//	m_nextColumn = m_pColumn->Clone();
//}
//void CColumnCommand::UnDo()
//{
//	m_pColumn->ShallowCopy(*m_prevColumn);
//}
//void CColumnCommand::ReDo()
//{
//	m_pColumn->ShallowCopy(*m_nextColumn);
//}

CColumnsCommand::CColumnsCommand(CGridView* pGrid,std::vector<CColumn*> columns, std::function<void()> action)
	:CDoCommandBase(pGrid), m_columns(columns), m_action(action){}
CColumnsCommand::CColumnsCommand(CGridView* pGrid, CColumn* pColumn, std::function<void()> action)
	:CDoCommandBase(pGrid), m_columns(), m_action(action){m_columns.push_back(pColumn);}
void CColumnsCommand::Do()
{
	for(auto pColumn : m_columns){
		m_prevColumns.push_back(pColumn->Clone());
	}
	CDoCommandBase::Do();
	m_action();

	for(auto pColumn : m_columns){
		m_nextColumns.push_back(pColumn->Clone());
	}
}
void CColumnsCommand::UnDo()
{
	for(size_t i=0;i<m_prevColumns.size();i++){
		m_columns[i]->ShallowCopy(*m_prevColumns[i]);
	}
	CDoCommandBase::UnDo();
}
void CColumnsCommand::ReDo()
{
	for(size_t i=0;i<m_nextColumns.size();i++){
		m_columns[i]->ShallowCopy(*m_nextColumns[i]);
	}
	CDoCommandBase::ReDo();
}

MoveColumnCommand::MoveColumnCommand(CGridView* pGrid,int pos, std::shared_ptr<CColumn> spColumn )
	:CDoCommandBase(pGrid), 
	m_nextPos(pos), 
	m_prevPos(spColumn->GetAllIndex()), 
	m_spColumn(spColumn){}

void MoveColumnCommand::Do()
{
	CDoCommandBase::Do();
	m_pGrid->MoveColumnImpl(m_nextPos, m_spColumn);
}
void MoveColumnCommand::UnDo()
{
	auto pos = (m_prevPos<m_nextPos)?m_prevPos:m_prevPos+1;
	m_pGrid->MoveColumnImpl(pos, m_spColumn);
	CDoCommandBase::UnDo();
}
void MoveColumnCommand::ReDo()
{
	m_pGrid->MoveColumnImpl(m_nextPos, m_spColumn);
	CDoCommandBase::ReDo();
}

InsertColumnCommand::InsertColumnCommand(CGridView* pGrid,int pos, std::shared_ptr<CColumn> spColumn )
	:CDoCommandBase(pGrid), 
	m_pos(pos), 
	m_spColumn(spColumn){}

void InsertColumnCommand::Do()
{
	CDoCommandBase::Do();
	m_pGrid->InsertColumnImpl(m_pos, m_spColumn);
}
void InsertColumnCommand::UnDo()
{
	m_pGrid->EraseColumnImpl(m_spColumn);
	CDoCommandBase::UnDo();
}
void InsertColumnCommand::ReDo()
{
	m_pGrid->InsertColumnImpl(m_pos, m_spColumn);
	CDoCommandBase::ReDo();
}

EraseColumnCommand::EraseColumnCommand(CGridView* pGrid, std::shared_ptr<CColumn> spColumn )
	:CDoCommandBase(pGrid), 
	m_pos(spColumn->GetAllIndex()), 
	m_spColumn(spColumn){}

void EraseColumnCommand::Do()
{
	CDoCommandBase::Do();
	m_pGrid->EraseColumnImpl(m_spColumn);
}
void EraseColumnCommand::UnDo()
{
	m_pGrid->InsertColumnImpl(m_pos, m_spColumn);
	CDoCommandBase::UnDo();
}
void EraseColumnCommand::ReDo()
{
	m_pGrid->EraseColumnImpl(m_spColumn);
	CDoCommandBase::ReDo();
}