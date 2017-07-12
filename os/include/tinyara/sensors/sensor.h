/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
/**
 * @file sensor.h
 * @brief Sensor API definition
 * @version 0.3
 */

#ifndef __SENSOR_H__
#define __SENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <debug.h>

/****************************************************************************
 * Public Types
 ****************************************************************************/
/**
 * @brief Enumeration of sensor device type
 */
typedef enum {
	SENSOR_DEVICE_TYPE_ACCELEROMETER,
	SENSOR_DEVICE_TYPE_GYROSCOPE,
	SENSOR_DEVICE_TYPE_MAGNETIC_FIELD,
	SENSOR_DEVICE_TYPE_TEMPERATURE,
	SENSOR_DEVICE_TYPE_HUMIDITY,
	SENSOR_DEVICE_TYPE_PRESSURE,
	SENSOR_DEVICE_TYPE_LIGHT,
	SENSOR_DEVICE_TYPE_DUST,
	SENSOR_DEVICE_TYPE_END
}
sensor_device_type_e;

/**
 * @brief Enumeration of sensor device attribute
 */
typedef enum {
	SENSOR_IOCTL_ID_GET_TRIGGER_TYPE,
	SENSOR_IOCTL_ID_FREQUENCY,
	SENSOR_IOCTL_ID_CUSTOM = 0x8000,
} sensor_ioctl_id_e;

/**
 * @brief Enumeration of sensor trigger type
 */
typedef enum {
	SENSOR_TRIGGER_TYPE_DATA_READY,
	SENSOR_TRIGGER_TYPE_TIMER,
	SENSOR_TRIGGER_TYPE_END
} sensor_trigger_type_e;

/**
 * @brief Structure of sensor value
 */
typedef struct {
	union {
		int ival;
		float fval;
		double dval;
		char *p_str;
		void *p;
	};
} sensor_ioctl_value_t;

/**
 * @brief Structure of sensor data
 */
typedef struct {
	union {
		float v[3];
		struct {
			float x;
			float y;
			float z;
		};
		struct {
			float azimuth;
			float pitch;
			float roll;
		};
		int ival;
		float fval;
		double dval;
	};
} sensor_data_t;

/**
 * @brief Structure of sensor trigger information
 */
typedef struct {
	sensor_trigger_type_e type;
} sensor_trigger_info_t;

/**
 * @brief Definition of sensor_device_t type
 */
typedef struct sensor_device_t sensor_device_t;

/**
 * @brief Pointer definition to trigger callback function
 */
typedef int (*sensor_trigger_callback_t)(sensor_device_t *sensor, sensor_data_t *data);

/**
 * @brief Pointer definition to sensor operations
 */
typedef int (*sensor_init_t)(sensor_device_t *sensor);
typedef int (*sensor_deinit_t)(sensor_device_t *sensor);
typedef int (*sensor_activate_t)(sensor_device_t *sensor);
typedef int (*sensor_deactivate_t)(sensor_device_t *sensor);
typedef int (*sensor_ioctl_t)(sensor_device_t *sensor, int id, sensor_ioctl_value_t *val);
typedef int (*sensor_set_trigger_t)(sensor_device_t *sensor, sensor_trigger_info_t info, sensor_trigger_callback_t callback);
typedef int (*sensor_get_data_t)(sensor_device_t *sensor, sensor_data_t *data);

/**
 * @brief Structure of sensor operations
 */
typedef struct {
	sensor_init_t init;		/* initialize sensor device */
	sensor_deinit_t deinit;	/* deinitialize sensor device */
	sensor_activate_t activate;	/* activate sensor device */
	sensor_deactivate_t deactivate;	/* deactivate sensor device */
	sensor_ioctl_t ioctl;	/* ioctl on sensor device */
	sensor_set_trigger_t set_trigger;	/* set trigger information and calback function */
	sensor_get_data_t get_data;	/* get sensor data from device */
} sensor_operations_t;

