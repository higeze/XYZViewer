#pragma once
#include "UIElement.h"
#include "SheetEnums.h"
#include "MySize.h"
#include "MyRect.h"

class CCellProperty;
class CRect;
class CSize;
class CDC;
class CSheet;

class CCell;
class CEmptyCell;
class CTextCell;
class CSheetCell;
class CColumn;
class CRow;
class CContextMenuEventArgs;

class CMenu;

struct CellEventArgs;
struct CellContextMenuEventArgs;

class CCell:public CUIElement
{
public:
	static CMenu ContextMenu;
protected:
	typedef int size_type;
	typedef int coordinates_type;
	typedef std::wstring string_type;

	CSheet* m_pSheet;
	CRow* m_pRow;
	CColumn* m_pColumn;
	bool m_bSelected;
	bool m_bChecked;

	std::shared_ptr<CCellProperty> m_spProperty;

	CSize m_fitSize;
	CSize m_actSize;
	bool m_bFitMeasureValid;
	bool m_bActMeasureValid;

public:
	//Constructor
	CCell(CSheet* pSheet = nullptr, CRow* pRow = nullptr, CColumn* pColumn = nullptr, std::shared_ptr<CCellProperty> spProperty = nullptr, CMenu* pContextMenu= &CCell::ContextMenu);

	virtual ~CCell(){}

	//Operator

	bool operator<(const CCell& rhs)const;
	bool operator>(const CCell& rhs)const;

	//Size, Rect method
	virtual CSize GetInitSize(CDC* pDC);

	virtual CSize GetFitSize(CDC* pDC);

	virtual CSize GetActSize(CDC* pDC);

	virtual CSize MeasureContentSize(CDC* pDC);

	virtual CSize MeasureContentSizeWithFixedWidth(CDC* pDC);

	virtual CSize MeasureSize(CDC* pDC);

	virtual CSize MeasureSizeWithFixedWidth(CDC* pDC);

	void SetFitMeasureValid(const bool& b){m_bFitMeasureValid=b;}
	
	void SetActMeasureValid(const bool& b){m_bActMeasureValid=b;}

	virtual coordinates_type GetTop()const;
	virtual coordinates_type GetLeft()const;
	virtual CRect GetRect()const;

	//Accesser
	virtual CSheet* GetSheetPtr()const{return m_pSheet;}
	virtual CRow* GetRowPtr()const{return m_pRow;}
	virtual CColumn* GetColumnPtr()const{return m_pColumn;}

	virtual CRect CenterBorder2InnerBorder(CRect rcCenter);
	virtual CRect InnerBorder2Content(CRect rcInner);

	virtual CRect Content2InnerBorder(CRect rcContent);
	virtual CRect InnerBorder2CenterBorder(CRect rcInner);

	virtual void SetState(const UIElementState::Type& state);
	//Property
	virtual std::shared_ptr<CCellProperty> GetPropertyPtr();

	//Selected
	virtual bool GetSelected()const;
	virtual void SetSelected(const bool& selected);

	//Focused
	virtual bool GetFocused()const;

	//DoubleFocused
	virtual bool GetDoubleFocused()const;

	//Checked
	virtual bool GetChecked()const;
	virtual void SetChecked(const bool& bChecked);
	
	//Paint
	virtual void PaintBackground(CDC* pDC, CRect rc);
	virtual void PaintLine(CDC* pDC, CRect rc);
	virtual void PaintContent(CDC* pDC, CRect rc){/*Do Nothing*/}
	virtual void PaintFocus(CDC* pDC, CRect rc);
	
	//Event
	virtual void OnPaint(PaintEventArgs& e);
	virtual void OnLButtonDown(MouseEventArgs& e);
	virtual void OnLButtonUp(MouseEventArgs& e);
	virtual void OnLButtonClk(MouseEventArgs& e){/*Do Nothing*/}
	virtual void OnLButtonDblClk(MouseEventArgs& e);
	virtual void OnContextMenu(ContextMenuEventArgs& e);
	virtual void OnSetFocus(EventArgs& e);

	virtual void OnLButtonSingleClick(MouseEventArgs& e){/*Do Nothing*/}
	virtual void OnLButtonDoubleClick(MouseEventArgs& e){/*Do Nothing*/}

	virtual void OnSetCursor(SetCursorEventArgs& e){/*Do Nothing*/}

	virtual void OnKillFocus(EventArgs& e);
	virtual void OnKeyDown(KeyEventArgs& e){/*Do Nothing*/};
	//String
	virtual string_type GetString()const;
	virtual string_type GetSortString()const{return GetString();}

	virtual void SetString(const string_type& str);
	virtual void SetStringNotify(const string_type& str);
	virtual void SetStringCore(const string_type& str){/*Do Nothing*/};
	virtual bool Filter(const string_type& strFilter)const;

	//Compare
	virtual bool IsComparable()const=0;
	virtual Compares EqualCell(CCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CEmptyCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CTextCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CSheetCell* pCell, std::function<void(CCell*, Compares)> action);

	//Menu
	CMenu* const m_pContextMenu;
	virtual CMenu* GetContextMenuPtr(){return m_pContextMenu;}

	//Clipboard
	virtual bool IsClipboardCopyable()const{return false;}

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& archive, unsigned int version)
    {
    }

};

BOOST_CLASS_EXPORT_KEY(CCell);