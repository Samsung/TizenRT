/****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************

  * @file     gls.c
  * @brief    glucose service source file.
  * @details  Interface to access the glucose service.
  * @author   bill
  * @date     2017-6-8
  * @version  v1.0
  * *************************************************************************************
  */

#include "trace.h"
#include <string.h>
#include "gatt.h"
#include "gls.h"


/** @brief service related UUIDs. */
#define GATT_UUID_GLUCOSE                           0x1808
#define GATT_UUID_CHAR_GLC_MEASUREMENT              0x2A18
#define GATT_UUID_CHAR_GLC_MEASUREMENT_CONTEXT      0x2A34
#define GATT_UUID_CHAR_GLC_FEATURE                  0x2A51
#define GATT_UUID_CHAR_GLC_RACP                     0x2A52

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
/**  Value of glucose feature characteristic. */
static uint16_t  features;

/** glucose record access control point */
T_GLC_RACP glc_racp;
T_PATIENT_RECORD *p_new_record;
/** flags of notification and indication for characteristic */
static T_GLC_NOTIFY_INDICATE_FLAG glc_notify_indicate_flag = {0};

/** parameters used for splitting report records procedure */
uint16_t gls_num_records_to_report = 0;
uint16_t gls_current_record_to_report = 0;
uint16_t gls_report_offset = 0;
uint8_t gls_send_data_flag = 0;
/** Flag used for aborting procedure */
bool gls_abort_flag = false;
bool gls_abort_by_app_flag = false;

/**  Function pointer used to send event to application from GLS. Initiated in gls_add_service. */
static P_FUN_SERVER_GENERAL_CB pfn_gls_cb = NULL;

/** @brief  profile/service definition.  */
const T_ATTRIB_APPL gls_att_tbl[] = {
	/* <<Primary Service>>, .. */
	{
		(ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_PRIMARY_SERVICE),
			HI_WORD(GATT_UUID_PRIMARY_SERVICE),
			LO_WORD(GATT_UUID_GLUCOSE),      /* service UUID */
			HI_WORD(GATT_UUID_GLUCOSE)
		},
		UUID_16BIT_SIZE,                            /* bValueLen     */
		NULL,                                       /* pValueContext */
		GATT_PERM_READ                              /* wPermissions  */
	},
	/* <<Characteristic>>, .. */
	{
		ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHARACTERISTIC),
			HI_WORD(GATT_UUID_CHARACTERISTIC),
			GATT_CHAR_PROP_NOTIFY  /* characteristic properties */
			/* characteristic UUID not needed here, is UUID of next attrib. */
		},
		1,                                          /* bValueLen */
		NULL,
		GATT_PERM_READ                              /* wPermissions */
	},
	/*--- Glucose Measurement characteristic value ---*/
	{
		ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHAR_GLC_MEASUREMENT),
			HI_WORD(GATT_UUID_CHAR_GLC_MEASUREMENT)
		},
		0,                                          /* bValueLen */
		NULL,
		GATT_PERM_READ                              /* wPermissions */
	},
	/* client characteristic configuration */
	{
		(ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
		 ATTRIB_FLAG_CCCD_APPL),
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			/* NOTE: this value has an instantiation for each client, a write to */
			/* this attribute does not modify this default value:                */
			LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
			HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
		},
		2,                                          /* bValueLen */
		NULL,
		(GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
	}

#if GLC_MEASUREMENT_CONTEXT_SUPPORT
	,
	/* <<Characteristic>>, .. */
	{
		ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHARACTERISTIC),
			HI_WORD(GATT_UUID_CHARACTERISTIC),
			GATT_CHAR_PROP_NOTIFY,                    /* characteristic properties */
			//XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
			/* characteristic UUID not needed here, is UUID of next attrib. */
		},
		1,                                          /* bValueLen */
		NULL,
		GATT_PERM_READ                              /* wPermissions */
	},
	/*--- Glucose Measurement Context characteristic value ---*/
	{
		ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHAR_GLC_MEASUREMENT_CONTEXT),
			HI_WORD(GATT_UUID_CHAR_GLC_MEASUREMENT_CONTEXT)
		},
		0,                                          /* variable size */
		(void *)NULL,
		GATT_PERM_READ                              /* wPermissions */
	},
	/* client characteristic configuration */
	{
		(ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
		 ATTRIB_FLAG_CCCD_APPL),
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			/* NOTE: this value has an instantiation for each client, a write to */
			/* this attribute does not modify this default value:                */
			LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
			HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
		},
		2,                                          /* bValueLen */
		NULL,
		(GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
	}
#endif  /**< end of GLC_MEASUREMENT_CONTEXT_SUPPORT */

	,
	/* <<Characteristic>>, .. */
	{
		ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHARACTERISTIC),
			HI_WORD(GATT_UUID_CHARACTERISTIC),
			GATT_CHAR_PROP_READ                       /* characteristic properties */
			/* characteristic UUID not needed here, is UUID of next attrib. */
		},
		1,                                          /* bValueLen */
		NULL,
		GATT_PERM_READ                              /* wPermissions */
	},
	/*--- Glucose features characteristic value ---*/
	{
		ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHAR_GLC_FEATURE),
			HI_WORD(GATT_UUID_CHAR_GLC_FEATURE),
		},
		2,                                          /* bValueLen */
		NULL,
		GATT_PERM_READ                              /* wPermissions */
	}

	,
	/* <<Characteristic>>, .. */
	{
		ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHARACTERISTIC),
			HI_WORD(GATT_UUID_CHARACTERISTIC),
			(GATT_CHAR_PROP_WRITE |                   /* characteristic properties */
			 GATT_CHAR_PROP_INDICATE)
			/* characteristic UUID not needed here, is UUID of next attrib. */
		},
		1,                                          /* bValueLen */
		NULL,
		GATT_PERM_READ                              /* wPermissions */
	},
	/*--- Glucose Record Access Control Point value ---*/
	{
		ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHAR_GLC_RACP),
			HI_WORD(GATT_UUID_CHAR_GLC_RACP)
		},
		0,                                          /* bValueLen, 0 : variable length */
		NULL,
		(GATT_PERM_READ | GATT_PERM_WRITE)//GATT_PERM_WRITE_AUTHEN_REQ                  /* wPermissions */
	},
	/* client characteristic configuration */
	{
		(ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
		 ATTRIB_FLAG_CCCD_APPL),
		{                                           /* bTypeValue */
			LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
			/* NOTE: this value has an instantiation for each client, a write to */
			/* this attribute does not modify this default value:                */
			LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
			HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
		},
		2,                                          /* bValueLen */
		NULL,
		(GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
	}
};


/**
 * @brief       Prepare a new record in database.
 * @return void.
 *
 */
void gls_prepare_new_record()
{
	PROFILE_PRINT_INFO4("gls_prepare_new_record database head: %d, tail: %d, num: %d, seq_num: %d\n",
						glc_racp.record_db.head, glc_racp.record_db.tail, glc_racp.record_db.record_num,
						glc_racp.record_db.seq_num);
	p_new_record = &(glc_racp.record_db.records[glc_racp.record_db.tail]);
	memset(p_new_record, 0, sizeof(T_PATIENT_RECORD));
	glc_racp.record_db.seq_num++;
	p_new_record->glc_measurement_value.seq_num = glc_racp.record_db.seq_num;
#if GLC_MEASUREMENT_CONTEXT_SUPPORT
	p_new_record->glc_measurement_context.seq_num = glc_racp.record_db.seq_num;
#endif
}

/**
 * @brief       Push a new record into database.
 * @return void.
 *
 */
