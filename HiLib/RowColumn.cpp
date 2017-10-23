#include "RowColumn.h"
#include "Row.h"
#include "Column.h"

bool CRowColumn::operator<(const CRowColumn& rhs)const
{
	if(m_pRow!=rhs.m_pRow){
		return m_pRow->GetVisibleIndex()<rhs.m_pRow->GetVisibleIndex();
	}else{
		return m_pColumn->GetVisibleIndex()<rhs.m_pColumn->GetVisibleIndex();
	}
}