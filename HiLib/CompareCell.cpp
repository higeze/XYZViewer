#include "CompareCell.h"
#include "Sheet.h"

CCompareCell::string_type CCompareCell::GetString()const
{
	switch(m_pSheet->CheckEqualRow(m_pRow, m_pSheet->ColumnVisibleZero(), m_pSheet->ColumnVisibleEnd(), [](CCell* pCell, Compares)->void{})){
	case Compares::Same:
		return L"Same";
	case Compares::Diff:
		return L"Diff";
	case Compares::DiffNE:
		return L"Diff NE";
	default:
		return L"";
	}
}