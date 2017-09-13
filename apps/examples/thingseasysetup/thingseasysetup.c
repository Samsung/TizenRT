#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include "smartthings/ocf_things.h"
#include "thingseasysetup.h"	//tmp

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int thingseasysetup_main(int argc, char *argv[])
#endif
{
	printf("ocfeasysetup!!\n");
	
	task_create("easy setup sample", 100, 1024*10 + 2048, (main_t)ess_process, NULL);
	
	return 0;
}