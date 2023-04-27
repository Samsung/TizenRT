#ifndef PLATFORM_STDLIB_RTL8735B_H
#define PLATFORM_STDLIB_RTL8735B_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
}
#include <atomic>
#define _Atomic(X) std::atomic< X >
#else
#include <stdatomic.h>
#endif

#define _memcpy(dst, src, sz)	memcpy(dst, src, sz)
#define _memset(dst, val, sz)	memset(dst, val, sz)
#define _memcmp(dst, src, sz)	memcmp(dst, src, sz)
#define _strlen(str)	strlen(str)
#define _strcmp(s1, s2)	strcmp(s1, s2)
#define _strncpy(dst, src, cnt)	strncpy(dst, src, cnt)
#define DiagPrintf		printf

#endif //PLATFORM_STDLIB_RTL8195BHP_H
