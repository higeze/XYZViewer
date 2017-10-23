#include "Sheet.h"
#include "SheetEventArgs.h"
#include "CellProperty.h"
//
#include "MyRect.h"
#include "MyDC.h"
#include "MyFont.h"
#include "MyPen.h"
#include "Cell.h"
#include "Row.h"
#include "Column.h"
#include "RowColumn.h"
#include "ConsoleTimer.h"
#include "ApplicationProperty.h"
#include "Tracker.h"
#include "Dragger.h"
#include "Cursorer.h"
#include "SheetState.h"

extern std::shared_ptr<CApplicationProperty> g_spApplicationProperty;

CMenu CSheet::ContextMenu;

CSheet::CSheet(
	std::shared_ptr<CCellProperty> spHeaderProperty,
	std::shared_ptr<CCellProperty> spFilterProperty,
	std::shared_ptr<CCellProperty> spCellProperty,
	std::shared_ptr<CTracker> spTracker,
	std::shared_ptr<CDragger> spDragger,
	std::shared_ptr<CCursorer> spCursorer,
	CMenu* pContextMenu)
	:m_spHeaderProperty(spHeaderProperty),
	m_spFilterProperty(spFilterProperty),
	m_spCellProperty(spCellProperty),
	m_spTracker(spTracker),
	m_spDragger(spDragger),
	m_spCursorer(spCursorer),
	//m_operation(Operation::None),
	m_bSelected(false),
	m_bFocused(false),
	m_pContextMenu(pContextMenu?pContextMenu:&CSheet::ContextMenu)
{

	if(m_spTracker.get()==nullptr){
		m_spTracker = std::make_shared<CTracker>();
	}
	if(m_spDragger.get()==nullptr){
		m_spDragger = std::make_shared<CDragger>();
	}
	if(m_spCursorer.get()==nullptr){
		m_spCursorer = std::make_shared<CCursorer>();
	}

	m_mouseObservers.push_back(m_spTracker);
	m_mouseObservers.push_back(m_spDragger);
	m_mouseObservers.push_back(m_spCursorer);

	m_pState = CSheetState::Normal();
}

std::shared_ptr<CCell> CSheet::Cell(const std::shared_ptr<CRow>& spRow, const std::shared_ptr<CColumn>& spColumn)
{
	return spColumn->Cell(spRow.get());
}

std::shared_ptr<CCell>& CSheet::Cell( CRow* pRow,  CColumn* pColumn)
{
	return pColumn->Cell(pRow);
}

void CSheet::SetAllRowMeasureValid(bool valid)
{
	boost::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		rowData.DataPtr->SetMeasureValid(valid);
	});
}

void CSheet::SetAllColumnMeasureValid(bool valid)
{
	boost::for_each(m_columnAllDictionary,[&](const ColumnData& colData){
		colData.DataPtr->SetMeasureValid(valid);
	});
}

void CSheet::SetColumnAllCellMeasureValid(CColumn* pColumn, bool valid)
{
	boost::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		CSheet::Cell(rowData.DataPtr.get(), pColumn)->SetActMeasureValid(false);
	});
}

void CSheet::CellSizeChanged(CellEventArgs& e)
{
	e.CellPtr->GetColumnPtr()->SetMeasureValid(false);
	e.CellPtr->GetRowPtr()->SetMeasureValid(false);
	e.CellPtr->SetFitMeasureValid(false);
	e.CellPtr->SetActMeasureValid(false);
	PostUpdate(Updates::Column);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);
}
void CSheet::CellValueChanged(CellEventArgs& e)
{
//	e.CellPtr->GetColumnPtr()->SetMeasureValid(false);//TODO value change doesnt change width
	e.CellPtr->GetRowPtr()->SetMeasureValid(false);
	e.CellPtr->SetFitMeasureValid(false);
	e.CellPtr->SetActMeasureValid(false);
	PostUpdate(Updates::Column);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);
}

void CSheet::ColumnInserted(CColumnEventArgs& e)
{
	boost::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		rowData.DataPtr->SetMeasureValid(false);
	});

	PostUpdate(Updates::ColumnVisible);
	PostUpdate(Updates::Column);
	PostUpdate(Updates::RowVisible);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//
}

void CSheet::ColumnMoved(CColumnMovedEventArgs& e)
{
	PostUpdate(Updates::ColumnVisible);
	PostUpdate(Updates::Column);
	//PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//
}
void CSheet::ColumnErased(CColumnEventArgs& e)
{
	boost::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		rowData.DataPtr->SetMeasureValid(false);
	});

	PostUpdate(Updates::ColumnVisible);
	PostUpdate(Updates::Column);
	PostUpdate(Updates::RowVisible);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//
}
void CSheet::ColumnHeaderTrack(CColumnEventArgs& e)
{
	PostUpdate(Updates::Column);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//	
}
void CSheet::ColumnHeaderEndTrack(CColumnEventArgs& e)
{
	boost::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		e.m_pColumn->Cell(rowData.DataPtr.get())->SetActMeasureValid(false);
	});
	boost::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		rowData.DataPtr->SetMeasureValid(false);
	});
	PostUpdate(Updates::Column);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//
}

