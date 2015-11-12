// message_receiver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <string>
#include <iostream>
#include <memory>


int _tmain(int argc, _TCHAR* argv[])
{
	std::unique_ptr<char[]> msg(new char[300]);
	
	
	while (true)
	{
		HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\smpipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		260,
		0,
		0,
		NULL);
		if (hPipe == INVALID_HANDLE_VALUE)
		return -1;
		ConnectNamedPipe(hPipe, NULL);
		DWORD dwBytesRead;
		ReadFile(hPipe, msg.get(), 260, &dwBytesRead, NULL);
		if (dwBytesRead > 0)
			std::cout << msg.get() << std::endl;
		CloseHandle(hPipe);
	}
	return 0;
}