void gls_push_new_record()
{
	if (((glc_racp.record_db.head - glc_racp.record_db.tail + GLC_RACP_DATABASE_SIZE) %
		 GLC_RACP_DATABASE_SIZE) == 1) {
		glc_racp.record_db.head = (glc_racp.record_db.head + 1) % GLC_RACP_DATABASE_SIZE;
		glc_racp.record_db.tail = (glc_racp.record_db.tail + 1) % GLC_RACP_DATABASE_SIZE;
	} else {
		glc_racp.record_db.tail = (glc_racp.record_db.tail + 1) % GLC_RACP_DATABASE_SIZE;
		glc_racp.record_db.record_num++;
	}
	PROFILE_PRINT_INFO4("gls_push_new_record database head: %d, tail: %d, num: %d, seq_num: %d\n",
						glc_racp.record_db.head, glc_racp.record_db.tail, glc_racp.record_db.record_num,
						glc_racp.record_db.seq_num);
}

/**
 * @brief       Set a GLS parameter.
 *
 *              NOTE: You can call this function with a gulcose service parameter type and it will set the
 *                      gulcose service parameter.  Glucose service parameters are defined in @ref T_GLS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Glucose service parameter type: @ref T_GLS_PARAM_TYPE
 * @param[in]   len       Length of data to write
 * @param[in]   p_value Pointer to data to write.  This is dependent on
 *                      the parameter type and WILL be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_GLC_MEASUREMENT_FLAG ms_flag =
        {
            1, //time_offset
            1, //con_ts_loc
            GLC_FLAGS_UNITS_MOL_L_ON, //con_units
            1, //ss_annuciation
            0, //ctxt_info_follows
            0  //rfu
        };
        gls_set_parameter(GLS_PARAM_GLC_MS_FLAG, 1, &ms_flag);
    }
 * \endcode
 */
bool gls_set_parameter(T_GLS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
	bool ret = true;

	switch (param_type) {
	case GLS_PARAM_GLC_FEATURES:
		if (len == 2) {
			LE_ARRAY_TO_UINT16(features, (uint8_t *) p_value);
		} else {
			ret = false;
		}
		break;
	case GLS_PARAM_CTL_PNT_PROG_CLR:
		glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_RESERVED;
		gls_num_records_to_report = 0;
		gls_current_record_to_report = 0;
		gls_report_offset = 0;
		gls_send_data_flag = 0;
		gls_abort_flag = false; // Make sure Abort Flag is clear after RACP procedure is over!
		gls_abort_by_app_flag = false; // Make sure Abort by App Flag is clear after RACP procedure is over!
		break;
	case GLS_PARAM_GLC_MS_FLAG:
		if (len == 1) {
			memcpy(&(p_new_record->glc_measurement_value.flags), p_value, 1);
		} else {
			ret = false;
		}
		break;
	case GLS_PARAM_GLC_MS_BASE_TIME:
		if (len == sizeof(TIMESTAMP)) {
			memcpy(p_new_record->glc_measurement_value.base_time, p_value, sizeof(TIMESTAMP));
		} else {
			ret = false;
		}
		break;
#if (GLC_INCLUDE_TIME_OFFSET)
	case GLS_PARAM_GLC_MS_TIME_OFFSET:
		if (len == 2) {
			p_new_record->glc_measurement_value.time_offset = *(int16_t *)p_value;
			p_new_record->glc_measurement_value.flags.time_offset = 1;
		} else {
			ret = false;
		}
		break;
#endif
#if (GLC_INCLUDE_CONC_TS_LOC)
	case GLS_PARAM_GLC_MS_CONCENTRATION:
		if (len == sizeof(SFLOAT)) {
			memcpy(&(p_new_record->glc_measurement_value.concentration), p_value, sizeof(SFLOAT));
			p_new_record->glc_measurement_value.flags.con_ts_loc = 1;
		} else {
			ret = false;
		}
		break;
	case GLS_PARAM_GLC_MS_CONCENTRATION_UNITS:
		if (len == 1) {
			p_new_record->glc_measurement_value.flags.con_units = *(uint8_t *)p_value;
		} else {
			ret = false;
		}
		break;
	case GLS_PARAM_GLC_MS_TYPE_SAMPLE_LOCATION:
		if (len == 1) {
			p_new_record->glc_measurement_value.type_sample_location = *(uint8_t *)p_value;
		} else {
			ret = false;
		}
		break;
#endif
#if (GLC_INCLUDE_SS_ANNUNC)
	case GLS_PARAM_GLC_MS_SENSOR_STATUS_ANNUNCIATION:
		if (len == 2) {
			memcpy(&(p_new_record->glc_measurement_value.ss_annunciation), p_value, 2);
			p_new_record->glc_measurement_value.flags.ss_annuciation = 1;
		} else {
			ret = false;
		}
		break;
#endif
#if GLC_MEASUREMENT_CONTEXT_SUPPORT
	case GLS_PARAM_GLC_MS_CT_FLAG:
		if (len == 1) {
			p_new_record->glc_measurement_context.flags = *(T_GLC_MSR_CTXT_FLAG *)p_value;
		} else {
			ret = false;
		}
		break;
#if (GLC_INCLUDE_CARBOHYDRATE)
	case GLS_PARAM_GLC_MS_CT_CARBOHYDRATE_ID:
		if (len == 1) {
			p_new_record->glc_measurement_context.carbohydrate_ID = *(uint8_t *)p_value;
			p_new_record->glc_measurement_context.flags.carbohydrate = 1;
		} else {
			ret = false;
		}
		break;
	case GLS_PARAM_GLC_MS_CT_CARBOHYDRATE:
		if (len == sizeof(SFLOAT)) {
			memcpy(&(p_new_record->glc_measurement_context.carbohydrate), p_value, sizeof(SFLOAT));
		} else {
			ret = false;
		}
		break;
#endif
#if (GLC_INCLUDE_MEAL)
	case GLS_PARAM_GLC_MS_CT_MEAL:
		if (len == 1) {
			p_new_record->glc_measurement_context.meal = *(uint8_t *)p_value;
			p_new_record->glc_measurement_context.flags.meal = 1;
		} else {
			ret = false;
		}
		break;
#endif
#if (GLC_INCLUDE_TESTER_HEALTH)
	case GLS_PARAM_GLC_MS_CT_TESTER_HEALTH:
		if (len == 1) {
			p_new_record->glc_measurement_context.tester_health = *(uint8_t *)p_value;
			p_new_record->glc_measurement_context.flags.tester_health = 1;
		} else {
			ret = false;
		}
		break;
#endif
#if (GLC_INCLUDE_EXCERCISE)
	case GLS_PARAM_GLC_MS_CT_EXERCISE_DURATION:
		if (len == 2) {
			LE_ARRAY_TO_UINT16(p_new_record->glc_measurement_context.exercise_duration, (uint8_t *)p_value);
			p_new_record->glc_measurement_context.flags.excercise = 1;
		} else {
			ret = false;
		}
		break;
	case GLS_PARAM_GLC_MS_CT_EXERCISE_INTENSITY:
		if (len == 1) {
			p_new_record->glc_measurement_context.exercise_intensity = *(uint8_t *)p_value;
		} else {
			ret = false;
		}
		break;
#endif
#if (GLC_INCLUDE_MEDICATION)
	case GLS_PARAM_GLC_MS_CT_MEDICATION_ID:
		if (len == 1) {
			p_new_record->glc_measurement_context.medication_ID = *(uint8_t *)p_value;
			p_new_record->glc_measurement_context.flags.medication = 1;
		} else {
			ret = false;
		}
		break;
	case GLS_PARAM_GLC_MS_CT_MEDICATION:
		if (len == sizeof(SFLOAT)) {
			memcpy(&(p_new_record->glc_measurement_context.medication), p_value, sizeof(SFLOAT));
		} else {
			ret = false;
		}
		break;
	case GLS_PARAM_GLC_MS_CT_MEDICATION_UNITS:
		if (len == 1) {
			p_new_record->glc_measurement_context.flags.medication_units = *(uint8_t *)p_value;
		} else {
			ret = false;
		}
		break;
#endif
#if (GLC_INCLUDE_HbA1c)
	case GLS_PARAM_GLC_MS_CT_HbA1c:
		if (len == sizeof(SFLOAT)) {
			memcpy(&(p_new_record->glc_measurement_context.hb_a1c), p_value, sizeof(SFLOAT));
			p_new_record->glc_measurement_context.flags.hb_a1c = 1;
		} else {
			ret = false;
		}
		break;
#endif
#endif
	default:
		ret = false;
		break;
	}

	if (!ret) {
		PROFILE_PRINT_ERROR1("gls_set_parameter gls parameter set failed: %d\n", param_type);
	}

	return ret;
}

