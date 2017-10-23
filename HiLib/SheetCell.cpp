#include "SheetCell.h"
#include "CellProperty.h"
#include "Row.h"
#include "MyBrush.h"
#include "MyPen.h"
#include "MyRect.h"
#include "MyRgn.h"
//
#include "ChildIndexColumn.h"
#include "ChildRowHeaderColumn.h"
#include "ParentMapColumn.h"
//
#include "Tracker.h"
#include "Dragger.h"
#include "Cursorer.h"
#include "ResizeDlg.h"

#include "FunctionMenuItem.h"
#include "GridViewResource.h"
#include "SheetEventArgs.h"
#include "GridView.h"

CMenu CSheetCell::SheetCellContextMenu;

CSheetCell::CSheetCell(
	CSheet* pSheet,
	CRow* pRow,
	CColumn* pColumn,
	std::shared_ptr<CCellProperty> spProperty,
	std::shared_ptr<CCellProperty> spHeaderProperty,
	std::shared_ptr<CCellProperty> spFilterProperty,
	std::shared_ptr<CCellProperty> spCellProperty,
	CMenu* pMenu)
	:CSheet(spHeaderProperty,spFilterProperty,spCellProperty, std::make_shared<CTracker>(), std::make_shared<CSheetCellDragger>(), std::make_shared<CSheetCellCursorer>()),
	CCell(pSheet,pRow,pColumn,spProperty,pMenu)
{}

void CSheetCell::Resize()
{
	if(CanResizeRow() || CanResizeColumn()){
		CResizeDlg* pDlg = new CResizeDlg(this);
		g_hDlgModeless = pDlg->Create(m_pSheet->GetGridPtr()->m_hWnd);
		pDlg->ShowWindow(SW_SHOW);
	}else{
		
	}
}

void CSheetCell::CellSizeChanged(CellEventArgs& e)
{
	CSheet::CellSizeChanged(e);
	m_pSheet->CellSizeChanged(CellEventArgs(this));
}
void CSheetCell::CellValueChanged(CellEventArgs& e)
{
	CSheet::CellValueChanged(e);
	m_pSheet->CellValueChanged(CellEventArgs(this));
}

void CSheetCell::ColumnInserted(CColumnEventArgs& e)
{
	CSheet::ColumnInserted(e);
	m_pSheet->CellSizeChanged(CellEventArgs(this));
}
void CSheetCell::ColumnErased(CColumnEventArgs& e)
{
	CSheet::ColumnErased(e);
	m_pSheet->CellSizeChanged(CellEventArgs(this));
}

void CSheetCell::ColumnHeaderEndTrack(CColumnEventArgs& e)
{
	CSheet::ColumnHeaderEndTrack(e);
	m_pSheet->CellSizeChanged(CellEventArgs(this));
}

void CSheetCell::RowInserted(CRowEventArgs& e)
{
	CSheet::RowInserted(e);
	m_pSheet->CellSizeChanged(CellEventArgs(this));
}
void CSheetCell::RowErased(CRowEventArgs& e)
{
	CSheet::RowErased(e);
	m_pSheet->CellSizeChanged(CellEventArgs(this));
}

CSheetCell::coordinates_type CSheetCell::GetTop()const
{
	return CCell::GetTop();
}

CSheetCell::coordinates_type CSheetCell::GetLeft()const
{
	return CCell::GetLeft();
}

CRect CSheetCell::GetRect()const
{
	return CCell::GetRect();
}

CRect CSheetCell::GetPaintRect()
{
	return CCell::GetRect();//TODO
}

CPoint CSheetCell::GetScrollPos()const
{
	return CPoint(0,0);
}

CSize CSheetCell::MeasureSize(CDC* pDC)
{
	//Calc Content Rect
	CRect rcContent(CSheet::MeasureSize());
	//Calc InnerBorder, CenterBorder Rect
	CRect rcCenter(InnerBorder2CenterBorder(Content2InnerBorder(rcContent)));

	return rcCenter.Size();
}

CSize CSheetCell::MeasureSizeWithFixedWidth(CDC* pDC)
{
	return MeasureSize(pDC);
}

