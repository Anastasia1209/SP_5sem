#include "tests.h"

namespace tests
{
	BOOL TestGetFunction(ht::HtHandle* htHandle)
	{
		ht::Element* insertEl = new ht::Element("test1", 6, "test1", 6);

		ht::Insert(htHandle, insertEl);
		ht::Delete(htHandle, insertEl);

		const char ErrorCode[] = "-- not found element(GET)";

		ht::Element* getElement = ht::Get(htHandle, new ht::Element("test1", 6));

		return (memcmp(ht::GetLastError(htHandle), ErrorCode, sizeof(ErrorCode)));
	}

	BOOL TestDoubleInsert(ht::HtHandle* htHandle)
	{
		ht::Element* element = new ht::Element("test2", 6, "test2", 6);

		ht::Insert(htHandle, element);
		ht::Insert(htHandle, element);

		const char ErrorCode[] = "-- not found free memory";

		return(memcmp(ht::GetLastError(htHandle), ErrorCode, sizeof(ErrorCode)));
	}

	BOOL TestDeleteFunction(ht::HtHandle* htHandle)
	{
		ht::Element* element = new ht::Element("test3", 6, "test3", 6);

		ht::Insert(htHandle, element);
		ht::Delete(htHandle, element);
		
		const char ErrorCode[] = "-- not found element (DELETE) ";

		ht::Delete(htHandle, element);

		return(memcmp(ht::GetLastError(htHandle), ErrorCode, sizeof(ErrorCode)));
	}

	BOOL TestUpdateFunction(ht::HtHandle* htHandle)
	{
		ht::Element* element = new ht::Element("test3", 6, "test3", 6);

		const char ErrorCode[] = "-- not found element (UPDATE) ";

		ht::Update(htHandle, element, "new payLoad", 11);

		return(memcmp(ht::GetLastError(htHandle), ErrorCode, sizeof(ErrorCode)));
	}
}