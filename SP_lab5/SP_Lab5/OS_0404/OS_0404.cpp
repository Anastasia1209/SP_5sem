#pragma warning(disable : 4996)
#include <string>
#include <sstream>
#include <fstream>
#include "../OS13_HTCOM_LIB/pch.h"
#include "../OS13_HTCOM_LIB/OS13_HTCOM_LIB.h"

#pragma comment(lib, "D:/Univer/5sem/SP/SP_lab5/SP_Lab5/x64/Debug/OS13_HTCOM_LIB.lib")

using namespace std;

wchar_t* getWC(const char* c);
wchar_t* getFileName(const char* c);
string intToString(int number);
int charToInt(char* str);
string incrementPayload(char* str);

int main(int argc, char* argv[])
{
	try
	{
		const char* path;
		ofstream logFile("update.log", ios::app);
		OS13_HTCOM_HANDEL h = OS13_HTCOM::Init();
		ht::HtHandle* ht;
		if (argc > 1) {
			path = argv[1];
			ht = OS13_HTCOM::HT::open(h, getWC(argv[1]), true);
		}
		else {
			path = "../../MyTable.ht";
			ht = OS13_HTCOM::HT::open(h, L"../../MyTable.ht", true);
		}
		if (ht) {
			cout << "-- open: success" << endl;
			logFile << "-- open: success" << endl;
		}
		else
			throw "-- open: error";
		HANDLE event = CreateEvent(NULL, TRUE, FALSE, getFileName(path));
		while (WaitForSingleObject(event, 0) == WAIT_TIMEOUT) {
			int numberKey = rand() % 50;
			string key = intToString(numberKey);
			cout << key << endl;
			logFile << key << endl;

			ht::Element* element = OS13_HTCOM::HT::get(h,ht, OS13_HTCOM::Element::createGetElement(h,key.c_str(), key.length() + 1));
			if (element)
			{
				cout << "-- get: success" << endl;
				logFile << "-- get: success" << endl;
				string newPayload = incrementPayload((char*)element->payload);

				if (OS13_HTCOM::HT::update(h,ht, element, newPayload.c_str(), newPayload.length() + 1)) {
					cout << "-- update: success" << endl;
					logFile << "-- update: success" << endl;
				}
				else {
					cout << "-- update: error" << endl;
					logFile << "-- update: error" << endl;
				}
			}
			else {
				cout << "-- get: error" << endl;
				logFile << "-- get: error" << endl;
			}

			Sleep(1000);
		}
		OS13_HTCOM::HT::close(h, ht);
		OS13_HTCOM::Dispose(h);
	}
	catch (const char* msg)
	{
		cout << msg << endl;
	}
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
