#pragma once
#include <string>
#include <vector>
#include <Windows.h>
typedef std::basic_string<TCHAR> tstring;

#define REGEX_FILEEXT	L"\\.[^.\\\\\\/:*?\"<>|\\r\\n]+$"
#define REGEX_FILENAME_A	"[^\\\\\\/:*?\\\"<>|\\r\\n]+$"
#define REGEX_FILENAME_W	L"[^\\\\\\/:*?\\\"<>|\\r\\n]+$"

/****************/
/* tstring_cast */
/****************/
template<typename T>
inline const T* tstring_cast(const char* str,const wchar_t* wstr)
{
	return str;
}
template<>
inline const wchar_t* tstring_cast<wchar_t>(const char* str,const wchar_t* wstr)
{
	return wstr;
}

//template<class char_type,const char* pC,const wchar_t* pW>
//struct tstring_cast {
//	static const char_type value = pC;
//};
//
//template<const char* pC,const wchar_t* pW>
//struct tstring_cast<wchar_t,pC,pW> {
//	static const wchar_t value = pW;
//};


#define TSTRING_CAST(char_type,str) tstring_cast<char_type>(str,L##str)
/****************/
/* tchar_cast */
/****************/
/*
template<typename T>
inline const T tchar_cast(char ch,wchar_t wch)
{
	return ch;
}
template<>
inline const wchar_t tchar_cast<wchar_t>(char ch,wchar_t wch)
{
	return wch;
}
*/
template<class char_type,char c,wchar_t w>
struct tchar_cast {
	static const char_type value = c;
};

template<char c,wchar_t w>
struct tchar_cast<wchar_t,c,w> {
	static const wchar_t value = w;
};

#define	TCHAR_CAST(char_type,c) tchar_cast<char_type,c,L##c>::value

/*************/
/* GetBuffer */
/*************/

template<typename T>
T* GetBuffer(std::basic_string<T>& str,int n)
{
	str.resize(n+1);
	return &str[0];
}

/*****************/
/* ReleaseBuffer */
/*****************/
template<typename T>
void ReleaseBuffer(std::basic_string<T>& str)
{
	std::basic_string<T>::size_type size=str.find_first_of(TCHAR_CAST(T,'\0'));
	str.resize(size);
}

/*********/
/* IsNaN */
/*********/
template<typename char_type>
bool IsNaN(const std::basic_string<char_type>& str)
{
	size_t i=0;
	if(str[0]==TCHAR_CAST(char_type,'-')){
		i=1;
	}else{
		i=0;
	}
	
	for(size_t iLen=str.length();i<iLen;i++){
		if(!(::isdigit(str[i])||
			str[i]==TCHAR_CAST(char_type,'.')||
			str[i]==TCHAR_CAST(char_type,'e')||
			str[i]==TCHAR_CAST(char_type,'E')||
			str[i]==TCHAR_CAST(char_type,'+')||
			str[i]==TCHAR_CAST(char_type,'-'))){
				return true;
		}
	}
	return false;
}

std::wstring str2wstr(const std::string& str);
//std::wstring str2wstr(const std::wstring& str){return str;}

//std::string wstr2str(const std::string& str){return str;}
std::string wstr2str(const std::wstring& wstr);


template<typename T>
std::basic_string<T> RoundDouble2String(const double& arg, int precision)
{
	std::basic_string<T> result;
	std::basic_stringstream<T> sstream;
	sstream.setf(std::ios::fixed);
	sstream.precision(precision);
	if(!(sstream<<arg && sstream>>result)){
		throw std::exception("Error on RoundDouble2String");
	}	
	int dotIndex=result.find_first_of(TCHAR_CAST(T,'.'));
	if(dotIndex!=std::basic_string<T>::npos){
		int nonZeroIndex=dotIndex-1;
		for(unsigned i=dotIndex+1;i<result.size();i++){
			if(i == dotIndex+1 || result[i]!=TCHAR_CAST(T,'0')){
				nonZeroIndex=i;
			}		
		}
		result.erase(nonZeroIndex+1);
	}
	return result;
}

template<typename T,typename U>
std::basic_string<T> roundtostring(const U& arg, int precision)
{
	std::basic_string<T> result;
	std::basic_stringstream<T> sstream;
	sstream.setf(std::ios::fixed);
	sstream.precision(precision);
	if(!(sstream<<arg && sstream>>result))throw std::exception("Error on roundtostring");
	int dotIndex=result.find_first_of(L'.');
	if(dotIndex!=std::basic_string<T>::npos){
		int nonZeroIndex=dotIndex-1;
		for(unsigned i=dotIndex+1;i<result.size();i++){
			if(result[i]!=TCHAR_CAST(T,'0')){
				nonZeroIndex=i;
			}		
		}
		result.erase(nonZeroIndex+1);
	}
	return result;
}

namespace my
{
	template<typename T>
	bool find(const std::basic_string<T>& str1, const std::basic_string<T>& str2, bool matchCase, bool matchWholeWord)
	{
		bool find = false;
		if(matchCase && matchWholeWord){
			find = str1 == str2;
		}else if(!matchCase && matchWholeWord){
			find = boost::iequals(str1, str2);
		}else if(matchCase && !matchWholeWord){
			find = str1.find(str2) == std::wstring::npos;
		}else{
			find = !boost::algorithm::ifind_first(str1, str2).empty();
		}
		return find;
	}
}