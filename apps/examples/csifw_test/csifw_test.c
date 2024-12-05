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

#include "csifw_test.h"
#include "csifw/csifw_api.h"
#include <stdlib.h>
#include <stdio.h>


/****************************************************************************
 * csifw_test
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


// csifw_test start <intrval> <config_type>  // This will print service ID, Use this Service ID for other comands.
// csifw_test resume <s_id>
// csifw_test stop <s_id>
// csifw_test change_interval  <S_id> <interval>
// csifw_test detail 
// csifw_test exit <s_id>

#define COMMAND_START "start"
#define COMMAND_STOP "stop"
#define COMMAND_CHANGE_INTERVAL "change_interval"
#define COMMAND_RESUME "resume"
#define COMMAND_DETAIL "detail"
#define COMMAND_EXIT "exit"

// Define the mutex for protecting shared resources
static pthread_mutex_t g_client_mutex = PTHREAD_MUTEX_INITIALIZER;
#define MAX_CLIENT 5
// Function prototypes
void process_command(int argc, char *argv[]);
void start_service(int service_id); //start and resume service
void stop_service(int service_id);
void change_interval(int service_id, int interval);
void list_services();
void exit_services(int service_id);
char* getStatusStr(int status);

unsigned int g_interval;

typedef enum {
	START = 0,
	STOP,
	CHANGE_INTERVAL,
	EXIT
} CSIFW_TEST_COMMAND;

typedef struct client_info{
    CSIFW_TEST_COMMAND command;
    CSIFW_TEST_COMMAND status;
    int packet_count;
}csifw_test_client_info_t;

int g_client_count = 0;
csifw_test_client_info_t client_info[MAX_CLIENT];

static void demo_upd_raw_data_listener(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void* ptr)
{
	if(res == CSIFW_OK) {
		unsigned int app_id = *(unsigned int*)ptr;
		client_info[app_id].packet_count += 1;
		printf("\n%d. Raw Data received in APP[%d] [%d]\n", client_info[app_id].packet_count, app_id, csi_buff_len);
		return;
	}
	if(res == CSIFW_ERROR_WIFI_DIS_CONNECTED) {
		printf("<URGENT CALLBACK N/W CHANGE>[APP] wifi disconnected\n");
		printf("<URGENT CALLBACK N/W CHANGE>[APP] so service will PAUSE\n");
		return;
	}
	if (res == CSIFW_ERROR) {
		printf("[APP] Some error\n");
		return;
	}
	if(res == CSIFW_OK_WIFI_CONNECTED) {
		printf("<URGENT CALLBACK N/W CHANGE>[APP] wifi reconnected\n");
		printf("<URGENT CALLBACK N/W CHANGE>[APP] wifi reconnected\n");
	}
}

static void demo_upd_parsed_data_listener(CSIFW_RES res, int csi_data_len, float *csi_parsed_buff, void* ptr)
{
	printf("demo_upd_parsed_data_listener \n\n");
}



void csifw_test_main(int argc, char **argv){
	if (argc < 2) {
		printf("Usage: %s [start|stop|change_interval|detail|exit|resume] [options]\n", argv[0]);
		return;
	}
	int my_id = -1;
	// Process the command
	if (strcmp(argv[1], COMMAND_START) == 0) {
		if(argc != 4 ){
			 printf("Invalid Input \n");
			return;
		}
		if(g_client_count == MAX_CLIENT){
			   printf("No more client Can created\n");
			   return;
		}
		int interval = atoi(argv[2]);
		int config_type = atoi(argv[3]);
		my_id = g_client_count;
		++g_client_count;
		printf("\n\nNew Service ID is %d \n\n", my_id);
		CSIFW_RES res = csi_service_init(config_type, demo_upd_raw_data_listener, NULL, interval, &my_id);
		start_service(my_id);
	}else {
		process_command(argc, argv);
		return;
	}
   
	while(1)
	{
		pthread_mutex_lock(&g_client_mutex);
		if(client_info[my_id].command != -1){
			if(client_info[my_id].command == START){
				if (csi_service_start() == CSIFW_ERROR) {
					printf("[Test APP %d]: CSI Manager START FAIL\n", my_id);
				}
				client_info[my_id].status = START;
			}
			else if(client_info[my_id].command == STOP){
				if (csi_service_stop(CSIFW_NORMAL) == CSIFW_ERROR) {
					printf("[Test APP %d]: CSI Manager STOP FAIL\n", my_id);
				}
				client_info[my_id].status = STOP;
			}
			else if(client_info[my_id].command == CHANGE_INTERVAL){
				if (csi_service_change_interval(g_interval) == CSIFW_ERROR) {
					printf("[Test APP %d]: CSI Manager STOP FAIL\n", my_id);
				}
			}
			else if(client_info[my_id].command == EXIT){
				client_info[my_id].status = EXIT;
				if (csi_service_stop(CSIFW_NORMAL) == CSIFW_ERROR) {
					printf("[Test APP %d]: CSI Manager STOP FAIL\n", my_id);
				}
				if (csi_service_deinit() == CSIFW_ERROR) {
					printf("[Test APP %d]: CSI Manager DEINIT FAIL\n", my_id);
					pthread_mutex_unlock(&g_client_mutex);
					break;
				}
			}
			client_info[my_id].command = -1;
		}
		pthread_mutex_unlock(&g_client_mutex);
		sleep(1);
	}
}

int getServiceID(int argc, char *argv[])
{
	int service_id = -1;
	if(argc >= 3){
		printf("Invalid Input \n");
		return -1;
	}
	service_id = atoi(argv[2]);
	if(service_id < 0 || service_id > g_client_count){
		printf("Invalid Service ID \n");
		return -1;
	}
	if(client_info[service_id].status == EXIT){
		printf("Service already exit \n");
		return -1;
	}
	return service_id;
}

void process_command(int argc, char *argv[]) {
	if (strcmp(argv[1], COMMAND_RESUME) == 0) {
		int s_id = getServiceID(argc, argv);
		if(s_id != -1){
			start_service(s_id);
		}
	} else if (strcmp(argv[1], COMMAND_STOP) == 0) {
		int s_id = getServiceID(argc, argv);
		if(s_id != -1){
			stop_service(s_id);
		}
	} else if (strcmp(argv[1], COMMAND_CHANGE_INTERVAL) == 0) {
		int s_id = getServiceID(argc, argv);
		if(s_id != -1){
			change_interval(s_id, atoi(argv[3]));
		}
	} else if (strcmp(argv[1], COMMAND_DETAIL) == 0) {
		list_services();
	} else if (strcmp(argv[1], COMMAND_EXIT) == 0) {
		int s_id = getServiceID(argc, argv);
		if(s_id != -1){
			exit_services(s_id);
		}
	} else {
		printf("Invalid command\n");
	}
}

char* getStatusStr(int status)
{
	if(status == START){
		return "Running";
	} else if(status == STOP){
		return "Stopped";
	} else if(status == EXIT){
		return "Terminated";
	}
}

void start_service(int service_id) {
	if(client_info[service_id].status == STOP){
		client_info[service_id].command = START;
	} else {
		printf("Current Staus of service[%d] is [%s]n", service_id, getStatusStr(client_info[service_id].status));
	}
}

void stop_service(int service_id) {
	if(client_info[service_id].status == START){
		client_info[service_id].command = STOP;
	} else {
		printf("Current Staus of service[%d] is [%s]n", service_id, getStatusStr(client_info[service_id].status));
	}
	
}

void change_interval(int service_id, int interval) {
	if(client_info[service_id].status == START){
		g_interval = interval;
		client_info[service_id].command = CHANGE_INTERVAL;
	} else {
		printf("Current Staus of service[%d] is [%s]n", service_id, getStatusStr(client_info[service_id].status));
	}
}

void list_services() {
	printf("Listing all services\n\n");
	printf("CSI Data Interval %d ms \n\n", csi_service_get_current_interval());
	for(int i = 0; i < g_client_count; ++i){
		printf("Service ID[%d] Status[%s] Packets_count[%d] \n", i, getStatusStr(client_info[i].status), client_info[i].packet_count);
	}
	printf("\n");
}

void exit_services(int service_id){
	printf("exit service %d\n", service_id);
	client_info[service_id].command = EXIT;
}

