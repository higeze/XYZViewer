#include "TextCell.h"
#include "CellProperty.h"
#include "MyString.h"
#include "MyRect.h"
#include "MySize.h"
#include "MyDC.h"
#include "MyRgn.h"

#include "InplaceEdit.h"
#include "Sheet.h"
#include "Row.h"
#include "Column.h"
#include "GridView.h"

CTextCell::~CTextCell()
{
	if(m_pEdit){
		m_pEdit->SendMessage(WM_CLOSE,0,0);
		m_pEdit = nullptr;
	}
}

void CTextCell::PaintContent(CDC* pDC, CRect rcPaint)
{
	//if(GetFocused()){
	//	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	//}else if(GetSelected()){
	//	pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	//}else if(GetChecked()){
	//	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	//}else{
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	//}
	HFONT hFont=(HFONT)pDC->SelectFont(*m_spProperty->GetFontPtr());
	string_type str=GetString();
	//pDC->FillRect(rcPaint,*m_spProperty->GetCheckedBrushPtr());
	pDC->DrawTextEx(const_cast<LPTSTR>(str.c_str()),-1,rcPaint,GetFormat(),NULL);		
	pDC->SelectFont(hFont);
}

CSize CTextCell::MeasureContentSize(CDC* pDC)
{
	//Calc Content Rect
	HFONT hFont=(HFONT)pDC->SelectFont(*m_spProperty->GetFontPtr());
	CRect rcContent;
	std::basic_string<TCHAR> str=GetString();
	if(str.empty()){str=_T("a");}
	pDC->DrawTextEx(const_cast<LPTSTR>(str.c_str()),str.size(),rcContent,
		DT_CALCRECT|GetFormat()&~DT_WORDBREAK,NULL);
	pDC->SelectFont(hFont);
	return rcContent.Size();
}

//CSize CTextCell::MeasureSize(CDC* pDC)
//{
//	CRect rcContent(MeasureContentSize(pDC));
//
//	//Calc CenterBorder Rect
//	CRect rcCenter=(InnerBorder2CenterBorder(Content2InnerBorder(rcContent)));
//	m_bFitMeasureValid = true;
//	return rcCenter.Size();	
//}

CSize CTextCell::MeasureContentSizeWithFixedWidth(CDC* pDC)
{
	//Calc Content Rect
	CRect rcCenter(0,0,m_pColumn->GetWidth(),0);
	CRect rcContent(InnerBorder2Content(CenterBorder2InnerBorder(rcCenter)));
	//Calc Content Rect
	HFONT hFont=(HFONT)pDC->SelectFont(*m_spProperty->GetFontPtr());
	rcContent.SetRect(0,0,rcContent.Width(),0);
	std::basic_string<TCHAR> str=GetString();
	if(str.empty()){str=_T("a");}
	pDC->DrawTextEx(const_cast<LPTSTR>(str.c_str()),str.size(),rcContent,
		DT_CALCRECT|GetFormat(),NULL);
	pDC->SelectFont(hFont);
	return rcContent.Size();
}

//CSize CTextCell::MeasureSizeWithFixedWidth(CDC* pDC)
//{
//	Calc Content Rect
//	CRect rcContent(MeasureContentSizeWithFixedWidth(pDC));
//
//	Calc CenterBorder Rect
//	CRect rcCenter=(InnerBorder2CenterBorder(Content2InnerBorder(rcContent)));
//	m_bActMeasureValid = true;
//	return rcCenter.Size();	
//}

void CTextCell::OnEdit(EventArgs& e)
{
	CRect rcContent(InnerBorder2Content(CenterBorder2InnerBorder(GetRect())));
	auto spCell = std::static_pointer_cast<CTextCell>(CSheet::Cell(m_pRow, m_pColumn));
	SetState(UIElementState::Hot);//During Editing, Keep Hot
	m_pEdit = new CInplaceEdit(
		[spCell]() -> std::basic_string<TCHAR>{
			return spCell->GetString();
		},
		[spCell](const std::basic_string<TCHAR>& str) -> void{
			spCell->SetString(str);
		},
		[spCell](const std::basic_string<TCHAR>& str) -> void{
			if(spCell->CanSetStringOnEditing()){
				spCell->SetString(str);
			}
		},
		[spCell]()->void{
			spCell->m_pEdit=NULL;
			spCell->SetState(UIElementState::Normal);//After Editing, Change Normal
		},
		GetFormat());

	m_pEdit->Create(m_pSheet->GetGridPtr()->m_hWnd,rcContent);
	m_pEdit->SetWindowText(GetString().c_str());
	m_pEdit->SetFont(*m_spProperty->GetFontPtr());
	//CRect rcRect(m_pEdit->GetRect());
	//CRect rcPadding(m_spProperty->GetPadding());
	//rcRect.DeflateRect(rcPadding);
	//m_pEdit->SetRect(rcRect);
	//m_pEdit->SetMargins(2,2);
	m_pEdit->SetFocus();
	m_pEdit->SetSel(0,-1);
	m_pEdit->ShowWindow(SW_SHOW);
}

