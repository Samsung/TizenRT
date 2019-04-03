#include <stdlib.h>
#include <tinyara/security_hal.h>
#include "hal_test_utils.h"

void hal_test_free_buffer(hal_data *data)
{
	if (data->data) {
		free(data->data);
		data->data = NULL;
	}
	if (data->priv) {
		free(data->priv);
		data->priv = NULL;
	}
	data->data_len = 0;
	data->priv_len = 0;
}

int hal_test_malloc_buffer(hal_data *data, int buf_len)
{
	data->data = (unsigned char *)malloc(buf_len);
	if (!data->data) {
		return -1;
	}
	return 0;
}

int hal_test_malloc_buffer_priv(hal_data *data, int buf_len)
{
	data->priv = (unsigned char *)malloc(buf_len);
	if (!data->priv) {
		return -1;
	}
	return 0;
}
void hal_test_init_buffer(hal_data *data)
{
	if (data) {
		data->data = NULL;
		data->data_len = 0;
		data->priv = NULL;
		data->priv_len = 0;
	}
}
