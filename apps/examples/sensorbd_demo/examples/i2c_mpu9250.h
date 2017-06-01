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
/****************************************************************************
 * examples/examples/sensorbd/examples/i2c_mpu9250.h
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __APPS_EXAMPLES_SENSORBD_EXAMPLES_I2C_MPU9250_H
#define __APPS_EXAMPLES_SENSORBD_EXAMPLES_I2C_MPU9250_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MPU9250_ADDR (0x68)

#define MPU9250_DEFAULT_ADDRESS         0xD1
#define MPU9250_ALT_DEFAULT_ADDRESS     0xD2

#define MPU9250_SELF_TEST_X_GYRO        0x00
#define MPU9250_SELF_TEST_Y_GYRO        0x01
#define MPU9250_SELF_TEST_Z_GYRO        0x02

#define MPU9250_SELF_TEST_X_ACCEL       0x0D
#define MPU9250_SELF_TEST_Y_ACCEL       0x0E
#define MPU9250_SELF_TEST_Z_ACCEL       0x0F

#define MPU9250_XG_OFFSET_H             0x13
#define MPU9250_XG_OFFSET_L             0x14
#define MPU9250_YG_OFFSET_H             0x15
#define MPU9250_YG_OFFSET_L             0x16
#define MPU9250_ZG_OFFSET_H             0x17
#define MPU9250_ZG_OFFSET_L             0x18
#define MPU9250_SMPLRT_DIV              0x19
#define MPU9250_CONFIG                  0x1A
#define MPU9250_GYRO_CONFIG             0x1B
#define MPU9250_ACCEL_CONFIG            0x1C
#define MPU9250_ACCEL_CONFIG2           0x1D
#define MPU9250_LP_ACCEL_ODR            0x1E
#define MPU9250_WOM_THR                 0x1F

#define MPU9250_FIFO_EN                 0x23
#define MPU9250_I2C_MST_CTRL            0x24
#define MPU9250_I2C_SLV0_ADDR           0x25
#define MPU9250_I2C_SLV0_REG            0x26
#define MPU9250_I2C_SLV0_CTRL           0x27
#define MPU9250_I2C_SLV1_ADDR           0x28
#define MPU9250_I2C_SLV1_REG            0x29
#define MPU9250_I2C_SLV1_CTRL           0x2A
#define MPU9250_I2C_SLV2_ADDR           0x2B
#define MPU9250_I2C_SLV2_REG            0x2C
#define MPU9250_I2C_SLV2_CTRL           0x2D
#define MPU9250_I2C_SLV3_ADDR           0x2E
#define MPU9250_I2C_SLV3_REG            0x2F
#define MPU9250_I2C_SLV3_CTRL           0x30
#define MPU9250_I2C_SLV4_ADDR           0x31
#define MPU9250_I2C_SLV4_REG            0x32
#define MPU9250_I2C_SLV4_DO             0x33
#define MPU9250_I2C_SLV4_CTRL           0x34
#define MPU9250_I2C_SLV4_DI             0x35
#define MPU9250_I2C_MST_STATUS          0x36
#define MPU9250_INT_PIN_CFG             0x37
#define MPU9250_INT_ENABLE              0x38

#define MPU9250_INT_STATUS              0x3A
#define MPU9250_ACCEL_XOUT_H            0x3B
#define MPU9250_ACCEL_XOUT_L            0x3C
#define MPU9250_ACCEL_YOUT_H            0x3D
#define MPU9250_ACCEL_YOUT_L            0x3E
#define MPU9250_ACCEL_ZOUT_H            0x3F
#define MPU9250_ACCEL_ZOUT_L            0x40
#define MPU9250_TEMP_OUT_H              0x41
#define MPU9250_TEMP_OUT_L              0x42
#define MPU9250_GYRO_XOUT_H             0x43
#define MPU9250_GYRO_XOUT_L             0x44
#define MPU9250_GYRO_YOUT_H             0x45
#define MPU9250_GYRO_YOUT_L             0x46
#define MPU9250_GYRO_ZOUT_H             0x47
#define MPU9250_GYRO_ZOUT_L             0x48
#define MPU9250_EXT_SENS_DATA_00        0x49
#define MPU9250_EXT_SENS_DATA_01        0x4A
#define MPU9250_EXT_SENS_DATA_02        0x4B
#define MPU9250_EXT_SENS_DATA_03        0x4C
#define MPU9250_EXT_SENS_DATA_04        0x4D
#define MPU9250_EXT_SENS_DATA_05        0x4E
#define MPU9250_EXT_SENS_DATA_06        0x4F
#define MPU9250_EXT_SENS_DATA_07        0x50
#define MPU9250_EXT_SENS_DATA_08        0x51
#define MPU9250_EXT_SENS_DATA_09        0x52
#define MPU9250_EXT_SENS_DATA_10        0x53
#define MPU9250_EXT_SENS_DATA_11        0x54
#define MPU9250_EXT_SENS_DATA_12        0x55
#define MPU9250_EXT_SENS_DATA_13        0x56
#define MPU9250_EXT_SENS_DATA_14        0x57
#define MPU9250_EXT_SENS_DATA_15        0x58
#define MPU9250_EXT_SENS_DATA_16        0x59
#define MPU9250_EXT_SENS_DATA_17        0x5A
#define MPU9250_EXT_SENS_DATA_18        0x5B
#define MPU9250_EXT_SENS_DATA_19        0x5C
#define MPU9250_EXT_SENS_DATA_20        0x5D
#define MPU9250_EXT_SENS_DATA_21        0x5E
#define MPU9250_EXT_SENS_DATA_22        0x5F
#define MPU9250_EXT_SENS_DATA_23        0x60

#define MPU9250_I2C_SLV0_DO             0x63
#define MPU9250_I2C_SLV1_DO             0x64
#define MPU9250_I2C_SLV2_DO             0x65
#define MPU9250_I2C_SLV3_DO             0x66
#define MPU9250_I2C_MST_DELAY_CTRL      0x67
#define MPU9250_SIGNAL_PATH_RESET       0x68
#define MPU9250_MOT_DETECT_CTRL         0x69
#define MPU9250_USER_CTRL               0x6A
#define MPU9250_PWR_MGMT_1              0x6B
#define MPU9250_PWR_MGMT_2              0x6C

#define MPU9250_FIFO_COUNTH             0x72
#define MPU9250_FIFO_COUNTL             0x73
#define MPU9250_FIFO_R_W                0x74
#define MPU9250_WHO_AM_I                0x75
#define MPU9250_XA_OFFSET_H             0x77
#define MPU9250_XA_OFFSET_L             0x78

#define MPU9250_YA_OFFSET_H             0x7A
#define MPU9250_YA_OFFSET_L             0x7B

#define MPU9250_ZA_OFFSET_H             0x7D
#define MPU9250_ZA_OFFSET_L             0x7E

/* reset values */

