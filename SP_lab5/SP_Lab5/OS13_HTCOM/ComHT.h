#pragma once
#include "Interface.h"

extern long g_cComponents;		// Количество активных компонентов
extern long g_cServerLocks;		// Счетчик блокировок

class ComHt : public IComHT, public IElement {
public:	
	
	STDMETHOD(QueryInterface(REFIID iid, void** ppv));
	STDMETHOD_(ULONG, AddRef(void));
	STDMETHOD_(ULONG, Release(void));
	STDMETHOD(create(ht::HtHandle** htHandle, int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const wchar_t* fileName));
	STDMETHOD(open(ht::HtHandle** htHandle, const wchar_t* fileName, bool isMapFile = false));
	STDMETHOD(snap(BOOL& rc, ht::HtHandle* htHandle));
	STDMETHOD(close(BOOL& rc, ht::HtHandle* htHandle));
	STDMETHOD(insert(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* element));
	STDMETHOD(remove(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* element));
	STDMETHOD(get(ht::Element** rcElement, ht::HtHandle* htHandle, const ht::Element* element));
	STDMETHOD(update(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* oldElement, const void* newPayload, int newPayloadLength));
	STDMETHOD(getLastError(const char** lastError, ht::HtHandle* htHandle));
	STDMETHOD(print(const ht::Element* element));
	STDMETHOD(createGetElement(ht::Element** getElement, const void* key, int keyLength));
	STDMETHOD(createInsertElement(ht::Element** newElement, const void* key, int keyLength, const void* payload, int  payloadLength));
	STDMETHOD(createUpdateElement(ht::Element** updateElement, const ht::Element* oldElement, const void* newPayload, int  newPayloadLength));
	
	ComHt();
	~ComHt();
private:
	long m_cRef;
};