void CSheet::ColumnHeaderFitWidth(CColumnEventArgs& e)
{
	e.m_pColumn->SetMeasureValid(false);
	boost::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		e.m_pColumn->Cell(rowData.DataPtr.get())->SetActMeasureValid(false);
	});
	boost::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		rowData.DataPtr->SetMeasureValid(false);
	});
	PostUpdate(Updates::Column);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//
}

void CSheet::RowInserted(CRowEventArgs& e)
{
	//boost::for_each(m_columnAllDictionary,[&](const ColumnData& colData){
	//	colData.DataPtr->SetMeasureValid(false);
	//});
	PostUpdate(Updates::RowVisible);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//
}

void CSheet::RowErased(CRowEventArgs& e)
{
	boost::for_each(m_columnAllDictionary,[&](const ColumnData& colData){
		colData.DataPtr->SetMeasureValid(false);
	});
	PostUpdate(Updates::RowVisible);
	PostUpdate(Updates::Column);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//
}

void CSheet::RowsErased(CRowsEventArgs& e)
{
	//Remeasure column width irritate user, therefore Column measure doesn't run.
	//boost::for_each(m_columnAllDictionary,[&](const ColumnData& colData){
	//	colData.DataPtr->SetMeasureValid(false);
	//});
	PostUpdate(Updates::RowVisible);
	PostUpdate(Updates::Column);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);//
}

void CSheet::Sorted()
{
	PostUpdate(Updates::RowVisible);
	PostUpdate(Updates::Row);
	PostUpdate(Updates::Invalidate);
}
void CSheet::SizeChanged()
{
	PostUpdate(Updates::Scrolls);
	PostUpdate(Updates::Invalidate);
}
void CSheet::Scroll()
{
	PostUpdate(Updates::Invalidate);
}

void CSheet::SubmitUpdate()
{
	boost::for_each(m_setUpdate,[&](const Updates type){
		switch(type){
		case Updates::RowVisible:
			{
				CONSOLETIMER_IF(g_spApplicationProperty->m_bDebug, "UpdateRowVisibleDictionary")
					UpdateRowVisibleDictionary();
			}
			break;
		case Updates::ColumnVisible:
			{
				CONSOLETIMER_IF(g_spApplicationProperty->m_bDebug, "UpdateColumnVisibleDictionary")
					UpdateColumnVisibleDictionary();
				break;
			}
		case Updates::Column:
			{
				CONSOLETIMER_IF(g_spApplicationProperty->m_bDebug, "UpdateColumn")
					UpdateColumn();
				break;
			}
		case Updates::Row:
			{
				CONSOLETIMER_IF(g_spApplicationProperty->m_bDebug, "UpdateRow")	
					UpdateRow();
				break;
			}
		case Updates::Scrolls:
			{
				CONSOLETIMER_IF(g_spApplicationProperty->m_bDebug, "UpdateScrolls")
					UpdateScrolls();
				break;
			}
		case Updates::Invalidate:
			Invalidate();
		default:
			break;
		}
	});
	m_setUpdate.clear();
}

void CSheet::PostUpdate(Updates type)
{
	if(type==Updates::All){
		m_setUpdate.insert(Updates::RowVisible);
		m_setUpdate.insert(Updates::ColumnVisible);
		m_setUpdate.insert(Updates::Column);
		m_setUpdate.insert(Updates::Row);
		m_setUpdate.insert(Updates::Scrolls);
		m_setUpdate.insert(Updates::Invalidate);
	}else{
		m_setUpdate.insert(type);
	}
}

CSheet::string_type CSheet::GetSheetString()const
{
	string_type str;

	boost::range::for_each(m_rowVisibleDictionary,[&](const RowData& rowData){
		bool bFirstLine=true;
		boost::range::for_each(this->m_columnVisibleDictionary,[&](const ColumnData& colData){
			auto pCell=colData.DataPtr->Cell(rowData.DataPtr.get());
			if(bFirstLine){
				bFirstLine=false;
			}else{
				str.append(L"\t");
			}
			str.append(pCell->GetString());
		});
		str.append(L"\r\n");
	});

	return str;
}

CSheet::row_type CSheet::VisibleRow(size_type rowDisp)
{
	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();
	auto iter=rowDictionary.find(rowDisp);
	if(iter!=rowDictionary.end()){
		return iter->DataPtr;
	}else{
		return row_type();
	}
}

CSheet::column_type CSheet::VisibleColumn(size_type colDisp)
{
	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	auto iter=colDictionary.find(colDisp);
	if(iter!=colDictionary.end()){
		return iter->DataPtr;
	}else{
		return column_type();
	}
}

CSheet::row_type CSheet::AllRow(size_type rowDisp)
{
	auto& rowDictionary=m_rowAllDictionary.get<IndexTag>();
	auto iter=rowDictionary.find(rowDisp);
	if(iter!=rowDictionary.end()){
		return iter->DataPtr;
	}else{
		return row_type();
	}
}

CSheet::column_type CSheet::AllColumn(size_type colDisp)
{
	auto& colDictionary=m_columnAllDictionary.get<IndexTag>();
	auto iter=colDictionary.find(colDisp);
	if(iter!=colDictionary.end()){
		return iter->DataPtr;
	}else{
		return column_type();
	}
}


