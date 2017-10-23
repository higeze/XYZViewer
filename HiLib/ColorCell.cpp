#include "ColorCell.h"
#include "MyColor.h"
#include "MyBrush.h"
#include "MyDC.h"
#include "Sheet.h"
#include "GridView.h"

CColorCell::CColorCell(CSheet* pSheet,CRow* pRow, CColumn* pColumn,std::shared_ptr<CCellProperty> spProperty,CColor color)
	:CTextCell(pSheet,pRow,pColumn,spProperty),
	m_color(color){}


CColor CColorCell::GetColor(){return m_color;}

void CColorCell::PaintBackground(CDC* pDC,CRect rcPaint)
{
	CBrush br(m_color);
	pDC->FillRect(rcPaint,br);
	if(GetFocused()){
		pDC->DrawFocusRect(rcPaint);
	}
}

void CColorCell::OnLButtonClk(MouseEventArgs& e)
{
	COLORREF CustColors[16];
	CHOOSECOLOR cc={0};
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=m_pSheet->GetGridPtr()->m_hWnd;

	cc.rgbResult=m_color;
	cc.lpCustColors=CustColors;
	cc.Flags=CC_FULLOPEN|CC_RGBINIT;
	if(ChooseColor(&cc)){
		if(m_color!=cc.rgbResult){
			m_color=cc.rgbResult;
			m_pSheet->CellPropertyChanged(this,L"value");
		}
	}
}

std::basic_string<TCHAR> CColorCell::GetString()const
{
	std::wstring wstr(L"RGB:");
	wstr.append(boost::lexical_cast<std::wstring>((int)m_color.GetR()));
	wstr.append(L", ");
	wstr.append(boost::lexical_cast<std::wstring>((int)m_color.GetG()));
	wstr.append(L", ");
	wstr.append(boost::lexical_cast<std::wstring>((int)m_color.GetB()));
	return wstr;
}
