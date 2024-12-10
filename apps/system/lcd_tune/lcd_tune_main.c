/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/types.h>

void show_usage(void)
{
	printf("TASH>> lcd_tune <command #>\n");
	printf("<command #>\n");
	printf("%d : Auto Calibration\n", LCD_CMD_AUTOCALI);
	printf("%d : Display Raw Data\n", LCD_CMD_RAWDATA);
	printf("%d : Display CPC\n", LCD_CMD_CPC);
	printf("%d : Set Intr Debug\n", LCD_CMD_INTRDBG);
}

int lcd_tune_main(int argc, char **argv)
{
	int ret;
	int cmd = -1;
	uint32_t param1 = 0;
	int param2 = 0;

	if (argc <= 1 ||  !strcmp(argv[1], "-h") ||!strcmp(argv[1], "--help")) {
		show_usage();
		return ERROR;
	}

	cmd = atoi(argv[1]);

	switch(cmd)
	{
		case LCD_CMD_AUTOCALI:
			printf("Auto Calibration\n");
			break;
		case LCD_CMD_RAWDATA:
			printf("Display raw data\n");
			break;
		case LCD_CMD_CPC:
			printf("Display CPC\n");
			break;

		case LCD_CMD_INTRDBG:
			param1 = strtoul(argv[2], NULL, 16);
			param2 = atoi(argv[3]);
			break;
	}

	if (cmd == LCD_CMD_INTRDBG) {
		ret = prctl(PR_LCD_TUNE, cmd, param1, param2);
	} else {
		ret = prctl(PR_LCD_TUNE, cmd);
	}
	if (ret < 0) {
		printf("Fail to prctl for cmd %d.\n", cmd);
		return ERROR;
	}

	return OK;
}
