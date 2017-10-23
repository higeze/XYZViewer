#include "GDIUIElement.h"

HTHEME CGDIUIElement::m_hButtonTheme = NULL; 

void CTextBlock::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	HFONT hFont = e.DCPtr->SelectFont(*m_spFont);

	CRect rc(GetRect());
	rc.DeflateRect(m_margin);
	//e.DCPtr->Rectangle(rc);
	if(m_spFont->GetEscapement() == 900){
		rc.SetRect(rc.left,rc.bottom,rc.left+rc.Height(),rc.top);
	}
	e.DCPtr->DrawTextExW(const_cast<LPWSTR>(m_wstrText.c_str()),-1,rc,*m_spFormat,NULL);

	e.DCPtr->SelectFont(hFont);
}

void CTextBlock::Measure(CDC* pDC)
{
	if(!m_bVisible){
		m_size = CSize();
		return;
	}

	HFONT hFont = pDC->SelectFont(*m_spFont);
	CRect rc;
	pDC->DrawTextExW(const_cast<LPWSTR>(m_wstrText.c_str()),-1,rc,DT_CALCRECT,NULL);
	rc.InflateRect(m_margin);
	m_size = rc.Size(); 
	pDC->SelectFont(hFont);
}

void CBindTextBlock::Measure(CDC* pDC)
{
	//if(m_getter)std::wcout<<m_getter()<<std::endl;
	if(!m_bVisible || !m_getter){
		m_size = CSize();
		return;
	}

	HFONT hFont = pDC->SelectFont(*m_spFont);
	CRect rc;
	pDC->DrawTextExW(const_cast<LPWSTR>(m_getter().c_str()),-1,rc,DT_CALCRECT,NULL);
	rc.InflateRect(m_margin);
	m_size = rc.Size(); 
	pDC->SelectFont(hFont);
}

void CBindTextBlock::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible || !m_getter)return;

	HFONT hFont = e.DCPtr->SelectFont(*m_spFont);

	CRect rc(GetRect());
	rc.DeflateRect(m_margin);
	if(m_spFont->GetEscapement() == 900){//DrawTextEx is available only if both orientation,escapement!=0
		auto x = rc.left, y = rc.bottom;
		if(*m_spFormat & DT_CENTER){
			y = (rc.bottom + rc.top)/2 + GetMeasuredSize().cx/2;
		}
		e.DCPtr->TextOutW(x, y, const_cast<LPWSTR>(m_getter().c_str()), m_getter().size());
	}else{
		e.DCPtr->DrawTextExW(const_cast<LPWSTR>(m_getter().c_str()),-1,rc,*m_spFormat,NULL);		
	}

	e.DCPtr->SelectFont(hFont);
}


void CCheckBox::Measure(CDC* pDC)
{
	if(!m_bVisible){
		m_size = CSize();
		return;
	}
	m_size.cx = m_widthheight+m_margin.left+m_margin.right;
	m_size.cy = m_widthheight+m_margin.top+m_margin.bottom; 
}

void CCheckBox::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	CRect rc(GetRect());
	rc.DeflateRect(m_margin);
	HTHEME hTheme = CGDIUIElement::GetButtonTheme();
	if(hTheme){
		int nStateId(0);

		switch(m_state){
		case UIElementState::Normal:
			nStateId=(m_getter())?CBS_CHECKEDNORMAL:CBS_UNCHECKEDNORMAL;
			break;
		case UIElementState::Pressed:
			nStateId=(m_getter())?CBS_CHECKEDPRESSED:CBS_UNCHECKEDPRESSED;
			break;
		case UIElementState::Hot:
			nStateId=(m_getter())?CBS_CHECKEDHOT:CBS_UNCHECKEDHOT;
			break;
		case UIElementState::Disabled:
			nStateId=(m_getter())?CBS_CHECKEDDISABLED:CBS_UNCHECKEDDISABLED;
			break;
		default:
			break;
		}

		::DrawThemeBackground(CGDIUIElement::GetButtonTheme(),*(e.DCPtr),BP_CHECKBOX,nStateId,rc,NULL);
	}else{
		UINT uState(DFCS_BUTTONCHECK);

		uState |= (m_getter())?DFCS_CHECKED:0;

		switch(m_state){
		case UIElementState::Normal:
			break;
		case UIElementState::Pressed:
			uState |= DFCS_PUSHED;
			break;
		case UIElementState::Hot:
			uState |= DFCS_HOT;
			break;
		case UIElementState::Disabled:
			uState |= DFCS_INACTIVE;
			break;
		default:
			break;
		}

		::DrawFrameControl(*(e.DCPtr), rc, DFC_BUTTON, uState);

	}


}

void CCheckBox::OnLButtonClk(MouseEventArgs& e)
{
	m_setter(!(m_getter()));
}

void CLineMark::Measure(CDC* pDC)
{
	if(!m_bVisible)return;

	m_size.cx = m_lineLength + m_margin.left + m_margin.right;
	m_size.cy = 2 * m_markRadius + m_margin.top + m_margin.bottom;
}

