#ifndef LOGGER_H_
#define LOGGER_H_

#include <windows.h>


namespace logger {

	__declspec(dllexport) enum LogLevel { LOGLEVEL_ERROR = 0, LOGLEVEL_SYSTEM, LOGLEVEL_WARNING, LOGLEVEL_INFO, LOGLEVEL_DEBUG };

	int level;
	HANDLE file;
	CRITICAL_SECTION logProtection;
	__declspec(dllexport) void LogInit(const char* logPath, const char* prefix, int logLevel);
	__declspec(dllexport) void LogWrite(const char* logMessage, LogLevel logSeverity);
}

#endif