/**
 * @brief   Get a GLS parameter.
 *
 *          NOTE: You can call this function with a gulcose parameter type and it will get a
 *          gulcose parameter. Glucose parameters are defined in @ref T_GLS_PARAM_TYPE.
 *
 * @param[in]   param_type Glucose parameter type: @ref T_GLS_PARAM_TYPE
 * @param[in,out]   len Pointer to the location to get the length of data.
 * @param[in,out]  p_value Pointer to the location to get the parameter value.  This is dependent on
 *                      the parameter type and will be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        int record_num;
        gls_get_parameter(GLS_PARAM_RECORD_NUM, &len, &record_num);
    }
 * \endcode
 */
bool gls_get_parameter(T_GLS_PARAM_TYPE param_type, uint8_t *len, void *p_value)
{
	bool ret = true;

	switch (param_type) {
	case GLS_PARAM_GLC_FEATURES:
		*len = sizeof(uint16_t);
		LE_UINT16_TO_ARRAY(p_value, features);
		break;
	case GLS_PARAM_RECORD_NUM:
		*len = sizeof(int);
		memcpy(p_value, &glc_racp.record_db.record_num, sizeof(int));
		break;
	case GLS_PARAM_RECORD_SEQ_NUM:
		*len = sizeof(uint16_t);
		memcpy(p_value, &glc_racp.record_db.seq_num, sizeof(uint16_t));
		break;
	default:
		*len = 0;
		ret = false;
		break;
	}

	if (!ret) {
		PROFILE_PRINT_ERROR1("gls_get_parameter gls parameter get failed: %d\n", param_type);
	}

	return ret;
}

/**
 * @brief       Send measurement notification data .
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   index  Index.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t battery_level = 90;
        bas_battery_level_value_notify(conn_id, bas_id, battery_level);
    }
 * \endcode
 */
bool gls_glc_measurement_notify(uint8_t conn_id, T_SERVER_ID service_id, uint8_t index)
{
	int current = (glc_racp.record_db.head + index) % GLC_RACP_DATABASE_SIZE;

	gls_send_data_flag = 1;

	T_GLC_MEASUREMENT_VALUE *pmeasurement_value =
		&glc_racp.record_db.records[current].glc_measurement_value;
	uint8_t temp_glc_measurement[sizeof(T_GLC_MEASUREMENT_VALUE)];
	uint8_t offset = 0;
	if (index >= glc_racp.record_db.record_num) {
		PROFILE_PRINT_ERROR0("gls_glc_measurement_notify glucose measurement value access overflow!");
		return false;
	}
	if (current == glc_racp.record_db.tail) {
		PROFILE_PRINT_ERROR0("gls_glc_measurement_notify glucose measurement database: empty!");
		return false;
	}

	memcpy(&temp_glc_measurement[offset], &pmeasurement_value->flags, 1);
	offset += 1;
	memcpy(&temp_glc_measurement[offset], &pmeasurement_value->seq_num, 2);
	offset += 2;
	memcpy(&temp_glc_measurement[offset], pmeasurement_value->base_time, 7);
	offset += 7;

#if (GLC_INCLUDE_TIME_OFFSET)
	if (pmeasurement_value->flags.time_offset) {
		memcpy(&temp_glc_measurement[offset], &pmeasurement_value->time_offset, 2);
		offset += 2;
	}
#endif
#if (GLC_INCLUDE_CONC_TS_LOC)
	if (pmeasurement_value->flags.con_ts_loc) {
		memcpy(&temp_glc_measurement[offset], pmeasurement_value->concentration, 2);
		offset += 2;
		memcpy(&temp_glc_measurement[offset], &pmeasurement_value->type_sample_location, 1);
		offset += 1;
	}
#endif
#if (GLC_INCLUDE_SS_ANNUNC)
	if (pmeasurement_value->flags.ss_annuciation) {
		memcpy(&temp_glc_measurement[offset], &pmeasurement_value->ss_annunciation, 2);
		offset += 2;
	}
#endif
	PROFILE_PRINT_INFO1("gls_glc_measurement_notify glucose measurement notification: index = %d \n",
						index);
	// send notification to client
	return server_send_data(conn_id, service_id, GLS_CHAR_GLC_MEASUREMENT_INDEX, temp_glc_measurement,
							offset, GATT_PDU_TYPE_NOTIFICATION);
}

#if GLC_MEASUREMENT_CONTEXT_SUPPORT
/**
 * @brief       Send measurement context notification data .
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   index  Index.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t battery_level = 90;
        bas_battery_level_value_notify(conn_id, bas_id, battery_level);
    }
 * \endcode
 */
bool gls_glc_measurement_context_notify(uint8_t conn_id, T_SERVER_ID service_id, uint8_t index)
{
	int current = (glc_racp.record_db.head + index) % GLC_RACP_DATABASE_SIZE;

	gls_send_data_flag = 2;

	T_GLC_MEASUREMENT_CONTEXT *pmeasurement_context =
		&glc_racp.record_db.records[current].glc_measurement_context;
	uint8_t temp_glc_measurement_ctxt[sizeof(T_GLC_MEASUREMENT_CONTEXT)];
	uint8_t offset = 0;
	if (index >= glc_racp.record_db.record_num) {
		PROFILE_PRINT_ERROR0("gls_glc_measurement_context_notify glucose measurement context access overflow!");
		return false;
	}
	if (current == glc_racp.record_db.tail) {
		PROFILE_PRINT_ERROR0("gls_glc_measurement_context_notify glucose measurement database: Empty!");
		return false;
	}

	memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->flags, 1);
	offset += 1;
	memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->seq_num, 2);
	offset += 2;
#if (GLC_INCLUDE_EXT_FLAGS)
	if (pmeasurement_context->flags.ext_flags) {
		memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->ext_flags, 1);
		offset += 1;
	}
#endif
#if (GLC_INCLUDE_CARBOHYDRATE)
	if (pmeasurement_context->flags.carbohydrate) {
		memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->carbohydrate_ID, 1);
		offset += 1;
		memcpy(&temp_glc_measurement_ctxt[offset], pmeasurement_context->carbohydrate, 2);
		offset += 2;
	}
#endif
#if (GLC_INCLUDE_MEAL)
	if (pmeasurement_context->flags.meal) {
		memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->meal, 1);
		offset += 1;
	}
#endif
#if (GLC_INCLUDE_TESTER_HEALTH)
	if (pmeasurement_context->flags.tester_health) {
		memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->tester_health, 1);
		offset += 1;
	}
#endif
#if (GLC_INCLUDE_EXCERCISE)
	if (pmeasurement_context->flags.excercise) {
		memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->exercise_duration, 2);
		offset += 2;
		memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->exercise_intensity, 1);
		offset += 1;
	}
#endif
#if (GLC_INCLUDE_MEDICATION)
	if (pmeasurement_context->flags.medication) {
		memcpy(&temp_glc_measurement_ctxt[offset], &pmeasurement_context->medication_ID, 1);
		offset += 1;
		memcpy(&temp_glc_measurement_ctxt[offset], pmeasurement_context->medication, 2);
		offset += 2;
	}
#endif
#if (GLC_INCLUDE_HbA1c)
	if (pmeasurement_context->flags.hb_a1c) {
		memcpy(&temp_glc_measurement_ctxt[offset], pmeasurement_context->hb_a1c, 2);
		offset += 2;
	}
#endif
	PROFILE_PRINT_INFO1("gls_glc_measurement_context_notify glucose measurement context notification: index = %d \n",
						index);
	// send notification to client
	return server_send_data(conn_id, service_id, GLS_CHAR_GLC_MEASUREMENT_CONTEXT_INDEX,
							temp_glc_measurement_ctxt, offset, GATT_PDU_TYPE_NOTIFICATION);
}
#endif

