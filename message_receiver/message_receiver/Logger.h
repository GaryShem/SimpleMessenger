#include <windows.h>

namespace logger {

	__declspec(dllimport) enum LogLevel { LOGLEVEL_ERROR = 0, LOGLEVEL_SYSTEM = 1, LOGLEVEL_WARNING = 1, LOGLEVEL_INFO = 1, LOGLEVEL_DEBUG = 2 };

	__declspec(dllimport) void LogInit(const char* logPath, const char* prefix, int logLevel);
	__declspec(dllimport) void LogWrite(const char* logMessage, int logSeverity);

}