#include "HT.h"

namespace ht
{
	//Конструктор по умолчанию
	HtHandle::HtHandle()
	{
		this->capacity = 0;
		this->secSnapshotInterval = 0;
		this->maxKeyLength = 0;
		this->maxPayloadLength = 0;
		ZeroMemory(this->fileName, sizeof(this->fileName));//заполняет массив fileName нулями, чтобы исключить случайное содержимое.
		this->file = NULL;	//чтобы указать, что соответствующие ресурсы или файлы пока не открыты.
		this->fileMapping = NULL;
		this->addr = NULL;
		ZeroMemory(this->lastErrorMessage, sizeof(this->lastErrorMessage));
		this->lastSnaptime = 0;
		this->count = 0;
	}

	HtHandle::HtHandle(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const wchar_t* fileName) : HtHandle()
	{
		this->capacity = capacity;
		this->secSnapshotInterval = secSnapshotInterval;
		this->maxKeyLength = maxKeyLength;
		this->maxPayloadLength = maxPayloadLength;
		memcpy(this->fileName, fileName, sizeof(this->fileName));//копирует значение строки fileName в массив this->fileName
	}

	// ф-кция создания
	HtHandle* create(
		int	  capacity,					// емкость хранилища
		int   secSnapshotInterval,		// переодичность сохранения в сек.
		int   maxKeyLength,             // максимальный размер ключа
		int   maxPayloadLength,			// максимальный размер данных
		const wchar_t* fileName)		// имя файла 
	{
		HtHandle* htHandle = createHt(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, fileName);
		runSnapshotTimer(htHandle);

		return htHandle;
	}

	// создание хранилища
	HtHandle* createHt(
		int	  capacity,					// емкость хранилища
		int   secSnapshotInterval,		// переодичность сохранения в сек.
		int   maxKeyLength,             // максимальный размер ключа
		int   maxPayloadLength,			// максимальный размер данных
		const wchar_t* fileName)		// имя файла 
	{
		HANDLE hf = CreateFile(
			fileName,
			GENERIC_WRITE | GENERIC_READ,	// режим доступа к файлу
			NULL,	//режим совместного доступа к файлу УКАЗЫВАЕМ МОНОПОЛЬНЫЙ ДОСТУП
			NULL,	//атрибуты безопасности для файла
			OPEN_ALWAYS,	//если файл существует, он будет открыт. Если нет, он будет создан
			FILE_ATTRIBUTE_NORMAL,	//атрибуты файла
			NULL);	//атрибуты шаблона файла (атрибуты по умолчанию)
		if (hf == INVALID_HANDLE_VALUE)
			throw "create or open file failed";

		//вычисляется размер необходимого отображаемого файла
		int sizeMap = sizeof(HtHandle) + getSizeElement(maxKeyLength, maxPayloadLength) * capacity;

		//создание отображения файла
		// Отображение файла представляет собой область памяти, которая ассоциирована с файлом
		// на диске и позволяет процессам взаимодействовать с файлом через память.
		HANDLE hm = CreateFileMapping(
			hf,		//дескриптор файла
			NULL,	//атрибуты защиты отображения
			PAGE_READWRITE,	//режим доступа к отображению (отображение будет доступно для чтения и записи)
			0,	//начальная позиция в файле, с которой начинается отображение
			sizeMap,	// размер отображения
			fileName);	//имя для отображения файла, которое ассоциируется с отображением
		if (!hm)
			throw "create File Mapping failed";


		//отображение области памяти hm в адресное пространство процесса.
		LPVOID lp = MapViewOfFile(
			hm,		//дескриптор отображения файла 
			FILE_MAP_ALL_ACCESS,	//режим доступа к отображению (отображение будет доступно для всех операций)
			0,	//смещение (отображение начнется с начала файла)
			0,	// размер отображения внутри файла
			0);	//адрес отображения в адресном пространстве процесса(в начале адресного пространства процесса.)
		if (!lp)
			return NULL;

		ZeroMemory(lp, sizeMap);	//чтобы убедиться, что область памяти чиста и не содержит мусорных данных.

		//new размещает объект в предварительно выделенной области памяти, указанной указателем lp
		HtHandle* htHandle = new(lp) HtHandle(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, fileName);
		htHandle->file = hf;
		htHandle->fileMapping = hm;	//отображение файла 
		htHandle->addr = lp;
		htHandle->lastSnaptime = time(NULL);
		//создается мьютекс (mutex) для синхронизации доступа к этому хранилищу
		htHandle->mutex = CreateMutex(
			NULL,	//атрибуты мьютекса.
			FALSE,	//мьютекс создается в разблокированном состоянии т.е. другие потоки или процессы могут начать использовать мьютекс сразу после его создания.
			fileName);	// имя мьютекс

		return htHandle;
	}