/**
  * @brief Indicate glucose racp procedure result to client.
  *
  * @param[in] service_id         Service ID to notify.
  * @param[in] rsp_code          racp respondence code.
  * @return Result of indication.
  * @retval 0 FALSE
  * @retval 1 TRUE
  */
bool gls_racp_response(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
	bool ret;

	gls_send_data_flag = 3;

	uint16_t attrib_index = GLS_CHAR_GLC_RACP_INDEX;
	glc_racp.ctrl_point.operand[0] = glc_racp.ctrl_point.op_code;
	glc_racp.ctrl_point.operand[1] = rsp_code;
	glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_RESP_CODE;
	glc_racp.ctrl_point.op = GLC_RACP_OPERATOR_NULL;

	glc_racp.cp_length = sizeof(T_GLC_CTRL_POINT_OPCODE) + sizeof(T_GLC_CTRL_POINT_OPERATOR) + sizeof(
							 T_GLC_CTRL_POINT_OPCODE) + sizeof(T_GLC_CTRL_POINT_RESP_CODES);

	// send indication to client
	//ret = server_send_data(conn_id, service_id, attrib_index, (uint8_t *) &glc_racp.ctrl_point,
	// glc_racp.cp_length, GATT_PDU_TYPE_INDICATION);
	PROFILE_PRINT_INFO1("gls_racp_response  glucose racp resp: %d \n", rsp_code);

	glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_RESERVED;

	return ret;
}

/**
  * @brief Indicate number of glucose records to client.
  *
  * @param[in] service_id         Service ID to notify.
  * @param[in] num               Number of glucose records.
  * @return Result of indication.
  * @retval 0 FALSE
  * @retval 1 TRUE
  */
bool gls_racp_num_response(uint8_t conn_id, T_SERVER_ID service_id, uint16_t num)
{
	bool ret;

	gls_send_data_flag = 3;

	uint16_t attrib_index = GLS_CHAR_GLC_RACP_INDEX;
	LE_UINT16_TO_ARRAY(glc_racp.ctrl_point.operand, num);
	glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_NBR_OF_RECS_RESP;
	glc_racp.ctrl_point.op = GLC_RACP_OPERATOR_NULL;

	glc_racp.cp_length = sizeof(T_GLC_CTRL_POINT_OPCODE) + sizeof(T_GLC_CTRL_POINT_OPERATOR) + sizeof(
							 uint16_t);

	// send indication to client
	//ret = server_send_data(conn_id, service_id, attrib_index, (uint8_t *) &glc_racp.ctrl_point,
	// glc_racp.cp_length, GATT_PDU_TYPE_INDICATION);
	PROFILE_PRINT_INFO1("gls_racp_num_response glucose racp num response: %d \n", num);

	glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_RESERVED;

	return ret;
}

/**
  * @brief Indication in abort procedure.
  *
  * @param[in] service_id         Service ID to notify.
  * @return Result of indication.
  * @retval 0 FALSE
  * @retval 1 TRUE
  */
bool gls_abort_success_response(uint8_t conn_id, T_SERVER_ID service_id)
{
	glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_ABORT_OPERATION;

	PROFILE_PRINT_INFO0("gls_abort_success_response gls racp procedure has been aborted!");

	return gls_racp_response(conn_id, service_id, GLC_RACP_RESP_SUCCESS);
}

/**
  * @brief Compare two timestamps.
  *
  * @param[in] time1         Time to compare.
  * @param[in] time2         Time to compare.
  * @return Result of comparison.
  * @retval 1   Time1 > Time2
  * @retval 0   Time1 = Time2
  * @retval -1  Time1 < Time2
  */
int time_cmp(const TIMESTAMP time1, const TIMESTAMP time2)
{
	uint16_t year1, year2;
	LE_ARRAY_TO_UINT16(year1, (uint8_t *) time1);
	LE_ARRAY_TO_UINT16(year2, (uint8_t *) time2);
	if (year1 < year2) {
		return -1;
	} else if (year1 > year2) {
		return 1;
	} else {
		int length;
		for (length = 0; length < 5; length++) {
			if (time1[2 + length] < time2[2 + length]) {
				return -1;
			} else if (time1[2 + length] > time2[2 + length]) {
				return 1;
			} else
				;
		}
		return 0;
	}
}

/**
 * @brief       Calculate user facing time by adding basetime and timeoffset.
 *
 *
 * @param[in]   time_in  Connection id.
 * @param[in]   time_offset  Service id.
 * @param[in]   time_out  Battery level value.
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        user_face_time(time, 30, time);
    }
 * \endcode
 */
void user_face_time(TIMESTAMP time_in, int16_t time_offset, TIMESTAMP time_out)
{
	uint8_t carry;
	uint16_t year;
	LE_ARRAY_TO_UINT16(year, (uint8_t *) time_in);

	TIMESTAMP time_tmp = {0};
	time_tmp[4] = time_offset / 60; //hour
	time_tmp[5] = time_offset % 60; //minute

	time_out[6] = time_in[6] + time_tmp[6]; //second
	carry = 0;

	time_out[5] = time_in[5] + time_tmp[5] + carry; //minute
	if (time_out[5] > 59) {
		time_out[5] -= 60;
		carry = 1;
	} else {
		carry = 0;
	}

	time_out[4] = time_in[4] + time_tmp[4] + carry; //hour
	if (time_out[4] > 23) {
		time_out[4] -= 24;
		carry = 1;
	} else {
		carry = 0;
	}

	time_out[3] = time_in[3] + time_tmp[3] + carry; //day

//    switch (time_out[2])
//    {
//        case 2:
//            if(((year % 400) == 0) || (((year % 4) == 0) &&((year % 100) != 0))) //leap year
//            {
//                if(time_out[3] > 29)
//                {
//                    time_out[3] -= 29;
//                    carry = 1;
//                }
//                else
//                {
//                    carry = 0;
//                }
//            }
//            else
//            {
//                if(time_out[3] > 28)
//                {
//                    time_out[3] -= 28;
//                    carry = 1;
//                }
//                else
//                {
//                    carry = 0;
//                }
//            }
//            break;
//        case 1:
//        case 3:
//        case 5:
//        case 7:
//        case 8:
//        case 10:
//        case 12:
//            if(time_out[3] > 31)
//            {
//                time_out[3] -= 31;
//                carry = 1;
//            }
//            else
//            {
//                carry = 0;
//            }
//            break;
//        case 4:
//        case 6:
//        case 9:
//        case 11:
//            if(time_out[3] > 30)
//            {
//                time_out[3] -= 30;
//                carry = 1;
//            }
//            else
//            {
//                carry = 0;
//            }
//            break;
//        default:
//            break;
//    }

	uint8_t month_leap[12]      = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint8_t month_common[12]   = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (((year % 400) == 0) || (((year % 4) == 0) && ((year % 100) != 0))) { //leap year
		if (time_out[3] > month_leap[time_out[2]]) {
			time_out[3] -= month_leap[time_out[2]];
			carry = 1;
		} else {
			carry = 0;
		}
	} else {
		if (time_out[3] > month_common[time_out[2]]) {
			time_out[3] -= month_common[time_out[2]];
			carry = 1;
		} else {
			carry = 0;
		}
	}

	time_out[2] = time_in[2] + time_tmp[2] + carry; //month

	if (time_out[2] > 12) {
		time_out[2] -= 12;
		carry = 1;
	} else {
		carry = 0;
	}

	uint16_t year_in;
	uint16_t year_tmp;
	LE_ARRAY_TO_UINT16(year_in, (uint8_t *) time_in);
	LE_ARRAY_TO_UINT16(year_tmp, (uint8_t *) time_tmp);
	LE_UINT16_TO_ARRAY(time_out, year_in + year_tmp + carry); //year
}

/**
  * @brief Check the write parameters of RACP, used in report/report num/delete opcode
  * @returen None
  */