void CLineMark::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	HPEN hPen = e.DCPtr->SelectPen(*(m_getter()->GetPenPtr()));


	HBRUSH hBrush = e.DCPtr->SelectBrush(*(m_getter()->GetBrushPtr()));

	CRect rc(GetRect());
	rc.DeflateRect(m_margin);

	CPoint ptCenter(rc.CenterPoint());
	//Paint Line
	e.DCPtr->MoveToEx(rc.left, ptCenter.y);
	e.DCPtr->LineTo(rc.right, ptCenter.y);
	//Paint Mark
	e.DCPtr->Ellipse(ptCenter.x - m_markRadius, ptCenter.y - m_markRadius,
					ptCenter.x + m_markRadius, ptCenter.y + m_markRadius);

	e.DCPtr->SelectPen(hPen);
	e.DCPtr->SelectBrush(hBrush);
}

void CPanel::OnLButtonDown(MouseEventArgs& e)
{
	auto iter = boost::find_if(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement)->bool{
		return !!spElement->GetRect().PtInRect(e.Point);
	});
	if(iter!=m_children->end()){
		return (*iter)->OnLButtonDown(e);
	}
}
void CPanel::OnLButtonUp(MouseEventArgs& e)
{
	auto iter = boost::find_if(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement)->bool{
		return !!spElement->GetRect().PtInRect(e.Point);
	});
	if(iter!=m_children->end()){
		return (*iter)->OnLButtonUp(e);
	}
}

void CPanel::OnMouseMove(MouseEventArgs& e)
{
	auto iter = boost::find_if(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement)->bool{
		return !!spElement->GetRect().PtInRect(e.Point);
	});
	if(iter!=m_children->end()){
		if(!m_currentElement){
			(*iter)->OnMouseEnter(e);
			m_currentElement = (*iter);
		}else if(m_currentElement!=(*iter)){
			m_currentElement->OnMouseLeave(e);
			(*iter)->OnMouseEnter(e);
			m_currentElement = (*iter);
		}
		(*iter)->OnMouseMove(e);
	}else{
		if(m_currentElement){
			m_currentElement->OnMouseLeave(e);
		}
	}
}
void CPanel::OnMouseLeave(MouseEventArgs& e)
{
	if(m_currentElement){
		m_currentElement->OnMouseLeave(e);
		m_currentElement=nullptr;
	}
}


void CPanel::OnLButtonDblClk(MouseEventArgs& e)
{
	auto iter = boost::find_if(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement)->bool{
		return !!spElement->GetRect().PtInRect(e.Point);
	});
	if(iter!=m_children->end()){
		return (*iter)->OnLButtonDblClk(e);
	}
}

void CStackPanel::Measure(CDC* pDC)
{
	if(!m_bVisible)return;

	CSize size;

	if(m_orientation==Orientation::Horizontal){
		boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
			spElement->Measure(pDC);
			CSize sizeChild = spElement->GetMeasuredSize();
			size.cx+=sizeChild.cx;
			size.cy=max(size.cy, sizeChild.cy);
		});
	}else if(m_orientation==Orientation::Vertical){
		boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
			spElement->Measure(pDC);
			CSize sizeChild = spElement->GetMeasuredSize();
			size.cx=max(size.cx, sizeChild.cx);
			size.cy+=sizeChild.cy;
		});
	}
		size.cx+= m_margin.left+m_margin.right;
		size.cy+=m_margin.top+m_margin.bottom;
	m_size = size;
}

void CStackPanel::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	CRect rc(GetRect());
	rc.DeflateRect(m_margin);
	if(m_orientation==Orientation::Horizontal){
		int height = 0;
		boost::for_each(*m_children,[&](std::shared_ptr<CGDIUIElement>& spElement){
			CSize size = spElement->GetMeasuredSize();
			height=max(height, size.cy);
		});
			
		
		int left = rc.left;
		boost::for_each(*m_children,[&](std::shared_ptr<CGDIUIElement>& spElement){
			CSize size = spElement->GetMeasuredSize();
			spElement->SetRect(CRect(left, rc.top, left + size.cx, rc.top + height));
			spElement->OnPaint(e);
			left+=size.cx;
		});
	}else if(m_orientation==Orientation::Vertical){
		int width = 0;

		boost::for_each(*m_children,[&](std::shared_ptr<CGDIUIElement>& spElement){
			CSize size = spElement->GetMeasuredSize();
			width = max(width, size.cx);
		});
		int top = rc.top;

		boost::for_each(*m_children,[&](std::shared_ptr<CGDIUIElement>& spElement){
			CSize size = spElement->GetMeasuredSize();
			spElement->SetRect(CRect(rc.left, top, rc.left + width, top + size.cy));
			spElement->OnPaint(e);
			top+=size.cy;
		});
	}
}

void CDockPanel::SetDock(std::shared_ptr<CGDIUIElement> spElem, Dock dock)
{
	m_attachedProperties.insert(std::make_pair(spElem, dock));
}

