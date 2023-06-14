/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <osif.h>

#include <bt_api_config.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_gatts.h>
#include <rtk_service_config.h>
#include <rtk_gls.h>

#define GLUCOSE_SRV_UUID                    0x1808
#define GLC_MEASUREMENT_CHAR_UUID           0x2A18
#define GLC_MEASUREMENT_CONTEXT_CHAR_UUID   0x2A34
#define GLC_FEATURE_CHAR_UUID               0x2A51
#define GLC_RACP_CHAR_UUID                  0x2A52

#define RTK_BT_UUID_GLUCOSE_SRV                             BT_UUID_DECLARE_16(GLUCOSE_SRV_UUID)
#define RTK_BT_UUID_GLC_MEASUREMENT_CHAR                    BT_UUID_DECLARE_16(GLC_MEASUREMENT_CHAR_UUID)
#define RTK_BT_UUID_GLC_MEASUREMENT_CONTEXT_CHAR            BT_UUID_DECLARE_16(GLC_MEASUREMENT_CONTEXT_CHAR_UUID)
#define RTK_BT_UUID_GLC_FEATURE_CHAR                        BT_UUID_DECLARE_16(GLC_FEATURE_CHAR_UUID)
#define RTK_BT_UUID_GLC_RACP_CHAR                           BT_UUID_DECLARE_16(GLC_RACP_CHAR_UUID)

#define GLC_MEASUREMENT_CHAR_VAL_INDEX                      2   /* notify */
#define GLC_MEASUREMENT_CHAR_CCCD_INDEX                     3   /* cccd notify */
#define GLC_MEASUREMENT_CONTEXT_CHAR_VAL_INDEX              5   /* notify */
#define GLC_MEASUREMENT_CONTEXT_CHAR_CCCD_INDEX             6   /* cccd notify */
#define GLC_FEATURE_CHAR_VAL_INDEX                          8   /* read */
#define GLC_RACP_CHAR_VAL_INDEX                             10  /* indicate, write */
#define GLC_RACP_CHAR_CCCD_INDEX                            11  /* cccd indicate */

static rtk_bt_gatt_attr_t glucose_attrs[] = {
    /* Primary Service:  */
    RTK_BT_GATT_PRIMARY_SERVICE(RTK_BT_UUID_GLUCOSE_SRV),

    /* Characteristic:  */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_GLC_MEASUREMENT_CHAR,
                               RTK_BT_GATT_CHRC_NOTIFY,
                               RTK_BT_GATT_PERM_READ),
    RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),

    /* Characteristic:  */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_GLC_MEASUREMENT_CONTEXT_CHAR,
                               RTK_BT_GATT_CHRC_NOTIFY,
                               RTK_BT_GATT_PERM_READ),
    RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_GLC_FEATURE_CHAR,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_GLC_RACP_CHAR,
                               RTK_BT_GATT_CHRC_INDICATE | RTK_BT_GATT_PERM_WRITE,
                               RTK_BT_GATT_PERM_READ),
    RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),
};

static struct rtk_bt_gatt_service glucose_srv = RTK_BT_GATT_SERVICE(glucose_attrs,GLUCOSE_SRV_ID);

static uint8_t glc_msmt_cccd_ntf_en_map[RTK_BLE_GAP_MAX_LINKS] = {0};
static uint8_t glc_msmt_ctxt_cccd_ntf_en_map[RTK_BLE_GAP_MAX_LINKS] = {0};
static uint8_t racp_cccd_ind_en_map[RTK_BLE_GAP_MAX_LINKS] = {0};   

/**  Value of glucose feature characteristic. */
static uint16_t  features;

/** glucose record access control point */
T_GLC_RACP glc_racp;
T_PATIENT_RECORD *p_new_record;

/** parameters used for splitting report records procedure */
uint16_t gls_num_records_to_report = 0;
uint16_t gls_current_record_to_report = 0;
uint16_t gls_report_offset = 0;
uint8_t gls_send_data_flag = 0;
/** Flag used for aborting procedure */
bool gls_abort_flag = false;
bool gls_abort_by_app_flag = false;