T_GLC_CTRL_POINT_RESP_CODES gls_racp_check()
{
	T_GLC_CTRL_POINT_RESP_CODES ret = GLC_RACP_RESP_SUCCESS;

	PROFILE_PRINT_INFO3("gls_racp_check glucose racp: opcode = %d, operator = %d, length = %d \n",
						glc_racp.ctrl_point.op_code, glc_racp.ctrl_point.op, glc_racp.cp_length);

	if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_NULL) {
		ret = GLC_RACP_RESP_INVALID_OPERATOR;
	} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_ALL_RECS
			   || glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_FIRST
			   || glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_LAST) {
		if (glc_racp.cp_length != 2) {
			ret = GLC_RACP_RESP_INVALID_OPERAND;
		}
	} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_LT_EQ
			   || glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_GT_EQ
			   || glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_RANGE) {
		if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_RESERVED) {
			ret = GLC_RACP_RESP_INVALID_OPERAND;
		} else if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_SEQ_NBR) {
			if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_RANGE) {
				uint16_t min_seq;
				uint16_t max_seq;
				LE_ARRAY_TO_UINT16(min_seq, &glc_racp.ctrl_point.operand[1]);
				LE_ARRAY_TO_UINT16(max_seq, &glc_racp.ctrl_point.operand[1 + sizeof(uint16_t)]);
				if (min_seq > max_seq) {
					ret = GLC_RACP_RESP_INVALID_OPERAND;
				}
			}
		} else if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_TIME) {
			if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_RANGE) {
				if (time_cmp(&glc_racp.ctrl_point.operand[1],
							 &glc_racp.ctrl_point.operand[1 + sizeof(TIMESTAMP)]) > 0) {
					ret = GLC_RACP_RESP_INVALID_OPERAND;
				}
			}
		} else {
			/** glc_racp.ctrl_point.operand[0] > GLC_RACP_FILTER_TYPE_TIME */
			ret = GLC_RACP_RESP_OPERAND_NOT_SUPPORTED;
		}
	} else {
		/** glc_racp.ctrl_point.operator > GLC_RACP_OPERATOR_LAST */
		ret = GLC_RACP_RESP_OPERATOR_NOT_SUPPORTED;
	}
	return ret;
}

/**
  * @brief Find record by sequence number
  * @param[in] op           operator of the procedure.
  * @param[in] set_seq      sequence number referenced to.
  * @return Absolute position of the found record
  */
int gls_find_records_by_seq_num(T_GLC_CTRL_POINT_OPERATOR op, uint16_t set_seq)
{
	int find;
	if (op == GLC_RACP_OPERATOR_LT_EQ) {
		find = glc_racp.record_db.head;
		while ((find != glc_racp.record_db.tail) &&
			   (glc_racp.record_db.records[find].glc_measurement_value.seq_num <= set_seq)) {
			find = (find + 1) % GLC_RACP_DATABASE_SIZE;
		}
		if (find == glc_racp.record_db.head) { /**< no record found */
			find = glc_racp.record_db.tail;
		} else {
			find -= 1;    /**< record large than set_seq is found */
		}
	} else if (op == GLC_RACP_OPERATOR_GT_EQ) {
		find = glc_racp.record_db.head;
		while ((find != glc_racp.record_db.tail) &&
			   (glc_racp.record_db.records[find].glc_measurement_value.seq_num < set_seq)) {
			find = (find + 1) % GLC_RACP_DATABASE_SIZE;
		}
	} else {
		find = glc_racp.record_db.tail;
	}
	return find;
}

/**
  * @brief Find record by timestamp
  * @brief Should time_offset be taken into consideration when comparing user-interface time?
  *     PTS test shows that when writing the RACP using the User Facing Time filter type there is no timeoffset part.
  * @param[in] op           operator of the procedure.
  * @param[in] set_time     time referenced to.
  * @return Absolute position of the found record
  */
int gls_find_records_by_time(T_GLC_CTRL_POINT_OPERATOR op, TIMESTAMP set_time)
{
	int find;
	//TIMESTAMP user_time;
	if (op == GLC_RACP_OPERATOR_LT_EQ) {
		find = glc_racp.record_db.head;
		while (find != glc_racp.record_db.tail) {
			//user_face_time(glc_racp.record_db.records[find].glc_measurement_value.base_time, TimeOffset, user_time);
			//user_face_time(glc_racp.record_db.records[find].glc_measurement_value.base_time, glc_racp.record_db.records[find].glc_measurement_value.time_offset, user_time);
			//if (time_cmp(user_time, set_time) <= 0)
			if (time_cmp(glc_racp.record_db.records[find].glc_measurement_value.base_time, set_time) <= 0) {
				find = (find + 1) % GLC_RACP_DATABASE_SIZE;
			} else {
				break;
			}
		}
		if (find == glc_racp.record_db.head) { /**< no record found */
			find = glc_racp.record_db.tail;
		} else {
			find -= 1;    /**< record large than set_seq is found */
		}
	} else if (op == GLC_RACP_OPERATOR_GT_EQ) {
		find = glc_racp.record_db.head;
		while (find != glc_racp.record_db.tail) {
			//user_face_time(glc_racp.record_db.records[find].glc_measurement_value.base_time, TimeOffset, user_time);
			//user_face_time(glc_racp.record_db.records[find].glc_measurement_value.base_time, glc_racp.record_db.records[find].glc_measurement_value.time_offset, user_time);
			//if (time_cmp(user_time, set_time) < 0)
			if (time_cmp(glc_racp.record_db.records[find].glc_measurement_value.base_time, set_time) < 0) {
				find = (find + 1) % GLC_RACP_DATABASE_SIZE;
			} else {
				break;
			}
		}
	} else {
		find = glc_racp.record_db.tail;
	}
	return find;
}

/**
  * @brief Find records that meets the conditions.
  * @param[out] pnum    num of records
  * @param[out] pfirst  absolute position of the first record
  * @param[out] plast   absolute position of the last record
  * @return The check result of the procedure.
  */
