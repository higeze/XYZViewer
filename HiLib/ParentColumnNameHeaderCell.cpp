#include "ParentColumnNameHeaderCell.h"
#include "CellProperty.h"
#include "MyRect.h"
#include "MyColor.h"
#include "Sheet.h"
#include "Column.h"
#include "SheetEventArgs.h"

CMenu CParentColumnNameHeaderCell::ContextMenu;
const CRect CParentColumnNameHeaderCell::TRIANGLE_MARGIN=CRect(2,0,2,0);

Sorts CParentColumnNameHeaderCell::GetSort()const
{
	return m_pColumn->GetSort();
}

void CParentColumnNameHeaderCell::PaintContent(CDC* pDC,CRect rcPaint)
{
	switch(GetSort()){
	case Sorts::Up:
	case Sorts::Down:
		{
			CRect rcText(rcPaint);
			rcText.right-=TRIANGLE_WIDTH+TRIANGLE_MARGIN.left+TRIANGLE_MARGIN.right;
			CTextCell::PaintContent(pDC,rcText);
			CRect rcSort(rcPaint);
			rcSort.left=rcText.right;
			PaintSortMark(pDC,rcSort);
			break;
		}
	default:
		CTextCell::PaintContent(pDC,rcPaint);
		break;
	}
}

void CParentColumnNameHeaderCell::PaintSortMark(CDC* pDC,CRect rcPaint)
{
	std::array<CPoint,3> arPoint;

	switch(GetSort()){
	case Sorts::Down:
		{
			int top=(rcPaint.Height()-TRIANGLE_HEIGHT)/2;
			arPoint[0]=CPoint(TRIANGLE_MARGIN.left,top);
			arPoint[1]=CPoint(TRIANGLE_MARGIN.left+TRIANGLE_WIDTH/2,top+TRIANGLE_HEIGHT);
			arPoint[2]=CPoint(TRIANGLE_MARGIN.left+TRIANGLE_WIDTH,top);
			break;
		}
	case Sorts::Up:
		{
			int bottom=(rcPaint.Height()-TRIANGLE_HEIGHT)/2+TRIANGLE_HEIGHT;
			arPoint[0]=CPoint(TRIANGLE_MARGIN.left,bottom);
			arPoint[1]=CPoint(TRIANGLE_MARGIN.left+TRIANGLE_WIDTH/2,bottom-TRIANGLE_HEIGHT);
			arPoint[2]=CPoint(TRIANGLE_MARGIN.left+TRIANGLE_WIDTH,bottom);
			break;
		}

	default:
		return;
		break;
	}
	CPoint ptTopRight=rcPaint.TopLeft();
	for(auto iter=arPoint.begin(),end=arPoint.end();iter!=end;++iter){
		*iter+=ptTopRight;
	}
	HPEN hPen=pDC->SelectPen((HPEN)::GetStockObject(WHITE_PEN));
	HBRUSH hBr = pDC->SelectBrush((HBRUSH)::GetStockObject(LTGRAY_BRUSH));
	::Polygon(*pDC,&arPoint[0],3);
	pDC->SelectPen(hPen);
	pDC->SelectBrush(hBr);

}

CSize CParentColumnNameHeaderCell::GetSortSize()const
{
	switch(GetSort()){
	case Sorts::Up:
	case Sorts::Down:
		return CSize(TRIANGLE_WIDTH+TRIANGLE_MARGIN.left+TRIANGLE_MARGIN.right,
					TRIANGLE_HEIGHT+TRIANGLE_MARGIN.top+TRIANGLE_MARGIN.bottom);
		break;
	default:
		return CSize();
		break;
	}

}

CSize CParentColumnNameHeaderCell::MeasureContentSize(CDC* pDC)
{
	CSize size = CTextCell::MeasureContentSize(pDC);
	CSize sizeTri = GetSortSize();

	size.cx += sizeTri.cx;
	size.cy = max(size.cy, sizeTri.cy);

	return size;
}

CSize CParentColumnNameHeaderCell::MeasureContentSizeWithFixedWidth(CDC* pDC)
{
	CSize size = CTextCell::MeasureContentSizeWithFixedWidth(pDC);
	CSize sizeTri = GetSortSize();

	size.cx += sizeTri.cx;
	size.cy = max(size.cy, sizeTri.cy);

	return size;
}

void CParentColumnNameHeaderCell::OnLButtonClk(MouseEventArgs& e)
{
	m_pSheet->CellLButtonClk(CellEventArgs(this));
}
