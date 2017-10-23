#include "JScript.h"
#include "MyString.h"

using namespace std;
/*****************/
/* FreeFunctions */
/*****************/

_variant_t AutoWrap(int autoType,IDispatchPtr dispPtr, LPOLESTR ptName, int cArgs...) {

	//Check argument
	throw_if_false(dispPtr,"AutoWrap arguments");

	// Variables used...
	DISPPARAMS dp = {NULL,NULL,0,0};
	DISPID dispID;
        
	// Get DISPID for name passed...
	throw_if_failed(dispPtr->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID),
		"AutoWrap IDispatch::GetIDsOfNames");
	// Build DISPPARAMS
	dp.cArgs = cArgs;
	// Begin variable-argument list...
	// Allocate memory for arguments...
	std::vector<_variant_t> vvtArgs(cArgs+1);
	if(cArgs>0){
		va_list marker;
		va_start(marker, cArgs);
		// Extract arguments...
		for(int i=0; i<cArgs; i++) {
			vvtArgs[i] = va_arg(marker, VARIANT);
		}
		va_end(marker);
		dp.rgvarg = &(vvtArgs[0]);
	}
    
	// Handle special-case for property-puts!
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	if(autoType & DISPATCH_PROPERTYPUT) {
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}
    
	// Make the call!
	_variant_t vtRet;
	throw_if_failed(dispPtr->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, &vtRet, NULL, NULL),
		"AutoWrap IDispatch::Invoke");
	// End variable-argument section...
    return vtRet;
}

_variant_t JScriptArrayFromVector(IDispatchPtr dispJArrayPtr,std::vector<_variant_t>& vvtVector)
{
	//Create array object
	_variant_t vtArray=AutoWrap(DISPATCH_METHOD,dispJArrayPtr,L"Array",0);
	//Set Parameter to Array
	IDispatchExPtr dispExPtr;
	throw_if_failed(vtArray.pdispVal->QueryInterface(__uuidof(IDispatchEx),(void**)&dispExPtr),
		"JScriptArrayFromVector IDispatch::QueryInterface");
	DISPID idProp;
	DISPID idPropertyput=DISPID_PROPERTYPUT;
	DISPPARAMS dparams={NULL,&idPropertyput,1,1};
	for(size_t i=0,iSize=vvtVector.size();i<iSize;i++){
		wchar_t wc[10];
		::wsprintf(wc,L"%d",i);
		throw_if_failed(dispExPtr->GetDispID(_bstr_t(wc), fdexNameEnsure, &idProp),
			"JScriptArrayFromVector IDispatchEx::GetDispID");
		dparams.rgvarg=&vvtVector[i];
		throw_if_failed(dispExPtr->InvokeEx(idProp,LOCALE_USER_DEFAULT,
			DISPATCH_PROPERTYPUT,&dparams,NULL,NULL,NULL),
			"JScriptArrayFromVector IDispatchEx::InvokeEx");
	}

	return vtArray;
}

std::vector<_variant_t> VectorFromJScriptArray(IDispatchPtr dispArrayPtr)
{
	_variant_t vtSize=AutoWrap(DISPATCH_PROPERTYGET,dispArrayPtr,L"length",0);
	size_t iSize=(size_t)(vtSize.lVal);
	vector<_variant_t> vvtRet(iSize);
	for(size_t i=0;i<iSize;i++){
		wchar_t wc[10];
		::wsprintf(wc,L"%d",i);
		_variant_t vtElem=AutoWrap(DISPATCH_PROPERTYGET,dispArrayPtr,wc,0);
		vvtRet[i]=vtElem;
	}
	return vvtRet;
}