T_GLC_CTRL_POINT_RESP_CODES gls_find_records(uint16_t *p_num, int *p_first, int *p_last)
{
	uint16_t num_of_records = 0;
	int find1 = 0;
	int find2 = 0;
	T_GLC_CTRL_POINT_RESP_CODES ret;
	ret = gls_racp_check();
	if (ret == GLC_RACP_RESP_SUCCESS) {
		if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_ALL_RECS) {
			find1 = glc_racp.record_db.head;
			find2 = (glc_racp.record_db.tail - 1 + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
			num_of_records = glc_racp.record_db.record_num;
		} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_LT_EQ) {
			find1 = glc_racp.record_db.head;
			if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_SEQ_NBR) {
				uint16_t seq;
				LE_ARRAY_TO_UINT16(seq, &glc_racp.ctrl_point.operand[1]);
				find2 = gls_find_records_by_seq_num(GLC_RACP_OPERATOR_LT_EQ, seq);
			} else if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_TIME) {
				find2 = gls_find_records_by_time(GLC_RACP_OPERATOR_LT_EQ, &glc_racp.ctrl_point.operand[1]);
			} else {
				find2 = glc_racp.record_db.tail;
			}
			if (find2 == glc_racp.record_db.tail) {
				num_of_records = 0;
			} else {
				num_of_records = (find2 - glc_racp.record_db.head + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE
								 + 1;
			}
		} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_GT_EQ) {
			if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_SEQ_NBR) {
				uint16_t seq;
				LE_ARRAY_TO_UINT16(seq, &glc_racp.ctrl_point.operand[1]);
				find1 = gls_find_records_by_seq_num(GLC_RACP_OPERATOR_GT_EQ, seq);
			} else if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_TIME) {
				find1 = gls_find_records_by_time(GLC_RACP_OPERATOR_GT_EQ, &glc_racp.ctrl_point.operand[1]);
			} else {
				find1 = glc_racp.record_db.tail;
			}
			find2 = (glc_racp.record_db.tail - 1 + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
			num_of_records = (glc_racp.record_db.tail - find1 + GLC_RACP_DATABASE_SIZE) %
							 GLC_RACP_DATABASE_SIZE;
		} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_RANGE) {
			if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_SEQ_NBR) {
				uint16_t seq1;
				uint16_t seq2;
				LE_ARRAY_TO_UINT16(seq1, &glc_racp.ctrl_point.operand[1]);
				LE_ARRAY_TO_UINT16(seq2, &glc_racp.ctrl_point.operand[1 + 2]);

				find1 = gls_find_records_by_seq_num(GLC_RACP_OPERATOR_GT_EQ, seq1);
				find2 = gls_find_records_by_seq_num(GLC_RACP_OPERATOR_LT_EQ, seq2);
			} else if (glc_racp.ctrl_point.operand[0] == GLC_RACP_FILTER_TYPE_TIME) {
				find1 = gls_find_records_by_time(GLC_RACP_OPERATOR_GT_EQ, &glc_racp.ctrl_point.operand[1]);
				find2 = gls_find_records_by_time(GLC_RACP_OPERATOR_LT_EQ,
												 &glc_racp.ctrl_point.operand[1 + sizeof(TIMESTAMP)]);
			} else {
				find1 = glc_racp.record_db.tail;
				find2 = glc_racp.record_db.tail;
			}
			if (find1 == glc_racp.record_db.tail || find2 == glc_racp.record_db.tail) {
				num_of_records = 0;
			} else {
				num_of_records = (find2 - find1 + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE + 1;
			}
		} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_FIRST) {
			find1 = glc_racp.record_db.head;
			find2 = find1;
			if (glc_racp.record_db.record_num == 0) {
				num_of_records = 0;
			} else {
				num_of_records = 1;
			}
		} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_LAST) {
			find1 = (glc_racp.record_db.tail - 1 + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
			find2 = find1;
			if (glc_racp.record_db.record_num == 0) {
				num_of_records = 0;
			} else {
				num_of_records = 1;
			}
		} else {
			ret = GLC_RACP_RESP_OPERATOR_NOT_SUPPORTED;
			return ret;
		}

		if (p_num != 0) {
			*p_num = num_of_records;
		}
		if (p_first != 0) {
			*p_first = find1;
		}
		if (p_last != 0) {
			*p_last = find2;
		}
		PROFILE_PRINT_INFO3("gls_find_records glucose find records: num = %d, start = %d, end = %d\n",
							num_of_records, find1, find2);
	}
	return ret;
}

/**
  * @brief Report number of records that meet the conditions.
  * @param[in] service_id   The service ID of glucose service
  * @return None
  */
void gls_report_num_of_records(uint8_t conn_id, T_SERVER_ID service_id)
{
	T_GLC_CTRL_POINT_RESP_CODES ret;
	uint16_t num_of_records;
	int find1, find2;
	ret = gls_find_records(&num_of_records, &find1, &find2);
	if (ret != GLC_RACP_RESP_SUCCESS) {
		gls_racp_response(conn_id, service_id, ret);
	} else {
		gls_racp_num_response(conn_id, service_id, num_of_records);
	}
}

/**
  * @brief Report record
  * @param[in] service_ID   The service ID of glucose service
  * @param[in] index        The index of record
  * Attention: For the purpose of good user experience, the input parameter index of this function is the offset of the head, not the pointer of data record.
  */
void gls_report_record(uint8_t conn_id, T_SERVER_ID service_id, int index)
{
	PROFILE_PRINT_INFO1("gls_report_record: %d\n", index);
	gls_glc_measurement_notify(conn_id, service_id, index);
#if GLC_MEASUREMENT_CONTEXT_SUPPORT
	int current = (glc_racp.record_db.head + index) % GLC_RACP_DATABASE_SIZE;
	if (glc_racp.record_db.records[current].glc_measurement_value.flags.ctxt_info_follows == 1) {
		gls_glc_measurement_context_notify(conn_id, service_id, index);
	}
#endif
}

/**
  * @brief Report records that meet the conditions.
  * @param[in] service_id   The service ID of glucose service
  * @return None
  */
void gls_report_records(uint8_t conn_id, T_SERVER_ID service_id)
{
	T_GLC_CTRL_POINT_RESP_CODES ret;
	uint16_t num_of_records;
	int find1, find2;
	ret = gls_find_records(&num_of_records, &find1, &find2);
	if (ret == GLC_RACP_RESP_SUCCESS) {
		if (num_of_records == 0) {
			ret = GLC_RACP_RESP_NO_RECS_FOUND;
			gls_racp_response(conn_id, service_id, ret);
		} else {
//            int offset = (find1 - glc_racp.record_db.head + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
//            for (int index = 0; index < num_of_records; index++)
//            {
//                gls_report_record(service_ID, offset + index); // attention!
//                VoidCheckAbortFlag();
//            }
			gls_report_offset = (find1 - glc_racp.record_db.head + GLC_RACP_DATABASE_SIZE) %
								GLC_RACP_DATABASE_SIZE;
			gls_num_records_to_report = num_of_records;
			gls_current_record_to_report = 0;
			if (false == gls_report_records_task(conn_id, service_id)) {
				gls_set_parameter(GLS_PARAM_CTL_PNT_PROG_CLR, 0, NULL);
			}
		}
	} else {
		gls_racp_response(conn_id, service_id, ret);
	}
}

/**
 * @brief       Report records sub procedure.
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t battery_level = 90;
        bas_battery_level_value_notify(conn_id, bas_id, battery_level);
    }
 * \endcode
 */
bool gls_report_records_task(uint8_t conn_id, T_SERVER_ID service_id)
{
	bool ret = true;
	if (gls_abort_flag == true) {
		PROFILE_PRINT_INFO2("gls_report_records_task  Glucose current record = %d, total = %d, procedure abort successfully!\n",
							gls_current_record_to_report, gls_num_records_to_report);
		gls_current_record_to_report = gls_num_records_to_report; // stop transmitting any data
		ret = gls_abort_success_response(conn_id, service_id);
		gls_abort_flag = false;
		gls_abort_by_app_flag = false; // Abort procedure first, prior to abort by application
		return ret;
	}

	if (gls_abort_by_app_flag == true) {
		PROFILE_PRINT_INFO2("gls_report_records_task  Glucose current record = %d, total = %d, procedure abort by app successfully!\n",
							gls_current_record_to_report, gls_num_records_to_report);
		gls_current_record_to_report = gls_num_records_to_report; // stop transmitting any data
		ret = gls_racp_response(conn_id, service_id, GLC_RACP_RESP_PROC_NOT_COMPLETED);
		gls_abort_by_app_flag = false;
		return ret;
	}

	PROFILE_PRINT_INFO3("gls_report_records_task  Glucose report records, current = %d, total = %d, gls_send_data_flag = %d\n",
						gls_current_record_to_report, gls_num_records_to_report, gls_send_data_flag);

	if (gls_send_data_flag == 1) {
#if GLC_MEASUREMENT_CONTEXT_SUPPORT
		int current = (glc_racp.record_db.head + gls_report_offset + gls_current_record_to_report) %
					  GLC_RACP_DATABASE_SIZE;
		if (glc_racp.record_db.records[current].glc_measurement_value.flags.ctxt_info_follows == 1) {
			ret = gls_glc_measurement_context_notify(conn_id, service_id,
					gls_report_offset + gls_current_record_to_report);
			gls_current_record_to_report += 1;
			return ret;
		}
#else
		gls_current_record_to_report += 1;
#endif
	}

	if (gls_current_record_to_report < gls_num_records_to_report) {
		ret = gls_glc_measurement_notify(conn_id, service_id,
										 gls_report_offset + gls_current_record_to_report);
	} else {
		if (gls_send_data_flag != 3) {
			ret = gls_racp_response(conn_id, service_id, GLC_RACP_RESP_SUCCESS);
		} else { // clear control point
			gls_set_parameter(GLS_PARAM_CTL_PNT_PROG_CLR, 0, NULL);
		}
	}
	return ret;
}

