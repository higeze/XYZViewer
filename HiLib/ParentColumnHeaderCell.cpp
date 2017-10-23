#include "ParentColumnHeaderCell.h"
#include "Sheet.h"
#include "Column.h"
#include "Row.h"
#include "MyRect.h"
#include "MySize.h"
#include "MyDC.h"
#include "CellProperty.h"
#include "MyMenu.h"

CMenu CParentColumnHeaderCell::ContextMenu;
CMenu CParentDefaultColumnHeaderIndexCell::ContextMenu;

CParentColumnHeaderCell::string_type CParentColumnHeaderCell::GetString()const
{
	return string_type();
}

CParentColumnHeaderCell::string_type CParentColumnHeaderCell::GetSortString()const
{
	return string_type();
}

bool CParentColumnHeaderCell::Filter(const string_type& strFilter)const//Not filtered
{
	return true;
}

CParentColumnHeaderCell::string_type CParentColumnHeaderIndexCell::GetString()const
{
	if(m_pColumn->GetVisible()){
		return boost::lexical_cast<string_type>(m_pColumn->GetVisibleIndex()+1);
	}else{
		return string_type(L"0");
	}
}


