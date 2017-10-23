#pragma once
#include <Windows.h>

class CPoint:public tagPOINT
{
public:
	CPoint(){x=0;y=0;}
	CPoint(LONG lX,LONG lY){x=lX;y=lY;}
	//virtual ~CPoint(){}//Because of Struct

	void SetPoint(LONG lX,LONG lY){x=lX;y=lY;}
	void Offset(LONG lXOffset, LONG lYOffset){x+=lXOffset;y+=lYOffset;}
	void Offset(CPoint pt){x+=pt.x;y+=pt.y;}
	CPoint operator -() const {return CPoint(-x, -y);}
	CPoint operator +(CPoint pt) const
	{ return CPoint(x + pt.x, y + pt.y); }
	CPoint operator -(CPoint pt) const
	{ return CPoint(x - pt.x, y - pt.y); }
	CPoint& operator +=(const CPoint& pt)
	{ x+=pt.x;y+=pt.y;return *this; }

	bool operator!=(const CPoint& pt)const
	{return (x!=pt.x||y!=pt.y);}
	operator LPPOINT(){return this;}
};