void CSheetCell::OnLButtonDown(MouseEventArgs& e)
{
	CCell::OnLButtonDown(e);
	CSheet::OnLButtonDown(e);
	SubmitUpdate();
}

void CSheetCell::OnLButtonUp(MouseEventArgs& e)
{
	CCell::OnLButtonUp(e);
	CSheet::OnLButtonUp(e);
	SubmitUpdate();
}

void CSheetCell::OnMouseMove(MouseEventArgs& e)
{
	CCell::OnMouseMove(e);
	CSheet::OnMouseMove(e);
	SubmitUpdate();
}

void CSheetCell::OnMouseLeave(MouseEventArgs& e)
{
	CCell::OnMouseLeave(e);
	CSheet::OnMouseLeave(e);
	SubmitUpdate();
}

void CSheetCell::OnSetCursor(SetCursorEventArgs& e)
{
	CCell::OnSetCursor(e);
	CSheet::OnSetCursor(e);
	SubmitUpdate();
}

void CSheetCell::OnSetFocus(EventArgs& e)
{
	CCell::OnSetFocus(e);
	CSheet::OnSetFocus(e);
	SubmitUpdate();
}

void CSheetCell::OnKillFocus(EventArgs& e)
{
	CCell::OnKillFocus(e);
	CSheet::OnKillFocus(e);
	SubmitUpdate();
}

bool CSheetCell::IsComparable()const
{
	return true;
}

Compares CSheetCell::EqualCell(CCell* pCell, std::function<void(CCell*, Compares)> action)
{
	return pCell->EqualCell(this, action);
}
Compares CSheetCell::EqualCell(CEmptyCell* pCell, std::function<void(CCell*, Compares)> action)
{
	action(this, Compares::DiffNE);
	return Compares::DiffNE;
} 
Compares CSheetCell::EqualCell(CTextCell* pCell, std::function<void(CCell*, Compares)> action)
{
	action(this, Compares::Diff);
	return Compares::Diff;
} 
Compares CSheetCell::EqualCell(CSheetCell* pCell, std::function<void(CCell*, Compares)> action)
{//TODO check size, fixed and soon

	//if Minus size is difference, all cells are not equal
	if(MinusSize(m_columnVisibleDictionary)!=pCell->MinusSize(pCell->m_columnVisibleDictionary)||
		MinusSize(m_rowVisibleDictionary)!=pCell->MinusSize(pCell->m_rowVisibleDictionary)){
		boost::range::for_each(m_columnVisibleDictionary,[&](const ColumnData& colData){
			boost::range::for_each(this->m_rowVisibleDictionary,[&](const RowData& rowData){
				action(CSheet::Cell(rowData.DataPtr, colData.DataPtr).get(), Compares::Diff);
			});
		});
		action(this, Compares::Diff);
		return Compares::Diff;
	}

	auto comp = Compares::Same;
	//if size is difference, cell is not equal
	if(m_columnVisibleDictionary.size()!=pCell->m_columnVisibleDictionary.size() ||
		m_rowVisibleDictionary.size()!=pCell->m_rowVisibleDictionary.size()){
		comp = Compares::Diff;
	}
	//Larger size cells are checked
	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();

	for(auto colIter=colDictionary.begin(),colEnd=colDictionary.end();colIter!=colEnd;++colIter){
		for(auto rowIter=rowDictionary.begin(),rowEnd=rowDictionary.end();rowIter!=rowEnd;++rowIter){
			auto pCellMe=colIter->DataPtr->Cell(rowIter->DataPtr.get());
			auto pCellOther=pCell->VisibleCell(rowIter->Index,colIter->Index);
			if(pCellOther){
				auto cellComp = pCellOther->EqualCell(pCellMe.get(), action);//run equal cell to check
				comp = (comp==Compares::Same && cellComp==Compares::Same)?Compares::Same:Compares::Diff;
			}else{
				comp = Compares::Diff;
				action(this, comp);
			}
		}
	}
	action(this, comp);	
	return comp;
}