void CDockPanel::Measure(CDC* pDC)
{
	if(!m_bVisible)return;

	CSize size;

	for(auto& child : *m_children){
		child->Measure(pDC);
		CSize sizeChild = child->GetMeasuredSize();
		size.cx = max(size.cx, sizeChild.cx);
		size.cy += sizeChild.cy;
		//auto iter = m_attachedProperties.find(child);
		//if(iter!=m_attachedProperties.end()){

		//	switch(iter->second){
		//	case Dock::Top:

		//		break;
		//	case Dock::Bottom:
		//		break;
		//	default:
		//		break;
		//	}
		//}
	
	}
	size.cx += m_margin.left + m_margin.right;
	size.cy += m_margin.top + m_margin.bottom;
	m_size = size;
	//if(m_orientation==Orientation::Horizontal){
	//	boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
	//		spElement->Measure(pDC);
	//		CSize sizeChild = spElement->GetMeasuredSize();
	//		size.cx+=sizeChild.cx;
	//		size.cy=max(size.cy, sizeChild.cy);
	//	});
	//}else if(m_orientation==Orientation::Vertical){
	//	boost::for_each(*m_children,[&](const std::shared_ptr<CGDIUIElement>& spElement){
	//		spElement->Measure(pDC);
	//		CSize sizeChild = spElement->GetMeasuredSize();
	//		size.cx=max(size.cx, sizeChild.cx);
	//		size.cy+=sizeChild.cy;
	//	});
	//}
	//	size.cx+= m_margin.left+m_margin.right;
	//	size.cy+=m_margin.top+m_margin.bottom;
	//m_size = size;
}

void CDockPanel::OnPaint(PaintEventArgs& e)
{
	if(!m_bVisible)return;

	CRect rc(GetRect());
	CRect rcDefrate(rc);
	rcDefrate.DeflateRect(m_margin);

	//Separate tops & bottoms
	std::vector<std::shared_ptr<CGDIUIElement>> tops;
	std::vector<std::shared_ptr<CGDIUIElement>> bottoms;

	for(auto& child : *m_children){
		auto iter = m_attachedProperties.find(child);
		if(iter!=m_attachedProperties.end()){

			switch(iter->second){
			case Dock::Top:
			{
				tops.push_back(iter->first);
				break;
			}
			case Dock::Bottom:
			{
				bottoms.push_back(iter->first);
			}
			default:
				break;
			}
		}
	}
	//Paint Top elements
	int top = rcDefrate.top;
	for(auto& child : tops){
		CSize size = child->GetMeasuredSize();
		if(child->GetHorizontalAlignment()==HorizontalAlignment::Right){
			child->SetRect(CRect(rcDefrate.right - size.cx, top, rcDefrate.right, top + size.cy));			
		}else{
			child->SetRect(CRect(rcDefrate.left, top, rcDefrate.left + size.cx, top + size.cy));
		}
		child->OnPaint(e);
		top += size.cy;
	}

	//Paint bottom elements
	int bottom = rcDefrate.bottom;
	for(auto it = bottoms.rbegin();it!=bottoms.rend();it++){
		CSize size = (*it)->GetMeasuredSize();
		if((*it)->GetHorizontalAlignment()==HorizontalAlignment::Right){
			(*it)->SetRect(CRect(rcDefrate.right - size.cx, bottom - size.cy, rcDefrate.right, bottom));
		}else{
			(*it)->SetRect(CRect(rcDefrate.left, bottom - size.cy, rcDefrate.left + size.cx, bottom));
		}
		(*it)->OnPaint(e);
		bottom -= size.cy;
	}

	//if(m_orientation==Orientation::Horizontal){
	//	int height = 0;
	//	boost::for_each(*m_children,[&](std::shared_ptr<CGDIUIElement>& spElement){
	//		CSize size = spElement->GetMeasuredSize();
	//		height=max(height, size.cy);
	//	});
	//		
	//	
	//	int left = rc.left;
	//	boost::for_each(*m_children,[&](std::shared_ptr<CGDIUIElement>& spElement){
	//		CSize size = spElement->GetMeasuredSize();
	//		spElement->SetRect(CRect(left, rc.top, left + size.cx, rc.top + height));
	//		spElement->OnPaint(e);
	//		left+=size.cx;
	//	});
	//}else if(m_orientation==Orientation::Vertical){
	//	int width = 0;

	//	boost::for_each(*m_children,[&](std::shared_ptr<CGDIUIElement>& spElement){
	//		CSize size = spElement->GetMeasuredSize();
	//		width = max(width, size.cx);
	//	});
	//	int top = rc.top;

	//	boost::for_each(*m_children,[&](std::shared_ptr<CGDIUIElement>& spElement){
	//		CSize size = spElement->GetMeasuredSize();
	//		spElement->SetRect(CRect(rc.left, top, rc.left + width, top + size.cy));
	//		spElement->OnPaint(e);
	//		top+=size.cy;
	//	});
	//}
}


