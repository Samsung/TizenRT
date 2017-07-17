#ifndef _COMMON_MONITOR_INTERFACE_H
#define _COMMON_MONITOR_INTERFACE_H
#include <stdint.h>
#define PROC_BUFFER_LEN 128
#define PROC_MOUNTPOINT "/proc"

int readonce(FAR const char *filename, FAR char *buffer, uint8_t buflen);
int readfile(FAR const char *filename, FAR char *buffer, uint8_t buflen);
void mount_procfs(void);
#endif