#define WHOAMI_RESET_VAL                0x71
#define POWER_MANAGMENT_1_RESET_VAL     0x01
#define DEFAULT_RESET_VALUE             0x00

#define WHOAMI_DEFAULT_VAL              0x68

/* CONFIG register masks */

#define MPU9250_FIFO_MODE_MASK          0x40
#define MPU9250_EXT_SYNC_SET_MASK       0x38
#define MPU9250_DLPF_CFG_MASK           0x07

/* GYRO_CONFIG register masks */

#define MPU9250_XGYRO_CTEN_MASK         0x80
#define MPU9250_YGYRO_CTEN_MASK         0x40
#define MPU9250_ZGYRO_CTEN_MASK         0x20
#define MPU9250_GYRO_FS_SEL_MASK        0x18
#define MPU9250_FCHOICE_B_MASK          0x03

#define MPU9250_GYRO_FULL_SCALE_250DPS  0
#define MPU9250_GYRO_FULL_SCALE_500DPS  1
#define MPU9250_GYRO_FULL_SCALE_1000DPS 2
#define MPU9250_GYRO_FULL_SCALE_2000DPS 3

/* ACCEL_CONFIG register masks */

#define MPU9250_AX_ST_EN_MASK           0x80
#define MPU9250_AY_ST_EN_MASK           0x40
#define MPU9250_AZ_ST_EN_MASK           0x20
#define MPU9250_ACCEL_FS_SEL_MASK       0x18

#define MPU9250_FULL_SCALE_2G           0
#define MPU9250_FULL_SCALE_4G           1
#define MPU9250_FULL_SCALE_8G           2
#define MPU9250_FULL_SCALE_16G          3

/* ACCEL_CONFIG_2 register masks */

#define MPU9250_ACCEL_FCHOICE_B_MASK    0xC0
#define MPU9250_A_DLPF_CFG_MASK         0x03

/* LP_ACCEL_ODR register masks */

#define MPU9250_LPOSC_CLKSEL_MASK       0x0F

/* FIFO_EN register masks */

#define MPU9250_TEMP_FIFO_EN_MASK       0x80
#define MPU9250_GYRO_XOUT_MASK          0x40
#define MPU9250_GYRO_YOUT_MASK          0x20
#define MPU9250_GYRO_ZOUT_MASK          0x10
#define MPU9250_ACCEL_MASK              0x08
#define MPU9250_SLV2_MASK               0x04
#define MPU9250_SLV1_MASK               0x02
#define MPU9250_SLV0_MASK               0x01

