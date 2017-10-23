#include "FontCell.h"
#include "MyFont.h"
#include "MyDC.h"
#include "CellProperty.h"
#include "Sheet.h"
#include "MySize.h"
#include "Row.h"
#include "Column.h"
#include "GridView.h"

CFontCell::CFontCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty,CFont font)
	:CTextCell(pSheet,pRow,pColumn,spProperty),
	m_font(font){}

CFont CFontCell::GetFont(){return m_font;}

void CFontCell::PaintContent(CDC* pDC,CRect rcPaint)
{
	HFONT hFont=(HFONT)pDC->SelectFont(m_font);
	string_type str=GetString();
	pDC->DrawTextEx(const_cast<LPTSTR>(str.c_str()),-1,rcPaint,GetFormat(),NULL);		
	pDC->SelectFont(hFont);
}

void CFontCell::OnLButtonClk(MouseEventArgs& e)
{
	CHOOSEFONT cf={0};
	LOGFONT logFont={0};
	memcpy(&logFont,&m_font.GetLogFont(),sizeof(LOGFONT));
	cf.lStructSize=sizeof(CHOOSEFONT);
	cf.hwndOwner=m_pSheet->GetGridPtr()->m_hWnd;
	cf.lpLogFont=&logFont;
	cf.Flags=CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT;
	if(!ChooseFont(&cf))return;
	if(memcmp(&m_font.GetLogFont(),&logFont,sizeof(LOGFONT))!=0){
		m_font=logFont;
		m_pSheet->CellPropertyChanged(this,L"value");
	}
}

CFontCell::string_type CFontCell::GetString()const
{
	std::wstring wstr;
	wstr.append(m_font.GetFaceName());
	wstr.append(L",");
	wstr.append(boost::lexical_cast<std::wstring>(abs(m_font.GetPointSize())));
	return wstr;
}
