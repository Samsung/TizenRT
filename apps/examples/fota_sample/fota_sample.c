/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <tinyara/config.h>
#ifdef CONFIG_TASH
#include <apps/shell/tash.h>
#endif
#include <apps/system/fota_hal.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fota_sample
 *
 * Description:
 *      fota sample application
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int fota_sample(int argc, char *argv[])
#endif
{
	int ret = ERROR;
	int bin_id;
	int max_bins;
	uint32_t part_id;
	uint32_t next_part_id;
	char user_input;
	char buffer[] = "dummy";
	fotahal_handle_t fotahal_handle;
	fotahal_bin_header_t fotahal_header;

	printf("***********************************************************\n");
	printf(" fota update is in progress !!!\n");

	/* Example call sequence to use fota hal */

	/* open fota hal, get the handle to work with it */
	if ((fotahal_handle = fotahal_open()) == NULL) {
		printf("%s : fotahal_open error\n", __func__);
		goto open_error;
	}

	/* Get the current running partition */
	part_id = fotahal_get_partition(fotahal_handle);
	if (part_id == -1) {
		printf("%s : fotahal_get_partition error\n", __func__);
		goto part_error;
	}

	printf(" current running partition is  [ OTA%d ]\n", part_id);

	/* Decide the next partiton we can use, may be first free partition */
	for (next_part_id = FOTA_PARTITION_OTA0; (next_part_id < FOTA_PARTITION_MAX)
		 && next_part_id == part_id; next_part_id++) ;

	if (next_part_id >= FOTA_PARTITION_MAX) {
		printf("%s : No free partition left\n", __func__);
		goto part_error;
	}

	printf(" next fota update partition is [ OTA%d ]\n", next_part_id);

	/* FIXME:
	 * This is for fota sample test, Real application might need to handle
	 * it differently.
	 * We are not downloading binary from server.
	 * Let's post a banner here to inform user to manually download the binary
	 */

	printf("*******  oh!! Fota Server is Down !!! ******\n");
	printf(" please download fota binary manually using below step !!! \n");
	printf("        [ make download TINYARA_OTA%d ]     \n", next_part_id);
	printf("Is OTA%d binary manually downloaded? If yes press Y to continue [Y/N]\n", next_part_id);
	user_input = getchar();
	if (user_input != 'Y' && user_input != 'y') {
		printf(" fota update cancled !!!\n");
		printf("***********************************************************\n");
		goto fota_exit;
	}

	if (fotahal_set_partition(fotahal_handle, next_part_id) != FOTAHAL_RETURN_SUCCESS) {
		printf("%s : fotahal_set_partition error\n", __func__);
		goto part_error;
	}

	/* max_bins = 1; for testing */
	max_bins = 1;
	for (bin_id = 0; bin_id < max_bins; bin_id++) {
		/* Extract fota header from binary */
		/* But unfortunately, we have not received binary,
		 * hence for testing, construct binary header locally
		 */
		fotahal_header.fotahal_bin_id = bin_id;

		if (fotahal_set_binary(fotahal_handle, fotahal_header.fotahal_bin_id)
			!= FOTAHAL_RETURN_SUCCESS) {
			printf("%s : fotahal_set_binary error\n", __func__);
			goto part_error;
		}

		if (fotahal_write(fotahal_handle, buffer, 0) != FOTAHAL_RETURN_SUCCESS) {
			printf("%s : fotahal_write error\n", __func__);
			goto write_error;
		}

		/* Lets update boot param, to trigger fota update in next cycle */
		if (fotahal_update_bootparam(fotahal_handle) != FOTAHAL_RETURN_SUCCESS) {
			printf("%s : fotahal_update_bootparam error\n", __func__);
			goto param_error;
		}
	}

	if (fotahal_close(fotahal_handle) != FOTAHAL_RETURN_SUCCESS) {
		printf("%s : fotahal_close error\n", __func__);
		goto close_error;
	}

	printf(" fota update successfully downloaded !!!\n");
	printf(" Please reset your board to continue fota update !!!\n");
	printf("***********************************************************\n");
	ret = OK;

close_error:
param_error:
write_error:
fota_exit:
part_error:
	fotahal_close(fotahal_handle);
open_error:
	return ret;
}

