#pragma once
#include "MyRect.h"
#include "MySize.h"

#include "MyDC.h"
#include "MyFont.h"
#include "MyPen.h"

#include "UIElement.h"

enum class HorizontalAlignment{
	Left,
	Center,
	Right
};

class CGDIUIElement:public virtual CUIElement
{
private:
	static HTHEME m_hButtonTheme; 
public:
	static HTHEME GetButtonTheme()
	{
		if(!m_hButtonTheme){
			m_hButtonTheme = ::OpenThemeData(NULL,L"Button");
		}
		return m_hButtonTheme;
	}
protected:
	CGDIUIElement* m_pParent;
	CSize m_size;
	CRect m_rect;
	CRect m_margin;
	bool m_bVisible;
	HorizontalAlignment m_horizontalAlignment;

public:
	CGDIUIElement(CGDIUIElement* pParent = nullptr)
		:CUIElement(),m_pParent(pParent), m_size(CSize()), m_rect(CRect()), m_margin(CRect()), m_bVisible(true),m_horizontalAlignment(HorizontalAlignment::Left){}
	virtual ~CGDIUIElement(){}
	virtual CRect GetRect()const{return m_rect;}
	virtual void SetRect(const CRect& rect){m_rect=rect;}

	virtual CRect GetMargin()const{return m_margin;}
	virtual void SetMargin(const CRect& margin){m_margin=margin;}

	virtual bool GetVisible()const{return m_bVisible;}
	virtual void SetVisible(const bool& bVisible){m_bVisible=bVisible;}

	virtual void Measure(CDC* pDC){}
	virtual CSize GetMeasuredSize()const{return m_bVisible?m_size:CSize();}

	virtual void OnPaint(PaintEventArgs& e){}

	virtual CGDIUIElement* GetParent()const{return m_pParent;}

	HorizontalAlignment GetHorizontalAlignment()const{return m_horizontalAlignment;}
	void SetHorizontalAlignment(const HorizontalAlignment& ha){m_horizontalAlignment = ha;}
};

class CBindTextBlock:public CGDIUIElement
{
protected:
	std::function<std::wstring()> m_getter;
	std::function<void(const std::wstring&)> m_setter;

	std::shared_ptr<CFont> m_spFont;
	std::shared_ptr<UINT> m_spFormat;
public:
	CBindTextBlock(CGDIUIElement* pParent, std::shared_ptr<CFont> spFont, std::shared_ptr<UINT> spFormat, 
		std::function<std::wstring()> getter=nullptr, std::function<void(const std::wstring&)> setter = nullptr)
		:CGDIUIElement(pParent), m_spFont(spFont), m_spFormat(spFormat), m_getter(getter), m_setter(setter){}
	void SetGetter(std::function<std::wstring()> getter){m_getter = getter;}
	void SetSetter(std::function<void(const std::wstring&)> setter){m_setter = setter;}
	
	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);
};

class CTextBlock:public CGDIUIElement
{
protected:
	std::shared_ptr<CFont> m_spFont;
	std::shared_ptr<UINT> m_spFormat;
	std::wstring m_wstrText;
public:
	CTextBlock(CGDIUIElement* pParent, std::shared_ptr<CFont> spFont, std::shared_ptr<UINT> spFormat, std::wstring wstrText)
		:CGDIUIElement(pParent), m_spFont(spFont), m_spFormat(spFormat), m_wstrText(wstrText){}
	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);
	void SetText(const std::wstring& wstrText){m_wstrText = wstrText;}
};

class CCheckBox:public CGDIUIElement
{
private:
	std::function<bool()> m_getter;
	std::function<void(const bool&)> m_setter;
	int m_widthheight;
	std::shared_ptr<bool> m_spbool;
public:
	CCheckBox(CGDIUIElement* pParent, std::function<bool()> getter, std::function<void(const bool&)> setter)
		:CGDIUIElement(pParent), m_getter(getter), m_setter(setter), m_widthheight(20){}

	bool GetBool()const{return *m_spbool;}
	void SetBool(const bool& b){(*m_spbool)=b;}

	virtual void Measure(CDC* pDC);
	virtual void OnLButtonClk(MouseEventArgs& e);
	virtual void OnPaint(PaintEventArgs& e);
};

#include "SeriesProperty.h"