/**
 * @brief Structure of sensor device
 */
struct sensor_device_t {
	sensor_device_type_e type;	/* sensor device type */
	sensor_operations_t *ops;	/* sensor opertaions */
	void *priv;				/* private data of a specified device */
};

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define __NAME_STRING(name)   	#name
#define __SENSOR_CREATE_INSTANCE(name, _type, _ops, _priv) \
	static sensor_device_t g_sensor_##name = { \
		.type = _type, \
		.ops = _ops, \
		.priv = _priv, \
	}; \
	\
	sensor_device_t * name##_get_handle(void) { \
		return &g_sensor_##name; \
	}; \
	\
	sensor_operations_t * name##_get_ops_handle(void) { \
		return (&g_sensor_##name)->ops ? (&g_sensor_##name)->ops : NULL; \
	}

#define __SENSOR_EXTERNAL_FUNCTION_PROTOTYPE(name) \
	sensor_device_t * name##_get_handle(void); \
	sensor_operations_t * name##_get_ops_handle(void)

#define __SENSOR_GET_HANDLE(name) 			name##_get_handle()
#define __SENSOR_GET_OPS_HANDLE(name) 		name##_get_ops_handle()
#define __SENSOR_GET_NAME_STRING(name)  	__NAME_STRING(name)

/***************************
  For Debug
 ***************************/
#define SENSOR_DEBUG_ON				1
#if SENSOR_DEBUG_ON
#define SENSOR_DEBUG(format, ...)   dbg(format, ##__VA_ARGS__)
#else
#define SENSOR_DEBUG(x...) 			(void)0
#endif

/***************************
  For Sensor Driver Developers
 ***************************/

/****************************************************************************
 * Name: SENSOR_CREATE_INSTANCE
 *
 * Description:
 *   create a sensor device instance as sensor_device_t.
 *
 * Parameters:
 *   name - sensor device name.
 *          generally, SENSOR_NAME_XXXXX will be a name. it is in the specified sensor driver's header file.
 *   _type - sensor device type as sensor_device_type_e type.
 *   _ops - sensor operations handle as sensor_operations_t type.
 *   _priv - sensor device's private data handle as void type.
 *
 * Returned Value:
 *   none.
 *
 ****************************************************************************/
#define SENSOR_CREATE_INSTANCE(name, _type, _ops, _priv) 	__SENSOR_CREATE_INSTANCE(name, _type, _ops, _priv)

/****************************************************************************
 * Name: SENSOR_GET_NAME_STRING
 *
 * Description:
 *   declare external function prototype.
 *
 * Parameters:
 *   name - sensor device name.
 *          generally, SENSOR_NAME_XXXXX will be a name. it is in the specified sensor driver's header file.
 *
 * Returned Value:
 *   none.
 *
 ****************************************************************************/
#define SENSOR_EXTERNAL_FUNCTION_PROTOTYPE(name) 			__SENSOR_EXTERNAL_FUNCTION_PROTOTYPE(name)

/***************************
  For Upper Layer Developers
 ***************************/

/****************************************************************************
 * Name: SENSOR_GET_NAME_STRING
 *
 * Description:
 *   get a name as string type.
 *
 * Parameters:
 *   name - sensor device name.
 *          generally, SENSOR_NAME_XXXXX will be a name. it is in the specified sensor driver's header file.
 *
 * Returned Value:
 *   a name as string type is returned.
 *
 ****************************************************************************/
#define SENSOR_GET_NAME_STRING(name)    __SENSOR_GET_NAME_STRING(name)

