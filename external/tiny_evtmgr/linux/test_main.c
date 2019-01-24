#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "tiny_evtmgr.h"
#include "tiny_evtqueue.h"

extern int test_evt_handler(void);
extern int test_evt_queue(void);

int main(int argc, char *argv[])
{
	printf("[TM] start queue test\n");
	test_evt_queue();

	printf("[TM] start handler test\n");
    test_evt_handler();

	printf("[TM] Test is finished\n");
    return 0;
}

