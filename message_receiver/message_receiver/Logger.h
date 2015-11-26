#include <windows.h>

namespace logger {

	namespace logger {

		__declspec(dllimport) enum LogLevel { LOGLEVEL_ERROR = 0, LOGLEVEL_SYSTEM, LOGLEVEL_WARNING, LOGLEVEL_INFO, LOGLEVEL_DEBUG };

		__declspec(dllimport) void LogInit(const char* logPath, const char* prefix, int logLevel);
		__declspec(dllimport) void LogWrite(const char* logMessage, LogLevel logSeverity);
	}

}