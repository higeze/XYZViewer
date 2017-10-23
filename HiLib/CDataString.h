#pragma once

class CCDataString:public std::wstring
{
public:
	CCDataString():std::wstring(){}
	CCDataString(const std::wstring& wstr):std::wstring(wstr){}
	virtual ~CCDataString(){}
	CCDataString& operator=(const std::wstring& wstr){
		std::wstring::operator=(wstr);
		return *this;
	}
	CCDataString& operator=(const wchar_t* wstr){
		std::wstring::operator=(wstr);
		return *this;
	}

};