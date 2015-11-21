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
	std::unique_ptr<char[]> msg(new char[300]);

	std::string path = "D:\\Igor\\C++\\OSWindows\\CustomLogs\\";
	std::string prefix = "sm";

	logger::LogInit(path.c_str(), prefix.c_str(), 2);
	
	HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\smpipe", PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES, 260, 0, 0, NULL);

	while (true)
	{
		if (hPipe == INVALID_HANDLE_VALUE)
			return -1;
		ConnectNamedPipe(hPipe, NULL);
		logger::LogWrite("Client connected", logger::LOGLEVEL_INFO);
		DWORD dwBytesRead;
		ReadFile(hPipe, msg.get(), 260, &dwBytesRead, NULL);
		msg[dwBytesRead] = '\0';
		if (dwBytesRead > 0)
		{
			std::cout << msg.get() << std::endl;
			logger::LogWrite(msg.get(), logger::LOGLEVEL_INFO);
		}
		DisconnectNamedPipe(hPipe);
	}
	return 0;
}

