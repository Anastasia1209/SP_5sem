#pragma once
#include <objbase.h>

class CFactory : public IClassFactory
{
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	//HRESULT: Это тип возвращаемого значения метода. HRESULT - это код ошибки или успешного выполнения, который используется в COM для обозначения результата операции. Если метод завершается успешно, он возвращает S_OK, а если происходит ошибка, то код ошибки.
	//STDMETHODCALLTYPE: Это специальный макрос, который используется для обработки соглашений вызова методов COM.
	// QueryInterface - это один из методов интерфейса IUnknown, и он используется для запроса (получения) указателей на другие интерфейсы, поддерживаемые объектом.
	// riid и ppv предназначены для указания типа запрашиваемого интерфейса(ссылки на него) и возвращения указателя на этот интерфейс.
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);


	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUO, const IID& id, void** ppv);
	//Этот метод создает новый экземпляр объекта с указанным идентификатором id. 
	//Параметры pUO и ppv предназначены для передачи объекта, который может использоваться при создании, и для возвращения указателя на новый объект.
	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL b);
	//Этот метод блокирует или разблокирует сервер (например, сервер COM), чтобы предотвратить его выгрузку из памяти

	CFactory();
	~CFactory();

private:
	ULONG m_lRef;	// количества ссылок на объект CFactory
};