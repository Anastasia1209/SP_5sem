#pragma once
#include "pch.h"
#include "CFactory.h"
#include "SEQLOG.h"
#include "ICom.h"

extern ULONG g_ServerLocks;	//для отслеживания количества активных блокировок сервера

// конструктор класса
CFactory::CFactory() :m_lRef(1) {	//инициализация переменной m_lRef значением 1. Это счетчик ссылок на объект
	SEQ; LOG("CFactory::CFactory", ""); // Это вызов макросов SEQ и LOG для записи информации о создании объекта CFactory в журнал
	//SEQ генерирует уникальный порядковый номер для этой записи
	//LOG записывает сообщение в журнал.
};
CFactory::~CFactory() {
	SEQ; LOG("CFactory::~CFactory", ""); 
};

//реализация метода QueryInterface
HRESULT STDMETHODCALLTYPE CFactory::QueryInterface(REFIID riid, void** ppv)
{
	SEQ; // генерирует уникальный порядковый номер для записи в журнал.
	HRESULT rc = S_OK;	//S_OK успешное выполнение операции по умолчанию.
	*ppv = NULL;	//будет использоваться для передачи указателя на запрошенный интерфейс.
	//  определить, какой интерфейс был запрошен:
	if (riid == IID_IUnknown)	
		*ppv = (IUnknown*)this;	// указатель на текущий объект CFactory
	else if (riid == IID_IClassFactory)	
		*ppv = (IClassFactory*)this;
	else 
		rc = E_NOINTERFACE;	//запрошенный интерфейс не поддерживается.

	if (rc == S_OK)
		this->AddRef();	//увеличить счетчик ссылок объекта.
	LOG("Cfactory::QueryInterface rc = ", rc);
	return rc;
};
ULONG STDMETHODCALLTYPE CFactory::AddRef(void) {
	SEQ;
	InterlockedIncrement((LONG*)&(this->m_lRef));//атомарно увеличивает значение счетчика ссылок m_lRef на 1
	LOG("CFactory::AddRef m_lRef = ", this->m_lRef);
	return this->m_lRef;	//возвращает новое значение счетчика ссылок.
};
ULONG STDMETHODCALLTYPE CFactory::Release(void) {
	SEQ;

	ULONG rc = this->m_lRef;
	if ((rc = InterlockedDecrement((LONG*)&(this->m_lRef))) == 0) 
		delete this;
	LOG("CFactory::Release rc = ", rc);
	return rc;
};


HRESULT STDMETHODCALLTYPE CFactory::CreateInstance(IUnknown* pUO, const IID& id, void** ppv) {
	SEQ;
	HRESULT rc = E_UNEXPECTED;	// неожиданный сбой операции по умолчанию.

	OS12* pOs12;	//указатель на объект типа OS12, который будет создан и возвращен как результат.

	if (pUO != NULL)	//если переданный указатель pUO не равен NULL, это означает, что создается агрегированный объект (это связано с механизмом агрегации в COM).
		rc = CLASS_E_NOAGGREGATION;	//ошибка агрегации.
	else if ((pOs12 = new OS12()) == NULL)	//создается новый экземпляр объекта OS12. Если операция выделения памяти не удалась (то есть, new вернул NULL), то
		rc = E_OUTOFMEMORY;	//ошибка выделения памяти.
	else {
		rc = pOs12->QueryInterface(id, ppv); //получение указанного интерфейса (id) от новосозданного объекта OS12
		pOs12->Release();	//для уменьшения счетчика ссылок объекта, так как QueryInterface увеличивает счетчик ссылок
	}

	LOG("CFactory::CreateInstance rc = ", rc);
	return rc;
}
//для блокировки или разблокировки сервера COM.
HRESULT STDMETHODCALLTYPE CFactory::LockServer(BOOL b) {
	SEQ;
	HRESULT rc = S_OK;

	if (b)
		InterlockedIncrement((LONG*)&(g_ServerLocks));	//сервер заблокирован, увеличение числа активных блокировок сервера
	else 
		InterlockedDecrement((LONG*)&(g_ServerLocks));

	LOG("CFactory::LockServer b = ", b);
	return rc;
}