/****************************************************************************
 * Name: SENSOR_GET_HANDLE
 *
 * Description:
 *   get a sensor device handle.
 *
 * Parameters:
 *   name - sensor device name.
 *          generally, SENSOR_NAME_XXXXX will be a name. it is in the specified sensor driver's header file.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_GET_HANDLE(name) 		__SENSOR_GET_HANDLE(name)

/****************************************************************************
 * Name: SENSOR_GET_DEVICE_TYPE
 *
 * Description:
 *   get sensor device type as sensor_device_type_e type.
 *
 * Parameters:
 *   handle - sensor device handle.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_GET_DEVICE_TYPE(handle)	(handle ? handle->type : -1)

/****************************************************************************
 * Name: SENSOR_INIT
 *
 * Description:
 *   initialize  the specified sensor device.
 *
 * Parameters:
 *   handle - sensor device handle.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_INIT(handle) 			(handle && handle->ops ? handle->ops->init(handle) : -1)

/****************************************************************************
 * Name: SENSOR_DEINIT
 *
 * Description:
 *   deinitialize  the specified sensor device.
 *
 * Parameters:
 *   handle - sensor device handle.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_DEINIT(handle) 			(handle && handle->ops ? handle->ops->deinit(handle) : -1)

/****************************************************************************
 * Name: SENSOR_ACTIVATE
 *
 * Description:
 *   activate  the specified sensor device.
 *
 * Parameters:
 *   handle - sensor device handle.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_ACTIVATE(handle) 		(handle && handle->ops ? handle->ops->activate(handle) : -1)

/****************************************************************************
 * Name: SENSOR_DEACTIVATE
 *
 * Description:
 *   deactivate  the specified sensor device.
 *
 * Parameters:
 *   handle - sensor device handle.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_DEACTIVATE(handle) 		(handle && handle->ops ? handle->ops->deactivate(handle) : -1)

/****************************************************************************
 * Name: SENSOR_IOCTL
 *
 * Description:
 *   do ioctl on the specified sensor device.
 *
 * Parameters:
 *   handle - sensor device handle.
 *   id - ioctl id as sensor_ioctl_id_e type.
 *   pval -ioctl value as sensor_ioctl_value_t type.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_IOCTL(handle, id, pval) 	(handle && handle->ops ? handle->ops->ioctl(handle, id, pval) : -1)

/****************************************************************************
 * Name: SENSOR_SET_TRIGGER
 *
 * Description:
 *   set trigger on the specified sensor device.
 *
 * Parameters:
 *   handle - sensor device handle.
 *   info - trigger information as sensor_trigger_info_t type.
 *   callback - callback function as sensor_trigger_callback_t type.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_SET_TRIGGER(handle, info, callback)	(handle && handle->ops ? handle->ops->set_trigger(handle, info, callback) : -1)

/****************************************************************************
 * Name: SENSOR_GET_DATA
 *
 * Description:
 *   get data from the specified sensor device.
 *
 * Parameters:
 *   handle - sensor device handle.
 *   pdata - data as sensor_data_t type.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_GET_DATA(handle, pdata)	(handle && handle->ops ? handle->ops->get_data(handle, pdata) : -1)

/***************************
  Utilities
 ***************************/

/****************************************************************************
 * Name: SENSOR_TIME_GET
 *
 * Description:
 *   get time information as 'struct timeval' type.
 *
 * Parameters:
 *   time - the pointer of 'struct timeval' type variable.
 *
 * Returned Value:
 *   on success, 0 is returned. on failure, a negative value is returned.
 *
 ****************************************************************************/
#define SENSOR_TIME_GET(time)          gettimeofday(&time, NULL)

/****************************************************************************
 * Name: SENSOR_TIME_GET
 *
 * Description:
 *   get time differential in usec beween old time and current time.
 *
 * Parameters:
 *   old_time - old time's pointer variable that points 'struct timeval' type.
 *   cur_time - current time's pointer variable that points 'struct timeval' type.
 *
 * Returned Value:
 *   time differentil in usec
 *
 ****************************************************************************/
#define SENSOR_TIME_DIFF_USEC(old_time, cur_time) \
        ((cur_time.tv_sec * 1000000 + cur_time.tv_usec) - (old_time.tv_sec * 1000000 + old_time.tv_usec))

#ifdef __cplusplus
}
#endif
#endif							/* __SENSOR_H__ */
