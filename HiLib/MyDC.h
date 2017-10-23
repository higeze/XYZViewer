#pragma once
#include "MyRect.h"
#include "MyGdiObj.h"
#include "MyWin32.h"

class CFont;
class CBrush;
class CPen;

class CDC
{
protected:
	HDC m_hDC;
public:
	bool IsNull() const { return (m_hDC == NULL); }

	HDC GetHDC()const{return m_hDC;}
	CDC(HDC hDC=NULL):m_hDC(hDC){}
	CDC(const CDC&) = delete;
	CDC& operator=(const CDC&) = delete;

	virtual ~CDC()
	{
		if(m_hDC!=NULL){
			HDC hDC=m_hDC;
			m_hDC=NULL;
			::DeleteDC(hDC);
		}
	}

	void Attach(HDC hDC)
	{
		if(m_hDC!= NULL&& m_hDC!=hDC){
			::DeleteDC(m_hDC);
		}
		m_hDC = hDC;
	}
	HDC Detach()
	{
		HDC hDC=m_hDC;
		m_hDC=NULL;
		return hDC;
	}
	operator HDC()const{return m_hDC;}


	CPoint MoveToEx(CPoint pt)
	{
		CPoint ptLast;
		::MoveToEx(m_hDC,pt.x,pt.y,&ptLast);
		return ptLast;	
	}

	CPoint MoveToEx(int x, int y)
	{
		CPoint ptLast;
		::MoveToEx(m_hDC,x,y,&ptLast);
		return ptLast;	
	}
	BOOL LineTo(CPoint pt){return ::LineTo(m_hDC,pt.x,pt.y);}
	BOOL LineTo(int x, int y){return ::LineTo(m_hDC,x,y);}
	BOOL Rectangle(CRect rc){return ::Rectangle(m_hDC,rc.left,rc.top,rc.right,rc.bottom);}
	BOOL RoundRect(CRect rc, int nWidth, int nHeight){return ::RoundRect(m_hDC, rc.left, rc.top, rc.right, rc.bottom, nWidth, nHeight);}
	int SaveDC(){return ::SaveDC(m_hDC);}
	int RestoreDC(int nSavedDC){return ::RestoreDC(m_hDC,nSavedDC);}
	HGDIOBJ SelectObject(HGDIOBJ h){return ::SelectObject(m_hDC,h);}
	HBITMAP SelectBitmap(HBITMAP h){return (HBITMAP)::SelectObject(m_hDC,h);}
	HFONT SelectFont(HFONT hFont){return (HFONT)::SelectObject(m_hDC,hFont);}
	HBRUSH SelectBrush(HBRUSH hBrush){return (HBRUSH)::SelectObject(m_hDC,hBrush);}
	HPEN SelectPen(HPEN hPen){return (HPEN)::SelectObject(m_hDC,hPen);}
	
	int SetBkMode(int mode){return ::SetBkMode(m_hDC,TRANSPARENT);}
	int FillRect(const CRect& rc,HBRUSH hBr){return ::FillRect(m_hDC,&rc,hBr);}
	int DrawFocusRect(const CRect& rc){return ::DrawFocusRect(m_hDC,&rc);}
	COLORREF SetTextColor(COLORREF color){return ::SetTextColor(m_hDC,color);}
	BOOL BitBlt(int x,int y,int cx,int cy,HDC hDCSrc,int x1,int y1,int rop)
	{
		return ::BitBlt(m_hDC,x,y,cx,cy,hDCSrc,x1,y1,rop);
	}
	BOOL StretchBlt(int nXOriginDest,int nYOriginDest,int nWidthDest,int nHeightDest,HDC hdcSrc,int nXOriginSrc,int nYOriginSrc,int nWidthSrc,int nHeightSrc,DWORD dwRop)
	{
		return ::StretchBlt(m_hDC,nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,hdcSrc,nXOriginSrc,nYOriginSrc,nWidthSrc,nHeightSrc,dwRop);
	}	
	int DrawTextEx(LPTSTR lpchText,int cchText,LPRECT lprc,UINT format,LPDRAWTEXTPARAMS lpdtp)
	{
		return ::DrawTextEx(m_hDC,lpchText,cchText,lprc,format,lpdtp);
	}
	BOOL TextOut(int x, int y, LPCTSTR lpString, int c)
	{
		return ::TextOut(m_hDC, x, y,lpString, c);
	}
	BOOL DrawIconEx(HICON hIcon,RECT rc,UINT istepIfAniCur,HBRUSH hbrFlickerFreeDraw,UINT diFlags )
	{
		return ::DrawIconEx(m_hDC,rc.left,rc.top,hIcon,rc.right-rc.left,rc.bottom-rc.top,istepIfAniCur,hbrFlickerFreeDraw,diFlags);
	}

