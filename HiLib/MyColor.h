#pragma once
#include <Windows.h>
#include <WinGDI.h>
#include "MyString.h"
#include "MyFriendSerializer.h"

class CRGB
{
private:
	int m_R;
	int m_G;
	int m_B;
public:
	CRGB(int R,int G,int B)
	{
		SetR(R);SetG(G);SetB(B);
	}
	virtual ~CRGB(){}
	int GetR()const{return m_R;}
	void SetR(int R){m_R=max(min(R,255),0);}
	int GetG()const{return m_G;}
	void SetG(int G){m_G=max(min(G,255),0);}
	int GetB()const{return m_B;}
	void SetB(int B){m_B=max(min(B,255),0);}
};

class CHSV
{
private:
	int m_H;
	int m_S;
	int m_V;
public:
	CHSV(int H,int S,int V)
	{
		SetH(H);SetS(S);SetV(V);
	}
	virtual ~CHSV(){}
	int GetH()const{return m_H;}
	void SetH(int H)
	{
		H%=360;
		m_H=(H>=0)?H:H+360;
	}
	int GetS()const{return m_S;}
	void SetS(int S){m_S=max(min(S,255),0);}
	int GetV()const{return m_V;}
	void SetV(int V){m_V=max(min(V,255),0);}

};

CHSV RGB2HSV(const CRGB& rgb);

CRGB HSV2RGB(const CHSV& hsv);

class CColor
{
private:
	COLORREF m_cr;
public:
	CColor():m_cr(0){}
	CColor(COLORREF cr):m_cr(cr){}
	CColor(const CColor& cr):m_cr(cr.m_cr){}	
	virtual ~CColor(){}

	operator COLORREF() const{return m_cr;}

	CRGB GetRGB()const{return CRGB(GetRValue(m_cr),GetGValue(m_cr),GetBValue(m_cr));}
	CHSV GetHSV()const{return RGB2HSV(GetRGB());}
	void SetRGB(const CRGB& rgb)
	{
		m_cr=RGB(rgb.GetR(),rgb.GetG(),rgb.GetB());
	}
	void SetHSV(const CHSV& hsv)
	{
		CRGB rgb(HSV2RGB(hsv));
		SetRGB(rgb);
	}

	BYTE GetR()const{return GetRValue(m_cr);}
	BYTE GetG()const{return GetGValue(m_cr);}
	BYTE GetB()const{return GetBValue(m_cr);}
	//void SetR(BYTE byte){m_cr=RGB(max(min(byte,255),0),GetGValue(m_cr),GetBValue(m_cr));}
	//void SetG(BYTE byte){m_cr=RGB(GetRValue(m_cr),max(min(byte,255),0),GetBValue(m_cr));}
	//void SetB(BYTE byte){m_cr=RGB(GetRValue(m_cr),GetGValue(m_cr),max(min(byte,255),0));}

	FRIEND_SERIALIZER
	template <class Archive>
	void save(Archive& ar)const
	{
		int red(GetR()),green(GetG()),blue(GetB());
		ar("Red", red);
		ar("Green", green);
		ar("Blue", blue);
	}

	template <class Archive>
	void load(Archive& ar)
	{
		int red(0),green(0),blue(0);
		ar("Red", red);
		ar("Green", green);
		ar("Blue", blue);
		m_cr=RGB(red,green,blue);
	}
};

typedef std::shared_ptr<CColor> ColorPtr;