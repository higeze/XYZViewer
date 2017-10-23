#pragma once
#include "Row.h"

class CGridView;

template<typename T>
class CBindRow:public CParentRow
{
private:
	T m_dataContext;
public:
	CBindRow(CGridView* pGrid, T dataContext)
		:CParentRow(pGrid), m_dataContext(dataContext){}
	virtual ~CBindRow(){}
	T GetDataContext(){return m_dataContext;}
	void SetDataContext(const T& dataContext){m_dataContext = dataContext;}
};