/************/
/* CJScript */
/************/
STDMETHODIMP_(ULONG) CJScript::AddRef(){
	return ++m_uRefCount;
}
STDMETHODIMP_(ULONG) CJScript::Release(){
	--m_uRefCount;
	if (m_uRefCount <= 0) {
		delete this;
		return 0;
	}
	return m_uRefCount;
}
STDMETHODIMP CJScript::QueryInterface(const IID & riid,void **ppvObj){
	if (!ppvObj){
		return E_POINTER;
	}else if(IsEqualIID(riid, __uuidof(IActiveScriptSite)) || IsEqualIID(riid, IID_IUnknown)){
		AddRef();
		*ppvObj=this;
		return S_OK;
	}else{
		return E_NOINTERFACE;
	}
}
CJScript::CJScript() : m_uRefCount(1),m_asPtr(),m_aspPtr(NULL) {}
CJScript::~CJScript(){}
STDMETHODIMP CJScript::GetLCID(LCID*){return E_NOTIMPL;}
STDMETHODIMP CJScript::GetDocVersionString(BSTR *pbstrVersionString){return E_NOTIMPL;}
STDMETHODIMP CJScript::OnScriptTerminate(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo){return S_OK;}
STDMETHODIMP CJScript::OnStateChange(SCRIPTSTATE ssScriptState){return S_OK;}
STDMETHODIMP CJScript::OnScriptError(IActiveScriptError *pAse){
	if(pAse==NULL) return E_POINTER;

	DWORD dwSrcContext;
    ULONG ulLine;                          
    LONG lError;                             
	HRESULT hrsp=pAse->GetSourcePosition(&dwSrcContext,&ulLine,&lError);
	wchar_t wcError[1024] ;

	BSTR bstrTemp=0;
	HRESULT hr=pAse->GetSourceLineText(&bstrTemp);
	_bstr_t bstrLineText(bstrTemp,true);

	EXCEPINFO ei;
    HRESULT hrei=pAse->GetExceptionInfo(&ei);

	if(SUCCEEDED(hrei) && SUCCEEDED(hrsp)){
		wsprintfW(wcError, L"%s:\r\nLinePosition:%d\r\nLineText:%s\r\nDescription:%s",ei.bstrSource, ulLine + 1, bstrLineText, ei.bstrDescription) ;
	}else{
		wcscpy_s(wcError,L"Unknown error");
	}
	throw std::exception(wstr2str(wcError).c_str());
	
	return hrei;
}
STDMETHODIMP CJScript::OnEnterScript(void){return S_OK;}
STDMETHODIMP CJScript::OnLeaveScript(void){return S_OK;}
STDMETHODIMP CJScript::GetItemInfo(LPCOLESTR pstrName,DWORD dwReturnMask,IUnknown **ppunkItem,ITypeInfo **ppTypeInfo){
/*		if((dwReturnMask & SCRIPTINFO_IUNKNOWN)) {
			if(wcscmp(pstrName,SimpleMsgBox::GetClassName())==0) {
				*ppunkItem=(IUnknown*)new SimpleMsgBox();
				return S_OK;
			}
		}
*/		return TYPE_E_ELEMENTNOTFOUND;
	}

std::vector<_variant_t> CJScript::Run(LPOLESTR lpoleName,std::vector<_variant_t> vvarArg,LPCOLESTR lpoleScript) {
	//Create Instance
	m_asPtr.CreateInstance(L"JScript");
	throw_if_failed(m_asPtr->SetScriptSite(this),
		"CJScript::Run IActiveScript::CreateInstance");
	throw_if_failed(m_asPtr.QueryInterface(IID_IActiveScriptParse, &m_aspPtr),
		"CJScript::Run IActiveScript::QueryInterface");
	throw_if_failed(m_aspPtr->InitNew(),
		"CJScript::Run IActiveScriptParse::InitNew");
//	pAs->AddNamedItem(SimpleMsgBox::GetClassName()
//		,SCRIPTITEM_GLOBALMEMBERS | SCRIPTITEM_ISVISIBLE);

	IDispatchPtr scriptDispPtr;
	throw_if_failed(m_aspPtr->ParseScriptText(lpoleScript,NULL,NULL,NULL,0,0,SCRIPTTEXT_ISPERSISTENT,NULL,NULL),
		"CJScript::Run IActiveScriptParse::ParseScriptText");
	throw_if_failed(m_asPtr->SetScriptState(SCRIPTSTATE_CONNECTED),
		"CJScript::Run IActiveScriptParse::SetScriptState");
	throw_if_failed(m_asPtr->GetScriptDispatch(NULL,&scriptDispPtr),
		"CJScript::Run IActiveScriptParse::GetScriptDispatch");
	//Create Array object
	_variant_t vtArray=JScriptArrayFromVector(scriptDispPtr,vvarArg);
	//Run
	_variant_t vtRet=AutoWrap(DISPATCH_METHOD,scriptDispPtr,lpoleName,1,vtArray);

	return VectorFromJScriptArray(vtRet.pdispVal);
}

void CJScript::Close() {
	throw_if_failed(m_asPtr->SetScriptState(SCRIPTSTATE_CLOSED),
		"CJScript::Close IActiveScript::SetScriptState");
}