void CSheetCell::PaintContent(CDC* pDC, CRect rcPaint)
{
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rcPaint);
	pDC->SelectClipRgn(rgn);	
	CSheet::OnPaint(PaintEventArgs(pDC));
	pDC->SelectClipRgn(NULL);
	//auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();
	//auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	//PaintEventArgs e(nullptr,pDC);
	//for(auto colIter=colDictionary.rbegin(),colEnd=colDictionary.rend();colIter!=colEnd;++colIter){
	//	for(auto rowIter=rowDictionary.rbegin(),rowEnd=rowDictionary.rend();rowIter!=rowEnd;++rowIter){
	//		colIter->DataPtr->Cell(rowIter->DataPtr.get())->OnPaint(e);
	//	}
	//}
}

std::shared_ptr<CCellProperty> CSheetCell::GetHeaderPropertyPtr()
{
	return m_spHeaderProperty;
}
std::shared_ptr<CCellProperty> CSheetCell::GetCellPropertyPtr()
{
	return m_spProperty;
}

std::shared_ptr<CDC> CSheetCell::GetClientDCPtr()const
{
	return m_pSheet->GetClientDCPtr();
}
CGridView* CSheetCell::GetGridPtr()
{
	return m_pSheet->GetGridPtr();
}
//
//void CSheetCell::SetFocused(const bool& bFocused)
//{
//	CCell::SetFocused(bFocused);
//	CSheet::SetFocused(bFocused);
//}

bool CSheetCell::GetSelected()const
{
	return CCell::GetSelected();// || CSheet::GetSelected();
}

void CSheetCell::SetSelected(const bool& bSelected)
{
	//if(selected){
	//	CSheet::SelectAll();
	//}else{
	//	CSheet::DeselectAll();
	//}

	CCell::SetSelected(bSelected);
	//CSheet::SetSelected(bSelected);
}

bool CSheetCell::Filter(const string_type& strFilter)const
{
	bool bMatch = false;
	auto iterCol = boost::find_if(m_columnAllDictionary, [&](const ColumnData& colData)->bool{
		auto iterRow = boost::find_if(m_rowAllDictionary, [&](const RowData& rowData)->bool{
			return colData.DataPtr->Cell(rowData.DataPtr.get())->Filter(strFilter);
		});
		return iterRow!=m_rowAllDictionary.end();
	});

	return iterCol!=m_columnAllDictionary.end();
}
CMenu* CSheetCell::GetContextMenuPtr()
{
	//Copy base menu
	SheetCellContextMenu = CMenu(*CCell::GetContextMenuPtr());

	//Resize
	if(CanResizeRow() || CanResizeColumn()){
		//Create Inserted menu
		auto spMenuItem = std::make_shared<CFunctionMenuItem>();
		spMenuItem->SetID(IDM_RESIZE_SHEETCELL);
		spMenuItem->SetMask(MIIM_TYPE|MIIM_ID);
		spMenuItem->SetType(MFT_STRING);
		spMenuItem->SetTypeData(L"Resize");
		spMenuItem->SetFunction([&]()->void{Resize();});
		//Assign command
		GetGridPtr()->ReplaceCmdIDHandler(spMenuItem->GetID(),
			[spMenuItem](WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)->HRESULT
				{return spMenuItem->OnCommand(wNotifyCode,wID,hWndCtl,bHandled);});
		//Insert menu
		if(SheetCellContextMenu.GetMenuItemCount()>0){
			SheetCellContextMenu.InsertSeparator(SheetCellContextMenu.GetMenuItemCount(), TRUE);
		}
		SheetCellContextMenu.InsertMenuItem(SheetCellContextMenu.GetMenuItemCount(), TRUE, spMenuItem.get());
	}
	return &SheetCellContextMenu;
}
void CSheetCell::OnContextMenu(ContextMenuEventArgs& e)
{
	if(!Visible())return;
	auto roco = Point2RowColumn(e.Point);
	if(!roco.IsInvalid()){
		roco.GetColumnPtr()->Cell(roco.GetRowPtr())->OnContextMenu(e);
	}else{
		CCell::OnContextMenu(e);
	}
	SubmitUpdate();
}

CColumn* CSheetCell::GetParentColumnPtr(CCell* pCell)
{
	return m_pSheet->GetParentColumnPtr(this);
}

