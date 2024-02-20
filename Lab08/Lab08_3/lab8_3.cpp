#include <iostream>
#include <stdexcept>

using namespace std;

int main() {
    int x = 12;
    int* px = nullptr;

    try {
        cout << "code 1" << endl;
        cout << "x= " << *px << endl;
        cout << "code 2" << endl;
    }
    catch (...) {
        cerr << "Exception caught " << endl;

        px = &x;
    }

    cout << "x= " << *px << endl;

    return 0;
}
