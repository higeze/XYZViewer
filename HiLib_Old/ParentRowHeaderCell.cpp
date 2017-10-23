#include "ParentRowHeaderCell.h"
#include "Sheet.h"
#include "Row.h"
#include "Column.h"
#include "MyRect.h"
#include "MySize.h"
#include "MyDC.h"
#include "CellProperty.h"
#include "MyMenu.h"

CMenu CParentRowHeaderCell::ContextMenu;

CParentRowHeaderCell::string_type CParentRowHeaderCell::GetString()const
{
	return string_type();
}

CParentRowHeaderCell::string_type CParentRowHeaderCell::GetSortString()const
{
	return string_type();
}

bool CParentRowHeaderCell::Filter(const string_type& strFilter)const//Not filtered
{
	return true;
}

CParentRowHeaderCell::string_type CParentRowHeaderIndexCell::GetString()const
{
	return boost::lexical_cast<string_type>(m_pRow->GetVisibleIndex()+1);
}



