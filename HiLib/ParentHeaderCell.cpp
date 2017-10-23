#include "ParentHeaderCell.h"
#include "CellProperty.h"
#include "MyRect.h"
#include "MyColor.h"
#include "InplaceEdit.h"
#include "Sheet.h"
#include "GridView.h"



void CParentHeaderCell::TwoColorGradientFill(CDC* pDC,CRect rc,CColor& cr1,CColor& cr2, ULONG ulMode)
{
	// Create an array of TRIVERTEX structures that describe 
	// positional and color values for each vertex. For a rectangle, 
	// only two vertices need to be defined: upper-left and lower-right. 
	TRIVERTEX vertex[2] ;
	vertex[0].x     = rc.left;
	vertex[0].y     = rc.top;
	vertex[0].Red   = (cr1.GetR())<<8;
	vertex[0].Green = (cr1.GetG())<<8;
	vertex[0].Blue  = (cr1.GetB())<<8;
	vertex[0].Alpha = 0x0000;

	vertex[1].x     = rc.right;
	vertex[1].y     = rc.bottom; 
	vertex[1].Red   = (cr2.GetR())<<8;
	vertex[1].Green = (cr2.GetG())<<8;
	vertex[1].Blue  = (cr2.GetB())<<8;
	vertex[1].Alpha = 0x0000;

	// Create a GRADIENT_RECT structure that 
	// references the TRIVERTEX vertices. 
	GRADIENT_RECT gRect;
	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	// Draw a shaded rectangle. 
	GradientFill(*pDC, vertex, 2, &gRect, 1, ulMode);	
}

void CParentHeaderCell::PaintBackground(CDC* pDC,CRect rcPaint)
{
	if(m_pEdit){
		CRect rcContent(InnerBorder2Content(CenterBorder2InnerBorder(GetRect())));
		CRect rcCurEdit(m_pSheet->GetGridPtr()->GetScreenToClientRect(m_pEdit->GetWindowRect()));
		m_pSheet->GetGridPtr()->SetEditRect(rcContent);
		if(rcContent!=rcCurEdit){
			m_pEdit->MoveWindow(rcContent,FALSE);
		}
	}

	if(GetChecked()){
		std::shared_ptr<CBrush> spBrush;
		spBrush=m_spProperty->GetCheckedBrushPtr();
		pDC->FillRect(rcPaint,*spBrush);
	}else{

		if(auto p=std::dynamic_pointer_cast<CHeaderProperty>(m_spProperty)){
				CColor crTop(p->GetTopColor());
				CColor crBottom(p->GetBottomColor());
				CHSV hsvTop(crTop.GetHSV());
				CHSV hsvBottom(crBottom.GetHSV());
			if(m_state==UIElementState::Pressed){
				hsvTop.SetV(hsvTop.GetV()-20);
				hsvBottom.SetV(hsvBottom.GetV()-20);
				crTop.SetHSV(hsvTop);
				crBottom.SetHSV(hsvBottom);

				TwoColorGradientFill(pDC,rcPaint,crTop,crBottom,GRADIENT_FILL_RECT_V);
			}else if(m_state==UIElementState::Hot){
				hsvTop.SetV(hsvTop.GetV()+20);
				hsvBottom.SetV(hsvBottom.GetV()+20);
				crTop.SetHSV(hsvTop);
				crBottom.SetHSV(hsvBottom);
			
				TwoColorGradientFill(pDC,rcPaint,crTop,crBottom,GRADIENT_FILL_RECT_V);

			}else{
				TwoColorGradientFill(pDC,rcPaint,crTop,crBottom,GRADIENT_FILL_RECT_V);
			}
		}
	}

}