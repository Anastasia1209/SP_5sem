#include "pch.h"
#include "OS13_HTCOM_LIB.h"

IComHT* pIHT = nullptr;
IElement* pIElement = nullptr;

OS13_HTCOM_HANDEL OS13_HTCOM::Init()
{
    PIUNKNOWN pIUnknown = nullptr;
    try {
        if (!SUCCEEDED(CoInitialize(NULL)))
            throw std::runtime_error("Error initialize OLE32");

        if (!SUCCEEDED(CoCreateInstance(CLSID_ComHt, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown)))
            throw std::runtime_error("Error create instance CLSID_Math");

        return pIUnknown;
    }
    catch (std::runtime_error error) {
        IRES("Init: ", error.what());
        return nullptr;
    }
}
void OS13_HTCOM::Dispose(OS13_HTCOM_HANDEL h) {
    ((PIUNKNOWN)h)->Release();
    CoFreeUnusedLibraries();
    CoUninitialize();
}

ht::HtHandle* OS13_HTCOM::HT::create(OS13_HTCOM_HANDEL h, int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const wchar_t* fileName)
{
    ht::HtHandle** ht = new ht::HtHandle*;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt"); 
        else {
            if(!SUCCEEDED(pIHT->create(ht,capacity,secSnapshotInterval,maxKeyLength,maxPayloadLength,fileName)))
                throw std::runtime_error("Error create HT");
            else {
                pIHT->Release();
                return *ht;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("create: ", error.what());
    }

}

ht::HtHandle* OS13_HTCOM::HT::open(OS13_HTCOM_HANDEL h,const wchar_t* fileName, bool isMapFile)
{
    ht::HtHandle** ht = new ht::HtHandle*;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->open(ht,fileName, isMapFile)))
                throw std::runtime_error("Error open HT");
            else {
                pIHT->Release();
                return *ht;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("open: ", error.what());
    }

}

BOOL OS13_HTCOM::HT::snap(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle)
{
    BOOL rc = false;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->snap(rc, htHandle)))
                throw std::runtime_error("Error snap HT");
            else {
                pIHT->Release();
                return rc;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("snap: ", error.what());
    }

}
BOOL OS13_HTCOM::HT::close(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle)
{
    BOOL rc = false;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->close(rc, htHandle)))
                throw std::runtime_error("Error close HT");
            else {
                pIHT->Release();
                return rc;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("close: ", error.what());
    }

}
BOOL OS13_HTCOM::HT::insert(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle, const ht::Element* element)
{
    BOOL rc = false;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->insert(rc, htHandle,element)))
                throw std::runtime_error("Error insert HT");
            else {
                pIHT->Release();
                return rc;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("insert: ", error.what());
    }

}

BOOL OS13_HTCOM::HT::remove(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle, const ht::Element* element)
{
    BOOL rc = false;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->remove(rc, htHandle, element)))
                throw std::runtime_error("Error remove HT");
            else {
                pIHT->Release();
                return rc;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("remove: ", error.what());
    }
}

ht::Element* OS13_HTCOM::HT::get(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle, const ht::Element* element)
{
    ht::Element** rElement = new ht::Element*;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->get(rElement, htHandle, element)))
                throw std::runtime_error("Error get HT");
            else {
                pIHT->Release();
                return *rElement;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("get: ", error.what());
    }
}

BOOL OS13_HTCOM::HT::update(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle, const ht::Element* oldElement, const void* newPayload, int newPayloadLength)
{
    BOOL rc = false;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->update(rc, htHandle, oldElement, newPayload, newPayloadLength)))
                throw std::runtime_error("Error update HT");
            else {
                pIHT->Release();
                return rc;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("update: ", error.what());
    }
}

const char* OS13_HTCOM::HT::getLastError(OS13_HTCOM_HANDEL h, ht::HtHandle* htHandle)
{
    const char** rcLastError = new const char*;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->getLastError(rcLastError, htHandle)))
                throw std::runtime_error("Error get last error HT");
            else {
                pIHT->Release();
                return *rcLastError;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("getLastError: ", error.what());
    }
}

void OS13_HTCOM::HT::print(OS13_HTCOM_HANDEL h, const ht::Element* element)
{
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IComHt, (void**)&pIHT)))
            throw std::runtime_error("Error get interface IID_IComHt");
        else {
            if (!SUCCEEDED(pIHT->print(element)))
                throw std::runtime_error("Error print HT");
            else {
                pIHT->Release();
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("print: ", error.what());
    }
}
////
ht::Element* OS13_HTCOM::Element::createGetElement(OS13_HTCOM_HANDEL h, const void* key, int keyLength)
{
    ht::Element** rcElement = new ht::Element*;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IElement, (void**)&pIElement)))
            throw std::runtime_error("Error get interface IID_IElement");
        else {
            if (!SUCCEEDED(pIElement->createGetElement(rcElement, key, keyLength)))
                throw std::runtime_error("Error create element");
            else {
                pIElement->Release();
                return *rcElement;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("createGetElement: ", error.what());
    }
}

ht::Element* OS13_HTCOM::Element::createInsertElement(OS13_HTCOM_HANDEL h, const void* key, int keyLength, const void* payload, int  payloadLength)
{
    ht::Element** rcElement = new ht::Element*;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IElement, (void**)&pIElement)))
            throw std::runtime_error("Error get interface IID_IElement");
        else {
            if (!SUCCEEDED(pIElement->createInsertElement(rcElement, key, keyLength, payload, payloadLength)))
                throw std::runtime_error("Error create element");
            else {
                pIElement->Release();
                return *rcElement;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("createInsertElement: ", error.what());
    }
}

ht::Element* OS13_HTCOM::Element::createUpdateElement(OS13_HTCOM_HANDEL h, const ht::Element* oldElement, const void* newPayload, int  newPayloadLength)
{
    ht::Element** rcElement = new ht::Element*;
    try {
        if (!SUCCEEDED(((PIUNKNOWN)h)->QueryInterface(IID_IElement, (void**)&pIElement)))
            throw std::runtime_error("Error get interface IID_IElement");
        else {
            if (!SUCCEEDED(pIElement->createUpdateElement(rcElement, oldElement, newPayload, newPayloadLength)))
                throw std::runtime_error("Error update element");
            else {
                pIElement->Release();
                return *rcElement;
            }
        }
    }
    catch (std::runtime_error error) {
        IRES("createUpdateElement: ", error.what());
    }
}

