#include "pch.h";
#include "ComHT.h";
#include "ClassFactory.h"
#include "Registry.h"
HMODULE g_hModule;                                          // Описатель модуля
const wchar_t* g_szFriendlyName = L"OS13.ComHT";    // Дружественное имя компонента
const wchar_t* g_szVerIndProgID = L"OS13.ComHT.1.0";      // Не зависящий от версии ProgID
const wchar_t* g_szProgID = L"OS13.CompHT.1";              // ProgID
long g_cComponents = 0;		                                // Количество активных компонентов
long g_cServerLocks = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}




STDAPI DllRegisterServer()
{
	return RegisterServer(
		g_hModule,
		CLSID_ComHt,
		g_szFriendlyName,
		g_szVerIndProgID,
		g_szProgID
	);
}

STDAPI DllUnregisterServer()
{
	return UnregisterServer(
		CLSID_ComHt,
		g_szVerIndProgID,
		g_szProgID
	);
}

STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}
STDAPI DllInstall(BOOL b, PCWSTR s)
{
	return S_OK;
}
STDAPI DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv) {
	HRESULT rc = E_UNEXPECTED;
	ClassFactory* pF;
	if (clsid != CLSID_ComHt) rc = CLASS_E_CLASSNOTAVAILABLE;
	else if ((pF = new ClassFactory()) == NULL) rc = E_OUTOFMEMORY;
	else {
		rc = pF->QueryInterface(iid, ppv);
		pF->Release();
	}
	return rc;
}