/* I2C_MST_CTRL register masks */

#define MPU9250_MULT_MST_EN_MASK        0x80
#define MPU9250_WAIT_FOR_ES_MASK        0x40
#define MPU9250_SLV_3_FIFO_EN_MASK      0x20
#define MPU9250_I2C_MST_P_NSR_MASK      0x10
#define MPU9250_I2C_MST_CLK_MASK        0x0F

/* I2C_SLV0_ADDR register masks */

#define MPU9250_I2C_SLV0_RNW_MASK       0x80
#define MPU9250_I2C_ID_0_MASK           0x7F

/* I2C_SLV0_CTRL register masks */

#define MPU9250_I2C_SLV0_EN_MASK        0x80
#define MPU9250_I2C_SLV0_BYTE_SW_MASK   0x40
#define MPU9250_I2C_SLV0_REG_DIS_MASK   0x20
#define MPU9250_I2C_SLV0_GRP_MASK       0x10
#define MPU9250_I2C_SLV0_LENG_MASK      0x0F

/* I2C_SLV1_ADDR register masks */

#define MPU9250_I2C_SLV1_RNW_MASK       0x80
#define MPU9250_I2C_ID_1_MASK           0x7F

/* I2C_SLV1_CTRL register masks */

#define MPU9250_I2C_SLV1_EN_MASK        0x80
#define MPU9250_I2C_SLV1_BYTE_SW_MASK   0x40
#define MPU9250_I2C_SLV1_REG_DIS_MASK   0x20
#define MPU9250_I2C_SLV1_GRP_MASK       0x10
#define MPU9250_I2C_SLV1_LENG_MASK      0x0F

/* I2C_SLV2_ADDR register masks */

#define MPU9250_I2C_SLV2_RNW_MASK       0x80
#define MPU9250_I2C_ID_2_MASK           0x7F

/* I2C_SLV2_CTRL register masks */

#define MPU9250_I2C_SLV2_EN_MASK        0x80
#define MPU9250_I2C_SLV2_BYTE_SW_MASK   0x40
#define MPU9250_I2C_SLV2_REG_DIS_MASK   0x20
#define MPU9250_I2C_SLV2_GRP_MASK       0x10
#define MPU9250_I2C_SLV2_LENG_MASK      0x0F

/* I2C_SLV3_ADDR register masks */

#define MPU9250_I2C_SLV3_RNW_MASK       0x80
#define MPU9250_I2C_ID_3_MASK           0x7F

/* I2C_SLV3_CTRL register masks */

#define MPU9250_I2C_SLV3_EN_MASK        0x80
#define MPU9250_I2C_SLV3_BYTE_SW_MASK   0x40
#define MPU9250_I2C_SLV3_REG_DIS_MASK   0x20
#define MPU9250_I2C_SLV3_GRP_MASK       0x10
#define MPU9250_I2C_SLV3_LENG_MASK      0x0F

/* I2C_SLV4_ADDR register masks */

#define MPU9250_I2C_SLV4_RNW_MASK       0x80
#define MPU9250_I2C_ID_4_MASK           0x7F

/* I2C_SLV4_CTRL register masks */

#define MPU9250_I2C_SLV4_EN_MASK        0x80
#define MPU9250_SLV4_DONE_INT_EN_MASK   0x40
#define MPU9250_I2C_SLV4_REG_DIS_MASK   0x20
#define MPU9250_I2C_MST_DLY_MASK        0x1F

/* I2C_MST_STATUS register masks */

#define MPU9250_PASS_THROUGH_MASK       0x80
#define MPU9250_I2C_SLV4_DONE_MASK      0x40
#define MPU9250_I2C_LOST_ARB_MASK       0x20
#define MPU9250_I2C_SLV4_NACK_MASK      0x10
#define MPU9250_I2C_SLV3_NACK_MASK      0x08
#define MPU9250_I2C_SLV2_NACK_MASK      0x04
#define MPU9250_I2C_SLV1_NACK_MASK      0x02
#define MPU9250_I2C_SLV0_NACK_MASK      0x01

/* INT_PIN_CFG register masks */

#define MPU9250_ACTL_MASK               0x80
#define MPU9250_OPEN_MASK               0x40
#define MPU9250_LATCH_INT_EN_MASK       0x20
#define MPU9250_INT_ANYRD_2CLEAR_MASK   0x10
#define MPU9250_ACTL_FSYNC_MASK         0x08
#define MPU9250_FSYNC_INT_MODE_EN_MASK  0x04
#define MPU9250_BYPASS_EN_MASK          0x02