CSheet::cell_type CSheet::AllCell(size_type rowDisp, size_type colDisp)
{
	auto pRow=AllRow(rowDisp);
	auto pCol=AllColumn(colDisp);
	if(pRow && pCol){
		return pCol->Cell(pRow.get());
	}else{
		return cell_type();
	}
}

CSheet::cell_type CSheet::VisibleCell(size_type rowDisp, size_type colDisp)
{
	auto pRow=VisibleRow(rowDisp);
	auto pCol=VisibleColumn(colDisp);
	if(pRow && pCol){
		return pCol->Cell(pRow.get());
	}else{
		return cell_type();
	}
}


CSheet::size_type CSheet::RowPointer2VisibleIndex(const CRow* pRow)const
{
	return Pointer2Index(m_rowVisibleDictionary,pRow);
}

CSheet::size_type CSheet::RowPointer2AllIndex(const CRow* pRow)const
{
	return Pointer2Index(m_rowAllDictionary,pRow);
}
CRow* CSheet::RowAllIndex2Pointer(size_type rowAll)const
{
	return Index2Pointer(m_rowAllDictionary,rowAll);
}

CRow* CSheet::RowVisibleIndex2Pointer(size_type rowVisib)const
{
	return Index2Pointer(m_rowVisibleDictionary,rowVisib);
}

CSheet::size_type CSheet::ColumnPointer2AllIndex( const CColumn* pColumn ) const
{
	return Pointer2Index(m_columnAllDictionary,pColumn);
}

CSheet::size_type CSheet::ColumnPointer2VisibleIndex( const CColumn* pColumn ) const
{
	return Pointer2Index(m_columnVisibleDictionary,pColumn);
}

CColumn* CSheet::ColumnVisibleIndex2Pointer(size_type colVisib)const
{
	return Index2Pointer(m_columnVisibleDictionary,colVisib);
}

CColumn* CSheet::ColumnAllIndex2Pointer(size_type colVisib)const
{
	return Index2Pointer(m_columnAllDictionary,colVisib);
}

void CSheet::UpdateRowVisibleDictionary()
{
	//OnCursorClear(EventArgs(nullptr));
	UpdateVisibleDictionary(m_rowAllDictionary,m_rowVisibleDictionary);
}

void CSheet::UpdateColumnVisibleDictionary()
{
	//OnCursorClear(EventArgs(nullptr));
	UpdateVisibleDictionary(m_columnAllDictionary,m_columnVisibleDictionary);
}

void CSheet::UpdateRowPaintDictionary()
{
	CRect rcClip(GetPaintRect());
	CPoint ptOrigin(GetOriginPoint());
	rcClip.left = ptOrigin.x;
	rcClip.top = ptOrigin.y;
	UpdatePaintDictionary(m_rowVisibleDictionary,m_rowPaintDictionary,rcClip.top,rcClip.bottom,
		[](const coordinates_type& x,const RowData& data)->bool{return x<data.DataPtr->GetTop();},
		[](const RowData& data,const coordinates_type& x)->bool{return x>data.DataPtr->GetBottom();});
}

void CSheet::UpdateColumnPaintDictionary()
{
	CRect rcClip(GetPaintRect());
	CPoint ptOrigin(GetOriginPoint());
	rcClip.left = ptOrigin.x;
	rcClip.top = ptOrigin.y;
	UpdatePaintDictionary(m_columnVisibleDictionary,m_columnPaintDictionary,rcClip.left,rcClip.right,
		[](const coordinates_type& x,const ColumnData& data)->bool{return x<data.DataPtr->GetLeft();},
		[](const ColumnData& data,const coordinates_type& x)->bool{return x>data.DataPtr->GetRight();});
}

void CSheet::ResetColumnSort()
{
	//Reset Sort Mark
	for(auto iter=m_columnAllDictionary.begin(),end=m_columnAllDictionary.end();iter!=end;++iter){
		iter->DataPtr->SetSort(Sorts::None);
	}
}

void CSheet::Sort(CColumn* pCol, Sorts sort)
{
	auto& rowDictionary=m_rowAllDictionary.get<IndexTag>();
	std::vector<RowData> vRowMinusData,vRowPlusData;
	//Copy all to vector
	std::copy(rowDictionary.begin(),rowDictionary.find(0),std::back_inserter(vRowMinusData));
	std::copy(rowDictionary.find(0),rowDictionary.end(),std::back_inserter(vRowPlusData));
	//Sort
	switch(sort){
	case Sorts::Down:
		std::stable_sort(vRowPlusData.begin(),vRowPlusData.end(),[pCol](const RowData& lhs,const RowData& rhs)->bool{
			return pCol->Cell(lhs.DataPtr.get())->operator<(*(pCol->Cell(rhs.DataPtr.get())));
		});
		break;
	case Sorts::Up:
		std::stable_sort(vRowPlusData.begin(),vRowPlusData.end(),[pCol](const RowData& lhs,const RowData& rhs)->bool{
			return pCol->Cell(lhs.DataPtr.get())->operator>(*(pCol->Cell(rhs.DataPtr.get())));
		});
		break;
	default:
		break;
	}

	//Copy again
	m_rowAllDictionary.clear();
	std::copy(vRowMinusData.begin(),vRowMinusData.end(),std::inserter(m_rowAllDictionary,m_rowAllDictionary.begin()));
	for(auto begin=vRowPlusData.begin(),iter=vRowPlusData.begin(),end=vRowPlusData.end();iter!=end;++iter){
		iter->Index=iter-begin;
		m_rowAllDictionary.insert(*iter);
	}
	//Update
	Sorted();
}