	int SelectClipRgn(HRGN hRgn)
	{
		return ::SelectClipRgn(m_hDC, hRgn);
	}

	BOOL Ellipse(int left,int top, int right, int bottom)
	{
		return ::Ellipse(m_hDC, left, top, right, bottom);
	}

	HDC CreateCompatibleDC(HDC hDC = NULL)
	{
		Attach(::CreateCompatibleDC(hDC));
		return m_hDC;
	}

};

class CClientDC:public CDC
{
public:
	HWND m_hWnd;
	CClientDC(HWND hWnd):m_hWnd(hWnd)
	{
		m_hDC=::GetDC(m_hWnd);
	}

	virtual ~CClientDC()
	{
		if(m_hDC!=NULL){
			HDC hDC=m_hDC;
			m_hDC=NULL;
			::ReleaseDC(m_hWnd,hDC);
		}
	}

};
typedef std::shared_ptr<CClientDC> SPClientDC;

class CPaintDC:public CDC
{
public:
	HWND m_hWnd;
	PAINTSTRUCT m_ps;

	CPaintDC(HWND hWnd)
	{
		m_hWnd=hWnd;
		m_hDC=::BeginPaint(hWnd,&m_ps);
	}

	virtual ~CPaintDC()
	{
		::EndPaint(m_hWnd,&m_ps);
		HDC hDC=m_hDC;
		m_hDC=NULL;
	}
};

typedef std::shared_ptr<CPaintDC> SPPaint;

class CBitmap:public CGdiObj<HBITMAP>
{
public:

	CBitmap(HBITMAP hBitmap = NULL) : CGdiObj(hBitmap){}

	virtual ~CBitmap(){}

	operator HBITMAP()const{return (HBITMAP)m_hGdiObj;}

	HBITMAP CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight)
	{
		Attach(::CreateCompatibleBitmap(hDC, nWidth, nHeight));
		return m_hGdiObj;
	}

	int GetBitmap(BITMAP* pBitMap) const
	{
		return ::GetObject(m_hGdiObj, sizeof(BITMAP), pBitMap);
	}

	bool GetBitmap(BITMAP& bm) const
	{
		return (::GetObject(m_hGdiObj, sizeof(BITMAP), &bm) == sizeof(BITMAP));
	}

	CSize GetSize() const
	{
		CSize sz;
		BITMAP bm = { 0 };
		if(!GetBitmap(&bm)){
			return sz;
		}
		sz.cx = bm.bmWidth;
		sz.cy = bm.bmHeight;
		return sz;
	}


	HBITMAP CreateDIBSection(HDC hDC, CONST BITMAPINFO* lpbmi, UINT uColorUse, VOID** ppvBits, HANDLE hSection, DWORD dwOffset)
	{
		Attach(::CreateDIBSection(hDC, lpbmi, uColorUse, ppvBits, hSection, dwOffset));
		return m_hGdiObj;
	}
};


class CBufferDC:public CDC
{
protected:
	CBitmap m_bmp;
	HBITMAP m_hBmpOld;
public:
	CBufferDC()
		:m_bmp(NULL),m_hBmpOld(NULL){}
	CBufferDC(HDC hDC, int width, int height)
		:m_bmp(NULL),m_hBmpOld(NULL)
	{
		if(CreateCompatibleDC(hDC)==NULL){
			std::exception(("Error on CBufferDC Constructor:" + GetLastErrorString()).c_str());		
		}
		
		if(m_bmp.CreateCompatibleBitmap(hDC, width, height)==NULL){
			std::exception(("Error on CBufferDC Constructor:" + GetLastErrorString()).c_str());
		}

		m_hBmpOld=(HBITMAP)::SelectObject(m_hDC,(HBITMAP)m_bmp);
	}
	CSize GetSize()const{return m_bmp.GetSize();}
	HBITMAP GetBitMap()const{return (HBITMAP)m_bmp;}
	
	virtual ~CBufferDC()
	{
		HBITMAP hBmp=(HBITMAP)::SelectObject(m_hDC,m_hBmpOld);
	}

};

typedef std::shared_ptr<CBufferDC> SPBufferDC;

typedef std::unique_ptr<CBufferDC> UPBufferDC;

void DPtoHIMETRIC(LPSIZEL lpSizel);//For word, excel


