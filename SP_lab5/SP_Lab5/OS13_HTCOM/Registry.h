#pragma once
#include "Windows.h"
#include <objbase.h>
#include <assert.h>

static const GUID CLSID_ComHt =
{ 0x693c66d1, 0x3ce5, 0x4512, { 0xa3, 0x11, 0x78, 0x26, 0x18, 0xdb, 0xc7, 0xa6 } };

HRESULT RegisterServer(HMODULE hModule,const CLSID& clsid,const WCHAR* szFriendlyName,const WCHAR* szVerIndProgID,const WCHAR* szProgID);
HRESULT UnregisterServer(const CLSID& clsid,const WCHAR* szVerIndProgID,const WCHAR* szProgID);
