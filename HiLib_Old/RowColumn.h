#pragma once

class CRow;
class CColumn;

class CRowColumn
{
public:
	typedef int size_type;
private:
	static const size_type invalid_dataindex=-100;
	CRow* m_pRow;
	CColumn* m_pColumn;
public:
	CRowColumn(CRow* pRow=nullptr,CColumn* pColumn=nullptr):m_pRow(pRow),m_pColumn(pColumn){}
	virtual ~CRowColumn(){}
	CRow* GetRowPtr()const{return m_pRow;}
	CColumn* GetColumnPtr()const{return m_pColumn;}
	void SetColumnPtr(CColumn* pColumn){m_pColumn=pColumn;}
	void SetRowPtr(CRow* pRow){m_pRow=pRow;}
	void SetRowColumnPtr(CRow* pRow,CColumn* pColumn){m_pRow=pRow;m_pColumn=pColumn;}

	bool operator==(const CRowColumn& rhs)const
	{
		return (m_pRow==rhs.m_pRow && m_pColumn==rhs.m_pColumn);
	}

	bool operator!=(const CRowColumn& rhs)const
	{
		return (m_pRow!=rhs.m_pRow || m_pColumn!=rhs.m_pColumn);
	}

	CRowColumn& operator=(const CRowColumn& rhs)
	{
		m_pRow=rhs.m_pRow;m_pColumn=rhs.m_pColumn;
		return *this;
	}

	bool operator<(const CRowColumn& rhs)const;

	bool IsInvalid()const{return (m_pRow==nullptr || m_pColumn==nullptr);}
	//bool IsInRange(size_type r0,size_type c0,size_type r1,size_type c1)const
	//{
	//	return (r0<=m_pRow)&&(m_pRow<=r1)&&(c0<=m_pColumn)&&(m_pColumn<=c1);
	//
	//} 
};