void CSheet::Filter(size_type colDisp,std::function<bool(const cell_type&)> predicate)
{
	auto& colDictionary=m_columnAllDictionary.get<IndexTag>();
	auto colIter=colDictionary.find(colDisp);
	auto& rowDictionary=m_rowAllDictionary.get<IndexTag>();

	for(auto rowIter=rowDictionary.find(0),rowEnd=rowDictionary.end();rowIter!=rowEnd;++rowIter){
		if(predicate(colIter->DataPtr->Cell(rowIter->DataPtr.get()))){
			rowIter->DataPtr->SetVisible(true);
		}else{
			rowIter->DataPtr->SetVisible(false);		
		}
	}
}

void CSheet::ResetFilter()
{
	auto& rowDictionary=m_rowAllDictionary.get<IndexTag>();
	for(auto rowIter=rowDictionary.find(0),rowEnd=rowDictionary.end();rowIter!=rowEnd;++rowIter){
		rowIter->DataPtr->SetVisible(true);
	}

	auto& colDictionary=m_columnAllDictionary.get<IndexTag>();
	for(auto colIter=colDictionary.find(0),colEnd=colDictionary.end();colIter!=colEnd;++colIter){
		colIter->DataPtr->SetVisible(true);		
	}
}

void CSheet::MoveColumnImpl(size_type colTo, column_type spFromColumn)
{ 
	int from = spFromColumn->GetAllIndex();
	int to = colTo;
	if(from>=0 && to>=0  && to>from){
		to--;
	}
	if(from<0 && to<0 && to<from){
		to++;
	}

	Erase(m_columnAllDictionary, spFromColumn.get());
	InsertLeft(m_columnAllDictionary,to,spFromColumn);
	ColumnMoved(CColumnMovedEventArgs(spFromColumn.get(), from, to));
}

void CSheet::EraseColumnImpl(column_type spColumn)
{
	Erase(m_columnAllDictionary, spColumn.get());
	m_spCursorer->OnCursorClear(this, EventArgs());
	ColumnErased(CColumnEventArgs(spColumn.get()));
	DEBUG_OUTPUT_COLUMN_ALL_DICTIONARY
}

void CSheet::EraseRow(CRow* pRow)
{
	Erase(m_rowAllDictionary, pRow);
	m_spCursorer->OnCursorClear(this, EventArgs());
	RowErased(CRowEventArgs(pRow));
}

void CSheet::EraseRows(const std::vector<CRow*>& vpRow)
{
	EraseSome(m_rowAllDictionary, vpRow);
	m_spCursorer->OnCursorClear(this, EventArgs());
	RowsErased(CRowsEventArgs(vpRow));
}

void CSheet::InsertColumnImpl(size_type allIndex, column_type pColumn, bool notify)
{
	pColumn->InsertNecessaryRows();
	Insert(m_columnAllDictionary,allIndex,pColumn);
	if(notify){
		ColumnInserted(CColumnEventArgs(pColumn.get()));
	}
}

void CSheet::InsertColumnImplLeft(size_type allIndex, column_type pColumn)
{
	InsertLeft(m_columnAllDictionary,allIndex,pColumn);
	ColumnInserted(CColumnEventArgs(pColumn.get()));
}

void CSheet::InsertRow(size_type allIndex, row_type pRow, bool notify)
{
	Insert(m_rowAllDictionary,allIndex,pRow);
	if(notify){
		RowInserted(CRowEventArgs(pRow.get()));
	}
}

CSheet::coordinates_type CSheet::GetColumnInitWidth(CColumn* pColumn)
{
	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();
	coordinates_type maxWidth=0;
	CDC dc(::CreateCompatibleDC(*GetClientDCPtr().get()));	
	boost::for_each(rowDictionary, [&](const RowData& rowData){
		auto pCell=pColumn->Cell(rowData.DataPtr.get());
		maxWidth=max(pCell->GetInitSize(&dc).cx,maxWidth);
	});
	return maxWidth;
}

CSheet::coordinates_type CSheet::GetColumnFitWidth(CColumn* pColumn)
{
	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();
	coordinates_type maxWidth=0;
	CDC dc(::CreateCompatibleDC(*GetClientDCPtr().get()));	
	boost::for_each(rowDictionary, [&](const RowData& rowData){
		auto pCell=pColumn->Cell(rowData.DataPtr.get());
		maxWidth=max(pCell->GetFitSize(&dc).cx,maxWidth);
	});
	return maxWidth;
}

CSheet::coordinates_type CSheet::GetRowHeight(CRow* pRow)
{
	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	coordinates_type maxHeight=0;
	CDC dc(::CreateCompatibleDC(*GetClientDCPtr().get()));	
	boost::for_each(colDictionary,[&](const ColumnData& colData){
		auto pCell=colData.DataPtr->Cell(pRow);
		maxHeight=max(pCell->GetActSize(&dc).cy,maxHeight);
	});
	return maxHeight;
}