class CLineMark:public CGDIUIElement
{
private:
	std::function<std::shared_ptr<CSeriesProperty>()> m_getter;
	std::function<void(const std::shared_ptr<CSeriesProperty>&)> m_setter;

	static const int m_lineLength = 16;
	static const int m_markRadius = 3;
	//std::shared_ptr<CSeriesProperty> m_spSeriesProperty;

public:
	CLineMark(CGDIUIElement* pParent, std::function<std::shared_ptr<CSeriesProperty>()> getter, std::function<void(const std::shared_ptr<CSeriesProperty>&)> setter)
		:CGDIUIElement(pParent), m_getter(getter), m_setter(setter){}

	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);
	virtual void SetState(const UIElementState::Type& state)
	{
		if(GetState()!=UIElementState::Hot && state == UIElementState::Hot){
			auto spPen = m_getter()->GetPenPtr();
			m_getter()->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()*2, spPen->GetColor()));
		}else if(GetState()==UIElementState::Hot && state != UIElementState::Hot){
			auto spPen = m_getter()->GetPenPtr();
			m_getter()->SetPenPtr(std::make_shared<CPen>(spPen->GetPenStyle(), spPen->GetWidth()/2, spPen->GetColor()));
		}

		CGDIUIElement::SetState(state);
	}
};

namespace Orientation
{
	enum Type{
		Vertical,
		Horizontal
	};
}

class CPanel:public CGDIUIElement
{
protected:
	std::shared_ptr<CGDIUIElement> m_currentElement;
	std::shared_ptr<std::vector<std::shared_ptr<CGDIUIElement>>> m_children;
public:
	CPanel(CGDIUIElement* pParent):CGDIUIElement(pParent),m_children(std::make_shared<std::vector<std::shared_ptr<CGDIUIElement>>>()),m_currentElement(nullptr){}
	
	std::shared_ptr<std::vector<std::shared_ptr<CGDIUIElement>>> GetChildrenPtr(){return m_children;}
	
	virtual void OnLButtonDown(MouseEventArgs& e);
	virtual void OnLButtonUp(MouseEventArgs& e);
	virtual void OnMouseMove(MouseEventArgs& e);
//	virtual void OnMouseEnter(MouseEventArgs& e);
	virtual void OnMouseLeave(MouseEventArgs& e);

//	virtual void OnLButtonClk(MouseEventArgs& e);
	virtual void OnLButtonDblClk(MouseEventArgs& e);
//	std::shared_ptr<std::vector<std::shared_ptr<CGDIUIElement>>> GetChildrenPtr(){return m_children;}

};

class CStackPanel:public CPanel
{
protected:
	Orientation::Type m_orientation;
public:
	CStackPanel(CGDIUIElement* pParent):CPanel(pParent){}

	void SetOrientation(const Orientation::Type& orientation){m_orientation = orientation;}
		
	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);
};

//TODO Only Top and Bottom are implemented. Need to implement left and right;
enum class Dock {Top, Bottom};
class CDockPanel:public CPanel
{
protected:
	std::unordered_map<std::shared_ptr<CGDIUIElement>, Dock> m_attachedProperties;
public:
	CDockPanel(CGDIUIElement* pParent):CPanel(pParent){}

	void SetDock(std::shared_ptr<CGDIUIElement> spElem, Dock dock);
		
	virtual void Measure(CDC* pDC);
	virtual void OnPaint(PaintEventArgs& e);
};


//Image
template<WORD TIDB>
class CImage:public CGDIUIElement
{
private:
	enum{IDB=TIDB};
	CBitmap m_bitmap;
public:
	CImage(CGDIUIElement* pChart)
		:CGDIUIElement(pChart),
		m_bitmap(::LoadBitmap( ::GetModuleHandle(NULL), MAKEINTRESOURCE(IDB))){}

	virtual void Measure(CDC* pDC)
	{
		CSize size = m_bitmap.GetSize();
		size.cx += m_margin.left + m_margin.right;
		size.cy += m_margin.top + m_margin.bottom;
		m_size = size;
	}
	
	virtual void OnPaint(PaintEventArgs& e)
	{      
        CDC dc(::CreateCompatibleDC(*e.DCPtr));
        dc.SelectBitmap(m_bitmap);
        CRect rc = GetRect();
        ::BitBlt(*e.DCPtr, rc.left + m_margin.left, rc.top + m_margin.top, rc.Width(), rc.Height(), dc, 0, 0, SRCCOPY);
	}
};