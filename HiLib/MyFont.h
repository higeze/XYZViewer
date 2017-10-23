#pragma once

#include "MyGdiObj.h"
#include "MyPoint.h"
#include <new>
#include "MyFriendSerializer.h"

class CFont:/*private boost::noncopyable,*/public CGdiObj<HFONT>
{
private:
	//std::basic_string<TCHAR> m_strFaceName;
	//int m_nPointSize;
	//bool m_bBold;
	//bool m_bItalic;
	//HDC m_hDC;
	LOGFONT m_logFont;
//	CFont(){}
public:
	CFont(){}
	CFont(const CFont& font)
	{
		m_logFont=font.m_logFont;
		Attach(::CreateFontIndirect(&m_logFont));
	}
	virtual ~CFont(){}
	CFont& operator=(const CFont& font)
	{
		m_logFont=font.m_logFont;
		Attach(::CreateFontIndirect(&m_logFont));
		return *this;
	}
	LOGFONT GetLogFont()const{return m_logFont;}
	bool operator==(const CFont& font)
	{
		return ::memcmp(&m_logFont,&font.m_logFont,sizeof(LOGFONT))==0;
	}

	bool operator!=(const CFont& font)
	{
		return ::memcmp(&m_logFont,&font.m_logFont,sizeof(LOGFONT))!=0;
	}

	LONG GetEscapement(){return m_logFont.lfEscapement;}

	CFont(int nPointSize,std::basic_string<TCHAR> strFaceName, LONG escapement = 0, HWND hWnd = NULL,bool bBold = false,bool bItalic = false)
	{
		ZeroMemory(&m_logFont,sizeof(LOGFONT));
		m_logFont.lfEscapement = escapement;
		//m_logFont.lfOrientation = orientation;

		m_logFont.lfCharSet=DEFAULT_CHARSET;
		m_logFont.lfHeight=-abs(Point2Height(nPointSize,hWnd));
		if(bBold)
			m_logFont.lfWeight = FW_BOLD;
		if(bItalic)
			m_logFont.lfItalic = (BYTE)TRUE;
		_tcscpy_s(m_logFont.lfFaceName, strFaceName.c_str());
		Attach(::CreateFontIndirect(&m_logFont));
	}
	//CFont(int nPointSize,LPCTSTR lpszFaceName,HWND hWnd = NULL,bool bBold = false,bool bItalic = false)
	//{
	//	ZeroMemory(&m_logFont,sizeof(LOGFONT));
	//	m_logFont.lfCharSet=DEFAULT_CHARSET;
	//	m_logFont.lfHeight=-abs(Point2Height(nPointSize,hWnd));
	//	if(bBold)
	//		m_logFont.lfWeight = FW_BOLD;
	//	if(bItalic)
	//		m_logFont.lfItalic = (BYTE)TRUE;
	//	_tcscpy_s(m_logFont.lfFaceName, lpszFaceName);
	//	Attach(::CreateFontIndirect(&m_logFont));
	//}
	HFONT Create()
	{
		Attach(::CreateFontIndirect(&m_logFont));	
		return m_hGdiObj;
	}
	CFont(LOGFONT logFont)
		:m_logFont(logFont)
	{
		Attach(::CreateFontIndirect(&m_logFont));
	}
	HFONT CreateFontIndirect(LOGFONT logFont)
	{
		m_logFont=logFont;
		Attach(::CreateFontIndirect(&m_logFont));
		return m_hGdiObj;
	}
	int Height2Point(const LONG& lHeight,HWND hWnd=NULL)const
	{
		HDC hDC=GetDC(hWnd);
		int nPoint=::MulDiv(lHeight,72,GetDeviceCaps(hDC,LOGPIXELSY));
		::ReleaseDC(NULL,hDC);
		return nPoint;
	}

	LONG Point2Height(const int& nPoint,HWND hWnd=NULL)const
	{
		HDC hDC=GetDC(hWnd);
		LONG lHeight=::MulDiv(nPoint,::GetDeviceCaps(hDC,LOGPIXELSY),72);
		::ReleaseDC(NULL,hDC);
		return lHeight;
	}

	int GetPointSize()const{return Height2Point(abs(m_logFont.lfHeight));}
	std::basic_string<TCHAR> GetFaceName()const{return std::wstring(m_logFont.lfFaceName);}
	LONG GetHeight()const{return abs(m_logFont.lfHeight);}

	operator LPLOGFONT()
	{
		return &m_logFont;
	}

	FRIEND_SERIALIZER
	template<class Archive>
	void save(Archive& ar)
	{
		int nPointSize(Height2Point(m_logFont.lfHeight));
		std::wstring wstrFaceName(m_logFont.lfFaceName);
		bool bBold(m_logFont.lfWeight==FW_BOLD);
		bool bItalic(m_logFont.lfItalic==(BYTE)TRUE);
		ar("PointSize",nPointSize);
		ar("FaceName",wstrFaceName);		
		ar("Bold",bBold);
		ar("Italic",bItalic);
	}

	template<class Archive>
	void load(Archive& ar)
	{
		int nPointSize(10);
		std::wstring wstrFaceName(L"Arial");
		bool bBold(false);
		bool bItalic(false);
		ar("PointSize",nPointSize);
		ar("FaceName",wstrFaceName);		
		ar("Bold",bBold);
		ar("Italic",bItalic);
		ZeroMemory(&m_logFont,sizeof(LOGFONT));
		m_logFont.lfCharSet=DEFAULT_CHARSET;
		m_logFont.lfHeight=Point2Height(nPointSize);
		if(bBold)
			m_logFont.lfWeight = FW_BOLD;
		if(bItalic)
			m_logFont.lfItalic = (BYTE)TRUE;
		_tcscpy_s(m_logFont.lfFaceName, wstrFaceName.c_str());
		Create();
	}
};
typedef std::shared_ptr<CFont> SPFont;