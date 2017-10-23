#pragma once
#include "IMouseObserver.h"
#include "IKeyObserver.h"
#include "RowColumn.h"
#include "Sheet.h"
//Pre-Declaration
class CSheet;
class CGridView;
class CSheetState;
struct EventArgs;
struct MouseEventArgs;
struct SetCursorEventArgs;

struct RC
{
	typedef int size_type;
	RC(size_type row, size_type col)
		:Row(row), Column(col){}
	size_type Row;
	size_type Column;
};

class CCursorer:public IMouseObserver, public IKeyObserver
{
private:
	typedef int size_type;
	typedef int coordinates_type;
	typedef std::shared_ptr<CCell> cell_type;
	typedef std::shared_ptr<CColumn> column_type;
	typedef std::shared_ptr<CRow> row_type;

	static const size_type COLUMN_INDEX_INVALID = 9999;
	static const size_type COLUMN_INDEX_MAX = 1000;
	static const size_type COLUMN_INDEX_MIN = -1000;
	static const coordinates_type MIN_COLUMN_WIDTH=2;
	static const coordinates_type RESIZE_AREA_HARF_WIDTH=4;

private:
	CRowColumn m_rocoOld;
	CRowColumn m_rocoCurrent;
	CRowColumn m_rocoAnchor;
	CRowColumn m_rocoFocused;
	CRowColumn m_rocoDoubleFocused;
public:
	CCursorer(){}
	virtual ~CCursorer(){}