/* INT_ENABLE register masks */

#define MPU9250_WOM_EN_MASK             0x40
#define MPU9250_FIFO_OFLOW_EN_MASK      0x10
#define MPU9250_FSYNC_INT_EN_MASK       0x08
#define MPU9250_RAW_RDY_EN_MASK         0x01

/* INT_STATUS register masks */

#define MPU9250_WOM_INT_MASK            0x40
#define MPU9250_FIFO_OFLOW_INT_MASK     0x10
#define MPU9250_FSYNC_INT_MASK          0x08
#define MPU9250_RAW_DATA_RDY_INT_MASK   0x01

/* I2C_MST_DELAY_CTRL register masks */

#define MPU9250_DELAY_ES_SHADOW_MASK    0x80
#define MPU9250_I2C_SLV4_DLY_EN_MASK    0x10
#define MPU9250_I2C_SLV3_DLY_EN_MASK    0x08
#define MPU9250_I2C_SLV2_DLY_EN_MASK    0x04
#define MPU9250_I2C_SLV1_DLY_EN_MASK    0x02
#define MPU9250_I2C_SLV0_DLY_EN_MASK    0x01

/* SIGNAL_PATH_RESET register masks */

#define MPU9250_GYRO_RST_MASK           0x04
#define MPU9250_ACCEL_RST_MASK          0x02
#define MPU9250_TEMP_RST_MASK           0x01

/* MOT_DETECT_CTRL register masks */

#define MPU9250_ACCEL_INTEL_EN_MASK     0x80
#define MPU9250_ACCEL_INTEL_MODE_MASK   0x40

/* USER_CTRL register masks */

#define MPU9250_FIFO_EN_MASK            0x40
#define MPU9250_I2C_MST_EN_MASK         0x20
#define MPU9250_I2C_IF_DIS_MASK         0x10
#define MPU9250_FIFO_RST_MASK           0x04
#define MPU9250_I2C_MST_RST_MASK        0x02
#define MPU9250_SIG_COND_RST_MASK       0x01

/* PWR_MGMT_1 register masks */

#define MPU9250_H_RESET_MASK            0x80
#define MPU9250_SLEEP_MASK              0x40
#define MPU9250_CYCLE_MASK              0x20
#define MPU9250_GYRO_STANDBY_CYCLE_MASK 0x10
#define MPU9250_PD_PTAT_MASK            0x08
#define MPU9250_CLKSEL_MASK             0x07

/* PWR_MGMT_2 register masks */

#define MPU9250_DISABLE_XA_MASK         0x20
#define MPU9250_DISABLE_YA_MASK         0x10
#define MPU9250_DISABLE_ZA_MASK         0x08
#define MPU9250_DISABLE_XG_MASK         0x04
#define MPU9250_DISABLE_YG_MASK         0x02
#define MPU9250_DISABLE_ZG_MASK         0x01

#define MPU9250_DISABLE_XYZA_MASK       0x38
#define MPU9250_DISABLE_XYZG_MASK       0x07

/* Magnetometer register maps */

#define MPU9250_MAG_ADDRESS             0x0C

#define MPU9250_MAG_WIA                 0x00
#define MPU9250_MAG_INFO                0x01
#define MPU9250_MAG_ST1                 0x02
#define MPU9250_MAG_XOUT_L              0x03
#define MPU9250_MAG_XOUT_H              0x04
#define MPU9250_MAG_YOUT_L              0x05
#define MPU9250_MAG_YOUT_H              0x06
#define MPU9250_MAG_ZOUT_L              0x07
#define MPU9250_MAG_ZOUT_H              0x08
#define MPU9250_MAG_ST2                 0x09
#define MPU9250_MAG_CNTL                0x0A
#define MPU9250_MAG_RSV                 0x0B	/* reserved mystery meat */
#define MPU9250_MAG_ASTC                0x0C
#define MPU9250_MAG_TS1                 0x0D
#define MPU9250_MAG_TS2                 0x0E
#define MPU9250_MAG_I2CDIS              0x0F
#define MPU9250_MAG_ASAX                0x10
#define MPU9250_MAG_ASAY                0x11
#define MPU9250_MAG_ASAZ                0x12

/* Magnetometer register masks */
#define MPU9250_WIA_MASK                0x48

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
int mpu9250_main(int argc, char *argv[]);

#endif							/* __APPS_EXAMPLES_SENSORBD_EXAMPLES_I2C_MPU9250_H */
