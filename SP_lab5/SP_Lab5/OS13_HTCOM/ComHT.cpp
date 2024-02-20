#include "pch.h"
#include "ComHT.h"

#ifdef _WIN64
#pragma comment(lib, "D:/Univer/5sem/SP/SP_lab5/lab3/x64/Debug/OS10_HTAPI.lib")
#else
#pragma comment(lib, "../../lab3/Debug/OS10HTAPI.lib")
#endif


ComHt::ComHt() {
	m_cRef = 1;
	InterlockedIncrement(&g_cComponents);
}
ComHt::~ComHt() {
	InterlockedDecrement(&g_cComponents);
}

HRESULT STDMETHODCALLTYPE ComHt::QueryInterface(REFIID iid, void** ppv) {
	if (iid == IID_IUnknown)
		*ppv = (IComHT*)this;
	else if (iid == IID_IComHt)
		*ppv = (IComHT*)this;
	else if (iid == IID_IElement)
		*ppv = (IElement*)this;
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)*ppv)->AddRef();
	return S_OK;
}
STDMETHODIMP_(ULONG) ComHt::AddRef() {
	return InterlockedIncrement(&m_cRef);
}
STDMETHODIMP_(ULONG) ComHt::Release() {
	if (InterlockedDecrement(&m_cRef) == 0) {
		delete this;
		return 0;
	}
	return m_cRef;
}

STDMETHODIMP ComHt::create(ht::HtHandle** htHandle, int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const wchar_t* fileName)
{
	*htHandle = ht::create(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, fileName);
	return S_OK;
}
STDMETHODIMP ComHt::open(ht::HtHandle** htHandle, const wchar_t* fileName, bool isMapFile)
{
	*htHandle = ht::open(fileName, isMapFile);
	return S_OK;
}

STDMETHODIMP ComHt::snap(BOOL& rc, ht::HtHandle* htHandle)
{
	rc = ht::snap(htHandle);
	return S_OK;
}

STDMETHODIMP ComHt::close(BOOL& rc, ht::HtHandle* htHandle)
{
	rc = ht::close(htHandle);
	return S_OK;
}

STDMETHODIMP ComHt::insert(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* element)
{
	rc = ht::insert(htHandle, element);
	return S_OK;
}

STDMETHODIMP ComHt::remove(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* element)
{
	rc = ht::remove(htHandle, element);
	return S_OK;
}

STDMETHODIMP ComHt::get(ht::Element** rcElement, ht::HtHandle* htHandle, const ht::Element* element)
{
	*rcElement = ht::get(htHandle, element);
	return S_OK;
}

STDMETHODIMP ComHt::update(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* oldElement, const void* newPayload, int newPayloadLength)
{
	rc = ht::update(htHandle, oldElement, newPayload, newPayloadLength);
	return S_OK;
}

STDMETHODIMP ComHt::getLastError(const char** lastError, ht::HtHandle* htHandle)
{
	*lastError = ht::getLastError(htHandle);
	return S_OK;
}

STDMETHODIMP ComHt::print(const ht::Element* element)
{
	ht::print(element);
	return S_OK;
}

STDMETHODIMP ComHt::createGetElement(ht::Element** getElement, const void* key, int keyLength)
{
	*getElement = new ht::Element(key, keyLength);
	return S_OK;
}

STDMETHODIMP ComHt::createInsertElement(ht::Element** newElement, const void* key, int keyLength, const void* payload, int  payloadLength)
{
	*newElement = new ht::Element(key, keyLength);
	return S_OK;
}

STDMETHODIMP ComHt::createUpdateElement(ht::Element** updateElement, const ht::Element* oldElement, const void* newPayload, int  newPayloadLength)
{
	*updateElement = new ht::Element(oldElement, oldElement, newPayloadLength);
	return S_OK;
}