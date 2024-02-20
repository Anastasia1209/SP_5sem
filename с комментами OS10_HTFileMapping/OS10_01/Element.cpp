#include "Element.h"

namespace ht
{
	//Конструктор по умолчанию
	Element::Element() 
	{
		key = nullptr;
		keyLength = 0;
		payload = nullptr;
		payloadLength = 0;
	}

	// для создания объекта Element только с ключом
	Element::Element(const void* key, int keyLength) : Element() //get 
	{
		this->key = key;
		this->keyLength = keyLength;

		//: Element()
		//означает, что при создании объекта с ключом и длиной ключа, конструктор сначала вызывает конструктор по умолчанию
		//для настройки базовых значений, а затем выполняет свою собственную инициализацию, устанавливая переданный ключ и его длину.
	}


	//для создания объекта Element с ключом и значением
	Element::Element(const void* key, int keyLength, const void* payload, int  payloadLength) : Element() //insert
	{
		this->key = key;
		this->keyLength = keyLength;
		this->payload = payload;
		this->payloadLength = payloadLength;
	}

	//для обновления существующего объекта
	Element::Element(const Element* oldElement, const void* newPayload, int  newPayloadLength) : Element() //update
	{
		this->key = oldElement->key;	// копирует информацию о ключе из старого элемента
		this->keyLength = oldElement->keyLength;
		this->payload = newPayload;		//устанавливает новое значение
		this->payloadLength = newPayloadLength;

		delete oldElement;
	}

	//вычисляет общий размер в байтах, необходимый для хранения объекта Element с заданной мах длиной ключа и маx длиной значения
	int getSizeElement(int maxKeyLength, int   maxPayloadLength)         // size element
	{
		return maxKeyLength + maxPayloadLength + sizeof(int) * 2;
		//sizeof(int) * 2 - двух целых чисел, используемых для хранения длин ключа и значения.
	}
}