#include "stdafx.h"
#include "Logger.h"
#include <string>
#include <sstream>

void logger::LogInit(const char* logPath, const char* prefix, int logLevel)
{
	SYSTEMTIME st;
	std::ostringstream ss;
	GetSystemTime(&st);
	ss << logPath << prefix << "_" << st.wDay << "." << st.wMonth << "." << st.wYear << ".txt";
	std::string fullFilename = ss.str();
	file = CreateFile(fullFilename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(file, NULL, NULL, FILE_END);

	InitializeCriticalSection(&logProtection);

	logger::level = logLevel;
}

void logger::LogWrite(const char* logMessage, LogLevel logSeverity)
{
	EnterCriticalSection(&logProtection);
	if (file != INVALID_HANDLE_VALUE)
	{
		if (logSeverity <= level)
		{
			SYSTEMTIME st;
			GetSystemTime(&st);
			std::ostringstream msg;
			msg << st.wHour << "." << st.wMinute << "." << st.wSecond;
			switch (logSeverity)
			{
			case LOGLEVEL_DEBUG:
				msg << " (DEBUG) ";
				break;
			case LOGLEVEL_INFO:
				msg << " (INFO) ";
				break;
			case LOGLEVEL_SYSTEM:
				msg << " (SYSTEM) ";
				break;
			case LOGLEVEL_WARNING:
				msg << " (WARNING) ";
				break;
			case LOGLEVEL_ERROR:
				msg << " (ERROR) ";
				break;
			}
			msg << logMessage << "\r\n";
			WriteFile(file, msg.str().c_str(), strlen(msg.str().c_str()), NULL, NULL);
		}
	}
	LeaveCriticalSection(&logProtection);
}