#ifndef LOGGER_H_
#define LOGGER_H_

#include <windows.h>
#include <string>
#include <sstream>

namespace logger {

	__declspec(dllexport) enum LogLevel { LOGLEVEL_ERROR = 0, LOGLEVEL_SYSTEM = 1, LOGLEVEL_WARNING = 1, LOGLEVEL_INFO = 1, LOGLEVEL_DEBUG = 2 };

	int level;
	std::string filename;
	CRITICAL_SECTION logProtection;
	__declspec(dllexport) void LogInit(const char* logPath, const char* prefix, int logLevel);
	__declspec(dllexport) void LogWrite(const char* logMessage, int logSeverity);
}

#endif