void CSheet::UpdateRow()
{
	if(!Visible()){return;}

	auto& rowDictionary = m_rowVisibleDictionary.get<IndexTag>();
	CPoint ptScroll=GetScrollPos();
	coordinates_type top=m_spCellProperty->GetPenPtr()->GetLeftTopPenWidth();
	for(auto iter = rowDictionary.begin(),end=rowDictionary.end();iter!=end;++iter){
		iter->DataPtr->SetTopWithoutSignal(top);
		top+=iter->DataPtr->GetHeight();
	}
}

void CSheet::UpdateColumn()
{
	if(!Visible()){return;}

	auto& colDictionary = m_columnVisibleDictionary.get<IndexTag>();
	coordinates_type left= m_spCellProperty->GetPenPtr()->GetLeftTopPenWidth();
	for(auto iter = colDictionary.begin(),end=colDictionary.end();iter!=end;++iter){
		iter->DataPtr->SetSheetLeftWithoutSignal(left);
		left+=iter->DataPtr->GetWidth();
	}
}

bool CSheet::Empty()const
{
	return m_rowAllDictionary.empty() || m_columnAllDictionary.empty();
}
bool CSheet::Visible()const
{
	return (!m_rowVisibleDictionary.empty()) && (!m_columnVisibleDictionary.empty());
}

CPoint CSheet::GetOriginPoint()
{
	if(!Visible()){
		MessageBox(NULL,L"CSheet::GetOriginPoint",NULL,0);
		throw std::exception("CSheet::GetOriginPoint");	
	}

	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();

	auto colMinusIter=colDictionary.find(-1);
	auto colPlusIter=colDictionary.find(0);
	auto rowMinusIter=rowDictionary.find(-1);
	auto rowPlusIter=rowDictionary.find(0);
	coordinates_type x=0;
	coordinates_type y=0;
	if(colMinusIter!=colDictionary.end()){
		x=colMinusIter->DataPtr->GetRight();
	}else{
		x=colPlusIter->DataPtr->GetLeft();
	}
	if(rowMinusIter!=rowDictionary.end()){
		y=rowMinusIter->DataPtr->GetBottom();
	}else{
		y=rowPlusIter->DataPtr->GetTop();
	}

	return CPoint(x,y);
}


CSize CSheet::MeasureSize()const
{
	CRect rc(CSheet::GetRect());
	return CSize(rc.Width(), rc.Height());
}

//GetRect return Sheet(All) Rect in Window Client
//GetCellsRect return Cells(=Scrollable Cells) Rect in Window Client
//GetCellsClipRect return Cells(=Scrollable Cells) Clip(=Paint area) Rect in Window Client
CRect CSheet::GetRect()const
{
	if(!Visible()){return CRect();}

	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();

	coordinates_type left=colDictionary.begin()->DataPtr->GetLeft();
	coordinates_type top=rowDictionary.begin()->DataPtr->GetTop();
	coordinates_type right=boost::prior(colDictionary.end())->DataPtr->GetRight();
	coordinates_type bottom=boost::prior(rowDictionary.end())->DataPtr->GetBottom();

	CRect rc(left, top, right, bottom);

	auto outerPenWidth = m_spCellProperty->GetPenPtr()->GetLeftTopPenWidth();
	rc.InflateRect(outerPenWidth, outerPenWidth);
	return rc;
}


CRect CSheet::GetCellsRect()
{
	CPoint ptScroll(GetScrollPos());
	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();

	if(GetRowVisibleSize()==0 ||GetColumnVisibleSize()==0 ){
		return CRect(0,0,0,0);
	}

	coordinates_type left=colDictionary.find(0)->DataPtr->GetLeft();
	coordinates_type top=rowDictionary.find(0)->DataPtr->GetTop();
	coordinates_type right=boost::prior(colDictionary.end())->DataPtr->GetRight();
	coordinates_type bottom=boost::prior(rowDictionary.end())->DataPtr->GetBottom();

	return CRect(left,top,right,bottom);
}

