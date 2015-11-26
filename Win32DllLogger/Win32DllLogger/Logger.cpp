#include "stdafx.h"
#include "Logger.h"

void logger::LogInit(const char* logPath, const char* prefix, int logLevel)
{
	SYSTEMTIME st;
	std::ostringstream ss;
	GetSystemTime(&st);
	ss << logPath << prefix << "_" << st.wDay << "." << st.wMonth << "." << st.wYear << ".txt";
	filename = ss.str();
	std::string temp = filename;
	InitializeCriticalSection(&logProtection);

	level = logLevel;
}

void logger::LogWrite(const char* logMessage, int logSeverity)
{
	EnterCriticalSection(&logProtection);
	HANDLE file = CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file != INVALID_HANDLE_VALUE)
	{
		if (logSeverity <= level)
		{
			SetFilePointer(file, NULL, NULL, FILE_END);
			SYSTEMTIME st;
			GetSystemTime(&st);
			std::ostringstream msgStream;
			msgStream << st.wHour << "." << st.wMinute << "." << st.wSecond;
			switch (logSeverity)
			{
			case 2:
				msgStream << " (DEBUG) ";
				break;
			case 1:
				msgStream << " (WARNING/SYSTEM/INFO) ";
				break;
			case 0:
				msgStream << " (ERROR) ";
				break;
			}
			msgStream << logMessage << "\r\n";
			std::string msg = msgStream.str();
			size_t length = strlen(msg.c_str());
			WriteFile(file, msg.c_str(), length, NULL, NULL);
		}
	}
	CloseHandle(file);
	LeaveCriticalSection(&logProtection);
}