void gls_prepare_new_record(void)
{
    printf("gls_prepare_new_record database head: %d, tail: %d, num: %d, seq_num: %d\n",
                        glc_racp.record_db.head, glc_racp.record_db.tail, glc_racp.record_db.record_num,
                        glc_racp.record_db.seq_num);
    p_new_record = &(glc_racp.record_db.records[glc_racp.record_db.tail]);
    memset(p_new_record, 0, sizeof(*p_new_record));
    glc_racp.record_db.seq_num++;
    p_new_record->glc_measurement_value.seq_num = glc_racp.record_db.seq_num;
    p_new_record->glc_measurement_context.seq_num = glc_racp.record_db.seq_num;
}

void gls_push_new_record(void)
{
    if (((glc_racp.record_db.head - glc_racp.record_db.tail + GLC_RACP_DATABASE_SIZE) %
         GLC_RACP_DATABASE_SIZE) == 1) {
        glc_racp.record_db.head = (glc_racp.record_db.head + 1) % GLC_RACP_DATABASE_SIZE;
        glc_racp.record_db.tail = (glc_racp.record_db.tail + 1) % GLC_RACP_DATABASE_SIZE;
    } else {
        glc_racp.record_db.tail = (glc_racp.record_db.tail + 1) % GLC_RACP_DATABASE_SIZE;
        glc_racp.record_db.record_num++;
    }
    printf("gls_push_new_record database head: %d, tail: %d, num: %d, seq_num: %d\n",
                        glc_racp.record_db.head, glc_racp.record_db.tail, glc_racp.record_db.record_num,
                        glc_racp.record_db.seq_num);
}


uint16_t glc_msmt_val_regroup(uint8_t *dst, T_GLC_MEASUREMENT_VALUE *src)
{
    uint16_t actual_len = 0;

    memcpy(&dst[actual_len], &src->flags, 1);
    actual_len += 1;
    memcpy(&dst[actual_len], &src->seq_num, sizeof(uint16_t));
    actual_len += sizeof(uint16_t);
    memcpy(&dst[actual_len], src->base_time, sizeof(TIMESTAMP));
    actual_len += sizeof(TIMESTAMP);

    if (src->flags.time_offset) {
        memcpy(&dst[actual_len], &src->time_offset, sizeof(uint16_t));
        actual_len += sizeof(uint16_t);
    }
    if (src->flags.con_ts_loc) {
        memcpy(&dst[actual_len], src->concentration, 2);
        actual_len += 2;
        memcpy(&dst[actual_len], &src->type_sample_location, 1);
        actual_len += 1;
    }
    if (src->flags.ss_annunciation) {
        memcpy(&dst[actual_len], &src->ss_annunciation, 2);
        actual_len += 2;
    }

    return actual_len;
}


uint16_t glc_msmt_ctxt_val_regroup(uint8_t *dst, T_GLC_MEASUREMENT_CONTEXT *src)
{
    uint16_t actual_len = 0;

    memcpy(&dst[actual_len], &src->flags, 1);
    actual_len += 1;
    memcpy(&dst[actual_len], &src->seq_num, 2);
    actual_len += 2;
    if (src->flags.ext_flags) {
        memcpy(&dst[actual_len], &src->ext_flags, 1);
        actual_len += 1;
    }
    if (src->flags.carbohydrate) {
        memcpy(&dst[actual_len], &src->carbohydrate_ID, 1);
        actual_len += 1;
        memcpy(&dst[actual_len], src->carbohydrate, 2);
        actual_len += 2;
    }
    if (src->flags.meal) {
        memcpy(&dst[actual_len], &src->meal, 1);
        actual_len += 1;
    }
    if (src->flags.tester_health) {
        memcpy(&dst[actual_len], &src->tester_health, 1);
        actual_len += 1;
    }
    if (src->flags.exercise) {
        memcpy(&dst[actual_len], &src->exercise_duration, 2);
        actual_len += 2;
        memcpy(&dst[actual_len], &src->exercise_intensity, 1);
        actual_len += 1;
    }
    if (src->flags.medication) {
        memcpy(&dst[actual_len], &src->medication_ID, 1);
        actual_len += 1;
        memcpy(&dst[actual_len], src->medication, 2);
        actual_len += 2;
    }
    if (src->flags.hb_a1c) {
        memcpy(&dst[actual_len], src->hb_a1c, 2);
        actual_len += 2;
    }

    return actual_len;
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
            }
        }
        return 0;
    }
}


