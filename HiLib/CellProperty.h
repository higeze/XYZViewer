#pragma once
#include "MyFont.h"
#include "MyColor.h"
#include "MyBrush.h"
#include "MyRect.h"
#include "MyPen.h"

#include "MyFriendSerializer.h"

enum class PicturePositon
{
	Fill,
	Fit,
	Stretch,
	Tile,
	Center
};

class CBackgroundProperty
{
public:
	std::shared_ptr<CBrush> m_brush;
	std::shared_ptr<bool> m_usePicture;
	std::shared_ptr<std::wstring> m_picturePath;
	std::shared_ptr<PicturePositon> m_picturePosition;

	CBackgroundProperty()
		:m_brush(std::make_shared<CBrush>(RGB(246, 246, 246))),//asbestos
		m_usePicture(std::make_shared<bool>(false)),
		m_picturePath(std::make_shared<std::wstring>()),
		m_picturePosition(std::make_shared<PicturePositon>(PicturePositon::Center)){}
	virtual ~CBackgroundProperty(){}

	FRIEND_SERIALIZER

    template <class Archive>
    void serialize(Archive& ar)
    {
		ar("BackgroundBrush",m_brush);
		ar("UsePicture",m_usePicture);
		ar("PicturePath",m_picturePath);
		ar("PicturePosition",m_picturePosition);
    }
};

class CDrawTextProperty
{
public:
	UINT m_uFormat;
	CDrawTextProperty():m_uFormat(0){}
	CDrawTextProperty(UINT uFormat/*,DRAWTEXTPARAMS dtp*/):m_uFormat(uFormat)/*,m_dtp(dtp)*/{}
	virtual ~CDrawTextProperty(){}
	//DRAWTEXTPARAMS m_dtp;
	UINT GetFormat()const{return m_uFormat;}
	//DRAWTEXTPARAMS GetDrawTextParams()const{return m_dtp;}

	FRIEND_SERIALIZER
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar("Format",m_uFormat);
    }

};
typedef std::shared_ptr<CDrawTextProperty> DrawTextPropertyPtr;

class CCellProperty
{
private:
	SPFont m_spFont;
	SPPen m_spPen;
	SPPen m_spPenFocused;
	SPBrush m_spBrBackground;
	SPBrush m_spBrFocused;
	SPBrush m_spBrSelected;
	SPBrush m_spBrChecked;

	//DrawTextPropertyPtr m_spDrawTextProperty;
	SPRect m_spPadding;

public:
	CCellProperty()
		//:m_spFont(std::make_shared<CFont>(8,L"Arial")),
		//m_spPen(std::make_shared<CPen>(PS_SOLID,2,RGB(150,150,150))),
		//m_spBrBackground(std::make_shared<CBrush>(RGB(255,255,255))),
		//m_spBrFocused(std::make_shared<CBrush>(RGB(195,224,226))),
		//m_spBrSelected(std::make_shared<CBrush>(RGB(0,128,255))),
		//m_spBrChecked(std::make_shared<CBrush>(RGB(255,255,0))),
		//m_spDrawTextProperty(std::make_shared<CDrawTextProperty>(DT_LEFT|DT_TOP|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL)),
		//m_spPadding(std::make_shared<CRect>(2,2,2,2)){};
		:m_spFont(std::make_shared<CFont>(9,L"Meiryo UI")),
		m_spPen(std::make_shared<CPen>(PS_SOLID,1,RGB(221,206,188))),
		m_spPenFocused(std::make_shared<CPen>(PS_SOLID, 1, RGB(22, 160, 133))),
		m_spBrBackground(std::make_shared<CBrush>(RGB(246,246,246))),
		m_spBrFocused(std::make_shared<CBrush>(RGB(195,224,226))),
		m_spBrSelected(std::make_shared<CBrush>(RGB(195,224,226)/*RGB(52,152,219)*/)),
		m_spBrChecked(std::make_shared<CBrush>(RGB(241,196,15))),
		//m_spDrawTextProperty(std::make_shared<CDrawTextProperty>(DT_LEFT|DT_TOP|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL)),
		m_spPadding(std::make_shared<CRect>(2,2,2,2)){};

