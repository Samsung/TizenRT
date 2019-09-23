#include <stdio.h>
#include "port_logger.h"

#define DEBUG_BUF_SIZE 128

int ss_simple_logger(const char *fmt, ...)
{
	char str[DEBUG_BUF_SIZE];

	va_list argp;
	va_start( argp, fmt );
	int ret = vsnprintf( str, DEBUG_BUF_SIZE, fmt, argp );
	va_end( argp );

	if( ret >= 0 && ret < DEBUG_BUF_SIZE - 1 )
	{
		str[ret]     = '\0';
	}

	printf("%s\n", str);
}
