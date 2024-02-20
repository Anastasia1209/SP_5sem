﻿#pragma warning(disable : 4996)
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "../OS13_HTCOM_LIB/pch.h"
#include "../OS13_HTCOM_LIB/OS13_HTCOM_LIB.h"

#ifdef _WIN64
#pragma comment(lib, "D:/Univer/5sem/SP/SP_lab5/SP_Lab5/x64/Debug/OS13_HTCOM_LIB.lib")
#else
#pragma comment(lib, "../Debug/OS13_HTCOM_LIB.lib")
#endif

using namespace std;

wchar_t* getWC(const char* c);
wchar_t* getFileName(const char* c);

int main(int argc, char* argv[])
{
	HANDLE event = CreateEvent(NULL,TRUE, FALSE, getFileName(argv[1]));
	OS13_HTCOM_HANDEL h = OS13_HTCOM::Init();
	ht::HtHandle* ht = nullptr;

	wchar_t* fileName = getWC(argv[1]);
	ht = OS13_HTCOM::HT::open(h, fileName, false);
	if (ht)
	{
		cout << "==============  HT-Storage Start  ==============" << endl;
		wcout << "filename:\t\t" << ht->fileName << endl;
		cout << "secSnapshotInterval:\t" << ht->secSnapshotInterval << endl;
		cout << "capacity:\t\t" << ht->capacity << endl;
		cout << "maxKeyLength:\t\t" << ht->maxKeyLength << endl;
		cout << "maxPayloadLength:\t" << ht->maxPayloadLength << endl << endl;

		while (WaitForSingleObject(event, 0) == WAIT_TIMEOUT) {
			SleepEx(0, TRUE);
		}
		OS13_HTCOM::HT::snap(h, ht);
		OS13_HTCOM::HT::close(h, ht);
	}
	else
		cout << "-- open: error" << endl; 
	OS13_HTCOM::Dispose(h);
}

wchar_t* getFileName(const char* c)
{
	const char* lastSlash = strrchr(c, '/');
	const char* filename = (lastSlash != NULL) ? lastSlash + 1 : c;
	return getWC(filename);
}

wchar_t* getWC(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}
