#pragma once
#include "IDoCommand.h"
//#include "MyPoint.h"
#include "Cursorer.h"


class CGridView;

class CDoCommandBase:public IDoCommand
{
private:
	std::vector<RC> m_prevFocusedRCs;
	std::vector<RC> m_prevSelectedRCs;
	std::vector<std::shared_ptr<CRow>> m_prevSelectedRows;
	std::vector<std::shared_ptr<CColumn>> m_prevSelectedColumns;
protected:
	CGridView* m_pGrid;
public:
	CDoCommandBase(CGridView* pGrid);
	virtual ~CDoCommandBase(){}
	virtual void Do()override;
	virtual void UnDo()override;
	virtual void ReDo()override;
};



//class CFullCommand:public CDoCommandBase
//{
//private:
//	std::function<void()> m_action;
//	std::wstring m_prevString;
//	std::wstring m_nextString;
//
//public:
//	CFullCommand(CGridView* pGrid, std::function<void()> action);
//	virtual ~CFullCommand(){}
//	virtual void Do()override;
//	virtual void UnDo()override;
//	virtual void ReDo()override;
//};

//class CColumn;
//class CColumnCommand:public IDoCommand
//{
//private:
//	CColumn* m_pColumn;
//	CGridView* m_pGrid;
//	std::function<void()> m_action;
//	std::shared_ptr<CColumn> m_prevColumn;
//	std::shared_ptr<CColumn> m_nextColumn;
//
//public:
//	CColumnCommand(CGridView* pGrid, CColumn* pColumn, std::function<void()> action);
//	virtual ~CColumnCommand(){}
//	virtual void Do()override;
//	virtual void UnDo()override;
//	virtual void ReDo()override;
//};

class CRowColumn;
class CRow;
class CColumn;

class CColumnsCommand:public CDoCommandBase
{
private:
	std::vector<CColumn*> m_columns;
	std::function<void()> m_action;
	std::vector<std::shared_ptr<CColumn>> m_prevColumns;
	std::vector<std::shared_ptr<CColumn>> m_nextColumns;


public:
	CColumnsCommand(CGridView* pGrid, std::vector<CColumn*> columns, std::function<void()> action);
	CColumnsCommand(CGridView* pGrid, CColumn* pColumn, std::function<void()> action);
	virtual ~CColumnsCommand(){}
	virtual void Do()override;
	virtual void UnDo()override;
	virtual void ReDo()override;
};

class MoveColumnCommand:public CDoCommandBase
{
private:
	int m_prevPos;
	int m_nextPos;

	std::shared_ptr<CColumn> m_spColumn;
public:
	MoveColumnCommand(CGridView* pGrid,int pos, std::shared_ptr<CColumn> spColumn );
	virtual ~MoveColumnCommand(){}
	virtual void Do()override;
	virtual void UnDo()override;
	virtual void ReDo()override;
};

class InsertColumnCommand:public CDoCommandBase
{
private:
	int m_pos;

	std::shared_ptr<CColumn> m_spColumn;
public:
	InsertColumnCommand(CGridView* pGrid,int pos, std::shared_ptr<CColumn> spColumn );
	virtual ~InsertColumnCommand(){}
	virtual void Do()override;
	virtual void UnDo()override;
	virtual void ReDo()override;
};

class EraseColumnCommand:public CDoCommandBase
{
private:
	int m_pos;

	std::shared_ptr<CColumn> m_spColumn;
public:
	EraseColumnCommand(CGridView* pGrid, std::shared_ptr<CColumn> spColumn );
	virtual ~EraseColumnCommand(){}
	virtual void Do()override;
	virtual void UnDo()override;
	virtual void ReDo()override;
};
