/*
 *  app-core
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jayoun Lee <airjany@samsung.com>, Sewook Park <sewook7.park@samsung.com>, Jaeho Lee <jaeho81.lee@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */



#ifndef __APPCORE_COMMON_H__
#define __APPCORE_COMMON_H__

/**
 * @file    appcore-common.h
 * @version 1.1
 * @brief   This file contains APIs of the Appcore library
 */

/**
 * @addtogroup APPLICATION_FRAMEWORK
 * @{
 *
 * @defgroup Appcore Appcore
 * @version  1.1
 * @brief    A base library for application
 *
 */

/**
 * @addtogroup Appcore
 * @{
 */

#include <libintl.h>
#include <app/bundle.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _
#define _(str) gettext(str)  /**< gettext alias */
#endif
#define gettext_noop(str) (str)	/**< keyword for xgettext
				  to extract translatable strings */
#define N_(str) gettext_noop(str)  /**< gettext_noop alias */

/**
 * System global events related to the application
 * @see appcore_set_event_callback()
 */
enum appcore_event {
	APPCORE_EVENT_UNKNOWN,
			/**< Unknown event */
	APPCORE_EVENT_LOW_MEMORY,
			/**< Low memory */
	APPCORE_EVENT_LOW_BATTERY,
			/**< Low battery */
	APPCORE_EVENT_LANG_CHANGE,
			/**< Language setting is changed */
	APPCORE_EVENT_REGION_CHANGE,
			/**< Region setting is changed */
	APPCORE_EVENT_SUSPENDED_STATE_CHANGE,
			/**< The suspended state is changed */
	APPCORE_EVENT_UPDATE_REQUESTED,
			/**< Update requested */
};

/**
 * Rotaion modes
 * @see appcore_set_rotation_cb(), appcore_get_rotation_state()
 */
enum appcore_rm {
	APPCORE_RM_UNKNOWN,
		    /**< Unknown mode */
	APPCORE_RM_PORTRAIT_NORMAL,
			     /**< Portrait mode */
	APPCORE_RM_PORTRAIT_REVERSE,
			      /**< Portrait upside down mode */
	APPCORE_RM_LANDSCAPE_NORMAL,
			      /**< Left handed landscape mode */
	APPCORE_RM_LANDSCAPE_REVERSE,
				/**< Right handed landscape mode */
};

/**
 * Time format
 * @see appcore_get_timeformat()
 */
enum appcore_time_format {
	APPCORE_TIME_FORMAT_UNKNOWN,
	APPCORE_TIME_FORMAT_12,
	APPCORE_TIME_FORMAT_24,
};

enum appcore_suspended_state {
	APPCORE_SUSPENDED_STATE_WILL_ENTER_SUSPEND = 0,
	APPCORE_SUSPENDED_STATE_DID_EXIT_FROM_SUSPEND
};

/**
 * Appcore operations which are called during the application life-cycle
 * @see appcore_efl_main()
 */
struct appcore_ops {
	void *data;
	    /**< Callback data */
	int (*create) (void *);
		       /**< Called before main loop \n
			If this returns non-zero, Appcore does not run the main loop. */
	int (*terminate) (void *);
			  /**< Called after main loop */
	int (*pause) (void *);
		      /**< Called when every window goes back */
	int (*resume) (void *);
		       /**< Called when any window comes on top */
	int (*reset) (bundle *, void *);
				/**< Called at the first idler
				  and every relaunching */
	void *reserved[6];
		   /**< Reserved */
	};

