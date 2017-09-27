#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#include "sdkapi_logger.h"
#include "string.h"

static const uint16_t LINE_BUFFER_SIZE = 255;
static const char *LEVEL[] __attribute__((unused)) = {
	"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
};

static char *pVersion = NULL;
void SDKAPI_LogInit()
{
	/* Initialize pVersion variable */
	pVersion = NULL;
}

void SDKAPI_LogShutdown()
{
	/* Free the memory allocated for pVersion */
	free(pVersion);
}

void SDKAPI_LogSetVersion(char *version)
{
	if (NULL == version) {
		return;
	}

	if (NULL != pVersion) {
		free(pVersion);
	}

	/* Allocate memory for pVersion and set the version information */
	pVersion = (char *)malloc(strlen(version) + 1);
	if (NULL == pVersion) {
		printf("Error while allocating memory for version");
		return;
	}
	memset(pVersion, 0, strlen(version) + 1);
	strncpy(pVersion, version, strlen(version));
}

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param logStr - log string
 */
void SDKAPI_Log(SDKAPI_LogLevel level, const char *tag, const char *funcName, const int16_t lineNum, const char *logStr)
{
	if (!logStr || !tag) {
		return;
	}

	struct timeval tv;
	time_t nowtime = 0;
	struct tm *nowtm = NULL;
	char tmbuf[64] = { 0 };

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%m/%d %H:%M:%S", nowtm);

	if (pVersion) {
		printf("[%s %s] %s %s %s %d %s\n", tmbuf, pVersion, LEVEL[level], tag, funcName, lineNum, logStr);
	} else {
		printf("[%s] %s %s %s %d %s\n", tmbuf, LEVEL[level], tag, funcName, lineNum, logStr);
	}
}

/**
 * Output a variable argument list log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void SDKAPI_LogV(SDKAPI_LogLevel level, const char *tag, const char *funcName, const int16_t lineNum, const char *format, ...)
{
	if (!format || !tag) {
		return;
	}

	/* Using Variable arguments to extract each argument and passing it SDKAPI_Log API */
	char buffer[MAX_LOG_V_BUFFER_SIZE] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof buffer - 1, format, args);
	va_end(args);
	SDKAPI_Log(level, tag, funcName, lineNum, buffer);
}
