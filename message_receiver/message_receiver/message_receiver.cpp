// message_receiver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <string>
#include <iostream>
#include <memory>
#include "Logger.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char buffer[300];
	char prefix[] = "sm";
	GetCurrentDirectory(300, buffer);
	strncpy_s(buffer, 300, "..\\", strlen("..\\"));
	logger::LogInit(buffer, prefix, 2);
	
	HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\smpipe", PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES, 260, 0, 0, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		logger::LogWrite("Could not create connection", logger::LOGLEVEL_ERROR);
		return -1;
	}

	while (true)
	{
		ConnectNamedPipe(hPipe, NULL);
		logger::LogWrite("Client connected", logger::LOGLEVEL_SYSTEM);
		DWORD dwBytesRead;
		ReadFile(hPipe, buffer, 260, &dwBytesRead, NULL);
		logger::LogWrite("Message received", logger::LOGLEVEL_DEBUG);
		buffer[dwBytesRead] = '\0';
		if (dwBytesRead > 0)
		{
			std::cout << buffer << std::endl;
			logger::LogWrite(buffer, logger::LOGLEVEL_INFO);
		}
		DisconnectNamedPipe(hPipe);
		logger::LogWrite(buffer, logger::LOGLEVEL_SYSTEM);
	}
	return 0;
}

