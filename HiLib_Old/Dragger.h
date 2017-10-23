#pragma once
#include "IMouseObserver.h"
//Pre-Declaration
class CSheet;
struct MouseEventArgs;
struct SetCursorEventArgs;

class CDragger:public IMouseObserver
{
private:
	typedef int size_type;
	typedef int coordinates_type;
	static const size_type COLUMN_INDEX_INVALID = 9999;
	static const size_type COLUMN_INDEX_MAX = 1000;
	static const size_type COLUMN_INDEX_MIN = -1000;
	static const coordinates_type MIN_COLUMN_WIDTH=2;
	static const coordinates_type RESIZE_AREA_HARF_WIDTH=4;

private:
	size_type m_dragFromIndex;
	size_type m_dragToIndex;
public:
	CDragger():m_dragFromIndex(COLUMN_INDEX_INVALID), m_dragToIndex(COLUMN_INDEX_INVALID){}
	virtual ~CDragger(){}
	size_type GetDragToAllIndex(){return m_dragToIndex;}
	size_type GetDragFromAllIndex(){return m_dragFromIndex;}
	
	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e);

	virtual CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e);


	virtual CSheetState* OnDragLButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnDragLButtonUp(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnDragLButtonDblClk(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnDragRButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnDragMouseMove(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnDragMouseLeave(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnDragSetCursor(CSheet* pSheet, SetCursorEventArgs& e);


private:

	CSheetState* OnColumnHeaderBeginDrag(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnColumnHeaderDrag(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnColumnHeaderEndDrag(CSheet* pSheet, MouseEventArgs& e);
private:
	bool IsDragable(CSheet* pSheet, std::pair<size_type, size_type> visibleIndexes);

};

/*! @class CSheetCellDragger
    @brief Dragger behavior for CSheetCell
*/
class CSheetCellDragger:public CDragger
{
public:
    /*! Constructor*/
	CSheetCellDragger():CDragger(){}
	virtual ~CSheetCellDragger(){}
	/*! Do not drag Header for CSheetCell*/
	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e);
};
