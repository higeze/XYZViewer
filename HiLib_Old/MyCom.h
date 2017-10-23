#pragma once

#include <comutil.h>
#include <comip.h>
#include <comdef.h>
/*
#include <objbase.h>
#include <dispex.h>
#include <exception>
#include <iostream>
#include <string>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"comsupp.lib")
#pragma comment(lib,"ole32.lib")
*/
#include "msxml6.tlh"

template<class TElemPtr>
std::vector<MSXML2::IXMLDOMElementPtr> GetChildElementsByTagName(TElemPtr pElem,_bstr_t tagName)
{
	MSXML2::IXMLDOMNodeListPtr pNodeList=pElem->GetchildNodes();
	std::vector<MSXML2::IXMLDOMElementPtr> vpElem;
	for(long l=0,lLength=pNodeList->length;l<lLength;++l){
		if(pNodeList->item[l]->nodeName==tagName){
			vpElem.push_back(pNodeList->item[l]);
		}
	}
	return vpElem;
}
/*****************/
/* FreeFunctions */
/*****************/
inline void throw_if_failed(HRESULT hRes)
{
	if(FAILED(hRes)){
		_com_issue_error(hRes);
	}
}

inline void throw_if_true(bool b)
{
	if(b){
		_com_issue_error(b);
	}
}

inline void throw_if_false(bool b)
{
	if(!b){
		_com_issue_error(b);
	}
}

/*****************/
/* CCoInitializer */
/*   From MSDN   */
/*****************/

class CCoInitializer
{
private:
	bool coinitialized_;
	CCoInitializer(const CCoInitializer&){};
	CCoInitializer operator=(const CCoInitializer&);
public:
	explicit CCoInitializer(DWORD dwCoInit=COINIT_APARTMENTTHREADED):coinitialized_(false)
	{
		throw_if_failed(::CoInitializeEx(NULL,dwCoInit));
		coinitialized_=true;
	}
	virtual ~CCoInitializer()
	{
		if(coinitialized_){
			::CoUninitialize();
		}
	}
};

class COleInitializer
{
private:
	bool oleinitialized_;
	COleInitializer(const COleInitializer&){};
	COleInitializer operator=(const COleInitializer&);
public:
	explicit COleInitializer():oleinitialized_(false)
	{
		throw_if_failed(::OleInitialize(NULL));
		oleinitialized_=true;
	}
	virtual ~COleInitializer()
	{
		if(oleinitialized_){
			::OleUninitialize();
		}
	}
};