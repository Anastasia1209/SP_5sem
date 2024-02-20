#pragma once
#ifdef _WIN64
#pragma comment(lib, "../x64/debug/OS10_HTAPI.lib")
#else
#pragma comment(lib, "../debug/OS10_HTAPI.lib")
#endif

#include "../OS10_HTAPI/pch.h"
#include "../OS10_HTAPI/HT.h"

namespace tests
{
	BOOL TestGetFunction(ht::HtHandle* htHandle);
	BOOL TestDoubleInsert(ht::HtHandle* htHandle);
	BOOL TestDeleteFunction(ht::HtHandle* htHandle);
	BOOL TestUpdateFunction(ht::HtHandle* htHandle);
}
