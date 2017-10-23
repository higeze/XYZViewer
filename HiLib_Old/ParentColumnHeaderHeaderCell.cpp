#include "ParentColumnHeaderHeaderCell.h"
#include "Sheet.h"
#include "Column.h"
#include "Row.h"
#include "MyRect.h"
#include "MySize.h"
#include "MyDC.h"
#include "CellProperty.h"

CParentColumnHeaderHeaderCell::string_type CParentColumnHeaderHeaderCell::GetString()const
{
	return string_type();
}

CParentColumnHeaderHeaderCell::string_type CParentColumnHeaderHeaderCell::GetSortString()const
{
	return string_type();
}

bool CParentColumnHeaderHeaderCell::Filter(const string_type& strFilter)const//Not filtered
{
	return true;
}


