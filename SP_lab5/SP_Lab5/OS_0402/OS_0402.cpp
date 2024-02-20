#pragma warning(disable : 4996)
#include <string>
#include <sstream>
#include <fstream>
#include "../OS13_HTCOM_LIB/pch.h"
#include "../OS13_HTCOM_LIB/OS13_HTCOM_LIB.h"
using namespace std;

#pragma comment(lib, "D:/Univer/5sem/SP/SP_lab5/SP_Lab5/x64/Debug/OS13_HTCOM_LIB.lib")

string intToString(int number);
wchar_t* getWC(const char* c);
wchar_t* getFileName(const char* c);
int main(int argc, char* argv[])
{
	try
	{
		const char* path;
		ofstream logFile("insert.log", ios::app);
		OS13_HTCOM_HANDEL h = OS13_HTCOM::Init();
		ht::HtHandle* ht ;
		if (argc > 1) {
			path = argv[1];
			ht = OS13_HTCOM::HT::open(h,getWC(argv[1]), true);
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

			ht::Element* element = OS13_HTCOM::Element::createInsertElement(h,key.c_str(), key.length() + 1, "0", 2);
			if (OS13_HTCOM::HT::insert(h,ht, element)) {
				cout << "-- insert: success" << endl;
				logFile << "-- insert: success" << endl;
			}
			else {
				cout << "-- insert: error" << endl;
				logFile << "-- insert: error" << endl;
			}
			delete element;

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