bool gls_abort_success_response(uint16_t conn_handle)
{
    glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_ABORT_OPERATION;

    printf("gls_abort_success_response gls racp procedure has been aborted!");

    return gls_racp_response(conn_handle, GLC_RACP_RESP_SUCCESS);
}

T_GLC_CTRL_POINT_RESP_CODES gls_racp_check(void)
{
    T_GLC_CTRL_POINT_RESP_CODES ret = GLC_RACP_RESP_SUCCESS;

    printf("gls_racp_check glucose racp: opcode = %d, operator = %d, length = %d \n",
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
        printf("gls_find_records glucose find records: num = %d, start = %d, end = %d\n",
                            num_of_records, find1, find2);
    }
    return ret;
}

bool gls_report_records_task(uint16_t conn_handle)
{
    bool ret = true;
    if (gls_abort_flag == true) {
        printf("gls_report_records_task  Glucose current record = %d, total = %d, procedure abort successfully!\n",
                            gls_current_record_to_report, gls_num_records_to_report);
        gls_current_record_to_report = gls_num_records_to_report; // stop transmitting any data
        ret = gls_abort_success_response(conn_handle);
        gls_abort_flag = false;
        gls_abort_by_app_flag = false; // Abort procedure first, prior to abort by application
        return ret;
    }

    if (gls_abort_by_app_flag == true) {
        printf("gls_report_records_task  Glucose current record = %d, total = %d, procedure abort by app successfully!\n",
                            gls_current_record_to_report, gls_num_records_to_report);
        gls_current_record_to_report = gls_num_records_to_report; // stop transmitting any data
        ret = gls_racp_response(conn_handle, GLC_RACP_RESP_PROC_NOT_COMPLETED);
        gls_abort_by_app_flag = false;
        return ret;
    }

    printf("gls_report_records_task  Glucose report records, current = %d, total = %d, gls_send_data_flag = %d\n",
                        gls_current_record_to_report, gls_num_records_to_report, gls_send_data_flag);

    if (gls_send_data_flag == 1) {
#if GLC_MEASUREMENT_CONTEXT_SUPPORT
        int current = (glc_racp.record_db.head + gls_report_offset + gls_current_record_to_report) %
                      GLC_RACP_DATABASE_SIZE;
        if (glc_racp.record_db.records[current].glc_measurement_value.flags.ctxt_info_follows == 1) {
            ret = gls_glc_measurement_context_notify(conn_handle,
                    gls_report_offset + gls_current_record_to_report);
            gls_current_record_to_report += 1;
            return ret;
        }
#else
        gls_current_record_to_report += 1;
#endif
    }

    if (gls_current_record_to_report < gls_num_records_to_report) {
        ret = gls_glc_measurement_notify(conn_handle,
                                         gls_report_offset + gls_current_record_to_report);
    } else {
        if (gls_send_data_flag != 3) {
            ret = gls_racp_response(conn_handle, GLC_RACP_RESP_SUCCESS);
        } else { // clear control point
            gls_set_parameter(GLS_PARAM_CTL_PNT_PROG_CLR, 0, NULL);
        }
    }
    return ret;
}

void gls_report_records(uint16_t conn_handle)
{
    T_GLC_CTRL_POINT_RESP_CODES ret;
    uint16_t num_of_records;
    int find1, find2;
    ret = gls_find_records(&num_of_records, &find1, &find2);
    if (ret == GLC_RACP_RESP_SUCCESS) {
        if (num_of_records == 0) {
            ret = GLC_RACP_RESP_NO_RECS_FOUND;
            gls_racp_response(conn_handle, ret);
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
            if (false == gls_report_records_task(conn_handle)) {
                gls_set_parameter(GLS_PARAM_CTL_PNT_PROG_CLR, 0, NULL);
            }
        }
    } else {
        gls_racp_response(conn_handle, ret);
    }
}