//CRect CSheet::GetCellsClipRect()
//{
//	//Calculate Origin
//	CPoint ptOrigin(GetOriginPoint());
//	CRect rcClip(GetRect());
//
//	rcClip.left=ptOrigin.x;
//	rcClip.top=ptOrigin.y;
//	//CRect rcCells(GetCellsRect());
//	//CRect rcClip;
//	//
//	//rcClip.IntersectRect(rcCells,rcClient);
//
//	return rcClip;
//}
void CSheet::OnPaintAll(PaintEventArgs& e)
{
	DEBUG_OUTPUT_COLUMN_VISIBLE_DICTIONARY

	if(!Visible())return;

	//Paint
	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();
	for(auto colIter=colDictionary.rbegin(),colEnd=colDictionary.rend();colIter!=colEnd;++colIter){
		for(auto rowIter=rowDictionary.rbegin(),rowEnd=rowDictionary.rend();rowIter!=rowEnd;++rowIter){
			colIter->DataPtr->Cell(rowIter->DataPtr.get())->OnPaint(e);
		}
	}
}
void CSheet::OnPaint(PaintEventArgs& e)
{

	DEBUG_OUTPUT_COLUMN_VISIBLE_DICTIONARY

	if(!Visible())return;
	//Update Paint Dictioanary
	UpdateRowPaintDictionary();
	UpdateColumnPaintDictionary();

	DEBUG_OUTPUT_COLUMN_PAINT_DICTIONARY


		//Paint
		//e.DCPtr->FillRect(GetCellsClipRect(),*(m_spCellProperty->GetBackgroundBrushPtr()));

	auto& colDictionary=m_columnPaintDictionary.get<IndexTag>();
	auto& rowDictionary=m_rowPaintDictionary.get<IndexTag>();
	for(auto colIter=colDictionary.rbegin(),colEnd=colDictionary.rend();colIter!=colEnd;++colIter){
		for(auto rowIter=rowDictionary.rbegin(),rowEnd=rowDictionary.rend();rowIter!=rowEnd;++rowIter){
			colIter->DataPtr->Cell(rowIter->DataPtr.get())->OnPaint(e);
		}
	}
}

void CSheet::UpdateAll()
{
	UpdateRowVisibleDictionary();//TODO today
	UpdateColumnVisibleDictionary();
	//	UpdateFitHeight();
	//	UpdateFitWidth();
	UpdateColumn();
	UpdateRow();

}

CSheet::cell_type CSheet::Point2Cell(const CPoint& ptClient)
{
	auto roco=Point2RowColumn(ptClient);
	if(roco.IsInvalid()){
		return cell_type();
	}else{
		return roco.GetColumnPtr()->Cell(roco.GetRowPtr());
	}
}


CRowColumn CSheet::Point2RowColumn(const CPoint& ptClient)
{
	auto ptOrigin=GetOriginPoint();

	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();
	auto rowIter=std::upper_bound(rowDictionary.begin(),rowDictionary.end(),ptClient.y,
		[ptOrigin](const int& y,const RowData& rowData)->bool{
			if(rowData.Index>=0){
				return y<max(ptOrigin.y,rowData.DataPtr->GetTop());
			}else{
				return y<rowData.DataPtr->GetTop();
			}
	});

	auto prevRowIter=boost::prior(rowIter);

	if(rowIter==rowDictionary.begin() || (rowIter==rowDictionary.end() && ptClient.y>prevRowIter->DataPtr->GetBottom())){
		return CRowColumn();
	}else{
		--rowIter;
	}

	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();
	auto colIter=std::upper_bound(colDictionary.begin(),colDictionary.end(),ptClient.x,
		[ptOrigin](const int& x,const ColumnData& colData)->bool{
			if(colData.Index>=0){
				return x<max(ptOrigin.x,colData.DataPtr->GetLeft());
			}else{
				return x<colData.DataPtr->GetLeft();
			}
	});
	auto prevColIter=boost::prior(colIter);

	if(colIter==colDictionary.begin() || (colIter==colDictionary.end() && ptClient.x>prevColIter->DataPtr->GetRight())){
		return CRowColumn();
	}else{
		colIter--;
	}
	auto roco=CRowColumn(rowIter->DataPtr.get(),colIter->DataPtr.get());
	DEBUG_OUTPUT_ROWCOLUMN(roco)
		return roco;
}
	CSheet::size_type CSheet::Y2VisibleRowIndex(coordinates_type y)
	{
		return Y2RowIndex(y, m_rowVisibleDictionary);
	}

	CSheet::size_type CSheet::X2VisibleColumnIndex(coordinates_type x)
	{
		return X2ColumnIndex(x, m_columnVisibleDictionary);
	}
std::pair<CSheet::size_type, CSheet::size_type> CSheet::Point2VisibleIndexes(const CPoint& ptClient)
{
	return std::make_pair(Y2VisibleRowIndex(ptClient.y), X2VisibleColumnIndex(ptClient.x));
}

void CSheet::OnContextMenu(ContextMenuEventArgs& e)
{
	if(!Visible())return;
	auto roco = Point2RowColumn(e.Point);
	if(roco.IsInvalid())return;
	roco.GetColumnPtr()->Cell(roco.GetRowPtr())->OnContextMenu(e);
}

void CSheet::OnRButtonDown(MouseEventArgs& e)
{
	m_pState = m_pState->OnRButtonDown(this, e);
}

void CSheet::OnLButtonDown(MouseEventArgs& e)
{
	m_pState = m_pState->OnLButtonDown(this, e);
}

void CSheet::OnLButtonUp(MouseEventArgs& e)
{
	m_pState = m_pState->OnLButtonUp(this, e);
}

void CSheet::OnLButtonDblClk(MouseEventArgs& e)
{
	m_pState = m_pState->OnLButtonDblClk(this, e);
}

void CSheet::OnLButtonDblClkTimeExceed(MouseEventArgs& e)
{
	m_pState = m_pState->OnLButtonDblClkTimeExceed(this, e);
}

void CSheet::OnMouseMove(MouseEventArgs& e)
{
	m_pState = m_pState->OnMouseMove(this, e);
}
void CSheet::OnSetCursor(SetCursorEventArgs& e)
{
	m_pState = m_pState->OnSetCursor(this, e);
}

