#pragma once
#include "../OS13_HTCOM/Interface.h"
#define OS13_HTCOM_HANDEL void*
#define PIUNKNOWN IUnknown*
#define IRES(s,r)  std::cerr<<s<<r<<std::endl

static const GUID CLSID_ComHt =
{ 0x693c66d1, 0x3ce5, 0x4512, { 0xa3, 0x11, 0x78, 0x26, 0x18, 0xdb, 0xc7, 0xa6 } };

namespace OS13_HTCOM {
	OS13_HTCOM_HANDEL Init();
	namespace HT {
		ht::HtHandle* create(OS13_HTCOM_HANDEL h, int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const wchar_t* fileName);
		ht::HtHandle* open(OS13_HTCOM_HANDEL h, const wchar_t* fileName, bool isMapFile = false);
		BOOL snap(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle);
		BOOL close(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle);
		BOOL insert(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle, const ht::Element* element);
		BOOL remove(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle, const ht::Element* element);
		ht::Element* get(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle, const ht::Element* element);
		BOOL update(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle, const ht::Element* oldElement, const void* newPayload, int newPayloadLength);
		const char* getLastError(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle);
		void print(OS13_HTCOM_HANDEL h, const ht::Element* element);
	}
	namespace Element {
		ht::Element* createGetElement(OS13_HTCOM_HANDEL h, const void* key, int keyLength);
		ht::Element* createInsertElement(OS13_HTCOM_HANDEL h, const void* key, int keyLength, const void* payload, int  payloadLength);
		ht::Element* createUpdateElement(OS13_HTCOM_HANDEL h, const ht::Element* oldElement, const void* newPayload, int  newPayloadLength);
	}
	void Dispose(OS13_HTCOM_HANDEL h);
}

