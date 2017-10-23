#pragma once

#include "MyPoint.h"
#include "MySize.h"
#include "MyFriendSerializer.h"

/*********/
/* CRect */
/*********/
class CRect:public tagRECT
{
public:
	//Constructor
	CRect()
	{
		SetRect(0,0,0,0);
	}

	CRect(LONG l,LONG t,LONG r,LONG b)
	{
		SetRect(l,t,r,b);
	}

	CRect(const RECT& rc)
	{
		SetRect(rc.left,rc.top,rc.right,rc.bottom);
	}

	CRect(const CSize& size)
	{
		SetRect(0,0,size.cx,size.cy);
	}

	//virtual ~CRect(){}//Because of Struct
	//Member function
	void SetRect(LONG l,LONG t,LONG r,LONG b)
	{
		::SetRect(this,l,t,r,b);
	}
	void MoveToX(int x){right=x+right-left;left=x;}
	void MoveToY(int y){bottom=y+bottom-top;top=y;}
	void MoveToXY(int x,int y)
	{
		right+=x-left;
		bottom+=y-top;
		left=x;
		top=y;
	}
	void MoveToXY(POINT pt)
	{
		MoveToXY(pt.x,pt.y);
	}
	void OffsetRect(LONG lX,LONG lY){::OffsetRect(this,lX,lY);}
	void OffsetRect(CPoint pt){::OffsetRect(this,pt.x,pt.y);}

	operator LPRECT(){return this;}
	operator LPCRECT() const{return this;};

	LONG Width()const{return right-left;}
	LONG Height()const{return bottom-top;}
	
	void InflateRect(int x,int y){::InflateRect(this, x, y);}
	void DeflateRect(int x,int y){::InflateRect(this, -x, -y);}
	BOOL PtInRect(POINT pt){return ::PtInRect(this,pt);}
	CPoint& TopLeft(){return *((CPoint*)this);}
	const CPoint& TopLeft()const{return *((CPoint*)this);}

	CPoint TopRight(){return CPoint(right,top);}
	CPoint CenterPoint() const
	{
		return CPoint((left + right) / 2, (top + bottom) / 2);
	}

	CSize Size() const
	{
		return CSize(right - left, bottom - top);
	}

	/*
	template<class char_type>
	friend std::basic_ostream<char_type>& operator <<(std::basic_ostream<char_type> &os, const CRect& rc)
	{
		os<<rc.left<<TCHAR_CAST(char_type,',')
			<<rc.top<<TCHAR_CAST(char_type,',')
			<<rc.right<<TCHAR_CAST(char_type,',')
			<<rc.bottom;
		return os;
	}

	template<class char_type>
	friend std::basic_istream<char_type>& operator >>(std::basic_istream<char_type> &is, CRect& rc)
	{
		std::basic_string<char_type> str;
		is>>str;

		std::vector<std::basic_string<char_type>> vstr;

		std::tr1::basic_regex<char_type> rx(L"\\s*\\,\\s*");
		std::tr1::regex_token_iterator<std::basic_string<char_type>::const_iterator> iter(str.begin(),str.end(),rx,-1);
		std::tr1::regex_token_iterator<std::basic_string<char_type>::const_iterator> end;
		while(iter!=end){
			vstr.push_back(*iter);
			++iter;
		}
		if(vstr.size()==4){
			rc.SetRect(myboost::lexical_cast<LONG>(vstr[0]),
				myboost::lexical_cast<LONG>(vstr[1]),
				myboost::lexical_cast<LONG>(vstr[2]),
				myboost::lexical_cast<LONG>(vstr[3]));
		}
		return is;
	}
	*/
	void InflateRect(LPCRECT lpRect)
	{
		left -= lpRect->left;
		top -= lpRect->top;
		right += lpRect->right;
		bottom += lpRect->bottom;
	}
	void DeflateRect(LPCRECT lpRect)
	{
		left += lpRect->left;
		top += lpRect->top;
		right -= lpRect->right;
		bottom -= lpRect->bottom;
	}

	BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2){return ::IntersectRect(this, lpRect1, lpRect2);}

	CRect operator+(LPCRECT lpRect)const
	{
		CRect rc(*this);
		rc.InflateRect(lpRect);
		return rc;
	}
	CRect& operator+=(LPCRECT lpRect)
	{
		this->InflateRect(lpRect);
		return *this;
	}

	CRect operator-(LPCRECT lpRect)const
	{
		CRect rc(*this);
		rc.DeflateRect(lpRect);
		return rc;
	}
	CRect& operator-=(LPCRECT lpRect)
	{
		this->DeflateRect(lpRect);
		return *this;
	}
	BOOL operator==(const RECT& rc)const{return ::EqualRect(this,&rc);}
	BOOL operator!=(const RECT& rc)const{return !::EqualRect(this,&rc);}	
	void operator &=(const RECT& rect){::IntersectRect(this, this, &rect);}

	FRIEND_SERIALIZER
	template<class Archive>
	void serialize(Archive& ar)
	{
		ar("Left",left);
		ar("Top",top);
		ar("Right",right);
		ar("Bottom",bottom);
	}
};
typedef std::shared_ptr<CRect> SPRect;
typedef std::shared_ptr<CRect> CRectPtr;