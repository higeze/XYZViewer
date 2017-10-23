#pragma once
#include "Cell.h"

class CInplaceEdit;

struct equal_double
	:public std::binary_function<const double&,const double&,bool>
{
private:
	double m_dEpsilon;
public:
	equal_double(double dEpsilon=1e-10):m_dEpsilon(dEpsilon){}
	bool operator()(const double& dLhs,const double& dRhs)
	{
		double dRelativeEpsilon=max(abs(dLhs),abs(dRhs))*m_dEpsilon;
		double dDiff=dLhs-dRhs;
		return (*((__int64*) & dDiff) & 0x7FFFFFFFFFFFFFFF) <= (*((__int64*)&dRelativeEpsilon));
	}
};

struct equal_wstring_compare_in_double
	:public std::binary_function<const std::wstring&,const std::wstring&,bool>
{
private:
	equal_double m_ed;
public:
	equal_wstring_compare_in_double():m_ed(){}
	bool operator()(const std::wstring& lhs,const std::wstring& rhs){
		if( lhs.empty() && rhs.empty() ){
			return true;
		}else if( lhs.empty() || rhs.empty() ){
			return false;
		}else{
			return m_ed(_wtof(lhs.c_str()),_wtof(rhs.c_str()));
		}
	}
};

class CTextCell:public CCell
{
protected:
	CInplaceEdit* m_pEdit;
public:
	CTextCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty, CMenu* pMenu = nullptr)
		:CCell(pSheet,pRow, pColumn,spProperty,pMenu),m_pEdit(NULL){}
	virtual ~CTextCell();
	virtual void PaintContent(CDC* pDC, CRect rcPaint);
	virtual void PaintBackground(CDC* pDC, CRect rc);

	//virtual CSize MeasureSize(CDC* pDC);
	//virtual CSize MeasureSizeWithFixedWidth(CDC* pDC);
	virtual CSize MeasureContentSize(CDC* pDC) override;
	virtual CSize MeasureContentSizeWithFixedWidth(CDC* pDC) override;
	virtual void OnEdit(EventArgs& e);
	virtual bool CanSetStringOnEditing()const{return true;}

	virtual UINT GetFormat()const{ return DT_LEFT|DT_TOP|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL; }

	virtual bool IsComparable()const;
	virtual Compares EqualCell(CCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CEmptyCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CTextCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CSheetCell* pCell, std::function<void(CCell*, Compares)> action);
};

class CStringCell:public CTextCell
{
protected:
	string_type m_string;
public:
	CStringCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty, string_type str,CMenu* pMenu=nullptr)
		:CTextCell(pSheet,pRow, pColumn,spProperty,pMenu),m_string(str){}
	virtual ~CStringCell(){}
	virtual string_type GetString()const;
	virtual void SetStringCore(const string_type& str);
};

class CEditableCell:public CTextCell
{
public:
	CEditableCell(CSheet* pSheet, CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty,CMenu* pMenu=nullptr)
		:CTextCell(pSheet,pRow, pColumn,spProperty,pMenu){}
	virtual ~CEditableCell(){}
	virtual void OnLButtonDown(MouseEventArgs& e);
};

class CEditableStringCell:public CStringCell
{
public:
	CEditableStringCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn, std::shared_ptr<CCellProperty> spProperty, string_type str,CMenu* pMenu=nullptr)
		:CStringCell(pSheet,pRow,pColumn,spProperty,str,pMenu){}
	virtual ~CEditableStringCell(){}
	virtual void OnLButtonDown(MouseEventArgs& e);
};
class CDcmParameter;

class CParameterCell:public CEditableCell
{
private:
	bool m_bFirstFocus;
public:
	CParameterCell(CSheet* pSheet=nullptr, CRow* pRow=nullptr, CColumn* pColumn=nullptr,std::shared_ptr<CCellProperty> spProperty=nullptr,CMenu* pMenu=nullptr)
		:CEditableCell(pSheet,pRow, pColumn,spProperty,pMenu),m_bFirstFocus(false){}
	virtual ~CParameterCell(){}
	virtual void OnLButtonDown(MouseEventArgs& e);
	virtual void OnLButtonSingleClick(MouseEventArgs& e);
	virtual void OnKillFocus(EventArgs& e);

	virtual bool CanSetStringOnEditing()const override{return false;}

};

