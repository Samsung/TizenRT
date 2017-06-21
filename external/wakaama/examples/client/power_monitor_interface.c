#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <stdint.h>
#include "power_monitor_interface.h"
#include "common_monitor_interface.h"


uint8_t pm_iobuffer[PROC_BUFFER_LEN];
  
void fetch_power_states(FAR char *powerstates)
{
    int ret;    
    uint8_t *filepath;
    ret = asprintf(&filepath, "%s/power/system/states", PROC_MOUNTPOINT);
    ret = readfile(filepath, pm_iobuffer, PROC_BUFFER_LEN);
    if (ret == OK)
    {
        strncpy(powerstates, pm_iobuffer, strlen(pm_iobuffer));
        printf("powerstates %s:%s:%d\n", powerstates, pm_iobuffer, strlen(pm_iobuffer));
    }
}

void fetch_battery_lifetime(FAR char *expectedlifetime)
{
    int ret;    
    uint8_t *filepath;
    ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
    printf("mount returns value %d\n", ret);
    ret = asprintf(&filepath, "%s/power/battery/runtime", PROC_MOUNTPOINT);
    ret = readfile(filepath, pm_iobuffer, PROC_BUFFER_LEN);
    if (ret == OK)
    {
        strncpy(expectedlifetime, pm_iobuffer, strlen(pm_iobuffer));
        printf("expectedlifetime %s:%s:%d\n", expectedlifetime, pm_iobuffer, strlen(pm_iobuffer));
    }    
}

void fetch_battery_level(FAR char * batterylevel)
{
    int ret;    
    uint8_t *filepath;    
    ret = asprintf(&filepath, "%s/power/battery/life", PROC_MOUNTPOINT);
    ret = readfile(filepath, pm_iobuffer, PROC_BUFFER_LEN);
    if (ret == OK)
    {
        strncpy(batterylevel, pm_iobuffer, strlen(pm_iobuffer));
        printf("batterylevel %s:%s:%d\n", batterylevel, pm_iobuffer, strlen(pm_iobuffer));
    }    
}