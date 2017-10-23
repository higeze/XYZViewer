#pragma once
#include "Sheet.h"
#include "Cell.h"

class CSheetCell:public CSheet,public CCell
{
protected:
	typedef CCell::size_type size_type;
	typedef CCell::string_type string_type;
	typedef CCell::coordinates_type coordinates_type;
public:
	virtual bool CanResizeRow() const{return false;}
	virtual bool CanResizeColumn() const{return false;}
	virtual void Resize();
	virtual void Resize(size_type row, size_type col){}

public:
	//Constructor
	CSheetCell(
		CSheet* pSheet = nullptr,
		CRow* pRow = nullptr,
		CColumn* pColumn = nullptr,
		std::shared_ptr<CCellProperty> spProperty = nullptr,
		std::shared_ptr<CCellProperty> spHeaderProperty = nullptr,
		std::shared_ptr<CCellProperty> spFilterProperty = nullptr,
		std::shared_ptr<CCellProperty> spCellProperty = nullptr,
		CMenu* pMenu=nullptr);
	virtual ~CSheetCell(){}
	static CMenu SheetCellContextMenu;
	virtual CMenu* GetContextMenuPtr()override;
	//Property
	virtual std::shared_ptr<CCellProperty> GetHeaderPropertyPtr();
	virtual std::shared_ptr<CCellProperty> GetCellPropertyPtr();
	//Rect
	virtual coordinates_type GetTop()const;
	virtual coordinates_type GetLeft()const;
	virtual CRect GetRect()const;
	virtual CRect GetPaintRect();

	virtual CSize MeasureSize(CDC* pDC);
	virtual CSize MeasureSizeWithFixedWidth(CDC* pDC);

	//virtual void SetFocused(const bool& bFocused);
	virtual bool GetSelected()const;
	virtual void SetSelected(const bool& bSelected);

	//Paint
	virtual void PaintContent(CDC* pDC, CRect rcPaint);
	
	//Event
	virtual void OnLButtonDown(MouseEventArgs& e);
	virtual void OnLButtonUp(MouseEventArgs& e);
	virtual void OnLButtonClk(MouseEventArgs& e){/*Do Nothing*/}
	virtual void OnLButtonDblClk(MouseEventArgs& e){/*Do Nothing*/}
	virtual void OnMouseMove(MouseEventArgs& e);
	virtual void OnMouseLeave(MouseEventArgs& e);

	virtual void OnContextMenu(ContextMenuEventArgs& e);

	virtual void OnSetCursor(SetCursorEventArgs& e);

	virtual void OnSetFocus(EventArgs& e);
	virtual void OnKillFocus(EventArgs& e);
	virtual void OnKeyDown(KeyEventArgs& e){/*Do Nothing*/};
	//String
	virtual string_type GetString()const{return CSheet::GetSheetString();};//TODO for test
	virtual void SetString(const string_type& str){/*Do Nothing*/};
	virtual bool CSheetCell::Filter(const string_type& strFilter)const;
	//Compare
	virtual bool IsComparable()const;
	virtual Compares EqualCell(CCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CEmptyCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CTextCell* pCell, std::function<void(CCell*, Compares)> action);
	virtual Compares EqualCell(CSheetCell* pCell, std::function<void(CCell*, Compares)> action);

	virtual CPoint GetScrollPos()const;
	std::shared_ptr<CDC> GetClientDCPtr()const;
	virtual CGridView* GetGridPtr();

virtual void CellSizeChanged(CellEventArgs& e);
virtual void CellValueChanged(CellEventArgs& e);
virtual void ColumnInserted(CColumnEventArgs& e);
virtual void ColumnErased(CColumnEventArgs& e);
virtual void ColumnHeaderEndTrack(CColumnEventArgs& e);
virtual void RowInserted(CRowEventArgs& e);
virtual void RowErased(CRowEventArgs& e);

virtual CColumn* GetParentColumnPtr(CCell* pCell)override;

		//virtual void UpdateData(size_type rowDataSize,size_type colDataSize);
/*
	void InsertRowIndex(size_type rowData,size_type rowDisp);
	void InsertColumnImplIndex(size_type colData,size_type colDisp);
*/
};