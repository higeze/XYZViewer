#pragma once
#include "TextCell.h"

class IInplaceEdit;
/**
 *  Filter Cell
 */
class CFilterCell:public CEditableCell
{
public:
	/**
	 *  Constructor
	 */
	CFilterCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu = nullptr)
		:CEditableCell(pSheet,pRow, pColumn,spProperty,pMenu){}
	/**
	 *  Destructor
	 */
	virtual ~CFilterCell(){}
	/**
	 *  Get string from column property
	 */
	virtual string_type GetString()const override;
	/**
	 *  Set string to column property
	 */
	virtual void SetString(const string_type& str)override;
	virtual void SetStringCore(const string_type& str)override;
	/**
	 *  Override to show message when cell is empty
	 */
	virtual void PaintContent(CDC* pDC, CRect rcPaint)override;
	/**
	 *  Comparable or not
	 */
	virtual bool IsComparable()const override
	{
		return false;
	}

};