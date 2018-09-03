/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <task_manager/task_manager.h>
#include "bundle.h"
#include "app_control.h"
#include "app_manager.h"
#include "service_app_main.h"
#include "message_port.h"
#include "app_utils.h"

#define TEST_APP_ID "com.example.testapp1"
#define TEST_APP_LOCAL_PORT_ID 1

/****************************************************************************
 * apptest_main
 ****************************************************************************/
char testkey[5] = "key";
char testvalue[7] = "value";
app_context_h context = NULL;

const char *testvalue_array[5] = {"testvalue_array one",
				"testvalue_array two",
				"testvalue_array three",
				"testvalue_array four",
				 "testvalue_array five"};

void resume_testapp(void *arg)
{
	int ret;

	sleep(2);
	printf("resume_testapp\n");
	
	ret = app_manager_resume_app(context);
	if (ret == APP_MANAGER_ERROR_NONE) {
		printf("app_manager_resume_app succeeded\n");
	} else {
		printf("app_manager_resume_app failed\n");
	}
}

int msg_cb(int local_port_id, const char *remote_app_id, const char *remote_port, bool trusted_remote_port, bundle *message, void *user_data)
{
	int ret;
	bundle *msg = message;
	char *str = (char*)malloc(sizeof(char) * 15);

	bundle_get_str(msg, "test1", &str);

	printf("remote cb str :%s\n", str);

	ret = message_port_unregister_local_port(TEST_APP_LOCAL_PORT_ID);
	if (ret == MESSAGE_PORT_ERROR_NONE) {
		printf("message_port_unregister_local_port succeeded\n");
	} else {
		printf("message_port_unregister_local_port failed\n");
	}
	
	return 0;
}

bool testapp_create_cb(void *user_data)
{
	printf("Create CB\n");
	return true;
}

void testapp_control_cb(app_control_h app_control, void *user_data)
{
	const char *testget_value;
	const char **testget_array = NULL;

	pthread_t tid;
	
	pid_t pid = getpid();
	char *appid = NULL;
	bool is_running = false;
	app_info_h appinfo = NULL;
	int ret;
	int len;
	
	/* get extra data array */	
	for (int i = 1; i <= 5; i++) {
		testget_value = NULL;
		testkey[3] = i + '0';
		testvalue[5] = i + '0';
		
		testget_value = bundle_get_val(app_control->data, testkey);
		if (testget_value == NULL) {
			printf("bundle_get_val failed\n");
			continue;
		}
		printf("bundle_get_val:%s\n", testget_value);
	}

	len = 5;
	testget_array = bundle_get_str_array(app_control->data, "testkey_array", &len);
	if (testget_array == NULL) {
		printf("bundle_get_str_array failed\n");;
	}

	for (int i = 0; i < 5; i++) {
		printf("bundle_get_str_array : %s\n", testget_array[i]);
	}

	/* app_manager test*/
	/* get appid */
	ret = app_manager_get_app_id(pid, &appid);
	if (ret == APP_MANAGER_ERROR_NONE && strcmp(appid, TEST_APP_ID) == 0) {
		printf("app_manager_get_app_id succeeded\n");
		printf("appid : %s\n", appid);
	} else {
		printf("app_manager_get_app_id failed\n");
		printf("appid : %s\n", appid);
	}

	/* get app context */
	ret = app_manager_get_app_context(appid, &context);
	if (ret == APP_MANAGER_ERROR_NONE) {
		printf("app_manager_get_app_context succeeded\n");
	} else {
		printf("app_manager_get_app_context failed\n");
	}

	/* get running state */
	ret = app_manager_is_running(appid, &is_running);
	if (ret == APP_MANAGER_ERROR_NONE && is_running == true) {
		printf("app_manager_is_running succeeded\n");
	} else {
		printf("app_manager_is_running failed\n");
	}

	/* get appinfo test */
	ret = app_manager_get_app_info(appid, &appinfo);
	if (ret == APP_MANAGER_ERROR_NONE) {
		printf("app_manager_get_app_info succeeded\n");
	} else {
		printf("app_manager_get_app_info failed\n");
	}

	/* resume test */
	pthread_create(&tid, NULL, (pthread_startroutine_t)resume_testapp, (void *)&context);
	ret = task_manager_pause(get_tm_handle_by_appid(appid), TM_RESPONSE_WAIT_INF);
	printf("task status : %d\n", ret);

	/*
	ret = app_control_destroy(app_control);
	if (ret == APP_CONTROL_ERROR_NONE) {
		printf("app_control_destroy succeeded\n");
	} else {
		printf("app_control_destroy failed\n");
	}
	*/
}

