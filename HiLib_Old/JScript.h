#pragma once

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"comsupp.lib")
#pragma comment(lib,"comsuppw.lib")
#pragma comment(lib,"ole32.lib")
#include <tchar.h>
#include <windows.h>
#include <objbase.h>
#include <comutil.h>
#include <dispex.h>
#include <activscp.h>
#include <vector>
#include <comip.h>

typedef _com_ptr_t<_com_IIID<IActiveScript,&__uuidof(IActiveScript)>> IActiveScriptPtr;
typedef _com_ptr_t<_com_IIID<IActiveScriptParse,&__uuidof(IActiveScriptParse)>> IActiveScriptParsePtr;
typedef _com_ptr_t<_com_IIID<IDispatch,&__uuidof(IDispatch)>> IDispatchPtr;
typedef _com_ptr_t<_com_IIID<IDispatchEx,&__uuidof(IDispatchEx)>> IDispatchExPtr;


/*****************/
/* FreeFunctions */
/*****************/
//typedef boostmodoki::optional<std::vector<_variant_t>> opvvt;
//typedef boostmodoki::optional<_variant_t> opvt;

inline void throw_if_failed(HRESULT hRes,const char* message)
{
	if(FAILED(hRes)){
		throw std::exception(message);
	}
}

inline void throw_if_false(bool b,const char* message)
{
	if(!b){
		throw std::exception(message);
	}
}

						 

_variant_t AutoWrap(int autoType,IDispatchPtr dispPtr, LPOLESTR ptName, int cArgs...);

_variant_t JScriptArrayFromVector(IDispatchPtr dispJArrayPtr,std::vector<_variant_t>& vvtVector);
std::vector<_variant_t> VectorFromJScriptArray(IDispatchPtr dispArrayPtr);

template<class T>
T* CoCreate(){return new T();}

template<class T>
void SafeRelease(T p){if(p){p->Release();p=NULL;}}


/************/
/* CJScript */
/************/
class CJScript:public IActiveScriptSite{
//<IUnknown>
public:
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP QueryInterface(const IID & riid,void **ppvObj);
private:
	ULONG m_uRefCount;
//</IUnknown>
private:
	IActiveScriptPtr m_asPtr;
	IActiveScriptParsePtr m_aspPtr;
//<ConstDestructor>
public:
	CJScript();
	virtual ~CJScript();
//</ConstDestructor>
public:
//<IActiveScriptSite>
	STDMETHODIMP GetLCID(LCID*);
	STDMETHODIMP GetDocVersionString(BSTR *pbstrVersionString);
	STDMETHODIMP OnScriptTerminate(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo);
	STDMETHODIMP OnStateChange(SCRIPTSTATE ssScriptState);
	STDMETHODIMP OnScriptError(IActiveScriptError *pAse);
	STDMETHODIMP OnEnterScript(void);
	STDMETHODIMP OnLeaveScript(void);
	STDMETHODIMP GetItemInfo(LPCOLESTR pstrName,DWORD dwReturnMask,IUnknown **ppunkItem,ITypeInfo **ppTypeInfo);
//</IActiveScriptSite>
//<Method>
	std::vector<_variant_t> Run(LPOLESTR lpoleName,std::vector<_variant_t> vvarArg,LPCOLESTR lpoleScript);
	void Close();

};

struct jscript_deleter
{
    void operator()( CJScript* pJscript ) const
    {
		SafeRelease(pJscript);
    }
};

typedef std::unique_ptr<CJScript, jscript_deleter> JScriptPtr;
