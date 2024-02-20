#pragma once
#include "INTERFACE.h"
#include <objbase.h>
extern long g_lObjs;
extern long g_lLocks;

class OS12 : public IAdder, public IMultiplier
{
protected:
	// Reference count
	long          m_lRef;	//количества ссылок на объект OS12
	// Эта переменная будет инкрементироваться при вызове метода AddRef() и декрементироваться при вызове метода Release(). Это необходимо для управления временем жизни объекта в COM.

public:
	OS12();	// конструктор
	~OS12(); //деструктор

public:
	// IUnknown
	STDMETHOD(QueryInterface(REFIID, void**));	//позволяет клиентам запрашивать указанный интерфейс
	// QueryInterface: Это имя метода. В контексте COM, QueryInterface - это один из методов интерфейса IUnknown, и он используется для запроса (получения) 
	// указателей на другие интерфейсы, поддерживаемые объектом. Этот метод позволяет клиентскому коду получить доступ к другим функциональным 
	// возможностям объекта, необходимым для его работы.
	// EFIID используется для указания типа запрашиваемого интерфейса
	// void** - это тип возвращаемого значения метода (указатель на указатель на интерфейс)
	STDMETHOD_(ULONG, AddRef());	// увеличения счетчика ссылок на объект
	STDMETHOD_(ULONG, Release());	//для уменьшения счетчика ссылок на объект

	// IAdder
	STDMETHOD(Add(const double, const double, double*));
	STDMETHOD(Sub(const double, const double, double*));
	// IMultiplier
	STDMETHOD(Mul(const double, const double, double*));
	STDMETHOD(Div(const double, const double, double*));
};