void gls_delete_records(uint16_t conn_handle)
{
    T_GLC_CTRL_POINT_RESP_CODES ret;
    uint16_t num_of_records;
    int find1, find2;
    ret = gls_find_records(&num_of_records, &find1, &find2);

    printf("gls_delete_records glucose delete records: num = %d, start = %d, end = %d\n",
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
    gls_racp_response(conn_handle, ret);
}


bool gls_glc_measurement_notify(uint16_t conn_handle, uint8_t index)
{
    int current = (glc_racp.record_db.head + index) % GLC_RACP_DATABASE_SIZE;
    uint16_t actual_len = 0;
    gls_send_data_flag = 1;
    rtk_bt_gatts_ntf_and_ind_param_t ntf_param = {0};
    T_GLC_MEASUREMENT_VALUE *pmeasurement_value =
        &glc_racp.record_db.records[current].glc_measurement_value;
    uint8_t temp_glc_measurement[sizeof(T_GLC_MEASUREMENT_VALUE)];

    if (index >= glc_racp.record_db.record_num) {
        printf("gls_glc_measurement_notify glucose measurement value access overflow!");
        return false;
    }
    if (current == glc_racp.record_db.tail) {
        printf("gls_glc_measurement_notify glucose measurement database: empty!");
        return false;
    }

    actual_len = glc_msmt_val_regroup(temp_glc_measurement, pmeasurement_value);
    ntf_param.app_id = GLUCOSE_SRV_ID;
    ntf_param.conn_handle = conn_handle;
    ntf_param.data = temp_glc_measurement;
    ntf_param.len = actual_len;
    ntf_param.index = GLC_MEASUREMENT_CHAR_VAL_INDEX;

    rtk_bt_gatts_notify(&ntf_param);
    return true;
}


bool gls_glc_measurement_context_notify(uint16_t conn_handle, uint8_t index)
{
    int current = (glc_racp.record_db.head + index) % GLC_RACP_DATABASE_SIZE;
    rtk_bt_gatts_ntf_and_ind_param_t ntf_param = {0};
    uint16_t actual_len = 0;
    gls_send_data_flag = 2;
    T_GLC_MEASUREMENT_CONTEXT *pmeasurement_context =
                                &glc_racp.record_db.records[current].glc_measurement_context;
    uint8_t temp_glc_measurement_ctxt[sizeof(T_GLC_MEASUREMENT_CONTEXT)];

    if (index >= glc_racp.record_db.record_num) {
        printf("gls_glc_measurement_context_notify glucose measurement context access overflow!");
        return false;
    }
    if (current == glc_racp.record_db.tail) {
        printf("gls_glc_measurement_context_notify glucose measurement database: Empty!");
        return false;
    }

    actual_len = glc_msmt_ctxt_val_regroup(temp_glc_measurement_ctxt, pmeasurement_context);
    ntf_param.app_id = GLUCOSE_SRV_ID;
    ntf_param.conn_handle = conn_handle;
    ntf_param.data = temp_glc_measurement_ctxt;
    ntf_param.len = actual_len;
    ntf_param.index = GLC_MEASUREMENT_CONTEXT_CHAR_VAL_INDEX;

    rtk_bt_gatts_notify(&ntf_param);
    return true;
}

bool gls_racp_response(uint16_t conn_handle, uint8_t rsp_code)
{
    bool ret = true;
    rtk_bt_gatts_ntf_and_ind_param_t ind_param = {0};
    gls_send_data_flag = 3;

    uint16_t attrib_index = GLC_RACP_CHAR_VAL_INDEX;
    glc_racp.ctrl_point.operand[0] = glc_racp.ctrl_point.op_code;
    glc_racp.ctrl_point.operand[1] = rsp_code;
    glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_RESP_CODE;
    glc_racp.ctrl_point.op = GLC_RACP_OPERATOR_NULL;

    glc_racp.cp_length = sizeof(T_GLC_CTRL_POINT_OPCODE) + sizeof(T_GLC_CTRL_POINT_OPERATOR) + sizeof(
                             T_GLC_CTRL_POINT_OPCODE) + sizeof(T_GLC_CTRL_POINT_RESP_CODES);

    ind_param.app_id = GLUCOSE_SRV_ID;
    ind_param.conn_handle = conn_handle;
    ind_param.index = attrib_index;
    ind_param.data = (uint8_t *) &glc_racp.ctrl_point;
    ind_param.len = glc_racp.cp_length;
    rtk_bt_gatts_indicate(&ind_param);

    printf("gls_racp_response  glucose racp resp: %d \n", rsp_code);

    glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_RESERVED;

    return ret;
}


bool gls_racp_num_response(uint16_t conn_handle, uint16_t num)
{
    bool ret = true;
    rtk_bt_gatts_ntf_and_ind_param_t ind_param = {0};
    gls_send_data_flag = 3;

    uint16_t attrib_index = GLC_RACP_CHAR_VAL_INDEX;
    LE_UINT16_TO_ARRAY(glc_racp.ctrl_point.operand, num);
    glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_NBR_OF_RECS_RESP;
    glc_racp.ctrl_point.op = GLC_RACP_OPERATOR_NULL;

    glc_racp.cp_length = sizeof(T_GLC_CTRL_POINT_OPCODE) + sizeof(T_GLC_CTRL_POINT_OPERATOR) + sizeof(
                             uint16_t);

    ind_param.app_id = GLUCOSE_SRV_ID;
    ind_param.conn_handle = conn_handle;
    ind_param.index = attrib_index;
    ind_param.data = (uint8_t *) &glc_racp.ctrl_point;
    ind_param.len = glc_racp.cp_length;
    rtk_bt_gatts_indicate(&ind_param);

    printf("gls_racp_num_response glucose racp num response: %d \n", num);

    glc_racp.ctrl_point.op_code = GLC_RACP_OPCODE_RESERVED;

    return ret;
}


bool gls_check_cccd(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return false;

    if (0 == glc_msmt_cccd_ntf_en_map[conn_id]) {
        return false;
    } else if (0 == glc_msmt_ctxt_cccd_ntf_en_map[conn_id]) {
        return false;
    } else if (0 == racp_cccd_ind_en_map[conn_id]) {
        return false;
    } else {
        return true;
    }
}

void gls_report_num_of_records(uint16_t conn_handle)
{
    T_GLC_CTRL_POINT_RESP_CODES ret;
    uint16_t num_of_records;
    int find1, find2;
    ret = gls_find_records(&num_of_records, &find1, &find2);
    if (ret != GLC_RACP_RESP_SUCCESS) {
        gls_racp_response(conn_handle, ret);
    } else {
        gls_racp_num_response(conn_handle, num_of_records);
    }
}

static void gls_ctl_pnt_write_ind_post_proc(uint16_t conn_handle,
                uint16_t attrib_index, uint16_t write_length, uint8_t *p_value)
{
	(void)attrib_index;
	
    T_GLC_CONTROL_POINT *ctrl_pnt = (T_GLC_CONTROL_POINT *)p_value;

    if (write_length > sizeof(T_GLC_CONTROL_POINT)) {
        return;
    }
    if (ctrl_pnt->op_code != GLC_RACP_OPCODE_ABORT_OPERATION) {
        memset(&glc_racp.ctrl_point, 0, sizeof(T_GLC_CONTROL_POINT));
        memcpy(&glc_racp.ctrl_point, ctrl_pnt, write_length);
        glc_racp.cp_length = write_length;

        switch (glc_racp.ctrl_point.op_code) {
        case GLC_RACP_OPCODE_REPORT_NBR_OF_RECS:
            gls_report_num_of_records(conn_handle);
            break;
        case GLC_RACP_OPCODE_REPORT_RECS:
            gls_report_records(conn_handle);
            break;
        case GLC_RACP_OPCODE_DELETE_RECS:
            gls_delete_records(conn_handle);
            break;
        default:
            gls_racp_response(conn_handle, GLC_RACP_RESP_OPCODE_NOT_SUPPORTED);
            break;
        }
    } else {
        if ((glc_racp.ctrl_point.op_code != GLC_RACP_OPCODE_RESERVED) && (gls_send_data_flag != 3)) {
            gls_abort_flag = true;
            //while (gls_abort_flag == TRUE) ; /* halt until AbortFlag is disabled when procedure in progress has been aborted */
        } else {
            gls_abort_success_response(conn_handle);
        }
    }
}

void gls_read_hdl(void *data)
{
    int i;
    uint16_t ret = 0;
    rtk_bt_gatts_read_ind_t *p_read_ind = (rtk_bt_gatts_read_ind_t*)data;
    rtk_bt_gatts_read_resp_param_t read_resp = {0};

    read_resp.app_id = p_read_ind->app_id;
    read_resp.conn_handle = p_read_ind->conn_handle;
    read_resp.cid = p_read_ind->cid;
    read_resp.index = p_read_ind->index;
    read_resp.err_code = 0;
    read_resp.seq = 0;
    
    if (GLC_FEATURE_CHAR_VAL_INDEX == p_read_ind->index) {
        read_resp.data = (uint8_t *) &features;
        read_resp.len = sizeof(features);
    } else {
        read_resp.err_code = 0x03;
        printf("[APP] GLS read event unknown index: %d\r\n", p_read_ind->index);
    }

    for (i = 0; i < 3; i++) {
        ret = rtk_bt_gatts_read_resp(&read_resp);
        if (RTK_BT_OK == ret) {
            printf("[APP] GLS response for client read succeed!\r\n");
            break;
        }
        osif_delay(50);
    }
    if (i >= 3) {
        printf("[APP] GLS response for client read failed, err: 0x%x\r\n", ret);
    }
}


void gls_write_hdl(void *data)
{
    int i;
    uint16_t ret = 0;
    rtk_bt_gatts_write_ind_t* p_write_ind = (rtk_bt_gatts_write_ind_t*)data;
    rtk_bt_gatts_write_resp_param_t write_resp = {0};
    write_resp.app_id = p_write_ind->app_id;
    write_resp.conn_handle = p_write_ind->conn_handle;
    write_resp.cid = p_write_ind->cid;
    write_resp.index = p_write_ind->index;
    write_resp.type = p_write_ind->type;

    if (GLC_RACP_CHAR_VAL_INDEX == p_write_ind->index) {
        if ((p_write_ind->len > sizeof(T_GLC_CONTROL_POINT)) || (p_write_ind->value == NULL)) {
            write_resp.err_code = 0x01;
        } else if (GLC_RACP_OPERATION_ACTIVE(glc_racp.ctrl_point.op_code) &&
                        (p_write_ind->value[0] != GLC_RACP_OPCODE_ABORT_OPERATION)) {
            printf("GLC_ERR_PROC_ALREADY_IN_PROGRESS\r\n");
            write_resp.err_code = 0x01;
        } else if (!gls_check_cccd(p_write_ind->conn_handle)) {
            write_resp.err_code = 0x01;
        } else {
            gls_ctl_pnt_write_ind_post_proc(p_write_ind->conn_handle, p_write_ind->index,
                                            p_write_ind->len, p_write_ind->value);
        }
    } else {
        write_resp.err_code = 0x01;
    }

    for (i = 0; i < 3; i++) {
        ret = rtk_bt_gatts_write_resp(&write_resp);
        if (RTK_BT_OK == ret) {
            printf("[APP] GLS response for client write succeed, index:%d\r\n", p_write_ind->index);
            break;
        }
        osif_delay(50);
    }
    if (i >= 3)
        printf("[APP] GLS response for client write failed, err: 0x%x\r\n", ret);
}


void gls_cccd_update_hdl(void *data)
{
    rtk_bt_gatts_cccd_ind_t* p_cccd_ind = (rtk_bt_gatts_cccd_ind_t*)data;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(p_cccd_ind->conn_handle, &conn_id) != RTK_BT_OK)
		return;

    if (GLC_MEASUREMENT_CHAR_CCCD_INDEX == p_cccd_ind->index) {
        if (p_cccd_ind->value & RTK_BT_GATT_CCC_NOTIFY) {
            glc_msmt_cccd_ntf_en_map[conn_id] = 1;
            printf("[APP] GLS glc measurement cccd, notify bit enable\r\n");
        } else {
            glc_msmt_cccd_ntf_en_map[conn_id] = 0;
            printf("[APP] GLS glc measurement cccd, notify bit disable\r\n");
        }
    } else if (GLC_MEASUREMENT_CHAR_CCCD_INDEX == p_cccd_ind->index) {
        if (p_cccd_ind->value & RTK_BT_GATT_CCC_NOTIFY) {
            glc_msmt_ctxt_cccd_ntf_en_map[conn_id] = 1;
            printf("[APP] GLS glc measurement context cccd, notify bit enable\r\n");
        } else {
            glc_msmt_ctxt_cccd_ntf_en_map[conn_id] = 0;
            printf("[APP] GLS glc measurement context cccd, notify bit disable\r\n");
        }
    } else if (GLC_RACP_CHAR_CCCD_INDEX == p_cccd_ind->index) {
        if (p_cccd_ind->value & RTK_BT_GATT_CCC_INDICATE) {
            glc_msmt_ctxt_cccd_ntf_en_map[conn_id] = 1;
            printf("[APP] GLS record access control point cccd, indicate bit enable\r\n");
        } else {
            glc_msmt_ctxt_cccd_ntf_en_map[conn_id] = 0;
            printf("[APP] GLS record access control point cccd, indicate bit disable\r\n");
        }
    } else {
        printf("[APP] GLS CCCD event unknown index: %d\r\n", p_cccd_ind->index);
    }
}