/**
 * @brief Delete records that meet the conditions.
 * @param[in] conn_id  Connection id.
 * @param[in] service_id   The service ID of glucose service
 * @return None
 */
void gls_delete_records(uint8_t conn_id, T_SERVER_ID service_id)
{
	T_GLC_CTRL_POINT_RESP_CODES ret;
	uint16_t num_of_records;
	int find1, find2;
	ret = gls_find_records(&num_of_records, &find1, &find2);

	PROFILE_PRINT_INFO3("gls_delete_records glucose delete records: num = %d, start = %d, end = %d\n",
						num_of_records, find1, find2);
	if (ret == GLC_RACP_RESP_SUCCESS) {
		if (num_of_records > 0) {
			if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_ALL_RECS) {
				glc_racp.record_db.head = glc_racp.record_db.tail;
				glc_racp.record_db.record_num = 0;
			} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_LT_EQ) {
				glc_racp.record_db.head = (find2 + 1) % GLC_RACP_DATABASE_SIZE;
				glc_racp.record_db.record_num -= num_of_records;
			} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_GT_EQ) {
				glc_racp.record_db.tail = find1;
				glc_racp.record_db.record_num -= num_of_records;
			} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_RANGE) {
				int front = (find1 - glc_racp.record_db.head + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
				int back = (glc_racp.record_db.tail - find2 - 1 + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
				int num2move, move_from, move_to, loop;
				if (front > back) {
					num2move = back;
					move_from = (find2 + 1) % GLC_RACP_DATABASE_SIZE;
					move_to = find1;
					for (loop = 0; loop < num2move; loop++) {
						glc_racp.record_db.records[move_to] = glc_racp.record_db.records[move_from];
						move_from = (move_from + 1) % GLC_RACP_DATABASE_SIZE;
						move_to = (move_to + 1) % GLC_RACP_DATABASE_SIZE;
					}
					glc_racp.record_db.tail = (glc_racp.record_db.tail - num_of_records + GLC_RACP_DATABASE_SIZE) %
											  GLC_RACP_DATABASE_SIZE;
					glc_racp.record_db.record_num -= num_of_records;
				} else {
					num2move = front;
					move_from = (find1 - 1 + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
					move_to = find2;
					for (loop = 0; loop < num2move; loop++) {
						glc_racp.record_db.records[move_to] = glc_racp.record_db.records[move_from];
						move_from = (move_from - 1 + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
						move_to = (move_to - 1 + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE;
					}
					glc_racp.record_db.head = (glc_racp.record_db.head + num_of_records) % GLC_RACP_DATABASE_SIZE;
					glc_racp.record_db.record_num -= num_of_records;
				}
			} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_FIRST) {
				glc_racp.record_db.head = (glc_racp.record_db.head + 1) % GLC_RACP_DATABASE_SIZE;
				glc_racp.record_db.record_num -= 1;
			} else if (glc_racp.ctrl_point.op == GLC_RACP_OPERATOR_LAST) {
				glc_racp.record_db.tail = (glc_racp.record_db.tail - 1 + GLC_RACP_DATABASE_SIZE) %
										  GLC_RACP_DATABASE_SIZE;
				glc_racp.record_db.record_num -= 1;
			} else {
				ret = GLC_RACP_RESP_OPERAND_NOT_SUPPORTED;
			}
		} else {
			ret = GLC_RACP_RESP_NO_RECS_FOUND;
		}
	}
	gls_racp_response(conn_id, service_id, ret);
}

/**
  * @brief read characteristic data from service.
  *
  * @param[in] conn_id  Connection id.
  * @param[in] service_id          ServiceID of characteristic data.
  * @param[in] attrib_index       Attribute index of getting characteristic data.
  * @param[in] offset            Used for Blob Read.
  * @param[in,out] p_length           length of getting characteristic data.
  * @param[in,out] pp_value            data got from service.
  * @return Profile procedure result
  */
T_APP_RESULT gls_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
							  uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
	T_APP_RESULT cause = APP_RESULT_SUCCESS;
	T_GLS_CALLBACK_DATA gls_upsteam_msg;
	PROFILE_PRINT_INFO2("gls_attr_read_cb attribIndex = %d offset %x", attrib_index, offset);
	*p_length = 0;
	switch (attrib_index) {
	case GLS_CHAR_GLC_FEATURE_INDEX: {
		/* Notify Application. */
		if (pfn_gls_cb) {
			gls_upsteam_msg.msg_data.read_value_index = GLS_EVT_READ_FEATURE;
			gls_upsteam_msg.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
			pfn_gls_cb(service_id, (void *)&gls_upsteam_msg);
		}
		*pp_value = (uint8_t *) &features;
		*p_length = sizeof(features);
	}
	break;
	default: {
		PROFILE_PRINT_ERROR1("gls_attr_read_cb, attr not found, index=%d", attrib_index);
		cause  = APP_RESULT_ATTR_NOT_FOUND;
	}
	break;
	}
	return cause;
}

/**
  * @brief  handle control point write (request).
  *
  * @param[in] conn_id      Connection id.
  * @param[in] service_id   Service ID.
  * @param[in] write_length Write request data length.
  * @param[in] p_value      Pointer to write request data.
  * @return none
  * @retval  void
  */
static void gls_ctl_pnt_write_ind_post_proc(uint8_t conn_id, T_SERVER_ID service_id,
		uint16_t attrib_index, uint16_t write_length, uint8_t *p_value)
{
	T_GLS_CALLBACK_DATA GLS_upsteam_msg;
	/** check if there is any operation running */
	if (write_length > sizeof(T_GLC_CONTROL_POINT)) {
		PROFILE_PRINT_ERROR1("gls_ctl_pnt_write_ind_post_proc  GLS Control Point request error: OpCode=0x%x, invalid write length!",
							 p_value[0]);
		return;
	}

	PROFILE_PRINT_INFO4("gls_ctl_pnt_write_ind_post_proc  Write GLS Control Point: OpCode = %d, operator = %d, length = %d, filter type = %d",
						p_value[0], p_value[1], write_length, p_value[2]);
	printf("gls_ctl_pnt_write_ind_post_proc  Write GLS Control Point: OpCode = %d, operator = %d, length = %d, filter type = %d\r\n", p_value[0], p_value[1],
		   write_length, p_value[2]);

	/* Notify Application. */
	if (pfn_gls_cb) {
		GLS_upsteam_msg.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
		memset(&GLS_upsteam_msg.msg_data.write, 0, sizeof(T_GLC_CONTROL_POINT));
		memcpy(&GLS_upsteam_msg.msg_data.write, p_value, write_length);
		pfn_gls_cb(service_id, (void *)&GLS_upsteam_msg);
	}
	printf("gls_ctl_pnt_write_ind_post_proc:glc_racp.ctrl_point.op_code = 0x%x, gls_send_data_flag = %d\r\n", glc_racp.ctrl_point.op_code, gls_send_data_flag);
	if (p_value[0] != GLC_RACP_OPCODE_ABORT_OPERATION) {
		memset(&glc_racp.ctrl_point, 0, sizeof(T_GLC_CONTROL_POINT));
		memcpy(&glc_racp.ctrl_point, p_value, write_length);
		glc_racp.cp_length = write_length;
		switch (glc_racp.ctrl_point.op_code) {
		case GLC_RACP_OPCODE_REPORT_NBR_OF_RECS:
			gls_report_num_of_records(conn_id, service_id);
			break;
		case GLC_RACP_OPCODE_REPORT_RECS:
			gls_report_records(conn_id, service_id);
			break;
		case GLC_RACP_OPCODE_DELETE_RECS:
			gls_delete_records(conn_id, service_id);
			break;
		default:
			gls_racp_response(conn_id, service_id, GLC_RACP_RESP_OPCODE_NOT_SUPPORTED);
			break;
		}
	} else {
		if ((glc_racp.ctrl_point.op_code != GLC_RACP_OPCODE_RESERVED) && (gls_send_data_flag != 3)) {
			gls_abort_flag = true;
			//while (gls_abort_flag == TRUE) ; /* halt until AbortFlag is disabled when procedure in progress has been aborted */
		} else {
			gls_abort_success_response(conn_id, service_id);
		}
	}
}