void testapp_terminate_cb(void *user_data)
{
	printf("Terminate CB\n");
}

int testapp(int argc, char *argv[])
{
	int ret;
	service_app_lifecycle_callback_s cb = {0,};

	cb.create = testapp_create_cb;
	cb.app_control = testapp_control_cb;
	cb.terminate = testapp_terminate_cb;

	printf("testapp start!!!!!!!\n");

	ret = message_port_register_local_port("testapp", msg_cb, NULL);
	if (ret == MESSAGE_PORT_ERROR_NONE) {
		printf("message_port_register_local_port succeeded\n");
	} else {
		printf("message_port_register_local_port failed\n");
	}
	
	service_app_main(argc, argv, &cb, NULL);
	printf("FINISHED\n");
	
	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int apptest_main(int argc, char *argv[])
#endif
{
	app_control_h control = NULL;
	int ret;
	bundle *b = NULL;
	bool exist;

	printf("apptest start!\n");

	ret = app_control_create(&control);
	if (ret == APP_CONTROL_ERROR_NONE) {
		printf("app_control_create succeeded\n");
	} else {
		printf("app_control_create failed\n");
	}

	ret = app_control_set_operation(control, APP_CONTROL_OPERATION_DEFAULT);
	if (ret == APP_CONTROL_ERROR_NONE) {
		printf("app_control_set_operation succeeded\n");
	} else {
		printf("app_control_set_operation failed\n");
	}

	ret = app_control_set_app_id(control, TEST_APP_ID);
	if (ret == APP_CONTROL_ERROR_NONE) {
		printf("app_control_set_app_id succeeded\n");
	} else {
		printf("app_control_set_app_id failed\n");
	}

	/* extra data */
	for (int i = 1; i <= 5; i++) {
		testkey[3] = i + '0';
		testvalue[5] = i + '0';	
		ret = app_control_add_extra_data(control, testkey, testvalue);
		if (ret == APP_CONTROL_ERROR_NONE) {
			printf("app_control_add_extra_data succeeded\n");
		} else {
			printf("app_control_add_extra_data failed\n");
		}
	}

	/* extra data array */
	ret = app_control_add_extra_data_array(control, "testkey_array", testvalue_array, 5);
	if (ret == APP_CONTROL_ERROR_NONE) {
		printf("app_control_add_extra_data_array succeeded\n");
	} else {
		printf("app_control_add_extra_data_array failed\n");
	}

	/* app control launch */
	ret = app_control_send_launch_request(control, NULL, NULL);
	if (ret == APP_CONTROL_ERROR_NONE) {
		printf("Succeeded to launch a testapp.\n");
	} else {
		printf("Failed to launch a testapp.\n");
	}

	sleep(5);
	printf("message port test\n");

	exist = false;

	ret = message_port_check_remote_port(TEST_APP_ID, "testapp", &exist);
	if (ret == MESSAGE_PORT_ERROR_NONE && exist == true) {
		printf("message_port_check_remote_port succeeded\n");
	} else {
		printf("message_port_check_remote_port failed\n");		
	}
	
	b = bundle_create();
	bundle_add_str(b, "test1", "gohome");
	/* bundle_add_str(b, "test2", "gogohome"); */
   
	ret = message_port_send_message(TEST_APP_ID, "testapp", b);
	if (ret == MESSAGE_PORT_ERROR_NONE) {
		printf("message_port_send_message succeeded\n");
	} else {
		printf("message_port_send_message failed\n");
	}
	
	return 0;
}
