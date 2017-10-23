#pragma once


/**
 *  CellEventArgs
 */
struct CellEventArgs
{
public:
	CCell* CellPtr;
	CellEventArgs(CCell* pCell):CellPtr(pCell){}
	virtual ~CellEventArgs(){}
};
/**
 *  CellEventArgs
 */
struct CellContextMenuEventArgs:public CellEventArgs
{
public:
	CPoint Point;
	CellContextMenuEventArgs(CCell* pCell,CPoint pt)
		:CellEventArgs(pCell),Point(pt){}
	virtual ~CellContextMenuEventArgs(){}

};
/**
 *  CRowEventArgs
 */
class CRowEventArgs
{
public:
	CRow* m_pRow;
	CRowEventArgs(CRow* pRow):m_pRow(pRow){}
	virtual ~CRowEventArgs(){}
};

/**
 *  CRowsEventArgs
 */
class CRowsEventArgs
{
public:
	std::vector<CRow*> m_rows;
	CRowsEventArgs(const std::vector<CRow*> rows):m_rows(rows){}
	virtual ~CRowsEventArgs(){}
};

/**
 *  CColumnEventArgs
 */
class CColumnEventArgs
{
public:
	CColumn* m_pColumn;
	CColumnEventArgs(CColumn* pColumn):m_pColumn(pColumn){}
	virtual ~CColumnEventArgs(){}
};

/**
 *  CColumnMovedEventArgs
 */
class CColumnMovedEventArgs:public CColumnEventArgs
{
public:
	typedef int size_type;
	size_type m_from;
	size_type m_to;
public:
	CColumnMovedEventArgs(CColumn* pColumn, size_type from, size_type to )
		:CColumnEventArgs(pColumn), m_from(from), m_to(to){}
	virtual ~CColumnMovedEventArgs(){}
};