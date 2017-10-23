#include "FilterCell.h"
#include "InplaceEdit.h"
#include "Sheet.h"
#include "CellProperty.h"
#include "Column.h"
#include "GridView.h"

CFilterCell::string_type CFilterCell::GetString()const
{
	return m_pColumn->GetFilter();
}

void CFilterCell::SetString(const string_type& str)
{
	//Filter cell undo redo is set when Post WM_FILTER
	if(GetString()!=str){
		string_type newString = str;
		boost::asio::deadline_timer* pTimer = static_cast<CGridView*>(m_pSheet)->GetTimerPtr();
		pTimer->expires_from_now(boost::posix_time::milliseconds(500));
		CCell* pCell = this;
		HWND hWnd = m_pSheet->GetGridPtr()->m_hWnd;
		pTimer->async_wait([hWnd,pCell,newString](const boost::system::error_code& error)->void{

			if(error == boost::asio::error::operation_aborted){
				::OutputDebugStringA("timer canceled\r\n");
			}else{
				::OutputDebugStringA("timer filter\r\n");
				pCell->CCell::SetString(newString);	
				::PostMessage(hWnd,WM_FILTER,NULL,NULL);
			}
		});
	}
}

void CFilterCell::SetStringCore(const string_type& str)
{
	m_pColumn->SetFilter(str);
}

void CFilterCell::PaintContent(CDC* pDC, CRect rcPaint)
{
	HFONT hFont=(HFONT)pDC->SelectFont(*m_spProperty->GetFontPtr());
	string_type str=GetString();
	if(!str.empty()){
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pDC->DrawTextEx(const_cast<LPTSTR>(str.c_str()),-1,rcPaint,GetFormat(),NULL);	
	}else{
		str = L"Filter items...";
		pDC->SetTextColor(RGB(210,210,210));
		pDC->DrawTextEx(const_cast<LPTSTR>(str.c_str()),-1,rcPaint,GetFormat()&~DT_WORDBREAK|DT_END_ELLIPSIS,NULL);	
	}
	pDC->SelectFont(hFont);
}