	CCellProperty(
		SPFont spFont,
		SPPen spPen,
		SPPen spPenFocused,
		SPBrush spBrBackground,
		SPBrush spBrFocused,
		SPBrush spBrSelected,
		SPBrush spBrChecked,
		DrawTextPropertyPtr spDrawTextProperty,
		SPRect spPadding)
		:m_spFont(spFont),
		m_spPen(spPen),
		m_spPenFocused(spPenFocused),
		m_spBrBackground(spBrBackground),
		m_spBrFocused(spBrFocused),
		m_spBrSelected(spBrSelected),
		m_spBrChecked(spBrChecked),
		//m_spDrawTextProperty(spDrawTextProperty),
		m_spPadding(spPadding){};

	virtual ~CCellProperty(){}

	virtual std::shared_ptr<CFont> GetFontPtr()const{return m_spFont;}
	virtual std::shared_ptr<CPen> GetPenPtr()const{return m_spPen;}
	virtual std::shared_ptr<CPen> GetFocusedPenPtr()const{return m_spPenFocused;}
	virtual std::shared_ptr<CBrush> GetBackgroundBrushPtr()const{return m_spBrBackground;}
	virtual std::shared_ptr<CBrush> GetFocusedBrushPtr()const{return m_spBrFocused;}
	virtual std::shared_ptr<CBrush> GetSelectedBrushPtr()const{return m_spBrSelected;}
	virtual std::shared_ptr<CBrush> GetCheckedBrushPtr()const{return m_spBrChecked;}
	//virtual std::shared_ptr<CDrawTextProperty> GetDrawTextPropertyPtr()const{return m_spDrawTextProperty;}
	//virtual UINT GetFormat()const{return m_spDrawTextProperty->GetFormat();}
	virtual CRect GetPadding()const{return *m_spPadding;}
	virtual std::shared_ptr<CRect> GetPaddingPtr(){return m_spPadding;}

	FRIEND_SERIALIZER
	template <class Archive>
    void serialize(Archive& ar)
    {
		ar("Font",m_spFont);
		ar("Pen",m_spPen);
		ar("FocusedPen",m_spPenFocused);
		ar("BackgroundBrush",m_spBrBackground);
		ar("FocusedBrush",m_spBrFocused);
		ar("SelectedBrush",m_spBrSelected);
		ar("CheckedBrush",m_spBrChecked);
		//ar("DrawTextProperty",m_spDrawTextProperty);
		ar("Padding",m_spPadding);
    }

};

typedef std::shared_ptr<CCellProperty> CellPropertyPtr;

class CHeaderProperty:public CCellProperty
{
private:
	CColor m_crTop;
	CColor m_crBottom;
public:
	//CHeaderProperty():CCellProperty(),m_crTop(RGB(206,185,205)),m_crBottom(RGB(233,158,231)){}
	CHeaderProperty():
	  CCellProperty(
		std::make_shared<CFont>(9,L"Meiryo UI"),
		std::make_shared<CPen>(PS_SOLID,1,RGB(211,206,188)),
		std::make_shared<CPen>(PS_SOLID, 1, RGB(22, 160, 133)),
		std::make_shared<CBrush>(RGB(239,239,232)),
		std::make_shared<CBrush>(RGB(195,224,226)),
		std::make_shared<CBrush>(RGB(195,224,226)),
		std::make_shared<CBrush>(RGB(241,196,15)),
		std::make_shared<CDrawTextProperty>(DT_LEFT|DT_TOP|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL),
		std::make_shared<CRect>(2,2,2,2)),
		m_crTop(RGB(232,232,232)),m_crBottom(RGB(232,232,242)){}
	virtual ~CHeaderProperty(){}
	CColor GetTopColor()const
	{
		return m_crTop;
	}
	CColor GetBottomColor()const
	{
		return m_crBottom;
	}

	FRIEND_SERIALIZER
    template <class Archive>
    void serialize(Archive& ar)
    {
		CCellProperty::serialize(ar);
		ar("TopColor",m_crTop);
		ar("BottomColor",m_crBottom);
    }
};

typedef std::shared_ptr<CHeaderProperty> HeadersPropertyPtr;