void CSheet::OnMouseLeave(MouseEventArgs& e)
{
	m_pState = m_pState->OnMouseLeave(this, e);
}

void CSheet::OnSetFocus(EventArgs& e)
{
	if(!Visible())return;
	m_spCursorer->OnCursorClear(this, e);
	m_bFocused = true;
}

void CSheet::OnKillFocus(EventArgs& e)
{
	if(!Visible())return;
	m_spCursorer->OnCursorClear(this, e);
	m_bFocused = false;
}

void CSheet::SelectRange(CRowColumn roco1, CRowColumn roco2)
{
	if(roco1.IsInvalid() || roco2.IsInvalid())return;

	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();
	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();

	auto	rowBeg=min(roco1.GetRowPtr()->GetVisibleIndex(),roco2.GetRowPtr()->GetVisibleIndex());
	auto	rowLast=max(roco1.GetRowPtr()->GetVisibleIndex(),roco2.GetRowPtr()->GetVisibleIndex());
	auto	colBeg=min(roco1.GetColumnPtr()->GetVisibleIndex(),roco2.GetColumnPtr()->GetVisibleIndex());
	auto	colLast=max(roco1.GetColumnPtr()->GetVisibleIndex(),roco2.GetColumnPtr()->GetVisibleIndex());

	for(auto colIter=colDictionary.find(colBeg),colEnd=colDictionary.find(colLast+1);colIter!=colEnd;++colIter){
		for(auto rowIter=rowDictionary.find(rowBeg),rowEnd=rowDictionary.find(rowLast+1);rowIter!=rowEnd;++rowIter){
			colIter->DataPtr->Cell(rowIter->DataPtr.get())->SetSelected(true);
		}
	}	
}

void CSheet::DeselectRange(CRowColumn roco1, CRowColumn roco2)
{
	if(roco1.IsInvalid() || roco2.IsInvalid())return;

	auto& rowDictionary=m_rowVisibleDictionary.get<IndexTag>();
	auto& colDictionary=m_columnVisibleDictionary.get<IndexTag>();

	auto	rowBeg=min(roco1.GetRowPtr()->GetVisibleIndex(),roco2.GetRowPtr()->GetVisibleIndex());
	auto	rowLast=max(roco1.GetRowPtr()->GetVisibleIndex(),roco2.GetRowPtr()->GetVisibleIndex());
	auto	colBeg=min(roco1.GetColumnPtr()->GetVisibleIndex(),roco2.GetColumnPtr()->GetVisibleIndex());
	auto	colLast=max(roco1.GetColumnPtr()->GetVisibleIndex(),roco2.GetColumnPtr()->GetVisibleIndex());

	for(auto colIter=colDictionary.find(colBeg),colEnd=colDictionary.find(colLast+1);colIter!=colEnd;++colIter){
		for(auto rowIter=rowDictionary.find(rowBeg),rowEnd=rowDictionary.find(rowLast+1);rowIter!=rowEnd;++rowIter){
			colIter->DataPtr->Cell(rowIter->DataPtr.get())->SetSelected(false);
		}
	}	
}

void CSheet::SelectAll()
{
	boost::range::for_each(m_columnAllDictionary,[&](const ColumnData& colData){
		colData.DataPtr->SetSelected(true);
	});
	boost::range::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		rowData.DataPtr->SetSelected(true);
	});
}

void CSheet::DeselectAll()
{
	boost::range::for_each(m_columnAllDictionary,[&](const ColumnData& colData){
		boost::range::for_each(m_rowAllDictionary,[&](const RowData& rowData){
			colData.DataPtr->Cell(rowData.DataPtr.get())->SetSelected(false);
		});
	});

	boost::range::for_each(m_columnAllDictionary,[&](const ColumnData& colData){
		colData.DataPtr->SetSelected(false);
	});

	boost::range::for_each(m_rowAllDictionary,[&](const RowData& rowData){
		rowData.DataPtr->SetSelected(false);
	});
}

void CSheet::UnhotAll()
{
	boost::range::for_each(m_columnAllDictionary,[&](const ColumnData& colData){
		boost::range::for_each(m_rowAllDictionary,[&](const RowData& rowData){
			colData.DataPtr->Cell(rowData.DataPtr.get())->SetState(UIElementState::Normal);
		});
	});
}

bool CSheet::IsFocusedCell(CRowColumn roco)const
{
	return m_spCursorer->GetFocusedRowColumn()==roco;
}

bool CSheet::IsDoubleFocusedCell(CRowColumn roco)const
{
	return m_spCursorer->GetDoubleFocusedRowColumn()==roco;
}


CSheet::size_type CSheet::GetRowVisibleSize()//TODO high
{
	return PlusSize(m_rowVisibleDictionary);
}

CSheet::size_type CSheet::GetColumnVisibleSize()
{
	return PlusSize(m_columnVisibleDictionary);
}

CSheet::size_type CSheet::GetAllRowSize()const
{
	return m_rowAllDictionary.size();
}

CSheet::size_type CSheet::GetAllColumnSize()const
{
	return m_columnAllDictionary.size();
}

