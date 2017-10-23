#pragma once
#include "TextCell.h"
#include "GDIUIElement.h"


/**
 * CBoolCell
 */
class CBoolCell:public CTextCell
{
private:
	bool m_bool;
	CCheckBox m_checkBox;
public:
	CBoolCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty,bool b);
	virtual ~CBoolCell(){}

	virtual void PaintContent(CDC* pDC,CRect rcPaint);
	virtual CSize MeasureSize(CDC* pDC);
	virtual CSize MeasureSizeWithFixedWidth(CDC* pDC);


	virtual string_type GetString()const;
	virtual void SetStringCore(const string_type& str);

	virtual void OnLButtonClk(MouseEventArgs& e);

	virtual bool IsComparable()const;
};
