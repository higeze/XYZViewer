#include <ole2.h>
#include <olectl.h>

//partial of atldef.h
#ifndef ATLASSERT
#define ATLASSERT(expr) _ASSERTE(expr)
#endif // ATLASSERT

#ifdef _ATL_DISABLE_NOINLINE
#define ATL_NOINLINE
#else
#define ATL_NOINLINE __declspec( noinline )
#endif

//partial of atlcomcli.h
/////////////////////////////////////////////////////////////////////////////
// Error to HRESULT helpers

ATL_NOINLINE inline HRESULT AtlHresultFromLastError() throw()
{
	DWORD dwErr = ::GetLastError();
	return HRESULT_FROM_WIN32(dwErr);
}

ATL_NOINLINE inline HRESULT AtlHresultFromWin32(__in DWORD nError) throw()
{
	return( HRESULT_FROM_WIN32( nError ) );
}

/////////////////////////////////////////////////////////////////////////////
// Smart Pointer helpers

ATLAPI_(IUnknown*) AtlComPtrAssign(__deref_out_opt IUnknown** pp, __in_opt IUnknown* lp);
ATLAPI_(IUnknown*) AtlComQIPtrAssign(__deref_out_opt IUnknown** pp, __in_opt IUnknown* lp, REFIID riid);

#ifndef _ATL_DLL

ATLINLINE ATLAPI_(IUnknown*) AtlComPtrAssign(__deref_out_opt IUnknown** pp, __in_opt IUnknown* lp)
{
	if (pp == NULL)
		return NULL;
		
	if (lp != NULL)
		lp->AddRef();
	if (*pp)
		(*pp)->Release();
	*pp = lp;
	return lp;
}

ATLINLINE ATLAPI_(IUnknown*) AtlComQIPtrAssign(__deref_out_opt IUnknown** pp, __in_opt IUnknown* lp, REFIID riid)
{
	if (pp == NULL)
		return NULL;

	IUnknown* pTemp = *pp;
	*pp = NULL;
	if (lp != NULL)
		lp->QueryInterface(riid, (void**)pp);
	if (pTemp)
		pTemp->Release();
	return *pp;
}

#endif // _ATL_DLL

/////////////////////////////////////////////////////////////////////////////
// COM Smart pointers

template <class T>
class _NoAddRefReleaseOnCComPtr : public T
{
	private:
		STDMETHOD_(ULONG, AddRef)()=0;
		STDMETHOD_(ULONG, Release)()=0;
};

inline __checkReturn HRESULT AtlSetChildSite(__inout IUnknown* punkChild, __in_opt IUnknown* punkParent)
{
	if (punkChild == NULL)
		return E_POINTER;

	HRESULT hr;
	IObjectWithSite* pChildSite = NULL;
	hr = punkChild->QueryInterface(__uuidof(IObjectWithSite), (void**)&pChildSite);
	if (SUCCEEDED(hr) && pChildSite != NULL)
	{
		hr = pChildSite->SetSite(punkParent);
		pChildSite->Release();
	}
	return hr;
}


