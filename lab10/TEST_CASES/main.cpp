#include "tests.h"

using namespace std;

int main()
{
	ht::HtHandle* ht = ht::Create(1000, 3, 10, 256, L"./files/HTspace.ht");
	if (ht)
		cout << "-- create: success" << endl;
	else
		throw "-- create: error";

	if (tests::TestGetFunction(ht))
		cout << "-- TestGetFunction: success" << endl;
	else
		cout << "-- TestGetFunction: error" << endl;

	if (tests::TestDoubleInsert(ht))
		cout << "-- TestDoubleInsert: success" << endl;
	else
		cout << "-- TestDoubleInsert: error" << endl;

	if (tests::TestDeleteFunction(ht))
		cout << "-- TestDeleteFunction: success" << endl;
	else
		cout << "-- TestDeleteFunction: error" << endl;

	if (tests::TestUpdateFunction(ht))
		cout << "-- TestUpdateFunction: success" << endl;
	else
		cout << "-- TestUpdateFunction: error" << endl;

	if (ht != nullptr)
		if (ht::Close(ht))
			cout << "-- close: success" << endl;
		else
			throw "-- close: error";
}