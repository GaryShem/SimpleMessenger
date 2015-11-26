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
	char msg[300];

	std::string path = "D:\\Igor\\C++\\OSWindows\\CustomLogs\\";
	std::string prefix = "sm";

	logger::LogInit(path.c_str(), prefix.c_str(), 2);
	
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
		ReadFile(hPipe, msg, 260, &dwBytesRead, NULL);
		msg[dwBytesRead] = '\0';
		if (dwBytesRead > 0)
		{
			std::cout << msg << std::endl;
			logger::LogWrite(msg, logger::LOGLEVEL_INFO);
		}
		DisconnectNamedPipe(hPipe);
		logger::LogWrite(msg, logger::LOGLEVEL_SYSTEM);
	}
	return 0;
}

