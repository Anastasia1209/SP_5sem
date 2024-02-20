#include "pch.h"
#include "ComHt.h"
#include "ClassFactory.h"

HRESULT STDMETHODCALLTYPE ClassFactory::QueryInterface(REFIID iid, void** ppv) {
	if (iid == IID_IUnknown)
		*ppv = (IUnknown*)this;
	else if (iid == IID_IClassFactory)
		*ppv = (IClassFactory*)this;
	else
		return E_NOINTERFACE;

	if (*ppv) {
		AddRef();
		return (S_OK);
	}
	else
		return (E_NOINTERFACE);
}
STDMETHODIMP_(ULONG) ClassFactory::AddRef() {
	return InterlockedIncrement(&m_cRef);
}
STDMETHODIMP_(ULONG) ClassFactory::Release() {
	if (InterlockedIncrement(&m_cRef) == 0) {
		delete this;
		return 0;
	}
	return m_cRef;
}
STDMETHODIMP ClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj) {

	HRESULT hr = E_UNEXPECTED;

	ComHt* pHt;

	if (pUnkOuter != NULL) 
		hr = CLASS_E_NOAGGREGATION;
	else if ((pHt = new ComHt()) == NULL)
		hr = E_OUTOFMEMORY;
	else {
		hr = pHt->QueryInterface(riid, ppvObj);
		pHt->Release();
	}

	return hr;
}

STDMETHODIMP  ClassFactory::LockServer(BOOL fLock) {//активные блокировки
	if (fLock)
		InterlockedIncrement(&g_cServerLocks);
	else
		InterlockedDecrement(&g_cServerLocks);

	return S_OK;
}