	// ---------------------------------------------------------- таймер
	BOOL runSnapshotTimer(HtHandle* htHandle)
	{
		// Ожидаемый таймер - это объект, который можно использовать для запуска операций по истечении определенного времени.
		htHandle->snapshotTimer = CreateWaitableTimer(
			0,		//указатель на объект безопасности
			FALSE,	//таймер будет создан в разблокированном состоянии и будет готов к срабатыванию
			0);		//имя таймера

		LARGE_INTEGER Li{};
		//определяет интервал времени, через который должен запускаться таймер
		Li.QuadPart = -(SECOND * htHandle->secSnapshotInterval);
		
		// настраиваем созданный ранее таймер
		SetWaitableTimer(htHandle->snapshotTimer, 
			&Li,
			1,	//таймер должен срабатывать один раз (если установлен в 0, таймер будет срабатывать периодически).
			snapAsync,	//это функция, которая будет вызвана при срабатывании таймера
			htHandle,
			FALSE);	//таймер не должен быть автоматически сброшен после срабатывания

		return true;
	}

	//по истеч таймера
	void CALLBACK snapAsync(LPVOID prm, DWORD, DWORD)
	{
		HtHandle* htHandle = (HtHandle*)prm;	//Преобразует указатель prm обратно в объект HtHandle
		if (snap(htHandle))
			std::cout << "-- spanshotAsync success" << std::endl;
	}


	// выполнить Snapshot
	BOOL snap(HtHandle* htHandle)  
	{
		WaitForSingleObject(htHandle->mutex, INFINITE);	//захват мьютекса
		if (!FlushViewOfFile(htHandle->addr, NULL)) {
			//FlushViewOfFile сохраняет все изменения в отображении файла
			writeLastError(htHandle, "-- snapshot error");
			ReleaseMutex(htHandle->mutex);	//освободить захваченный мьютекс
			return false;
		}
		htHandle->lastSnaptime = time(NULL);	//обновляет время последнего снимка
		ReleaseMutex(htHandle->mutex);
		return true;
	}



	// --------------------------------------------------------------------------------------------
	HtHandle* open
	(
		const wchar_t* fileName,         // имя файла
		bool isMapFile					 // true если открыть fileMapping; false если открыть файл; по умолчанию false
	)
	{
		HtHandle* htHandle;	//указатель на объект HtHandle после открытия
		if (isMapFile)
		{
			//открытия файла с использованием файлового отображения
			htHandle = openHtFromMapName(fileName);
		}
		else
		{
			// для открытия файла как обычного файла
			htHandle = openHtFromFile(fileName);
			if (htHandle)
				runSnapshotTimer(htHandle);	//запускает таймер снимка данных
		}

		return htHandle;
	}

	HtHandle* openHtFromFile(
		const wchar_t* fileName)
	{
		// Создает дескриптор файла и открывая его для чтения и записи
		HANDLE hf = CreateFile(
			fileName,
			GENERIC_WRITE | GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING, // сущ
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hf == INVALID_HANDLE_VALUE)
			return NULL;


		//Создает файловое отображение файла, связанное с открытым файлом
		HANDLE hm = CreateFileMapping(
			hf,
			NULL,
			PAGE_READWRITE,
			0, 0,
			fileName);
		if (!hm)
			return NULL;

		//Отображает созданное файловое отображение в адресное пространство текущего процесса
		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)
			return NULL;

		HtHandle* htHandle = (HtHandle*)lp;
		htHandle->file = hf;
		htHandle->fileMapping = hm;
		htHandle->addr = lp;
		htHandle->lastSnaptime = time(NULL);
		htHandle->mutex = CreateMutex(
			NULL,
			FALSE,
			fileName);

