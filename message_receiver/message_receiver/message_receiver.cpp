// message_receiver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <string>
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{

	HANDLE hMutex = CreateMutex(NULL, FALSE, "SimpleMessengerMutex");
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, "SimpleMessengerEvent");
	HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE,
		0, 300, "SimpleMessengerMapping");
	char* pBuffer = (char*)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	int i = 0;
	while (true)
	{
		WaitForSingleObject(hEvent, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		i++;
		std::cout << pBuffer << std::endl;
		ReleaseMutex(hMutex);
		if (i > 20)
			break;
	}
	CloseHandle(hMutex);
	CloseHandle(hEvent);
	CloseHandle(hFileMapping);
	return 0;
}

