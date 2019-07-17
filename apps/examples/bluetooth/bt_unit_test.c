/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <debug.h>
#include <bt_unit_test.h>

#include <tinyara/bluetooth/bluetooth.h>


void bt_enable_cb(int err)
{
	PRT("bt_enable() callback with error code:[%d] \n", err);
}

int bt_unit_test_main(int argc, char **argv)
{
	int ret = -1;

	PRT("Enter bt_unit_test_main()\n");

	PRT("testing bt_enable() \n");

	ret = bt_enable(bt_enable_cb);

	PRT("bt_enable rc[%d] \n", ret);


	return 0;
}