//CComPtrBase provides the basis for all other smart pointers
//The other smartpointers add their own constructors and operators
template <class T>
class CComPtrBase
{
protected:
	CComPtrBase() throw()
	{
		p = NULL;
	}
	CComPtrBase(__in int nNull) throw()
	{
		ATLASSERT(nNull == 0);
		(void)nNull;
		p = NULL;
	}
	CComPtrBase(__in_opt T* lp) throw()
	{
		p = lp;
		if (p != NULL)
			p->AddRef();
	}
public:
	typedef T _PtrClass;
	~CComPtrBase() throw()
	{
		if (p)
			p->Release();
	}
	operator T*() const throw()
	{
		return p;
	}
	T& operator*() const
	{
		ATLENSURE(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() throw()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnCComPtr<T>* operator->() const throw()
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}
	bool operator!() const throw()
	{
		return (p == NULL);
	}
	bool operator<(__in_opt T* pT) const throw()
	{
		return p < pT;
	}
	bool operator!=(__in_opt T* pT) const
	{
		return !operator==(pT);
	}
	bool operator==(__in_opt T* pT) const throw()
	{
		return p == pT;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	// Compare two objects for equivalence
	bool IsEqualObject(__in_opt IUnknown* pOther) throw()
	{
		if (p == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false;	// One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		return punk1 == punk2;
	}
	// Attach to an existing interface (does not AddRef)
	void Attach(__in T* p2) throw()
	{
		if (p)
			p->Release();
		p = p2;
	}
	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	__checkReturn HRESULT CopyTo(__deref_out_opt T** ppT) throw()
	{
		ATLASSERT(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = p;
		if (p)
			p->AddRef();
		return S_OK;
	}
	__checkReturn HRESULT SetSite(__in_opt IUnknown* punkParent) throw()
	{
		return AtlSetChildSite(p, punkParent);
	}
	__checkReturn HRESULT Advise(__in IUnknown* pUnk, __in const IID& iid, __out LPDWORD pdw) throw()
	{
		return AtlAdvise(p, pUnk, iid, pdw);
	}
	__checkReturn HRESULT CoCreateInstance(__in REFCLSID rclsid, __in_opt LPUNKNOWN pUnkOuter = NULL, __in DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		ATLASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
	__checkReturn HRESULT CoCreateInstance(__in LPCOLESTR szProgID, __in_opt LPUNKNOWN pUnkOuter = NULL, __in DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ATLASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}
	template <class Q>
	__checkReturn HRESULT QueryInterface(__deref_out_opt Q** pp) const throw()
	{
		ATLASSERT(pp != NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}
	T* p;
};

template <class T>
class CComPtr : public CComPtrBase<T>
{
public:
	CComPtr() throw()
	{
	}
	CComPtr(int nNull) throw() :
		CComPtrBase<T>(nNull)
	{
	}
	CComPtr(T* lp) throw() :
		CComPtrBase<T>(lp)

	{
	}
	CComPtr(__in const CComPtr<T>& lp) throw() :
		CComPtrBase<T>(lp.p)
	{
	}
	T* operator=(__in_opt T* lp) throw()
	{
        if(*this!=lp)
        {
    		return static_cast<T*>(AtlComPtrAssign((IUnknown**)&p, lp));
        }
        return *this;
	}
	template <typename Q>
	T* operator=(__in const CComPtr<Q>& lp) throw()
	{
        if( !IsEqualObject(lp) )
        {
    		return static_cast<T*>(AtlComQIPtrAssign((IUnknown**)&p, lp, __uuidof(T)));
        }
        return *this;
	}
	T* operator=(__in const CComPtr<T>& lp) throw()
	{
        if(*this!=lp)
        {
    		return static_cast<T*>(AtlComPtrAssign((IUnknown**)&p, lp));
        }
        return *this;
	}
};

//specialization for IDispatch
template <>
class CComPtr<IDispatch> : public CComPtrBase<IDispatch>
{
public:
	CComPtr() throw()
	{
	}
	CComPtr(IDispatch* lp) throw() :
		CComPtrBase<IDispatch>(lp)
	{
	}
	CComPtr(const CComPtr<IDispatch>& lp) throw() :
		CComPtrBase<IDispatch>(lp.p)
	{
	}
	IDispatch* operator=(IDispatch* lp) throw()
	{
        if(*this!=lp)
        {
            return static_cast<IDispatch*>(AtlComPtrAssign((IUnknown**)&p, lp));
        }
        return *this;
	}
	IDispatch* operator=(const CComPtr<IDispatch>& lp) throw()
	{
        if(*this!=lp)
        {
    		return static_cast<IDispatch*>(AtlComPtrAssign((IUnknown**)&p, lp.p));
        }
        return *this;
	}

// IDispatch specific stuff
	__checkReturn HRESULT GetPropertyByName(__in LPCOLESTR lpsz, __out VARIANT* pVar) throw()
	{
		ATLASSERT(p);
		ATLASSERT(pVar);
		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if (SUCCEEDED(hr))
			hr = GetProperty(dwDispID, pVar);
		return hr;
	}
	__checkReturn HRESULT GetProperty(__in DISPID dwDispID, __out VARIANT* pVar) throw()
	{
		return GetProperty(p, dwDispID, pVar);
	}
	__checkReturn HRESULT PutPropertyByName(__in LPCOLESTR lpsz, __in VARIANT* pVar) throw()
	{
		ATLASSERT(p);
		ATLASSERT(pVar);
		DISPID dwDispID;
		HRESULT hr = GetIDOfName(lpsz, &dwDispID);
		if (SUCCEEDED(hr))
			hr = PutProperty(dwDispID, pVar);
		return hr;
	}
	__checkReturn HRESULT PutProperty(__in DISPID dwDispID, __in VARIANT* pVar) throw()
	{
		return PutProperty(p, dwDispID, pVar);
	}
	__checkReturn HRESULT GetIDOfName(__in LPCOLESTR lpsz, __out DISPID* pdispid) throw()
	{
		return p->GetIDsOfNames(IID_NULL, const_cast<LPOLESTR*>(&lpsz), 1, LOCALE_USER_DEFAULT, pdispid);
	}
	// Invoke a method by DISPID with no parameters
	__checkReturn HRESULT Invoke0(__in DISPID dispid, __out_opt VARIANT* pvarRet = NULL) throw()
	{
		DISPPARAMS dispparams = { NULL, NULL, 0, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with no parameters
	__checkReturn HRESULT Invoke0(__in LPCOLESTR lpszName, __out_opt VARIANT* pvarRet = NULL) throw()
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke0(dispid, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with a single parameter
	__checkReturn HRESULT Invoke1(__in DISPID dispid, VARIANT* pvarParam1, __out_opt VARIANT* pvarRet = NULL) throw()
	{
		DISPPARAMS dispparams = { pvarParam1, NULL, 1, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with a single parameter
	__checkReturn HRESULT Invoke1(__in LPCOLESTR lpszName, VARIANT* pvarParam1, __out_opt VARIANT* pvarRet = NULL) throw()
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke1(dispid, pvarParam1, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with two parameters
	__checkReturn HRESULT Invoke2(__in DISPID dispid, __in VARIANT* pvarParam1, __in VARIANT* pvarParam2, __out_opt VARIANT* pvarRet = NULL) throw();
	// Invoke a method by name with two parameters
	__checkReturn HRESULT Invoke2(__in LPCOLESTR lpszName, __in VARIANT* pvarParam1, __in VARIANT* pvarParam2, __out_opt VARIANT* pvarRet = NULL) throw()
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = Invoke2(dispid, pvarParam1, pvarParam2, pvarRet);
		return hr;
	}
	// Invoke a method by DISPID with N parameters
	__checkReturn HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL) throw()
	{
		DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0};
		return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
	}
	// Invoke a method by name with Nparameters
	__checkReturn HRESULT InvokeN(LPCOLESTR lpszName, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL) throw()
	{
		HRESULT hr;
		DISPID dispid;
		hr = GetIDOfName(lpszName, &dispid);
		if (SUCCEEDED(hr))
			hr = InvokeN(dispid, pvarParams, nParams, pvarRet);
		return hr;
	}
	static __checkReturn HRESULT PutProperty(__inout IDispatch* p, __in DISPID dwDispID, __in VARIANT* pVar) throw()
	{
		ATLASSERT(p);
		ATLASSERT(pVar != NULL);
		if (pVar == NULL)
			return E_POINTER;
		
		if(p == NULL)
			return E_INVALIDARG;
		
		ATLTRACE(atlTraceCOM, 2, _T("CPropertyHelper::PutProperty\n"));
		DISPPARAMS dispparams = {NULL, NULL, 1, 1};
		dispparams.rgvarg = pVar;
		DISPID dispidPut = DISPID_PROPERTYPUT;
		dispparams.rgdispidNamedArgs = &dispidPut;

		if (pVar->vt == VT_UNKNOWN || pVar->vt == VT_DISPATCH || 
			(pVar->vt & VT_ARRAY) || (pVar->vt & VT_BYREF))
		{
			HRESULT hr = p->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF,
				&dispparams, NULL, NULL, NULL);
			if (SUCCEEDED(hr))
				return hr;
		}
		return p->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
				&dispparams, NULL, NULL, NULL);
	}
	static __checkReturn HRESULT GetProperty(__in IDispatch* p, __in DISPID dwDispID, __out VARIANT* pVar) throw()
	{
		ATLASSERT(p);
		ATLASSERT(pVar != NULL);
		if (pVar == NULL)
			return E_POINTER;
		
		if(p == NULL)
			return E_INVALIDARG;
			
		ATLTRACE(atlTraceCOM, 2, _T("CPropertyHelper::GetProperty\n"));
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		return p->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				&dispparamsNoArgs, pVar, NULL, NULL);
	}
};

template <class T, const IID* piid = &__uuidof(T)>
class CComQIPtr : public CComPtr<T>
{
public:
	CComQIPtr() throw()
	{
	}
	CComQIPtr(__in T* lp) throw() :
		CComPtr<T>(lp)
	{
	}
	CComQIPtr(__in const CComQIPtr<T,piid>& lp) throw() :
		CComPtr<T>(lp.p)
	{
	}
	CComQIPtr(__in_opt IUnknown* lp) throw()
	{
		if (lp != NULL)
			lp->QueryInterface(*piid, (void **)&p);
	}
	T* operator=(__in T* lp) throw()
	{
        if(*this!=lp)
        {
		    return static_cast<T*>(AtlComPtrAssign((IUnknown**)&p, lp));
        }
        return *this;
	}
	T* operator=(__in const CComQIPtr<T,piid>& lp) throw()
	{
        if(*this!=lp)
        {
    		return static_cast<T*>(AtlComPtrAssign((IUnknown**)&p, lp.p));
        }
        return *this;
	}
	T* operator=(__in IUnknown* lp) throw()
	{
        if(*this!=lp)
        {
    		return static_cast<T*>(AtlComQIPtrAssign((IUnknown**)&p, lp, *piid));
        }
        return *this;
	}
};

//Specialization to make it work
template<>
class CComQIPtr<IUnknown, &IID_IUnknown> : public CComPtr<IUnknown>
{
public:
	CComQIPtr() throw()
	{
	}
	CComQIPtr(__in IUnknown* lp) throw()
	{
		//Actually do a QI to get identity
		if (lp != NULL)
			lp->QueryInterface(__uuidof(IUnknown), (void **)&p);
	}
	CComQIPtr(__in const CComQIPtr<IUnknown,&IID_IUnknown>& lp) throw() :
		CComPtr<IUnknown>(lp.p)
	{
	}
	IUnknown* operator=(__in IUnknown* lp) throw()
	{
        if(*this!=lp)
        {
		    //Actually do a QI to get identity
		    return AtlComQIPtrAssign((IUnknown**)&p, lp, __uuidof(IUnknown));
        }
        return *this;
	}

	IUnknown* operator=(__in const CComQIPtr<IUnknown,&IID_IUnknown>& lp) throw()
	{
        if(*this!=lp)
        {
    		return AtlComPtrAssign((IUnknown**)&p, lp.p);
        }
        return *this;
	}
};

typedef CComQIPtr<IDispatch, &__uuidof(IDispatch)> CComDispatchDriver;

#define com_cast ATL::CComQIPtr
#ifndef _ATL_STREAM_MAX_SIZE
#define _ATL_STREAM_MAX_SIZE  0x100000
#endif
