#pragma once
#include "IMouseObserver.h"

//Pre-Declaration
class CSheet;
class CSheetState;
struct MouseEventArgs;
struct SetCursorEventArgs;

class CTracker:public IMouseObserver
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
	size_type m_coTrackLeftVisib;
public:
	CTracker():m_coTrackLeftVisib(COLUMN_INDEX_INVALID){}
	virtual ~CTracker(){}

	CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e) override;
	CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e) override;
	CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e) override;

	CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e) override;

	CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e) override;
	CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e) override;

	CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e) override;


	CSheetState* OnTrackLButtonDown(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnTrackLButtonUp(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnTrackLButtonDblClk(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnTrackRButtonDown(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnTrackMouseMove(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnTrackMouseLeave(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnTrackSetCursor(CSheet* pSheet, SetCursorEventArgs& e);

private:
	CSheetState* OnColumnHeaderDividerDblClk(CSheet* pSheet, MouseEventArgs& e, size_type col);
	CSheetState* OnColumnHeaderBeginTrack(CSheet* pSheet, MouseEventArgs& e, size_type col);
	CSheetState* OnColumnHeaderTrack(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnColumnHeaderEndTrack(CSheet* pSheet, MouseEventArgs& e);

private:
	bool IsTrackable(CSheet* pSheet, std::pair<size_type, size_type> visibleIndexes);

};