void CTextCell::PaintBackground(CDC* pDC, CRect rcPaint)
{
	if(m_pEdit){
		CRect rcContent(InnerBorder2Content(CenterBorder2InnerBorder(GetRect())));
		CRect rcCurEdit(m_pSheet->GetGridPtr()->GetScreenToClientRect(m_pEdit->GetWindowRect()));
		m_pSheet->GetGridPtr()->SetEditRect(rcContent);
		if(rcContent!=rcCurEdit){
			m_pEdit->MoveWindow(rcContent,FALSE);
		}
	}
	CCell::PaintBackground(pDC,rcPaint);
}
	
bool CTextCell::IsComparable()const{return true;}

Compares CTextCell::EqualCell(CCell* pCell, std::function<void(CCell*, Compares)> action)
{
	return pCell->EqualCell(this, action);
}

Compares CTextCell::EqualCell(CEmptyCell* pCell, std::function<void(CCell*, Compares)> action)
{
	action(this, Compares::DiffNE);
	return Compares::DiffNE;
}

Compares CTextCell::EqualCell(CTextCell* pCell, std::function<void(CCell*, Compares)> action)
{
	auto str=GetString();
	bool bEqual=true;
	if(IsNaN(str)){
		bEqual= str==pCell->GetString();
	}else{
		bEqual= equal_wstring_compare_in_double()(str,pCell->GetString());
	}
	action(this, bEqual?Compares::Same:Compares::Diff);
	return bEqual?Compares::Same:Compares::Diff;
}

Compares CTextCell::EqualCell(CSheetCell* pCell, std::function<void(CCell*, Compares)> action)
{
	action(this, Compares::Diff);
	return Compares::Diff;
}


CStringCell::string_type CStringCell::GetString()const
{
	return m_string;
}

void CStringCell::SetStringCore(const string_type& str)
{
	m_string=str;
}


void CEditableCell::OnLButtonDown(MouseEventArgs& e)
{
	OnEdit(e);
}


void CEditableStringCell::OnLButtonDown(MouseEventArgs& e)
{
	OnEdit(e);
}

void CParameterCell::OnLButtonDown(MouseEventArgs& e)
{
	//Do Nothing
}
void CParameterCell::OnLButtonSingleClick(MouseEventArgs& e)
{
	//if(!m_bFirstFocus){
	//	m_bFirstFocus=true;
	//	return;
	//}else{
	//	CEditableCell::OnLButtonDown(e);
	//}

	if(GetDoubleFocused()){
		OnEdit(e);
	}

}

void CParameterCell::OnKillFocus(EventArgs& e)
{
	m_bFirstFocus=false;
	CCell::OnKillFocus(e);
}

//void CParameterCell::OnEdit(EventArgs& e)
//{
//	CRect rcContent(InnerBorder2Content(CenterBorder2InnerBorder(GetRect())));
//	auto spCell = CSheet::Cell(m_pRow, m_pColumn);
//	SetState(UIElementState::Hot);//During Editing, Keep Hot
//	m_pEdit = new CInplaceEdit(
//		[spCell]() -> std::basic_string<TCHAR>{
//			return spCell->GetString();
//		},
//		[spCell](const std::basic_string<TCHAR>& str) -> void{
//			spCell->SetString(str);
//		},
//		[spCell](const std::basic_string<TCHAR>& str) -> void{
//			//SetString(str);
//		},
//		[spCell]()->void{
//			std::dynamic_pointer_cast<CParameterCell>(spCell)->m_pEdit=NULL;
//			spCell->SetState(UIElementState::Normal);//After Editing, Change Normal
//		},
//		m_spProperty->GetDrawTextPropertyPtr());
//
//	m_pEdit->Create(m_pSheet->GetGridPtr()->m_hWnd,rcContent);
//	m_pEdit->SetWindowText(GetString().c_str());
//	m_pEdit->SetFont(*m_spProperty->GetFontPtr());
//	//CRect rcRect(m_pEdit->GetRect());
//	//CRect rcPadding(m_spProperty->GetPadding());
//	//rcRect.DeflateRect(rcPadding);
//	//m_pEdit->SetRect(rcRect);
//	//m_pEdit->SetMargins(2,2);
//	m_pEdit->SetFocus();
//	m_pEdit->SetSel(0,-1);
//	m_pEdit->ShowWindow(SW_SHOW);
//}