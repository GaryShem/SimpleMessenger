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

	logger::level = static_cast<LogLevel>(logLevel);
}

void logger::LogWrite(const char* logMessage, int logSeverity)
{
	if (file != INVALID_HANDLE_VALUE)
	{
		if (logSeverity <= level)
		{
			WriteFile(file, logMessage, strlen(logMessage), NULL, NULL);
		}
	}
}