/**
 * @par Description:
 * Set the callback function which is called when the event occurs.
 *
 * @par Purpose:
 * This function sets a callback function for events from the system.
 * Callback function is invoked every time the event occurs.
 *
 * @par Typical use case:
 * To do something when predefined events (enum appcore_event) occur, use this API
 *
 * @par Method of function operation:
 * Using Heynoti subscription, Vconf changed callback, and AUL, Appcore invokes the registered callback function.
 *
 * @par Important notes:
 * Only one callback function can be set. If <I>cb</I> is NULL, unset the callback function about the event.\n
 * Default behavior is performed when the specified event callback doesn't have registered.
 *
 * @param[in] event System event
 * @param[in] cb callback function
 * @param[in] data callback function data
 *
 * @return 0 on success, -1 on error (<I>errno</I> set)
 *
 * @par Errors:
 * EINVAL - Invalid event type
 *
 * @pre None.
 * @post Callback is set/unset.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

static int _lang_changed(void *);
static int _low_battery(void *);
static int _low_memory(void *);
static int _region_changed(void *);

int add_callbacks(struct appdata *data)
{
	int r;

	r = appcore_set_event_callback(APPCORE_EVENT_LANG_CHANGE, _lang_changed,
									 data);
	if (r == -1) {
		// add exception handling
	}

	r = appcore_set_event_callback(APPCORE_EVENT_LOW_BATTERY, _low_battery,
									 data);
	if (r == -1) {
		// add exception handling
	}

	r = appcore_set_event_callback(APPCORE_EVENT_LOW_MEMORY, _low_memory,
								 data);
	if (r == -1) {
		// add exception handling
	}

	r = appcore_set_event_callback(APPCORE_EVENT_REGION_CHANGE,
					 _region_changed, data);
	if (r == -1) {
		// add exception handling
	}

	return 0;
}
 * @endcode
 *
 */
int appcore_set_event_callback(enum appcore_event event,
		int (*cb)(void *, void *), void *data);

/**
 * @par Description:
 * Set a rotation callback
 *
 * @par Purpose:
 * This function sets a callback function for rotation events. Callback function is invoked every time the rotation mode is changed.
 *
 * @par Typical use case:
 * To do something when the rotation mode is changed, use this API
 *
 * @par Method of function operation:
 * Appcore receives rotation change from Sensor framework. When Appcore receive the change, it invokes the registered callback function.
 *
 * @par Important notes:
 * Locks the rotation mode, the registered callback is not invoked.
 *
 * @param[in] cb callback function
 * @param[in] data callback function data
 *
 * @return 0 on success, -1 on error (<I>errno</I> set)
 *
 * @par Errors:
 * EINVAL - <I>cb</I> is NULL
 * EALREADY - rotation callback function already registered
 *
 * @pre Callback is not set.
 * @post None.
 * @see appcore_unset_rotation_cb(), appcore_get_rotation_state()
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

static int _rot_cb(enum appcore_rm, void *);

...
{
	int r;

	r = appcore_set_rotation_cb(_rot_cb, data);
	if (r == -1) {
		// add exception handling
	}

	...
}
 * @endcode
 *
 */
int appcore_set_rotation_cb(int (*cb) (void *event_info, enum appcore_rm, void *),
			    void *data);

/**
 * @par Description:
 * Unset a rotation callback
 *
 * @par Purpose:
 * This function unsets a callback function for rotation events.
 *
 * @return 0 on success, -1 on error
 *
 * @pre Callback is set by appcore_set_rotation_cb().
 * @post None.
 * @see appcore_set_rotation_cb(), appcore_get_rotation_state()
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

{
	int r;

	...

	r = appcore_unset_rotation_cb();
	if (r == -1) {
		// add exception handling
	}
	...
}
 * @endcode
 *
 */
int appcore_unset_rotation_cb(void);

/**
 * @par Description:
 * Get the current rotation mode.
 *
 * @par Purpose:
 * To get the current rotation mode, use this API.
 *
 * @par Method of function operation:
 * This function gets the current rotation mode from Sensor framework.
 *
 * @param[out] curr current rotation mode\n
 * If Sensor framework is not working, curr is set to APPCORE_RM_UNKNOWN.
 *
 * @return 0 on success, -1 on error (<I>errno</I> set)
 *
 * @par Errors:
 * EINVAL - <I>curr</I> is NULL
 *
 * @pre None.
 * @post None.
 * @see appcore_set_rotation_cb(), appcore_unset_rotation_cb()
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

{
	int r;
	enum appcore_rm curr;

	...

	r = appcore_get_rotation_state(&curr);
	if (r == -1) {
		// add exception handling
	}
	...
}
 * @endcode
 *
 */