	CRowColumn GetFocusedRowColumn(){return m_rocoFocused;}
	void SetFocusedRowColumn(CRowColumn roco){m_rocoFocused = roco;}
	//IMouseObserver
	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonUp(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnLButtonDblClk(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnMouseMove(CSheet* pSheet, MouseEventArgs& e);
	virtual CSheetState* OnMouseLeave(CSheet* pSheet, MouseEventArgs& e);
	CSheetState* OnSetCursor(CSheet* pSheet, SetCursorEventArgs& e);
	//IKeyObserver
	CSheetState* OnKeyDown(CGridView* pSheet, KeyEventArgs& e);


	CSheetState* OnCursorCtrl(CSheet* pSheet, EventArgs& e, CRowColumn roco);
	CSheetState* OnCursorShift(CSheet* pSheet, EventArgs& e, CRowColumn roco);
	CSheetState* OnCursor(CSheet* pSheet, EventArgs& e, CRowColumn roco);
	CSheetState* OnRowCursorCtrl(CSheet* pSheet, EventArgs& e, CRow* pRow);
	CSheetState* OnRowCursorShift(CSheet* pSheet, EventArgs& e, CRow* pRow);
	CSheetState* OnRowCursor(CSheet* pSheet, EventArgs& e, CRow* pRow);
	CSheetState* OnColumnCursorCtrl(CSheet* pSheet, EventArgs& e, CColumn* pColumn);
	CSheetState* OnColumnCursorShift(CSheet* pSheet, EventArgs& e, CColumn* pColumn);
	CSheetState* OnColumnCursor(CSheet* pSheet, EventArgs& e, CColumn* pColumn);
	CSheetState* OnCursorClear(CSheet* pSheet, EventArgs& e);

	CRowColumn GetFocusedRowColumn()const{return m_rocoFocused;}
	CRowColumn GetDoubleFocusedRowColumn()const{return m_rocoDoubleFocused;}

	std::vector<RC> GetFocusedRCs(CSheet* pSheet)const;
	std::vector<RC> GetSelectedRCs(CSheet* pSheet)const;
	std::vector<row_type> GetSelectedRows(CSheet* pSheet)const;
	std::vector<column_type> GetSelectedColumns(CSheet* pSheet)const;
	void SetFocusedRCs(CSheet* pSheet, std::vector<RC> rcs);
	void SetSelectedRCs(CSheet* pSheet, std::vector<RC> rocos);
	void SetSelectedRows(CSheet* pSheet, std::vector<row_type> rcs);
	void SetSelectedColumns(CSheet* pSheet, std::vector<column_type> cols);


	void Clear()
	{
		m_rocoOld=CRowColumn();
		m_rocoCurrent=CRowColumn();
		m_rocoAnchor=CRowColumn();
		m_rocoFocused=CRowColumn();
	}


	template<class T, class U>
	void DeselectBandRange(CSheet* pSheet, T& bandVisibleDictionary, U* pBand1, U* pBand2)
	{
		if(!pBand1 || !pBand2)return;
		auto& bandDictionary=bandVisibleDictionary.get<IndexTag>();
		auto beg=min(pBand1->GetVisibleIndex(),pBand2->GetVisibleIndex());
		auto last=max(pBand1->GetVisibleIndex(),pBand2->GetVisibleIndex());
		for(auto iter=bandDictionary.find(beg),end=bandDictionary.find(last+1);iter!=end;++iter){
			iter->DataPtr->SetSelected(false);
		}
	}
	template<class T, class U>
	void SelectBandRange(CSheet* pSheet, T& bandVisibleDictionary, U* pBand1, U* pBand2)
	{
		if(!pBand1 || !pBand2)return;
		auto& bandDictionary=bandVisibleDictionary.get<IndexTag>();
		auto beg=min(pBand1->GetVisibleIndex(),pBand2->GetVisibleIndex());
		auto last=max(pBand1->GetVisibleIndex(),pBand2->GetVisibleIndex());
		for(auto iter=bandDictionary.find(beg),end=bandDictionary.find(last+1);iter!=end;++iter){
			iter->DataPtr->SetSelected(true);
		}
	}

	template<class T, class U>
	void OnBandCursorCtrl(CSheet* pSheet, EventArgs& e,T& bandVisibleDictionary, U* pBand, CRowColumn& roco)
	{
		//m_rocoOld=m_rocoCurrent;//Old
		//m_rocoCurrent=roco;//Current
		//m_rocoAnchor=roco;//Anchor
		//if(m_rocoFocused!=roco){
		//	if(!m_rocoFocused.IsInvalid()){m_rocoFocused.GetColumnPtr()->Cell(m_rocoFocused.GetRowPtr())->OnKillFocus(e);}//Blur
		//	m_rocoFocused=roco;m_rocoFocused.GetColumnPtr()->Cell(m_rocoFocused.GetRowPtr())->OnSetFocus(e);//Focus
		//}
		pBand->SetSelected(true);//Select
	}

	template<class T, class U>
	void OnBandCursorShift(CSheet* pSheet, EventArgs& e,T& bandVisibleDictionary, U* pOldBand, U* pAnchorBand,U* pBand, CRowColumn& roco)
	{
		//m_rocoOld=m_rocoCurrent;//Old
		//m_rocoCurrent=roco;//Current
		//No Anchor change
		DeselectBandRange(pSheet, bandVisibleDictionary,pAnchorBand,pOldBand);
		SelectBandRange(pSheet, bandVisibleDictionary,pAnchorBand,pBand);
	}

	template<class T, class U>
	void OnBandCursor(CSheet* pSheet, EventArgs& e,T& bandVisibleDictionary, U* pBand, CRowColumn& roco)
	{
		//m_rocoOld=m_rocoCurrent;//Old
		//m_rocoCurrent=roco;//Current
		//m_rocoAnchor=roco;//Anchor
		//if(m_rocoFocused!=roco){
		//	if(!m_rocoFocused.IsInvalid()){m_rocoFocused.GetColumnPtr()->Cell(m_rocoFocused.GetRowPtr())->OnKillFocus(e);}//Blur
		//	m_rocoFocused=roco;if(!m_rocoFocused.IsInvalid()){m_rocoFocused.GetColumnPtr()->Cell(m_rocoFocused.GetRowPtr())->OnSetFocus(e);}//Focus
		//}
		if(pBand->GetSelected()){
		}else{
			pSheet->DeselectAll();
			pBand->SetSelected(true);//Select
		}
	}

};

class CSheetCellCursorer:public CCursorer
{
public:
	CSheetCellCursorer():CCursorer(){}
	virtual ~CSheetCellCursorer(){}
	virtual CSheetState* OnLButtonDown(CSheet* pSheet, MouseEventArgs& e) override;
	virtual CSheetState* OnRButtonDown(CSheet* pSheet, MouseEventArgs& e) override;
};