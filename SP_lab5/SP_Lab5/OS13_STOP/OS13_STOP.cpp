#pragma warning(disable : 4996)
#include <windows.h>
#include <iostream>

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

int main()
{
	char path[256];
	HANDLE event;
	int answer;
	bool flagExit = true;
	while (flagExit) {
		std::cout << "1 - Close table \n0 - Stop programm" << std::endl;
		std::cin >> answer;
		switch (answer)
		{
		case 1:
			std::cout << "Enter name of file" << std::endl;
			std::cin >> path;
			event = CreateEvent(NULL, TRUE, FALSE, getFileName(path));
			SetEvent(event);
			break;
		case 0:
			flagExit = false;
			break;
		default:
			break;
		}
	}
}