int appcore_get_rotation_state(enum appcore_rm *curr);

/**
 * @par Description:
 * Get the current time format.
 *
 * @par Purpose:
 * To get the current time format, use this API.
 *
 * @par Method of function operation:
 * This function gets the current time format from vconf.
 *
 * @param[out] timeformat current time format\n
 * If vconf is not working, timeformat is set to APPCORE_TIME_FORMAT_UNKNOWN.
 *
 * @return 0 on success, -1 on error (<I>errno</I> set)
 *
 * @par Errors:
 * EINVAL - <I>timeformat</I> is NULL
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

{
	int r;
	enum appcore_time_format timeformat;

	...

	r = appcore_get_timeformat(&timeformat);
	if (r == -1) {
		// add exception handling
	}
	...
}
 * @endcode
 *
 */
int appcore_get_timeformat(enum appcore_time_format *timeformat);

/**
 * @par Description:
 * Set the information for the internationalization.
 *
 * @par Purpose:
 * This function prepares to internationalize. To use gettext, use this API.
 *
 * @par Typical use case:
 * This function provides convenience for using gettext.
 *
 * @par Method of function operation:
 * Calls setlocale(), bindtextdomain() and textdomain() internally.
 *
 * @par Corner cases/exceptions:
 * If <I>dirname</I> is NULL, the previously set base directory is used. Typically, it is /usr/share/locale.
 *
 * @param[in] domainname a message domain name(text domain name) \n Must be a non-empty string.
 * @param[in] dirname the base directory for message catalogs belonging to the specified domain \n
 *
 * @return 0 on success, -1 on error (<I>errno</I> set)
 *
 * @par Errors:
 * EINVAL - <I>domain</I> is NULL
 *
 * @pre None.
 * @post The environment variable LANG is set or changed.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

{
	int r;

	...

	r = appcore_set_i18n("i18n_example", NULL);
	if (r == -1) {
		// add exception handling
	}
	...
}
 * @endcode
 *
 */
int appcore_set_i18n(const char *domainname, const char *dirname);

/**
 * @par Description:
 * Set the measuring start time
 *
 * @par Purpose:
 * To measure the time, the start time should be set. This function set the start point.
 *
 * @par Typical use case:
 * It is used to measure the time for doing something. \n
 * This function set the start point. And, appcore_measure_time() returns
 * the elapsed time from the start point.
 *
 * @see appcore_measure_time()
 *
 * @par Method of function operation:
 * Store the current time to the internal variable.
 *
 * @pre None.
 * @post Current time is saved to the internal space.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

{
	...
	appcore_measure_start();

	// do something

	printf("it takes %d msec to do something\n", appcore_measure_time());
	...
}
 * @endcode
 *
 */
void appcore_measure_start(void);

/**
 * @par Description:
 * Measure the time from appcore_measure_start()
 *
 * @par Purpose:
 * To measure the elapsed time from the start point.
 *
 * @par Typical use case:
 * It is used to measure the time for doing something. \n
 * This function returns the elapsed time from the start point set by appcore_measure_start().
 *
 * @see appcore_measure_start()
 *
 * @par Method of function operation:
 * This function subtracts the current time from the start point.
 *
 * @par Corner cases/exceptions:
 * If the start point is not set, returns 0.
 *
 * @return Milliseconds from appcore_measure_start()
 *
 * @pre None.
 * @post None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

{
	...
	appcore_measure_start();

	// do something

	printf("it takes %d msec to do something\n", appcore_measure_time());
	...
}
 * @endcode
 *
 */
int appcore_measure_time(void);

