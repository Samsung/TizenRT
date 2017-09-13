/*******************************************************************
 * Project Name: DADeviceCommercialization
 *
 * Copyright 2015 by Samsung Electronics, Inc. All rights reserved.
 *
 * Project Description: OIC Certified Refrigerator which support
 *                      Device discovery/control/monitoring
 *
 *******************************************************************/

/*******************************************************************
 *
 * @file      DaLogger.c
 * @brief     Describes about the DaLogger module
 *
 *******************************************************************/

// Defining _POSIX_C_SOURCE macro with 199309L (or greater) as value
// causes header files to expose definitions
// corresponding to the POSIX.1b, Real-time extensions
// (IEEE Std 1003.1b-1993) specification
//
// For this specific file, see use of clock_gettime,
// Refer to http://pubs.opengroup.org/stage7tc1/functions/clock_gettime.html
// and to http://man7.org/linux/man-pages/man2/clock_gettime.2.html
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <unistd.h>

// if we have unistd.h, we're a Unix system
#include <time.h>
#include <sys/time.h>

#include "thingslogger.h"
#include "thingsmalloc.h"
#include "string.h"

//#ifndef __TIZEN__
static const uint16_t LINE_BUFFER_SIZE = (16 * 2) + 16 + 1; // Show 16 bytes, 2 chars/byte, spaces between bytes, null termination

static const char *LEVEL[] __attribute__ ((unused)) = {"DEBUG", "INFOR", "WARNING", "ERROR", "FATAL"};
static char tsBuf[64]={0};

char *pVersion = NULL;
DALogLevel glevel = THINGS_INFO; // pkes LOG
void setDaLogLvel(int level)
{
	if(level > 4) return;
	 int lev  = level - 4;
	glevel = abs(lev);
}
void DALogInit()
{
	/* Initialize pVersion variable
	 *
	 */
	pVersion = NULL;
}

void DALogShutdown()
{
	/* Free the memory allocated for pVersion
	 *
	 */
	DAFree(pVersion);
}

void DALogSetVersion(char* version)
{
	if (NULL != pVersion)
	{
		DAFree(pVersion);
	}

	/* Allocate memory for pVersion and set the version information
	 *
	 */
	pVersion = (char*) DAMalloc(strlen(version) + 1);
	if (NULL == pVersion)
	{
		printf("Error while allocating memory for version");
		return;
	}
	memset(pVersion, 0, strlen(version) + 1);
	DAStrncpy(pVersion, version, strlen(version));
}

/**
 * Output a log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param logStr - log string
 */
void DALog(DALogLevel level, const char *tag, const char *funcName, const int16_t lineNum, const char *logStr)
{
    if (!logStr || !tag)
    {
        return;
    }
    if(level < glevel) return;

	struct timeval tv;
	time_t nowtime = 0;
	struct tm *nowtm = NULL;
	//char tmbuf[64]={0};
    char buf[64]={0};

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
    strftime(buf, sizeof buf, "%m/%d %H:%M:%S", nowtm);
	//strftime(tmbuf, sizeof tmbuf, "%m/%d %H:%M:%S", nowtm);
	//snprintf(buf, sizeof buf, "%s.%06ld", tmbuf, (long int)tv.tv_usec);

	/* If Version information is not NULL, display the Version Information
	 * Logger Format is given below
	 * [<MM/YY H:M:S.uS> <LOGGER_VERSION>] <LOG_LEVEL> <TAG> <API> <LINE_NO> <LOG_STR>
	 */
	if (pVersion)
	{
	    printf("[%s %s] %s %s %s %d %s\n", buf, pVersion, LEVEL[level], tag, funcName, lineNum, logStr);
	}
	else
	{
//	    printf("[%s] %s %s %s %d %s\n", buf, LEVEL[level], tag, funcName, lineNum, logStr);
	    printf("T%d [%s] %s %s %s %d %s\n", getpid(), buf, LEVEL[level], tag, funcName, lineNum, logStr);
	}

/*
	#ifdef __TIZEN__
    	dlog_print(DLOG_INFO, LOG_TAG_OCF, "[%s %s] %s %s %s %d %s\n", buf, pVersion, LEVEL[level], tag, funcName, lineNum, logStr);
    #else
    	printf("[%s %s] %s %s %s %d %s\n", buf, pVersion, LEVEL[level], tag, funcName, lineNum, logStr);
    #endif*/

}

/**
 * Output a variable argument list log string with the specified priority level.
 * Only defined for Linux and Android
 *
 * @param level  - DEBUG, INFO, WARNING, ERROR, FATAL
 * @param tag    - Module name
 * @param format - variadic log string
 */
void DALogv(DALogLevel level, const char *tag, const char *funcName, const int16_t lineNum, const char *format, ...)
{
    if (!format || !tag)
    {
        return;
    }
    if(level < glevel) return;
	/* Using Variable arguments to extract each argument and passing it DALog API
	 *
	 */
    char buffer[MAX_LOG_V_BUFFER_SIZE] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof buffer - 1, format, args);
    va_end(args);
    DALog(level, tag, funcName, lineNum, buffer);
}
//#endif

const char* __get_timestamp__()
{
	struct timeval tv;
	time_t nowtime = 0;
	struct tm *nowtm = NULL;

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
    strftime(tsBuf, sizeof tsBuf, "%m/%d %H:%M:%S", nowtm);

    return tsBuf;
}

