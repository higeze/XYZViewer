#include "UIElement.h"

void CUIElement::OnLButtonDown(MouseEventArgs& e)
{
	SetState(UIElementState::Pressed);
}

void CUIElement::OnLButtonUp(MouseEventArgs& e)
{
	if(m_state==UIElementState::Pressed || m_state==UIElementState::PressedLeave){
		OnLButtonClk(e);
	}
	SetState(UIElementState::Hot);
}

void CUIElement::OnMouseMove(MouseEventArgs& e)
{
	switch(m_state)
	{
	//Not in element
	case UIElementState::None:
	case UIElementState::Normal:
	case UIElementState::PressedLeave:
		OnMouseEnter(e);
		break;
	//Already in element
	case UIElementState::Hot:
	case UIElementState::Pressed:
		break;
	//Disabled
	case UIElementState::Disabled:
		break;
	}
	//if((m_state==UIElementState::Pressed || m_state==UIElementState::PressedLeave) && e.Flags==MK_LBUTTON){
	//	SetState(UIElementState::Pressed);
	//}else{
	//	SetState(UIElementState::Hot);
	//}
}

void CUIElement::OnMouseEnter(MouseEventArgs& e)
{
	if(m_state==UIElementState::PressedLeave && e.Flags==MK_LBUTTON){
		SetState(UIElementState::Pressed);
	}else{
		SetState(UIElementState::Hot);
	}
}

void CUIElement::OnMouseLeave(MouseEventArgs& e)
{
	if(m_state==UIElementState::Pressed && e.Flags==MK_LBUTTON){
		SetState(UIElementState::PressedLeave);	
	}else{
		SetState(UIElementState::Normal);
	}
}