/**
 * @par Description:
 * Measure the time from a time specified in environment variable.
 *
 * @par Purpose:
 * To measure the elapsed time from a time specified in environment variable.
 *
 * @par Typical use case:
 * It is used to measure the time from a time set by external.
 * This function returns the elapsed time from a time specified in environment variable.
 *
 * @see appcore_measure_start()
 *
 * @par Method of function operation:
 * This function subtracts the current time from a time specified in environment variable.
 *
 * @par Corner cases/exceptions:
 * If <I>envnm</I> is NULL, "APP_START_TIME" set by launcher is used.
 * If the environment variable is not set or invalid format, returns 0.
 *
 * @param[in] envnm environment variable name which has
 *  the start time (format: "%u %u", seconds, micro seconds)
 *
 * @return Milliseconds from a time specified in environment variable
 *
 * @pre None.
 * @post None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

{
	...

	// do something

	printf("it takes %d msec from APP_START\n",
		appcore_measure_time_from("APP_START_TIME"));
	...
}
 * @endcode
 *
 */
int appcore_measure_time_from(const char *envnm);

/**
 * Appcore UI operaions. Internal use only.
 */
struct ui_ops;

/**
 * @par Description:
 * Appcore init. Internal use only
 *
 * @par Important notes:
 * Except special case, NEVER use this. Use the appcore EFL or GTK instead of this.
 *
 * @param[in] name Application name used for text domain name
 * @param[in] ops UI operations
 * @param[in] argc a count of the arguments
 * @param[in] argv an array of pointers to the strings which are those arguments
 *
 * @return 0 on succes, -1 on error (<I>errno</I> set)
 *
 * @par Errors:
 * EINVAL - <I>ops</I> or <I>ops</I>'s callback is NULL \n
 * EALREADY - Appcore already in operation \n
 *
 * @pre None.
 * @post None.
 * @see appcore_exit()
 * @remarks Internal use only.
 *
 */
int appcore_init(const char *name, const struct ui_ops *ops,
		 int argc, char **argv);

/**
 * @par Description:
 * Appcore exit. Internal use only.
 *
 * @par Important notes:
 * Except special case, NEVER use this.
 *
 * @pre None.
 * @post None.
 * @see appcore_init()
 * @remarks Internal use only.
 *
 */
void appcore_exit(void);


/**
 * @par Description:
 * Utility function for memory flushing
 *
 * @par Purpose:
 * To flush memory
 *
 * @par Method of function operation:
 * Calls application-specific memory flushing tool (e.g., elm_flush_all() for EFL),
 * sqlite3_release_memory() if sqlite3 is used, and malloc_trim(). Also, trims native stack.
 *
 * @par Important notes:
 * Currently, this function is automatically called in the following 2 cases:\n
 * (1) when the application enters into the pause state and\n
 * (2) when low memory event is invoked and the application is on pause.\n
 * Developers can use this function when they want extra memory flush utility.
 *
 * @return 0 on success, -1 on error
 *
 * @pre Appcore is already initialized.
 * @post Memory for the application is flushed.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

{
	int r;

	...
	r = appcore_flush_memory();
	if (r == -1) {
		// add exception handling
	}

	...
}
 * @endcode
 *
 */
int appcore_flush_memory(void);

/**
 * @par Description:
 * Set a open callback
 * Only when application is running, if aul_open api is called, then this callback function is called.
 * If your open_cb function return -1, then appcore doesn't raise window.
 *
 * @param[in] cb callback function
 * @param[in] data callback function data
 *
 * @return 0 on success, -1 on error (<I>errno</I> set)
 *
 * @pre None
 * @post None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <appcore-common.h>

...

static int _open_cb(enum appcore_rm, void *);

...
{
	int r;

	r = appcore_set_open_cb(_open_cb, data);
	if (r == -1) {
		// add exception handling
	}

	...
}
 * @endcode
 *
 */
int appcore_set_open_cb(int (*cb) (void *), void *data);


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
/**
 * @}
 */
#endif				/* __APPCORE_COMMON_H__ */