void glucose_srv_callback(uint8_t event, void *data)
{
    switch (event) {
        case RTK_BT_GATTS_EVT_REGISTER_SERVICE: {
            rtk_bt_gatts_reg_ind_t* reg_srv_res = (rtk_bt_gatts_reg_ind_t*)data;
            if (RTK_BT_OK == reg_srv_res->reg_status) {
                printf("[APP] GLS register service succeed!\r\n");
            } else {
                printf("[APP] GLS register service failed, err: 0x%x\r\n", reg_srv_res->reg_status);
            }
            
            break;
        }
        case RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND: {
            rtk_bt_gatts_ntf_and_ind_ind_t* p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
            if (RTK_BT_OK == p_ind->err_code) {
                printf("[APP] GLS indicate succeed!\r\n");
            } else {
                printf("[APP] GLS indicate failed, err: 0x%x \r\n", p_ind->err_code);
            }
            break;
        }

        case RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND:{
            rtk_bt_gatts_ntf_and_ind_ind_t* p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
            if (RTK_BT_OK == p_ind->err_code) {
                printf("[APP] GLS notify succeed!\r\n");
            } else {
                printf("[APP] GLS notify failed, err: 0x%x \r\n", p_ind->err_code);
            }
            break;
        }
        
        case RTK_BT_GATTS_EVT_READ_IND: {
            gls_read_hdl(data);
            break;
        }

        case RTK_BT_GATTS_EVT_WRITE_IND:
            gls_write_hdl(data);
            break;

        case RTK_BT_GATTS_EVT_CCCD_IND:
            gls_cccd_update_hdl(data);
            break;
        default:
            break;
    }
}

