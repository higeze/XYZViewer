#include "MyString.h"

//std::wstring str2wstr(const std::string& str)
//{
//	return std::wstring(str.begin(),str.end());
//}
//
//std::string wstr2str(const std::wstring& wstr)
//{
//	return std::string(wstr.begin(),wstr.end());
//}

std::wstring str2wstr(const std::string& str)
{
	//Length
	auto len = ::MultiByteToWideChar(CP_THREAD_ACP, 0, str.c_str(), -1, NULL,0);
	std::vector<wchar_t> wstr(len+1,0);
	//Convert
	::MultiByteToWideChar(CP_THREAD_ACP, 0, str.c_str(), -1, &wstr[0], len);

	return &wstr[0];
}

std::string wstr2str(const std::wstring& wstr)
{
	//Length
	auto len = ::WideCharToMultiByte(CP_THREAD_ACP, 0, wstr.c_str(), -1, NULL, 0 ,NULL ,NULL);
	std::vector<char> str(len+1,'\0');
	//Convert
	::WideCharToMultiByte(CP_THREAD_ACP, 0, wstr.c_str(),wstr.size(), &str[0], len, NULL, NULL);

	return &str[0];
}

//std::wostream& operator <<(std::wostream &os, const std::string& str)
//{
//	os<<str2wstr(str);
//	return os;
//}
//std::wistream& operator >>(std::wistream &is, std::string& str)
//{
//	std::wstring wstr;
//	is>>wstr;
//	str = wstr2str(wstr);
//	return is;
//}
//
//std::ostream& operator <<(std::ostream &os, const std::wstring& wstr)
//{
//	os<<wstr2str(wstr);
//	return os;
//}
//std::istream& operator >>(std::istream &is, std::wstring& wstr)
//{
//	std::string str;
//	is>>str;
//	wstr = str2wstr(str);
//	return is;
//}
//
//std::wostream& operator <<(std::wostream &os, const char* str)
//{
//	//os<<str2wstr(str);
//	return os;
//}
//std::wistream& operator >>(std::wistream &is, char* str)
//{
//	//std::wstring wstr;
//	//is>>wstr;
//	//str = wstr2str(wstr);
//	return is;
//}
//
//std::ostream& operator <<(std::ostream &os, const char* wstr)
//{
//	//os<<wstr2str(wstr);
//	return os;
//}
//std::istream& operator >>(std::istream &is, char* wstr)
//{
//	//std::string str;
//	//is>>str;
//	//wstr = str2wstr(str);
//	return is;
//}