
#include <iostream>
#include <Windows.h>

#pragma warning(disable : 4996)

using namespace std;

int main(int argc, char* argv[]) {

	double x = 0;

    int cw = _controlfp(0, 0);
    cw &= ~(EM_OVERFLOW | EM_ZERODIVIDE);
    _controlfp(cw, _MCW_EM);


	__try {
        double y = 5;
		cout << "try-code 1" << endl;
		y /= x;
		cout << "y = " << y << endl;
		cout << "try-code 2" << endl;

	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

		DWORD exceptionCode = GetExceptionCode();
		cerr << "Exception caught (code: 0x" << std::hex << exceptionCode << "): Division by zero" << endl;
	
	}
 
	return 0;
}
