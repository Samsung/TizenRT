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

/// @file tc_termios.c

/// @brief Test Case Example for Termios API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <stdio.h>
#include "tc_internal.h"

/**
* @fn                   :tc_termios_tcsetattr_tcgetattr
* @brief                :Check NL to CRNL option
* @details              :Check the printed msg through terminal after disabling and enabling NL to CRNL
* API's covered         :tcsetattr, tcgetattr
* Preconditions         :disable TASH
* Postconditions        :none
* @return               :void
*/
static void tc_termios_tcsetattr_tcgetattr(void)
{
	int ret_chk;
	struct termios prev_tio;

	ret_chk = tcgetattr(fileno(stdin), &prev_tio);
	TC_ASSERT_EQ("tcgetattr", ret_chk, 0);
	if (ret_chk != 0) {
		printf("tc_termios_tcsetattr_tcgetattr fail : getattr %d\n", errno);
	}

	sleep(1);
	/* isatty() returns 1 if fd is an open file descriptor referring to a terminal */
	ret_chk = isatty(fileno(stdin));
	TC_ASSERT_EQ("isatty", ret_chk, 1);

	ret_chk = cfgetspeed(&prev_tio);
	TC_ASSERT_GEQ("cfgetspeed", ret_chk, 0);

	/* Failure case: invalid option */
	sleep(1);
	ret_chk = tcsetattr(fileno(stdin), 33, &prev_tio);
	TC_ASSERT_LT("tcsetattr", ret_chk, 0);

	sleep(1);
	prev_tio.c_oflag &= ~ONLCR;
	ret_chk = tcsetattr(fileno(stdin), TCSANOW, &prev_tio);
	TC_ASSERT_EQ("tcsetattr", ret_chk, 0);
	if (ret_chk != 0) {
		printf("tc_termios_tcsetattr_tcgetattr fail : setattr %d\n", errno);
	}

	sleep(1);
	printf("<< Disable (NL) to (CR-NL) >>\n");
	printf("-> This line is not adapted to Carriage-return\n");

	sleep(1);
	prev_tio.c_oflag |= ONLCR;
	ret_chk = tcsetattr(fileno(stdin), TCSANOW, &prev_tio);
	TC_ASSERT_EQ("tcsetattr", ret_chk, 0);
	if (ret_chk != 0) {
		printf("tc_termios_tcsetattr_tcgetattr fail : setattr %d\n", errno);
	}
	sleep(1);

	printf("\n");
	printf("<< Enable (NL) to (CR-NL) >>\n");
	printf("-> This line is adapted to Carriage-return\n");

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: termios
 ****************************************************************************/

int termios_main(void)
{
#ifndef CONFIG_TASH
	tc_termios_tcsetattr_tcgetattr();
#else
	printf("tc_termios will not be executed. CONFIG_TASH is set.\n");
#endif
	return 0;
}
