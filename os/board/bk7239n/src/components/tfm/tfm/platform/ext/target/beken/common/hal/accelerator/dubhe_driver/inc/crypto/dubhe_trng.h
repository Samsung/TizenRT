/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#ifndef DUBHE_TRNG_H
#define DUBHE_TRNG_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define DBH_TRNG_PARAM_INVALID -0x14000

#define DBH_TRNG_WORD_SIZE 0x4

#define DBH_TRNG_ADP_TEST_ERR_INTR  (0x00)
#define DBH_TRNG_REP_TEST_ERR_INTR  (0x01)
#define DBH_TRNG_CRNG_ERR_INTR      (0x02)
#define DBH_TRNG_VN_ERR_INTR        (0x03)
#define DBH_TRNG_AUTO_CORR_ERR_INTR (0x04)

typedef enum _dbh_trng_source {
    SRC_INTERNAL = 0,
    SRC_EXTERNAL = 1,
} DBH_TRNG_SOURCE;

typedef struct _dbh_trng_int {
    uint32_t vn_err:1;
    uint32_t crng_err:1;
    uint32_t rep_tst_err:1;
    uint32_t adap_tst_err:1;
    uint32_t rsvd:28;
} dbh_trng_int_t;

typedef struct _dbh_trng_entropy_src {
    uint32_t grp0_en:1;
    uint32_t grp1_en:1;
    uint32_t grp2_en:1;
    uint32_t grp3_en:1;
    uint32_t src_sel:1;
    uint32_t rsvd:27;
} dbh_trng_entropy_src_t;

typedef struct _dbh_trng_entropy_sample {
    uint32_t div:16;
    uint32_t dly:8;
    uint32_t rsvd:8;
} dbh_trng_entropy_sample_t;

typedef struct _dbh_trng_postproc {
    uint32_t prng_bypass:1;
    uint32_t vn_bypass:1;
    uint32_t crng_bypass:1;
    uint32_t rsvd3:8;
    uint32_t lfsr_drop_num:2;
    uint32_t lfsr_sel:1;
    uint32_t fast_mode:1;
    uint32_t rsvd15:17;
} dbh_trng_postproc_t;

typedef struct _dbh_trng_eval {
    uint32_t adap_tst_th:10;
    uint32_t rep_tst_th:6;
    uint32_t adap_tst_en:1;
    uint32_t rep_tst_en:1;
    uint32_t ac_tst_en:1;
    uint32_t ac_tst_th:4;
    uint32_t rsvd:9;
} dbh_trng_eval_t;

typedef struct _dbh_trng_ro {
    uint32_t grp0_taps_en:3;
    uint32_t rsvd3:1;
    uint32_t grp1_taps_en:3;
    uint32_t rsvd7:1;
    uint32_t grp2_taps_en:3;
    uint32_t rsvd11:1;
    uint32_t grp3_taps_en:3;
    uint32_t rsvd15:17;
} dbh_trng_ro_t;

typedef struct _dbh_trng_err_thr {
    uint32_t vn_err_th:4;
    uint32_t rsvd:4;
    uint32_t crng_err_th:8;
    uint32_t rep_err_th:8;
    uint32_t adap_err_th:8;
} dbh_trng_err_thr_t;

typedef struct _dbh_trng_config {
    dbh_trng_entropy_src_t src;
    dbh_trng_entropy_sample_t sample;
    dbh_trng_ro_t ro;
    dbh_trng_postproc_t postproc;
    dbh_trng_eval_t eval;
    dbh_trng_err_thr_t thr;
} dbh_trng_config_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * brief        Initialize Dubhe TRNG module
 *
 */
void arm_ce_trng_driver_init( void );

/**
 * brief        Dubhe TRNG seed read
 *
 */
int arm_ce_seed_read( unsigned char *buf, size_t buf_len );

/**
 * Collect TRNG output for characterization
 *
 * @regBase: Base address of the TRNG registers in the system memory map
 * @cfg: configuration for trng
 * @bulk_buf: Buffer for bulk data, size must equal or greater than bulk_size
 * @bulk_size: bulk_size for bulk data recommend 1024
 * @size: size of random data you want to dump measured in byte
 * @b_conf: weather need to config trng engine
 *
 * The function's output is 0 if the collection succeeds, or an error code
 */
int arm_ce_trng_calibration_dump( uint32_t regBase,
                                  dbh_trng_config_t *cfg,
                                  void *bulk_buf,
                                  size_t bulk_size,
                                  size_t size,
                                  bool b_conf );

#ifdef __cplusplus
}
#endif

#endif
/*************************** The End Of File*****************************/
