#pragma once
#include <objbase.h>

extern long g_cComponents;		
extern long g_cServerLocks;		

class ClassFactory : public IClassFactory
{
public:
	STDMETHOD(QueryInterface(REFIID iid, void** ppv));
	STDMETHOD_(ULONG, AddRef(void));
	STDMETHOD_(ULONG, Release(void));
	STDMETHOD(CreateInstance(IUnknown* pUO, const IID& id, void** ppv));
	STDMETHOD(LockServer(BOOL b));

	ClassFactory() : m_cRef(1) {}
	~ClassFactory() {};
private:
	long m_cRef;
};
