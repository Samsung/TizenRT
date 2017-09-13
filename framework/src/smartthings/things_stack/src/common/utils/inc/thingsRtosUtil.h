/*
 * oicnetwork.h
 *
 *  Created on: 2016. 10. 25.
 *      Author: eunseok
 */

#ifndef OCFRTOS_UTIL_H_
#define OCFRTOS_UTIL_H_
#include "pthread.h"
typedef enum {
	THINGS_STACK_PING_THREAD = 0,
	THINGS_STACK_WAITHANDLER_THREAD,
	THINGS_STACK_RESETLOOP_THREAD,
	THINGS_STACK_CICONNETION_INIT_THREAD,
	THINGS_STACK_CICONNETION_WAIT_THREAD,
	THINGS_STACK_WIFI_PROV_SET_THREAD,
	THINGS_STACK_CLOUD_REFRESH_THREAD,
	THINGS_STACK_MESSAGE_HANDLING_THREAD,
	THINGS_STACK_PRESENCE_NOTI_THREAD,
	THINGS_STACK_SERVEREXCETUE_LOOP_THREAD,
	THINGS_STACK_BASE_TIME_OUT_THREAD,
	THINGS_STACK_OICABORT_THREAD,
	THINGS_STACK_MAX_INDEX,
}ThingsStackthreadName;


int pthread_create_rtos(FAR pthread_t *thread, FAR const pthread_attr_t *attr,
                   pthread_startroutine_t start_routine, pthread_addr_t arg, ThingsStackthreadName eThreadname);

#endif /* OCFRTOS_UTIL_H_ */