uint16_t gls_set_parameter(T_GLS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    uint16_t ret = RTK_BT_OK;

    switch (param_type) {

    case GLS_PARAM_GLC_FEATURES:
        if (len == 2) {
            LE_ARRAY_TO_UINT16(features, (uint8_t *) p_value);
        } else {
            ret = RTK_BT_FAIL;
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
        if (1 == len) {
            memcpy(&(p_new_record->glc_measurement_value.flags), p_value, 1);
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_BASE_TIME:
        if (sizeof(TIMESTAMP) == len) {
            memcpy(&p_new_record->glc_measurement_value.base_time, p_value, len);
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_TIME_OFFSET:
        if (2 == len) {
            p_new_record->glc_measurement_value.time_offset = *(uint16_t *)p_value;
            p_new_record->glc_measurement_value.flags.time_offset = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CONCENTRATION:
        if (sizeof(SFLOAT) == len) {
            memcpy(&p_new_record->glc_measurement_value.concentration, p_value, len);
            p_new_record->glc_measurement_value.flags.con_ts_loc = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CONCENTRATION_UNITS:
        if (1 == len) {
            p_new_record->glc_measurement_value.flags.con_units = *(uint8_t *)p_value;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_TYPE_SAMPLE_LOCATION:
        if (1 == len) {
            p_new_record->glc_measurement_value.type_sample_location = *(uint8_t *)p_value;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_SENSOR_STATUS_ANNUNCIATION:
        if (2 == len) {
            memcpy(&(p_new_record->glc_measurement_value.ss_annunciation), p_value, 2);
            p_new_record->glc_measurement_value.flags.ss_annunciation = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_FLAG:
        if (1 == len) {
            p_new_record->glc_measurement_context.flags =  *(T_GLC_MSR_CTXT_FLAG *)p_value;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_CARBOHYDRATE_ID:
        if (1 == len) {
            p_new_record->glc_measurement_context.carbohydrate_ID = *(uint8_t *)p_value;
            p_new_record->glc_measurement_context.flags.carbohydrate = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_CARBOHYDRATE:
        if (sizeof(SFLOAT) == len) {
            memcpy(&p_new_record->glc_measurement_context.carbohydrate, p_value, len);
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_MEAL:
        if (1 == len) {
            p_new_record->glc_measurement_context.meal = *(uint8_t *)p_value;
            p_new_record->glc_measurement_context.flags.meal = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_TESTER_HEALTH:
        if (1 == len) {
            p_new_record->glc_measurement_context.tester_health = *(uint8_t *)p_value;
            p_new_record->glc_measurement_context.flags.tester_health = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_EXERCISE_DURATION:
        if (2 == len) {
            p_new_record->glc_measurement_context.exercise_duration = *(uint16_t *)p_value;
            p_new_record->glc_measurement_context.flags.exercise = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_EXERCISE_INTENSITY:
        if (1 == len) {
            p_new_record->glc_measurement_context.exercise_intensity = *(uint8_t *)p_value;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_MEDICATION_ID:
        if (1 == len) {
            p_new_record->glc_measurement_context.medication_ID = *(uint8_t *)p_value;
            p_new_record->glc_measurement_context.flags.exercise = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_MEDICATION:
        if (sizeof(SFLOAT) == len) {
            memcpy(&p_new_record->glc_measurement_context.medication, p_value, len);
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_MEDICATION_UNITS:
        if (1 == len) {
            p_new_record->glc_measurement_context.flags.medication_units = *(uint8_t *)p_value;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    case GLS_PARAM_GLC_MS_CT_HbA1c:
        if (sizeof(SFLOAT) == len) {
            memcpy(&p_new_record->glc_measurement_context.hb_a1c, p_value, len);
            p_new_record->glc_measurement_context.flags.hb_a1c = 1;
        } else {
            ret = RTK_BT_FAIL;
        }
        break;

    default:
        ret = RTK_BT_FAIL;
        break;
    }

    return ret;
}


uint16_t gls_get_parameter(T_GLS_PARAM_TYPE param_type, uint8_t *len, void *p_value)
{
    uint16_t ret = RTK_BT_OK;

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
        ret = RTK_BT_FAIL;
        break;
    }

    return ret;
}


uint16_t glucose_srv_add(void)
{
    glucose_srv.type = GATT_SERVICE_OVER_BLE;
    glucose_srv.server_info = 0;
    glucose_srv.user_data = NULL;
    glucose_srv.register_status = 0;

    return rtk_bt_gatts_register_service(&glucose_srv);
}

void gls_disconnect(uint16_t conn_handle) 
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

    glc_msmt_cccd_ntf_en_map[conn_id] = 0;
    glc_msmt_ctxt_cccd_ntf_en_map[conn_id] = 0;
    racp_cccd_ind_en_map[conn_id] = 0;
}

void gls_status_deinit(void)
{
    memset(glc_msmt_cccd_ntf_en_map, 0, sizeof(glc_msmt_cccd_ntf_en_map));
    memset(glc_msmt_ctxt_cccd_ntf_en_map, 0, sizeof(glc_msmt_ctxt_cccd_ntf_en_map));
    memset(racp_cccd_ind_en_map, 0, sizeof(racp_cccd_ind_en_map));
}


