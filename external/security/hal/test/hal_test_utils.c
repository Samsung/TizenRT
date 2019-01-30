#include <stdlib.h>
#include <security_hal.h>
#include "hal_test_utils.h"

void hal_test_free_buffer(hal_data *data)
{
	if (data->data) {
		free(data->data);
		data->data = NULL;
	}
	data->data_len = 0;
}


void hal_test_init_buffer(hal_data *data)
{
	if (data) {
		data->data = NULL;
		data->data_len = 0;
	}
}
