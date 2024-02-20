#include "HT.h"

using namespace std;

int main()
{
	ht::HtHandle* ht = nullptr;
	try
	{
		ht = ht::Create(
			1000,	// capacity
			1,		// seconds snapshot
			20,		// max key length
			256,	// max payload length
			L"./files/HTspace.ht"
		);
		if (ht)
			cout << "-- create: success" << endl;
		else
			throw "-- create: error";
	
		if (ht::Insert(ht, new ht::Element("myKey", 10, "payloadData", 15)))
			cout << "-- insert: success" << endl;
		else
			throw "-- insert: error";

		ht::Element* hte = ht::Get(ht, new ht::Element("myKey", 10));
		if (hte)
			cout << "-- get: success" << endl;
		else
			throw "-- get: error";
	
		ht::print(hte);

		if (ht::Update(ht, hte, "newPayloadData", 20))
			cout << "-- update: success" << endl;
		else
			throw "-- update: error";

		if (ht::Snap(ht))
			cout << "-- snapSync: success" << endl;
		else
			throw "-- snap: error";

		hte = ht::Get(ht, new ht::Element("myKey", 5));
		if (hte)
			cout << "-- get: success" << endl;
		else
			throw "-- get: error";

		ht::print(hte);

		SleepEx(3000, TRUE);	

		if (ht::Delete(ht, hte))
			cout << "-- Delete: success" << endl;
		else
			throw "-- Delete: error";

		hte = ht::Get(ht, new ht::Element("myKey", 5));
		if (hte)
			cout << "-- get: success" << endl;
		else
			throw "-- get: error";
	}
	catch (const char* msg)
	{
		cout << msg << endl;
	
		if (ht != nullptr)
			cout << ht::GetLastError(ht) << endl;
	}

	try
	{
		if (ht != nullptr)
			if (ht::Close(ht))
				cout << "-- close: success" << endl;
			else
				throw "-- close: error";
	}
	catch (const char* msg)
	{
		cout << msg << endl;
	}
}