/**
  * @brief Check CCCD configuration
  * @return Result of check
  * @retval TRUE
  * @retval FALSE
  */
bool gls_check_cccd()
{
	if (0 == glc_notify_indicate_flag.GLC_measurement_notify_enable) {
		return false;
	} else if (0 == glc_notify_indicate_flag.GLC_measurement_context_notify_enable) {
		return false;
	} else if (0 == glc_notify_indicate_flag.GLC_RACP_indicate_enable) {
		return false;
	} else {
		return true;
	}
}

/**
  * @brief write characteristic data from service.
  *
  * @param[in] conn_id          Connection id.
  * @param[in] service_id       ServiceID to be written.
  * @param[in] attrib_index     Attribute index of characteristic.
  * @param[in] length           Length of value to be written.
  * @param p_value              Value to be written.
  * @return Profile procedure result
  */
T_APP_RESULT gls_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
							   T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
							   P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
	T_APP_RESULT cause = APP_RESULT_SUCCESS;
	PROFILE_PRINT_INFO2("gls_attr_write_cb attrib_index = %d, length = %x", attrib_index, length);
	printf("gls_attr_write_cb attrib_index = %d, length = %x\r\n", attrib_index, length);
	switch (attrib_index) {
	case GLS_CHAR_GLC_RACP_INDEX: {
		printf("glc_racp.ctrl_point.op_code = 0x%x\r\n", glc_racp.ctrl_point.op_code);
		/* Attribute value has variable size, make sure written value size is valid. */
		if ((length > sizeof(T_GLC_CONTROL_POINT)) || (p_value == NULL)) {
			cause = APP_RESULT_INVALID_VALUE_SIZE;
		}
		/* Make sure Control Point is not "Process already in progress". */
		else if (GLC_RACP_OPERATION_ACTIVE(glc_racp.ctrl_point.op_code) &&
				 (p_value[0] != GLC_RACP_OPCODE_ABORT_OPERATION)) {
			printf("GLC_ERR_PROC_ALREADY_IN_PROGRESS\r\n");
			cause = (T_APP_RESULT)(ATT_ERR | GLC_ERR_PROC_ALREADY_IN_PROGRESS);
		}
		/* Make sure Control Point is configured indication enable. */
		else if (!gls_check_cccd()) {
			cause = (T_APP_RESULT)(ATT_ERR | GLC_ERR_CCCD_IMPROPERLY_CONFIGURED);
		} else {
			//wCause = GLS_Hanlde_CtlPntProc(ServiceId, wLength, pValue);
			/** handle RACP request after sending write response */
			PROFILE_PRINT_INFO2("gls_attr_write_cb opcode: old = %d, new = %d\n", glc_racp.ctrl_point.op_code,
								p_value[0]);
			printf("gls_attr_write_cb opcode: old = %d, new = %d\r\n", glc_racp.ctrl_point.op_code, p_value[0]);
			*p_write_ind_post_proc = gls_ctl_pnt_write_ind_post_proc;
		}

	}
	break;

	default: {
		PROFILE_PRINT_ERROR1("gls_attr_write_cb attribIndex = %d not found", attrib_index);
		cause  = APP_RESULT_ATTR_NOT_FOUND;
	}
	break;
	}
	return cause;
}

/**
  * @brief update CCCD bits from stack.
  *
  * @param[in] conn_id          Connection id.
  * @param service_id          Service ID.
  * @param index          Attribute index of characteristic data.
  * @param ccc_bits         CCCD bits from stack.
  * @return None
  */
void gls_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
	T_GLS_CALLBACK_DATA gls_upsteam_msg;
	bool handle = true;
	PROFILE_PRINT_INFO2("gls_cccd_update_cb  Index = %d ccc_bits %x", index, ccc_bits);
	switch (index) {
	case GLS_CHAR_GLC_MEASUREMENT_CCCD_INDEX: {
		if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY) {
			// Enable Notification
			glc_notify_indicate_flag.GLC_measurement_notify_enable = 1;
			gls_upsteam_msg.msg_data.notify_indicate_index = GLS_EVT_GLC_MEASUREMENT_NOTIFY_ENABLE;
		} else {
			// Disable Notification
			glc_notify_indicate_flag.GLC_measurement_notify_enable = 0;
			gls_upsteam_msg.msg_data.notify_indicate_index = GLS_EVT_GLC_MEASUREMENT_NOTIFY_DISABLE;
		}
	}
	break;
#if GLC_MEASUREMENT_CONTEXT_SUPPORT
	case GLS_CHAR_GLC_MEASUREMENT_CONTEXT_CCCD_INDEX: {
		if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY) {
			// Enable Notification
			glc_notify_indicate_flag.GLC_measurement_context_notify_enable = 1;
			gls_upsteam_msg.msg_data.notify_indicate_index = GLS_EVT_GLC_MEASUREMENT_CONTEXT_NOTIFY_ENABLE;
		} else {
			// Disable Notification
			glc_notify_indicate_flag.GLC_measurement_context_notify_enable = 0;
			gls_upsteam_msg.msg_data.notify_indicate_index = GLS_EVT_GLC_MEASUREMENT_CONTEXT_NOTIFY_DISABLE;
		}
	}
	break;
#endif
	case GLS_CHAR_GLC_RACP_CCCD_INDEX: {
		if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE) {
			// Enable Notification
			glc_notify_indicate_flag.GLC_RACP_indicate_enable = 1;
			gls_upsteam_msg.msg_data.notify_indicate_index = GLS_EVT_GLC_RACP_INDICATE_ENABLE;
		} else {
			// Disable Notification
			glc_notify_indicate_flag.GLC_RACP_indicate_enable = 0;
			gls_upsteam_msg.msg_data.notify_indicate_index = GLS_EVT_GLC_RACP_INDICATE_DISABLE;
		}
	}
	break;
	default: {
		handle = false;
		PROFILE_PRINT_ERROR1("gls_cccd_update_cb index = %d not found", index);
	}
	break;
	}
	/* Notify Application. */
	if (pfn_gls_cb && (handle == true)) {
		gls_upsteam_msg.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
		pfn_gls_cb(service_id, (void *)&gls_upsteam_msg);
	}
}

/**
  * @brief Simple Profile Service Callbacks.
  */
const T_FUN_GATT_SERVICE_CBS gls_cbs = {
	gls_attr_read_cb,  // Read callback function pointer
	gls_attr_write_cb, // Write callback function pointer
	gls_cccd_update_cb  // CCCD update callback function pointer
};

/**
  * @brief       Add gulcose service to the BLE stack database.
  *
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval Others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
     void profile_init()
     {
         server_init(1);
         gls_id = gls_add_service(app_handle_profile_message);
     }
  * \endcode
  */
T_SERVER_ID gls_add_service(void *p_func)
{
	T_SERVER_ID service_id;
	if (false == server_add_service(&service_id,
									(uint8_t *)gls_att_tbl,
									sizeof(gls_att_tbl),
									gls_cbs)) {
		PROFILE_PRINT_ERROR1("gls_add_service: ServiceId %d", service_id);
		service_id = 0xff;
		return service_id;
	}
	glc_racp.record_db.record_num = 0;
	glc_racp.record_db.head = 0;
	glc_racp.record_db.tail = 0;
	glc_racp.record_db.seq_num = GLC_RACP_INIT_SEQ_NBR_DEFAULT;
	pfn_gls_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
	return service_id;
}

/**
 * @brief       Abort RACP procedure by app.
 *
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        gls_abort_racp_procedure();
    }
 * \endcode
 */
void gls_abort_racp_procedure(void)
{
	if ((glc_racp.ctrl_point.op_code != GLC_RACP_OPCODE_RESERVED) && (gls_send_data_flag != 3)) {
		gls_abort_by_app_flag = true;
	}
}
