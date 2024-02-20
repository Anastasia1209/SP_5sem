#include <iostream>
#include <Windows.h>

using namespace std;

int main(int argc, char* argv[]) {

    int x = 12;
    int *px = NULL;

    __try {
        cout << "code 1" << endl;
        cout << "x= " << *px << endl;
        cout << "code 2" << endl;

    }
    __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH){
        DWORD exceptionCode = GetExceptionCode();
        cerr << "Exception caught (code: 0x" << std::hex << exceptionCode << ")" << endl;

        px = &x;
    }
    cout << "x= " << *px << endl;

    return 0;
}
