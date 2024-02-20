#pragma warning(disable : 4996)

#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>

#include "../OS14_HTCOM_LIB/pch.h"
#include "../OS14_HTCOM_LIB/OS14_HTCOM_LIB.h"

#pragma comment(lib, "D:/Univer/5sem/SP/os14/x64/Debug/OS14_HTCOM_LIB.lib")


using namespace std;

wchar_t* getWC(const char* c);
wchar_t* getFileName(const char* c);
string intToString(int number);
int charToInt(char* str);
string incrementPayload(char* str);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");


	try
	{
		const char* path = argv[1];
		OS14_HTCOM_HANDEL h = OS14_HTCOM::Init();

		wchar_t* fileName = getWC(argv[1]);

		ht::HtHandle* ht;

		if (argc == 4)
		{
			wchar_t* username = getWC(argv[2]);
			wchar_t* password = getWC(argv[3]);

			ht = OS14_HTCOM::HT::open(h, fileName, username, password, true);
		}
		else
		{
			ht = OS14_HTCOM::HT::open(h, fileName, true);
		}

		if (ht) {
			cout << "-- open: success" << endl;
		}
		else
			throw "-- open: error";

		HANDLE event = CreateEvent(NULL, TRUE, FALSE, getFileName(path));

		while (WaitForSingleObject(event, 0) == WAIT_TIMEOUT) {
			int numberKey = rand() % 50;
			string key = intToString(numberKey);
			cout << key << endl;

			ht::Element* element = OS14_HTCOM::HT::get(h, ht, OS14_HTCOM::Element::createGetElement(h, key.c_str(), key.length() + 1));
			if (element)
			{
				cout << "-- get: success" << endl;
				string newPayload = incrementPayload((char*)element->payload);

				if (OS14_HTCOM::HT::update(h, ht, element, newPayload.c_str(), newPayload.length() + 1)) {
					cout << "-- update: success" << endl;
				}
				else {
					cout << "-- update: error" << endl;
				}
			}
			else {
				cout << "-- get: error" << endl;
			}

			Sleep(1000);
		}

		OS14_HTCOM::HT::close(h, ht);

		OS14_HTCOM::Dispose(h);
	}
	catch (const char* e) { cout << e << endl; }
	catch (int e) { cout << "HRESULT: " << e << endl; }

}

wchar_t* getWC(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}

wchar_t* getFileName(const char* c)
{
	const char* lastSlash = strrchr(c, '/');
	const char* filename = (lastSlash != NULL) ? lastSlash + 1 : c;
	return getWC(filename);
}

string intToString(int number)
{
	stringstream convert;
	convert << number;

	return convert.str();
}

int charToInt(char* str)
{
	stringstream convert;
	convert << str;
	int num;
	convert >> num;

	return num;
}

string incrementPayload(char* str)
{
	int oldNumberPayload = charToInt(str);
	int newNumberPayload = oldNumberPayload + 1;
	string newPayload = intToString(newNumberPayload);

	return newPayload;
}