#include <things_task.h>

things_result things_task_sleep(unsigned int milliseconds)
{
	if (!usleep(milliseconds * 1000))
		return THINGS_SUCCESS;

	return THINGS_FALSE;
}