		return htHandle;
	}

	//открывает хранилище (storage) с данными
	HtHandle* openHtFromMapName(
		const wchar_t* fileName)
	{
		// открывает существующее файловое отображение по его имени 
		HANDLE hm = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,
			false,	//отображение будет открываться только в пределах текущего сеанса
			fileName);
		if (!hm)
			return NULL;

		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)
			return NULL;

		HtHandle* htHandle = new HtHandle();
		//Содержимое отображения файла (представленного указателем lp) копируется в созданный объект 
		memcpy(htHandle, lp, sizeof(HtHandle));

		htHandle->file = NULL;
		htHandle->fileMapping = hm;
		htHandle->addr = lp;
		htHandle->snapshotTimer = NULL;

		return htHandle;	// указатель на объект htHandle, который представляет открытое хранилище данных
	}
	// --------------------------------------------------------------------------------------------






	BOOL insert		// добавить элемент в хранилище
	(
		HtHandle* htHandle,            // управление HT
		const Element* element)              // элемент
	{
		if (htHandle->count >= htHandle->capacity) // проверка на свободные ячейки
		{
			writeLastError(htHandle, "-- not found free memory");
			return false;
		}

		// для блокировки доступа к хранилищу
		WaitForSingleObject(htHandle->mutex, INFINITE);// блокирует мьютексом
		int freeIndex = findFreeIndex(htHandle, element);

		writeToMemory(htHandle, element, freeIndex);
		incrementCount(htHandle);
		ReleaseMutex(htHandle->mutex);

		return true;
	}


	// =====================================================================================================================

	int findFreeIndex(
		const HtHandle* htHandle,           // управление HT	//указатель на объект HtHandle, который управляет хранилищем данных.
		const Element* element)				// элемент
	{
		//Вычисляется начальный индекс с использованием хеш-функции
		int index = hashFunction((char*)element->key, htHandle->capacity);
		//Хеш-функция преобразует ключ элемента в число, которое затем используется для определения индекса в пределах емкости хранилища.


		Element* foundElement = new Element();	//временный объект для чтения данных из хранилища
		do //пока foundElement содержит данные, индекс занят
		{
			//проверяет, не вышел ли индекс за пределы максимальной емкости хранилища
			if (index >= htHandle->capacity)
			{
				index = -1;
				break;
			}

			readFromMemory(htHandle, foundElement, index);	//для чтения элемента из хранилища по текущему индексу index и сохранения его в объект foundElement
			//рассчитывает следующий индекс
			index = nextHash(index, (char*)element->key, htHandle->capacity);

		} 
		// пока foundElement содержит данные, индекс занят, и цикл продолжает поиск следующего свободного индекса.
		while (
			foundElement->keyLength != NULL &&
			foundElement->payloadLength != NULL
			);

		delete foundElement;
		return index - 1;
	}

	// --- хэш ---

	int hashFunction(const char* key, int capacity)
	{
		unsigned long i = 0;
		for (int j = 0; key[j]; j++)
			i += key[j];	//прибавляется ASCII-код текущего символа строки
		return i % capacity;	//остаток от деления
	}


	int nextHash(int currentHash, const char* key, int capacity)
	{
		return ++currentHash;
	}
	// -----------


	// --- память ---
	BOOL writeToMemory(const HtHandle* const htHandle, const Element* const element, int index)
	{
		LPVOID lp = htHandle->addr;	//адрес начала хранилища

		lp = (HtHandle*)lp + 1;	//сдвигает указатель на следующую ячейку памяти
		lp = (byte*)lp + getSizeElement(htHandle->maxKeyLength, htHandle->maxPayloadLength) * index;

		//Копируется ключ элемента(element->key) в память по текущему указателю lp.
		memcpy(lp, element->key, element->keyLength);
		//Указатель lp сдвигается на размер максимальной длины ключа. Это переводит указатель на следующее место в памяти для записи длины ключа
		lp = (byte*)lp + htHandle->maxKeyLength;
		//Копируется длина ключа
		memcpy(lp, &element->keyLength, sizeof(int));
		lp = (int*)lp + 1;	// сдвигается на 1 интовую единицу
		//Копируются данные элемента
		memcpy(lp, element->payload, element->payloadLength);
		lp = (byte*)lp + htHandle->maxPayloadLength;
		//Копируется длина данных элемента
		memcpy(lp, &element->payloadLength, sizeof(int));

		return true;
	}

	Element* readFromMemory(const HtHandle* const htHandle, Element* const element, int index)
	{
		LPVOID lp = htHandle->addr;

		lp = (HtHandle*)lp + 1;
		lp = (byte*)lp + getSizeElement(htHandle->maxKeyLength, htHandle->maxPayloadLength) * index;

		element->key = lp;	//
		lp = (byte*)lp + htHandle->maxKeyLength;
		element->keyLength = *(int*)lp;	//ключа из памяти
		lp = (int*)lp + 1;
		element->payload = lp;
		lp = (byte*)lp + htHandle->maxPayloadLength;
		element->payloadLength = *(int*)lp;

		return element;
	}

	// -----------
	// 
	// =====================================================================================================================







	Element* get     //  читать элемент из хранилища
	(
		HtHandle* htHandle,            // управление HT
		const Element* element)              // элемент 
	{
		int index = findIndex(htHandle, element);
		if (index < 0)
		{
			writeLastError(htHandle, "-- not found element (GET)");
			return NULL;
		}

		Element* foundElement = new Element();
		readFromMemory(htHandle, foundElement, index);

		return foundElement;
	}

	
	BOOL update     //  изменить элемент в хранилище
	(
		HtHandle* htHandle,            // управление HT
		const Element* oldElement,          // старый элемент (ключ, размер ключа)
		const void* newPayload,          // новые данные  
		int             newPayloadLength)     // размер новых данных
	{
		//Блокируем мьютекс
		WaitForSingleObject(htHandle->mutex, INFINITE);
		int index = findIndex(htHandle, oldElement);
		if (index < 0)
		{
			writeLastError(htHandle, "-- not found element (UPDATE)");
			ReleaseMutex(htHandle->mutex);
			return false;
		}

		Element* updateElement = new Element(oldElement, newPayload, newPayloadLength);
		writeToMemory(htHandle, updateElement, index);
		ReleaseMutex(htHandle->mutex);

		return true;
	}

	BOOL remove      // удалить элемент в хранилище
	(
		HtHandle* htHandle,            // управление HT (ключ)
		const Element* element)				 // элемент 
	{
		WaitForSingleObject(htHandle->mutex, INFINITE);
		int index = findIndex(htHandle, element);
		if (index < 0)
		{
			writeLastError(htHandle, "-- not found element (DELETE)");
			ReleaseMutex(htHandle->mutex);
			return false;
		}

		clearMemoryByIndex(htHandle, index);
		decrementCount(htHandle);
		ReleaseMutex(htHandle->mutex);

		return true;
	}

	// ============================== find index =====================================================

	int findIndex(
		const HtHandle* htHandle,           // управление HT
		const Element* element)				// элемент
	{
		//Вычисляется хеш элемента
		int index = hashFunction((char*)element->key, htHandle->capacity);

		Element* foundElement = new Element();
		// пока не будет найден элемент с искомым ключом
		do
		{
			if (index >= htHandle->capacity)
			{
				index = -1;
				break;
			}

			readFromMemory(htHandle, foundElement, index);
			index = nextHash(index, (char*)element->key, htHandle->capacity);
		} while (
			memcmp(foundElement->key, element->key, element->keyLength) != NULL);

		delete foundElement;
		return index - 1;
	}



	BOOL clearMemoryByIndex(const HtHandle* const htHandle, int index)
	{
		LPVOID lp = htHandle->addr;
		int sizeElement = getSizeElement(htHandle->maxKeyLength, htHandle->maxPayloadLength);

		lp = (HtHandle*)lp + 1;
		lp = (byte*)lp + sizeElement * index;

		//для заполнения памяти, на которую указывает lp, нулями
		ZeroMemory(lp, sizeElement);

		return true;
	}

	//=====================================================================================================



	void print                               // распечатать элемент 
	(
		const Element* element)              // элемент 
	{
		std::cout << "Element:" << std::endl;
		std::cout << "{" << std::endl;
		std::cout << "\t\"key\": \"" << (char*)element->key << "\"," << std::endl;
		std::cout << "\t\"keyLength\": " << element->keyLength << "," << std::endl;
		std::cout << "\t\"payload\": \"" << (char*)element->payload << "\"," << std::endl;
		std::cout << "\t\"payloadLength\": " << element->payloadLength << std::endl;
		std::cout << "}" << std::endl;
	}

	BOOL close        // snap и закрыть HT  и  очистить htHandle
	(
		const HtHandle* htHandle)           // управление HT (File, FileMapping)
	{
		HANDLE hf = htHandle->file;
		HANDLE hfm = htHandle->fileMapping;

		// Отменяется ожидаемый таймер снимка
		if (!CancelWaitableTimer(htHandle->snapshotTimer))
			throw "cancel snapshotTimer failed";
		//Освобождается отображение представления файла
		if (!UnmapViewOfFile(htHandle->addr))
			throw "unmapping view failed";
		if (!CloseHandle(hfm))
			throw "close File Mapping failed";
		if (hf != NULL)
			if (!CloseHandle(hf))
				throw "close File failed";

		return true;
	}










	int incrementCount(HtHandle* const htHandle)
	{
		return ++htHandle->count;
	}

	int decrementCount(HtHandle* const htHandle)
	{
		return --htHandle->count;
	}

	const char* writeLastError(HtHandle* const htHandle, const char* msg)
	{
		memcpy(htHandle->lastErrorMessage, msg, sizeof(htHandle->lastErrorMessage));
		return htHandle->lastErrorMessage;
	}

	const char* getLastError  // получить сообщение о последней ошибке
	(
		const HtHandle* htHandle)				// управление HT
	{
		return htHandle->lastErrorMessage;
	}
}