CSheet::size_type CSheet::GetPlusRowSize()const
{
	return PlusSize(m_rowAllDictionary);
}
CSheet::size_type CSheet::GetPlusColumnSize()const
{
	return PlusSize(m_columnAllDictionary);
}

Compares CSheet::CheckEqualRange(RowDictionary::iterator rowBegin, RowDictionary::iterator rowEnd,ColumnDictionary::iterator colBegin, ColumnDictionary::iterator colEnd, std::function<void(CCell*, Compares)> action)
{
	Compares comp = Compares::Same;
	//if(m_columnAllDictionary.size()-MinusSize(m_columnAllDictionary)>=2){
		//auto& colDictionary=m_columnAllDictionary.get<IndexTag>();
		for(auto rowIter=rowBegin;rowIter!=rowEnd;++rowIter){
			auto cm = CheckEqualRow(rowIter->DataPtr.get(), colBegin, colEnd, action);
			if(comp == Compares::DiffNE || cm == Compares::DiffNE){
				comp = Compares::DiffNE;
			}else if(comp == Compares::Diff || cm == Compares::Diff){
				comp = Compares::Diff;
			}else{
				comp = Compares::Same;
			}
		}
	//}
	return comp;
}

Compares CSheet::CheckEqualRow(CRow* pRow, ColumnDictionary::iterator colBegin, ColumnDictionary::iterator colEnd, std::function<void(CCell*, Compares)> action)
{
	if(colBegin==colEnd || colBegin->Index>=boost::prior(colEnd)->Index){
		return Compares::Same;
	}
	Compares comp = Compares::Same;
	//auto& colDictionary=m_columnAllDictionary.get<IndexTag>();
	//auto colIter=colDictionary.find(0);
	//auto colEnd=boost::prior(colDictionary.end());
	std::shared_ptr<CCell> pCellComp;
	std::shared_ptr<CCell> pCell, pCellNext;
	for(auto colIter = colBegin;colIter!=boost::prior(colEnd);++colIter){

		pCell=colIter->DataPtr->Cell(pRow);
		pCellNext=boost::next(colIter)->DataPtr->Cell(pRow);

		if(pCell->IsComparable()){
			if(!pCellComp){
				//First Comparable Cell is Not Checked.
				pCell->EqualCell(pCell.get(), action);
				//pCell->SetChecked(false);
			}
			pCellComp=pCell;
		}

		if(pCellNext->IsComparable() && pCellComp){
			//Check Equal		
			auto cm = pCellComp->EqualCell(pCellNext.get(), action);

			/////
			//if(cm==Compares::Same){
			//	pCellNext->SetChecked(false);
			//}else{
			//	pCellNext->SetChecked(true);
			//}


			if(comp == Compares::DiffNE || cm == Compares::DiffNE){
				comp = Compares::DiffNE;
			}else if(comp == Compares::Diff || cm == Compares::Diff){
				comp = Compares::Diff;
			}else{
				comp = Compares::Same;
			}
		}
	}	

	if(pCellNext->IsComparable() && !pCellComp){
		//Final lonely Comparable Cell is Not Checked
		pCellNext->EqualCell(pCellNext.get(), action);
		//pCellNext->SetChecked(false);
	}
	return comp;
}

void CSheet::Clear()
{
	m_setUpdate.clear();

	m_rowAllDictionary.clear();
	m_rowVisibleDictionary.clear();
	m_rowPaintDictionary.clear();

	m_columnAllDictionary.clear();
	m_columnVisibleDictionary.clear();
	m_columnPaintDictionary.clear();

	m_spCursorer->Clear();

	m_rocoContextMenu=CRowColumn();
	m_rocoMouse = CRowColumn();

	//m_coTrackLeftVisib=COLUMN_INDEX_INVALID;

	//m_operation=Operation::None;

	m_pHeaderColumn=column_type();


	m_rowHeader=row_type();


	m_bSelected = false;
	m_bFocused = false;
}

CSheet::size_type CSheet::ColumnVisibleIndex2AllIndex(size_type visibleIndex)
{
	return VisibleIndex2AllIndex(visibleIndex, m_columnVisibleDictionary, m_columnAllDictionary);
}

std::pair<CSheet::size_type,CSheet::size_type> CSheet::GetMinMaxAllRowIndex()const
{
	return GetMinMaxAllIndex(m_rowAllDictionary);
}

std::pair<CSheet::size_type,CSheet::size_type> CSheet::GetMinMaxAllColumnIndex()const
{
	return GetMinMaxAllIndex(m_columnAllDictionary);
}

std::pair<CSheet::size_type,CSheet::size_type> CSheet::GetMinMaxVisibleRowIndex()const
{
	return GetMinMaxVisibleIndex(m_rowVisibleDictionary);
}

std::pair<CSheet::size_type,CSheet::size_type> CSheet::GetMinMaxVisibleColumnIndex()const
{
	return GetMinMaxVisibleIndex(m_columnVisibleDictionary);
}

std::shared_ptr<CColumn> CSheet::GetVisibleZeroColumnPointer()const
{
	return m_columnVisibleDictionary.find(0)->DataPtr;
}

std::shared_ptr<CColumn> CSheet::GetVisibleLastColumnPointer()const
{
	return boost::prior(m_columnVisibleDictionary.end())->DataPtr;
}



