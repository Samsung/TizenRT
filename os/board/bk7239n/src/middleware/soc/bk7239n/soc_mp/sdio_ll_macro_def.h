// Copyright 2020-2025 Beken 
// 
// Licensed under the Apache License, Version 2.0 (the "License");           
// you may not use this file except in compliance with the License.            
// You may obtain a copy of the License at                                     
//                                                                             
//     http://www.apache.org/licenses/LICENSE-2.0                              
//                                                                             
// Unless required by applicable law or agreed to in writing, software         
// distributed under the License is distributed on an "AS IS" BASIS,         
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    
// See the License for the specific language governing permissions and         
// limitations under the License.                                              

/***********************************************************************************************************************************
* This file is generated from BK7256_ADDR Mapping_20211224_format_change_highlight_20220113_update.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7256,GENARATE TIME:2022-04-02 18:47:01                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

static inline void sdio_ll_set_sd_soft_resetn(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X2_ADDR);
    reg_value &= ~(SDIO_SD_SOFT_RESETN_MASK << SDIO_SD_SOFT_RESETN_POS);
    reg_value |= ((value & SDIO_SD_SOFT_RESETN_MASK) << SDIO_SD_SOFT_RESETN_POS);
    REG_WRITE(SDIO_REG0X2_ADDR,reg_value);
}

static inline uint32_t sdio_ll_get_reg0x4_value(void)
{
    return REG_READ(SDIO_REG0X4_ADDR);
}

static inline void sdio_ll_set_reg0x4_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X4_ADDR,value);
}

/* REG_0x00:reg0x0->SD_CMD_START:0x0[0],sd host(Slave) start send command; Host:If SW set it to 1, sdio auto clear this bit and then start send command.; Slave:SW set it to 1, sdio will receive and response command if host send command, then sdio clear this bit; 0:If this bit is 0, sdio will no reponse command to host and reject command from host side.,0x0,RW/AutoC*/
static inline uint32_t sdio_ll_get_sd_cmd_start(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_START_POS) & SDIO_SD_CMD_START_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_start(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value &= ~(SDIO_SD_CMD_START_MASK << SDIO_SD_CMD_START_POS);
    reg_value |= ((value & SDIO_SD_CMD_START_MASK) << SDIO_SD_CMD_START_POS);
    REG_WRITE(SDIO_REG0X4_ADDR,reg_value);
}

/* REG_0x00:reg0x0->SD_CMD_RSP:0x0[1],Host Only; sd host need slave respond the command or not.; 1: need respond; 0: not need respond,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_cmd_rsp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_RSP_POS) & SDIO_SD_CMD_RSP_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_rsp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value &= ~(SDIO_SD_CMD_RSP_MASK << SDIO_SD_CMD_RSP_POS);
    reg_value |= ((value & SDIO_SD_CMD_RSP_MASK) << SDIO_SD_CMD_RSP_POS);
    REG_WRITE(SDIO_REG0X4_ADDR,reg_value);
}

/* REG_0x00:reg0x0->SD_CMD_LONG:0x0[2],Host Only; sd host need slave respond a long command or short 1: need long response ; 0: need short response,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_cmd_long(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_LONG_POS) & SDIO_SD_CMD_LONG_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_long(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value &= ~(SDIO_SD_CMD_LONG_MASK << SDIO_SD_CMD_LONG_POS);
    reg_value |= ((value & SDIO_SD_CMD_LONG_MASK) << SDIO_SD_CMD_LONG_POS);
    REG_WRITE(SDIO_REG0X4_ADDR,reg_value);
}

/* REG_0x00:reg0x0->SD_CMD_CRC_CHECK:0x0[3],sd host(Slave?) need to check the slave respond command crc or not ; 1: need check crc  ; 0: not check crc,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_cmd_crc_check(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_CRC_CHECK_POS) & SDIO_SD_CMD_CRC_CHECK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_crc_check(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value &= ~(SDIO_SD_CMD_CRC_CHECK_MASK << SDIO_SD_CMD_CRC_CHECK_POS);
    reg_value |= ((value & SDIO_SD_CMD_CRC_CHECK_MASK) << SDIO_SD_CMD_CRC_CHECK_POS);
    REG_WRITE(SDIO_REG0X4_ADDR,reg_value);
}

/* REG_0x00:reg0x0->CMD_INDEX:0x0[9:4],Host Only; sd host send command index value,0x0,RW*/
static inline uint32_t sdio_ll_get_cmd_index(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value = ((reg_value >> SDIO_CMD_INDEX_POS) & SDIO_CMD_INDEX_MASK);
    return reg_value;
}

static inline void sdio_ll_set_cmd_index(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X4_ADDR);
    reg_value &= ~(SDIO_CMD_INDEX_MASK << SDIO_CMD_INDEX_POS);
    reg_value |= ((value & SDIO_CMD_INDEX_MASK) << SDIO_CMD_INDEX_POS);
    REG_WRITE(SDIO_REG0X4_ADDR,reg_value);
}

static inline uint32_t sdio_ll_get_reg0x5_value(void)
{
    return REG_READ(SDIO_REG0X5_ADDR);
}

static inline void sdio_ll_set_reg0x5_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X5_ADDR,value);
}

/* REG_0x01:reg0x1->CMD_ARGUMENT:0x1[31:0],Host Only; sd host send command argument value; Host:CMD index …,0x0,RW*/
static inline uint32_t sdio_ll_get_cmd_argument(void)
{
    return REG_READ(SDIO_REG0X5_ADDR);
}

static inline void sdio_ll_set_cmd_argument(uint32_t value)
{
    REG_WRITE(SDIO_REG0X5_ADDR,value);
}

static inline uint32_t sdio_ll_get_reg0x6_value(void)
{
    return REG_READ(SDIO_REG0X6_ADDR);
}

static inline void sdio_ll_set_reg0x6_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X6_ADDR,value);
}

/* REG_0x02:reg0x2->SD_CMD_TIMER:0x2[31:0],Host Only; sd host wait the slave respond timer; What's the unit?,0xffffffff,RW*/
static inline uint32_t sdio_ll_get_sd_cmd_timer(void)
{
    return REG_READ(SDIO_REG0X6_ADDR);
}

static inline void sdio_ll_set_sd_cmd_timer(uint32_t value)
{
    REG_WRITE(SDIO_REG0X6_ADDR,value);
}

static inline uint32_t sdio_ll_get_reg0x7_value(void)
{
    return REG_READ(SDIO_REG0X7_ADDR);
}

static inline void sdio_ll_set_reg0x7_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X7_ADDR,value);
}

/* REG_0x03:reg0x3->SD_DATA_EN:0x3[0],Host Only; sd host start receive slave data.; 1: start slave wr data to line set this bit; If host want to read more then 512bytes data, whether need to set it at every block?,0x0,RW/AutoC*/
static inline uint32_t sdio_ll_get_sd_data_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_EN_POS) & SDIO_SD_DATA_EN_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value &= ~(SDIO_SD_DATA_EN_MASK << SDIO_SD_DATA_EN_POS);
    reg_value |= ((value & SDIO_SD_DATA_EN_MASK) << SDIO_SD_DATA_EN_POS);
    REG_WRITE(SDIO_REG0X7_ADDR,reg_value);
}

/* REG_0x03:reg0x3->SD_DATA_STOP_EN:0x3[1],Host Only; sd host stop selfside transmid data; 1: stop; 0: start; Host can send CMD52 with REG0,Bit[0]=1 to slave and then slave stop transmid data,0x0,RW/AutoC*/
static inline uint32_t sdio_ll_get_sd_data_stop_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_STOP_EN_POS) & SDIO_SD_DATA_STOP_EN_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_stop_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value &= ~(SDIO_SD_DATA_STOP_EN_MASK << SDIO_SD_DATA_STOP_EN_POS);
    reg_value |= ((value & SDIO_SD_DATA_STOP_EN_MASK) << SDIO_SD_DATA_STOP_EN_POS);
    REG_WRITE(SDIO_REG0X7_ADDR,reg_value);
}

/* REG_0x03:reg0x3->SD_DATA_BUS:0x3[2],sd host(Slave) received data bus ; 1: 4 lines data  ; 0: 1 line data; When switch this bit, should check there is no data transmitting on the data line,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_data_bus(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_BUS_POS) & SDIO_SD_DATA_BUS_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_bus(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value &= ~(SDIO_SD_DATA_BUS_MASK << SDIO_SD_DATA_BUS_POS);
    reg_value |= ((value & SDIO_SD_DATA_BUS_MASK) << SDIO_SD_DATA_BUS_POS);
    REG_WRITE(SDIO_REG0X7_ADDR,reg_value);
}

/* REG_0x03:reg0x3->SD_DATA_MUL_BLK:0x3[3],Host Only; sd host received data multiple blocks ; 1: multiple blocks  ; 0: single block; If use bytes mode, should set this bit to single block mode.,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_data_mul_blk(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_MUL_BLK_POS) & SDIO_SD_DATA_MUL_BLK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_mul_blk(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value &= ~(SDIO_SD_DATA_MUL_BLK_MASK << SDIO_SD_DATA_MUL_BLK_POS);
    reg_value |= ((value & SDIO_SD_DATA_MUL_BLK_MASK) << SDIO_SD_DATA_MUL_BLK_POS);
    REG_WRITE(SDIO_REG0X7_ADDR,reg_value);
}

/* REG_0x03:reg0x3->SD_DATA_BLK_SIZE:0x3[15:4],Host Only; sd host send data block size(Bytes) to slave, host controller uses this value to controll send how many bytes to slave with every block.; a.Host with SD-CARD mode:write valid, read invalid.; b.Host with SDIO-Device mode:write valid,read invalid(the length has been send to device by CMD53),0x0,RW*/
static inline uint32_t sdio_ll_get_sd_data_blk_size(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_BLK_SIZE_POS) & SDIO_SD_DATA_BLK_SIZE_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_blk_size(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value &= ~(SDIO_SD_DATA_BLK_SIZE_MASK << SDIO_SD_DATA_BLK_SIZE_POS);
    reg_value |= ((value & SDIO_SD_DATA_BLK_SIZE_MASK) << SDIO_SD_DATA_BLK_SIZE_POS);
    REG_WRITE(SDIO_REG0X7_ADDR,reg_value);
}

/* REG_0x03:reg0x3->SD_START_WR_EN:0x3[16],sd host(Slave) write data enable：start write data to data wires.,0x0,RW/AutoC*/
static inline uint32_t sdio_ll_get_sd_start_wr_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value = ((reg_value >> SDIO_SD_START_WR_EN_POS) & SDIO_SD_START_WR_EN_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_start_wr_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value &= ~(SDIO_SD_START_WR_EN_MASK << SDIO_SD_START_WR_EN_POS);
    reg_value |= ((value & SDIO_SD_START_WR_EN_MASK) << SDIO_SD_START_WR_EN_POS);
    REG_WRITE(SDIO_REG0X7_ADDR,reg_value);
}

/* REG_0x03:reg0x3->SD_BYTE_SEL:0x3[17],sd host(Slave) received data byte arrangement ; 1: sd_rd = {sd_data[7:0],sd_data[15:8],sd_data[23:16],sd_data[31:24]} ; 0: sd_rd = sd_data[31:0],0x0,RW*/
static inline uint32_t sdio_ll_get_sd_byte_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value = ((reg_value >> SDIO_SD_BYTE_SEL_POS) & SDIO_SD_BYTE_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_byte_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value &= ~(SDIO_SD_BYTE_SEL_MASK << SDIO_SD_BYTE_SEL_POS);
    reg_value |= ((value & SDIO_SD_BYTE_SEL_MASK) << SDIO_SD_BYTE_SEL_POS);
    REG_WRITE(SDIO_REG0X7_ADDR,reg_value);
}

/* REG_0x07:reg0x7->SD_TX_BYTE_SEL:0x3[18],sd host(Slave) sent data byte arrangement ; 1: sd_rd = {sd_data[7:0],sd_data[15:8],sd_data[23:16],sd_data[31:24]} ; 0: sd_rd = sd_data[31:0],0x0,RW*/
static inline uint32_t sdio_ll_get_sd_tx_byte_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value = ((reg_value >> SDIO_SD_TX_BYTE_SEL_POS) & SDIO_SD_TX_BYTE_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_tx_byte_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X7_ADDR);
    reg_value &= ~(SDIO_SD_TX_BYTE_SEL_MASK << SDIO_SD_TX_BYTE_SEL_POS);
    reg_value |= ((value & SDIO_SD_TX_BYTE_SEL_MASK) << SDIO_SD_TX_BYTE_SEL_POS);
    REG_WRITE(SDIO_REG0X7_ADDR,reg_value);
}

static inline uint32_t sdio_ll_get_reg0x8_value(void)
{
    return REG_READ(SDIO_REG0X8_ADDR);
}

static inline void sdio_ll_set_reg0x8_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X8_ADDR,value);
}

/* REG_0x04:reg0x4->SD_DATA_TIMER:0x4[31:0],Host:; sd host read data from slave till timeout,; what's write timeout mean?; Slave:; slave wait host write data to line till timeout,it doesn't check host read data time.,0xffffffff,RW*/
static inline uint32_t sdio_ll_get_sd_data_timer(void)
{
    return REG_READ(SDIO_REG0X8_ADDR);
}

static inline void sdio_ll_set_sd_data_timer(uint32_t value)
{
    REG_WRITE(SDIO_REG0X8_ADDR,value);
}

static inline uint32_t sdio_ll_get_reg0x9_value(void)
{
    return REG_READ(SDIO_REG0X9_ADDR);
}

/* REG_0x05:reg0x5->SD_RSP_AGUMENT_0:0x5[31:0],Host:sd host received slave command argument value; ; Slave:sd slave received host command argument.; The argument maybe CMD52/CMD53 and other value(Though other CMD isn't error by ASIC).Every CMD Response INT should check this value.,0x0,R*/
static inline uint32_t sdio_ll_get_sd_rsp_agument_0(void)
{
    return REG_READ(SDIO_REG0X9_ADDR);
}

static inline uint32_t sdio_ll_get_reg0xa_value(void)
{
    return REG_READ(SDIO_REG0XA_ADDR);
}

/* REG_0x06:reg0x6->SD_RSP_AGUMENT_1:0x6[31:0],Host only; sd host received slave command argument value,0x0,R*/
static inline uint32_t sdio_ll_get_sd_rsp_agument_1(void)
{
    return REG_READ(SDIO_REG0XA_ADDR);
}

static inline uint32_t sdio_ll_get_reg0xb_value(void)
{
    return REG_READ(SDIO_REG0XB_ADDR);
}

/* REG_0x07:reg0x7->SD_RSP_AGUMENT_2:0x7[31:0],Host only; sd host received slave command argument value,0x0,R*/
static inline uint32_t sdio_ll_get_sd_rsp_agument_2(void)
{
    return REG_READ(SDIO_REG0XB_ADDR);
}

static inline uint32_t sdio_ll_get_reg0xc_value(void)
{
    return REG_READ(SDIO_REG0XC_ADDR);
}

/* REG_0x08:reg0x8->SD_RSP_AGUMENT_3:0x8[31:0],Host only; sd host received slave command argument value,0x0,R*/
static inline uint32_t sdio_ll_get_sd_rsp_agument_3(void)
{
    return REG_READ(SDIO_REG0XC_ADDR);
}

static inline uint32_t sdio_ll_get_reg0xd_value(void)
{
    return REG_READ(SDIO_REG0XD_ADDR);
}

static inline void sdio_ll_set_reg0xd_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0XD_ADDR,value);
}

static inline uint32_t sdio_ll_get_int_status(void)
{
    return REG_READ(SDIO_REG0XD_ADDR);
}

/* REG_0x09:reg0x9->SD_CMD_SEND_NORSP_END_INT:0x9[0],Host only; sd host send no response command end int.; Some CMD no needs response,so CMD sends finish then the sdio produced the CMD send finish INT,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_sd_cmd_send_norsp_end_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_SEND_NORSP_END_INT_POS) & SDIO_SD_CMD_SEND_NORSP_END_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_send_norsp_end_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value &= ~(SDIO_SD_CMD_SEND_NORSP_END_INT_MASK << SDIO_SD_CMD_SEND_NORSP_END_INT_POS);
    reg_value |= ((value & SDIO_SD_CMD_SEND_NORSP_END_INT_MASK) << SDIO_SD_CMD_SEND_NORSP_END_INT_POS);
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* REG_0x09:reg0x9->SD_CMD_SEND_RSP_END_INT:0x9[1],Host only; sd host receive slave command end int,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_sd_cmd_send_rsp_end_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_SEND_RSP_END_INT_POS) & SDIO_SD_CMD_SEND_RSP_END_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_send_rsp_end_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value &= ~(SDIO_SD_CMD_SEND_RSP_END_INT_MASK << SDIO_SD_CMD_SEND_RSP_END_INT_POS);
    reg_value |= ((value & SDIO_SD_CMD_SEND_RSP_END_INT_MASK) << SDIO_SD_CMD_SEND_RSP_END_INT_POS);
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* REG_0x09:reg0x9->SD_CMD_SEND_RSP_TIMEOUT_INT:0x9[2],sd host wait slave command over time int(What's diffrence with "no response command end int"),0x0,R/W1C*/
static inline uint32_t sdio_ll_get_sd_cmd_send_rsp_timeout_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_SEND_RSP_TIMEOUT_INT_POS) & SDIO_SD_CMD_SEND_RSP_TIMEOUT_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_send_rsp_timeout_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value &= ~(SDIO_SD_CMD_SEND_RSP_TIMEOUT_INT_MASK << SDIO_SD_CMD_SEND_RSP_TIMEOUT_INT_POS);
    reg_value |= ((value & SDIO_SD_CMD_SEND_RSP_TIMEOUT_INT_MASK) << SDIO_SD_CMD_SEND_RSP_TIMEOUT_INT_POS);
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* REG_0x09:reg0x9->SD_DATA_REC_END_INT:0x9[3],sd host receive slave data end int,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_sd_data_rec_end_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_REC_END_INT_POS) & SDIO_SD_DATA_REC_END_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_rec_end_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value &= ~(SDIO_SD_DATA_REC_END_INT_MASK << SDIO_SD_DATA_REC_END_INT_POS);
    reg_value |= ((value & SDIO_SD_DATA_REC_END_INT_MASK) << SDIO_SD_DATA_REC_END_INT_POS);
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* REG_0x09:reg0x9->SD_DATA_WR_END_INT:0x9[4],sd host write data end int,it means slave has read data finish,notify host can send next block.(Slave has to write REG0x10 Bit[1] to 1(clear to 0 and then set to 1) to notify host send next block,or host can't send next block data),0x0,R/W1C*/
static inline uint32_t sdio_ll_get_sd_data_wr_end_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_WR_END_INT_POS) & SDIO_SD_DATA_WR_END_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_wr_end_int(uint32_t value)
{
    REG_WRITE(SDIO_REG0XD_ADDR, ((value & SDIO_SD_DATA_WR_END_INT_MASK) << SDIO_SD_DATA_WR_END_INT_POS));
}

/* REG_0x09:reg0x9->SD_DATA_TIME_OUT_INT:0x9[5],sd host wait slave data over time int,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_sd_data_time_out_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_TIME_OUT_INT_POS) & SDIO_SD_DATA_TIME_OUT_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_time_out_int(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value &= ~(SDIO_SD_DATA_TIME_OUT_INT_MASK << SDIO_SD_DATA_TIME_OUT_INT_POS);
    reg_value |= ((value & SDIO_SD_DATA_TIME_OUT_INT_MASK) << SDIO_SD_DATA_TIME_OUT_INT_POS);
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* REG_0x09:reg0x9->RX_FIFO_NEED_READ:0x9[6],sd host fifo memory need read int; Host:If FIFO has more then count of threshhold data,enable INT to CPU.; Slave:valid,0x0,R*/
static inline uint32_t sdio_ll_get_rx_fifo_need_read(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_RX_FIFO_NEED_READ_POS)&SDIO_RX_FIFO_NEED_READ_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->TX_FIFO_NEED_WRITE:0x9[7],sd host fifo memory need write int; Host:If FIFO has less then count of threshhold data,enable INT to CPU.; Slave:valid,0x0,R*/
static inline uint32_t sdio_ll_get_tx_fifo_need_write(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_TX_FIFO_NEED_WRITE_POS)&SDIO_TX_FIFO_NEED_WRITE_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->RX_OV:0x9[8],sd host fifo memory overflow int?; rx fifo full, host slave wr too many full, slave host wr too many data.; What's the overflow condition?,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_rx_ov(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_RX_OV_POS) & SDIO_RX_OV_MASK);
    return reg_value;
}

static inline void sdio_ll_set_rx_ov(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value &= ~(SDIO_RX_OV_MASK << SDIO_RX_OV_POS);
    reg_value |= ((value & SDIO_RX_OV_MASK) << SDIO_RX_OV_POS);
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* REG_0x09:reg0x9->TX_FIFO_EMPT:0x9[9],sd host fifo memory empty int,0x0,R*/
static inline uint32_t sdio_ll_get_tx_fifo_empt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_TX_FIFO_EMPT_POS)&SDIO_TX_FIFO_EMPT_MASK);
    return reg_value;
}

static inline void sdio_ll_clear_tx_fifo_empt(void)
{
    REG_WRITE(SDIO_REG0XD_ADDR, ((1 & SDIO_TX_FIFO_EMPT_MASK) << SDIO_TX_FIFO_EMPT_POS));
}

/* REG_0x09:reg0x9->SD_RSP_CMD_CRC_OK:0x9[10],sd host/slave received peer side slave/host command crc check indication signal 1: crc ok  ,0x0,R*/
static inline uint32_t sdio_ll_get_sd_rsp_cmd_crc_ok(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_RSP_CMD_CRC_OK_POS)&SDIO_SD_RSP_CMD_CRC_OK_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->SD_RSP_CMD_CRC_FAIL:0x9[11],sd host/slave received peer side slave/host command crc check indication signal 1: crc fail ,0x0,R*/
static inline uint32_t sdio_ll_get_sd_rsp_cmd_crc_fail(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_RSP_CMD_CRC_FAIL_POS)&SDIO_SD_RSP_CMD_CRC_FAIL_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->SD_DATA_CRC_OK:0x9[12],sd host/slave received peer side slave/host data crc check indication signal 1: crc ok,0x0,R*/
static inline uint32_t sdio_ll_get_sd_data_crc_ok(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_CRC_OK_POS)&SDIO_SD_DATA_CRC_OK_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->SD_DATA_CRC_FAIL:0x9[13],sd host/slave received peer side slave/host data crc check indication signal 1: crc fail,0x0,R*/
static inline uint32_t sdio_ll_get_sd_data_crc_fail(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_CRC_FAIL_POS)&SDIO_SD_DATA_CRC_FAIL_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->SD_RSP_INDEX:0x9[19:14],Host only; sd host received slave command index value,0x0,R*/
static inline uint32_t sdio_ll_get_sd_rsp_index(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_SD_RSP_INDEX_POS)&SDIO_SD_RSP_INDEX_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->WR_STATUS:0x9[22:20],sd host write data result(response from slave device); 0:; 1:; 2:Is right, else is error.; (Workaround:BK7256 read this value is 0x5 though slave responsed is 0x2); 3:; 4:; 5:; 6:; 7:,0x0,R*/
static inline uint32_t sdio_ll_get_wr_status(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_WR_STATUS_POS)&SDIO_WR_STATUS_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->DATA_BUSY:0x9[23],sd slave busy signal after sd host send special command and write data.Host check this bit to confirm slave is whether busy?,0x0,R*/
static inline uint32_t sdio_ll_get_data_busy(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_DATA_BUSY_POS)&SDIO_DATA_BUSY_MASK);
    return reg_value;
}

/* REG_0x09:reg0x9->CMD_S_RES_END_INT:0x9[24],Slave only; Slave has finish reponsed the CMD to host side.,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_cmd_s_res_end_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_CMD_S_RES_END_INT_POS) & SDIO_CMD_S_RES_END_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_cmd_s_res_end_int(uint32_t value)
{
    uint32_t reg_value;
    //reg_value = REG_READ(SDIO_REG0XD_ADDR);
    //reg_value &= ~(SDIO_CMD_S_RES_END_INT_MASK << SDIO_CMD_S_RES_END_INT_POS);
    //reg_value |= ((value & SDIO_CMD_S_RES_END_INT_MASK) << SDIO_CMD_S_RES_END_INT_POS);

    //write clear feature
    reg_value = ((value & SDIO_CMD_S_RES_END_INT_MASK) << SDIO_CMD_S_RES_END_INT_POS);
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* REG_0x09:reg0x9->DAT_S_WR_WAI_INT:0x9[25],Slave:Host write and slave is reading,host should stop, INT notify CPU slave has received the data from host.; Slave write finish.; W1:Clear the finish status.; 1.If clear means slave can receive data again?; Read data-out first or clear INT first?; 2.The name of WR_WAT is hard to understand,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_dat_s_wr_wai_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_DAT_S_WR_WAI_INT_POS) & SDIO_DAT_S_WR_WAI_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_dat_s_wr_wai_int(uint32_t value)
{
    uint32_t reg_value;
    //reg_value = REG_READ(SDIO_REG0XD_ADDR);
    //reg_value &= ~(SDIO_DAT_S_WR_WAI_INT_MASK << SDIO_DAT_S_WR_WAI_INT_POS);
    //reg_value |= ((value & SDIO_DAT_S_WR_WAI_INT_MASK) << SDIO_DAT_S_WR_WAI_INT_POS);

    //write clear feature
    reg_value = ((value & SDIO_DAT_S_WR_WAI_INT_MASK) << SDIO_DAT_S_WR_WAI_INT_POS);
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* REG_0x09:reg0x9->dat_s_rd_bus_int:0x9[26],If RX FIFO full(only valid for FULL status) or read finish(once block finish or all blocks finish?), the value is 1 means slave can't receive data,then CPU deals the received data and clear it to receive data again.This bit mean is similar with REG0x15 Bit[14].,0x0,R/W1C*/
static inline uint32_t sdio_ll_get_dat_s_rd_bus_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_DAT_S_RD_BUS_INT_POS) & SDIO_DAT_S_RD_BUS_INT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_dat_s_rd_bus_int(uint32_t value)
{
    uint32_t reg_value;
    //reg_value = REG_READ(SDIO_REG0XD_ADDR);
    //reg_value &= ~(SDIO_DAT_S_RD_BUS_INT_MASK << SDIO_DAT_S_RD_BUS_INT_POS);
    //reg_value |= ((value & SDIO_DAT_S_RD_BUS_INT_MASK) << SDIO_DAT_S_RD_BUS_INT_POS);

    //write clear feature
    reg_value = (value & SDIO_DAT_S_RD_BUS_INT_MASK) << SDIO_DAT_S_RD_BUS_INT_POS;
    REG_WRITE(SDIO_REG0XD_ADDR,reg_value);
}

/* Slave write multi-block data from host side complete interrupt status:Write 1 to clear to 0 */
static inline uint32_t sdio_ll_get_dat_wrsts_err_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_DAT_WRSTS_ERR_INT_POS) & SDIO_DAT_WRSTS_ERR_INT_MASK);
    return reg_value;
}

/* Slave read multi-block data from host side complete interrupt status:Write 1 to clear to 0 */
static inline void sdio_ll_set_dat_wrsts_err_int(uint32_t value)
{
    uint32_t reg_value;

    //read clear feature
    reg_value = (value & SDIO_DAT_WRSTS_ERR_INT_MASK) << SDIO_DAT_WRSTS_ERR_INT_POS;
    REG_WRITE(SDIO_REG0XD_ADDR, reg_value);
}

/* Slave write multi-block data to host side complete interrupt status:Write 1 to clear to 0 */
static inline uint32_t sdio_ll_get_dat_s_wr_blk_int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XD_ADDR);
    reg_value = ((reg_value >> SDIO_DAT_S_WR_BLK_INT_POS) & SDIO_DAT_S_WR_BLK_INT_MASK);
    return reg_value;
}

/* Slave write multi-block data to host side complete interrupt status:Write 1 to clear to 0 */
static inline void sdio_ll_set_dat_s_wr_blk_int(uint32_t value)
{
    uint32_t reg_value;

    //write clear feature
    reg_value = (value & SDIO_DAT_S_WR_BLK_INT_MASK) << SDIO_DAT_S_WR_BLK_INT_POS;
    REG_WRITE(SDIO_REG0XD_ADDR, reg_value);
}

static inline uint32_t sdio_ll_get_reg0xe_value(void)
{
    return REG_READ(SDIO_REG0XE_ADDR);
}

static inline void sdio_ll_set_reg0xe_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0XE_ADDR,value);
}

/* REG_0x0a:reg0xa->SD_CMD_SEND_NORSP_END_MASK:0xa[0],sd host send no response command end mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_cmd_send_norsp_end_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_SEND_NORSP_END_MASK_POS) & SDIO_SD_CMD_SEND_NORSP_END_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_send_norsp_end_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SD_CMD_SEND_NORSP_END_MASK_MASK << SDIO_SD_CMD_SEND_NORSP_END_MASK_POS);
    reg_value |= ((value & SDIO_SD_CMD_SEND_NORSP_END_MASK_MASK) << SDIO_SD_CMD_SEND_NORSP_END_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->SD_CMD_SEND_RSP_END_MASK:0xa[1],sd host receive slave command end mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_cmd_send_rsp_end_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_SEND_RSP_END_MASK_POS) & SDIO_SD_CMD_SEND_RSP_END_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_send_rsp_end_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SD_CMD_SEND_RSP_END_MASK_MASK << SDIO_SD_CMD_SEND_RSP_END_MASK_POS);
    reg_value |= ((value & SDIO_SD_CMD_SEND_RSP_END_MASK_MASK) << SDIO_SD_CMD_SEND_RSP_END_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->SD_CMD_SEND_RSP_TIMEOUT_MASK:0xa[2],sd host wait slave command over time mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_cmd_send_rsp_timeout_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CMD_SEND_RSP_TIMEOUT_MASK_POS) & SDIO_SD_CMD_SEND_RSP_TIMEOUT_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_cmd_send_rsp_timeout_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SD_CMD_SEND_RSP_TIMEOUT_MASK_MASK << SDIO_SD_CMD_SEND_RSP_TIMEOUT_MASK_POS);
    reg_value |= ((value & SDIO_SD_CMD_SEND_RSP_TIMEOUT_MASK_MASK) << SDIO_SD_CMD_SEND_RSP_TIMEOUT_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->SD_DATA_REC_END_MASK:0xa[3],sd host receive slave data end mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_data_rec_end_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_REC_END_MASK_POS) & SDIO_SD_DATA_REC_END_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_rec_end_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SD_DATA_REC_END_MASK_MASK << SDIO_SD_DATA_REC_END_MASK_POS);
    reg_value |= ((value & SDIO_SD_DATA_REC_END_MASK_MASK) << SDIO_SD_DATA_REC_END_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->SD_DATA_WR_END_MASK:0xa[4],sd host write data end mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_data_wr_end_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_WR_END_MASK_POS) & SDIO_SD_DATA_WR_END_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_wr_end_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SD_DATA_WR_END_MASK_MASK << SDIO_SD_DATA_WR_END_MASK_POS);
    reg_value |= ((value & SDIO_SD_DATA_WR_END_MASK_MASK) << SDIO_SD_DATA_WR_END_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->SD_DATA_TIME_OUT_MASK:0xa[5],sd host wait slave data over time mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_data_time_out_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_SD_DATA_TIME_OUT_MASK_POS) & SDIO_SD_DATA_TIME_OUT_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_data_time_out_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SD_DATA_TIME_OUT_MASK_MASK << SDIO_SD_DATA_TIME_OUT_MASK_POS);
    reg_value |= ((value & SDIO_SD_DATA_TIME_OUT_MASK_MASK) << SDIO_SD_DATA_TIME_OUT_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->RX_FIFO_NEED_READ_MASK:0xa[6],sd host fifo memory need read mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_rx_fifo_need_read_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_RX_FIFO_NEED_READ_MASK_POS) & SDIO_RX_FIFO_NEED_READ_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_rx_fifo_need_read_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_RX_FIFO_NEED_READ_MASK_MASK << SDIO_RX_FIFO_NEED_READ_MASK_POS);
    reg_value |= ((value & SDIO_RX_FIFO_NEED_READ_MASK_MASK) << SDIO_RX_FIFO_NEED_READ_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->TX_FIFO_NEED_WRITE_MASK:0xa[7],sd host fifo memory need write mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_tx_fifo_need_write_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_TX_FIFO_NEED_WRITE_MASK_POS) & SDIO_TX_FIFO_NEED_WRITE_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_tx_fifo_need_write_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_TX_FIFO_NEED_WRITE_MASK_MASK << SDIO_TX_FIFO_NEED_WRITE_MASK_POS);
    reg_value |= ((value & SDIO_TX_FIFO_NEED_WRITE_MASK_MASK) << SDIO_TX_FIFO_NEED_WRITE_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->RX_OV_MASK:0xa[8],sd host fifo memory overflow mask; Slave Invalid,0x0,RW*/
static inline uint32_t sdio_ll_get_rx_ov_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_RX_OV_MASK_POS) & SDIO_RX_OV_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_rx_ov_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_RX_OV_MASK_MASK << SDIO_RX_OV_MASK_POS);
    reg_value |= ((value & SDIO_RX_OV_MASK_MASK) << SDIO_RX_OV_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->TX_FIFO_EMPT_MASK:0xa[9],sd host fifo memory empty mask：; a.0:Mask INT,not report to CPU, 1：If empty,INT report to CPU(If FIFO is empty,SDIO module will be clock gate, FIFO not empty, clock restart); b.Must after CMD53; workaround:multi-block has to set 1,0x0,RW*/
static inline uint32_t sdio_ll_get_tx_fifo_empt_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_TX_FIFO_EMPT_MASK_POS) & SDIO_TX_FIFO_EMPT_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_tx_fifo_empt_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_TX_FIFO_EMPT_MASK_MASK << SDIO_TX_FIFO_EMPT_MASK_POS);
    reg_value |= ((value & SDIO_TX_FIFO_EMPT_MASK_MASK) << SDIO_TX_FIFO_EMPT_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->CMD_S_RES_END_INT_MASK:0xa[10],0:Mask INT,not report to CPU, 1：INT report to CPU,0x0,RW*/
static inline uint32_t sdio_ll_get_cmd_s_res_end_int_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_CMD_S_RES_END_INT_MASK_POS) & SDIO_CMD_S_RES_END_INT_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_cmd_s_res_end_int_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_CMD_S_RES_END_INT_MASK_MASK << SDIO_CMD_S_RES_END_INT_MASK_POS);
    reg_value |= ((value & SDIO_CMD_S_RES_END_INT_MASK_MASK) << SDIO_CMD_S_RES_END_INT_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->DAT_S_WR_WAI_INT_MASK:0xa[11],1.Enable slave write data finish INT report to CPU.,0x0,RW*/
static inline uint32_t sdio_ll_get_dat_s_wr_wai_int_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_DAT_S_WR_WAI_INT_MASK_POS) & SDIO_DAT_S_WR_WAI_INT_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_dat_s_wr_wai_int_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_DAT_S_WR_WAI_INT_MASK_MASK << SDIO_DAT_S_WR_WAI_INT_MASK_POS);
    reg_value |= ((value & SDIO_DAT_S_WR_WAI_INT_MASK_MASK) << SDIO_DAT_S_WR_WAI_INT_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->DAT_S_RD_BUS_INT_MASK:0xa[12],1:Enable busy INT report to CPU.,0x0,RW*/
static inline uint32_t sdio_ll_get_dat_s_rd_bus_int_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_DAT_S_RD_BUS_INT_MASK_POS) & SDIO_DAT_S_RD_BUS_INT_MASK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_dat_s_rd_bus_int_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_DAT_S_RD_BUS_INT_MASK_MASK << SDIO_DAT_S_RD_BUS_INT_MASK_POS);
    reg_value |= ((value & SDIO_DAT_S_RD_BUS_INT_MASK_MASK) << SDIO_DAT_S_RD_BUS_INT_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->TX_FIFO_NEED_WRITE_MASK_CG:0xa[13],1:sd host fifo memory need write mask for clk gate writing use only,RW*/
static inline uint32_t sdio_ll_get_tx_fifo_need_write_mask_cg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_TX_FIFO_NEED_WRITE_MASK_CG_POS) & SDIO_TX_FIFO_NEED_WRITE_MASK_CG_MASK);
    return reg_value;
}

static inline void sdio_ll_set_tx_fifo_need_write_mask_cg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_TX_FIFO_NEED_WRITE_MASK_CG_MASK << SDIO_TX_FIFO_NEED_WRITE_MASK_CG_POS);
    reg_value |= ((value & SDIO_TX_FIFO_NEED_WRITE_MASK_CG_MASK) << SDIO_TX_FIFO_NEED_WRITE_MASK_CG_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->WRITE_WAIT_JUMP_SEL:0xa[14],0:jump to write_busy 1:jump to idle,RW*/
static inline uint32_t sdio_ll_get_write_wait_jump_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_WRITE_WAIT_JUMP_SEL_POS) & SDIO_WRITE_WAIT_JUMP_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_write_wait_jump_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_WRITE_WAIT_JUMP_SEL_MASK << SDIO_WRITE_WAIT_JUMP_SEL_POS);
    reg_value |= ((value & SDIO_WRITE_WAIT_JUMP_SEL_MASK) << SDIO_WRITE_WAIT_JUMP_SEL_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* REG_0x0a:reg0xa->IDLE_STOP_JUMP_SEL:0xa[15],0：rec stop not jump to busy 1:rec stop will jump to busy,RW*/
static inline uint32_t sdio_ll_get_idle_stop_jump_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value = ((reg_value >> SDIO_IDLE_STOP_JUMP_SEL_POS) & SDIO_IDLE_STOP_JUMP_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_idle_stop_jump_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_IDLE_STOP_JUMP_SEL_MASK << SDIO_IDLE_STOP_JUMP_SEL_POS);
    reg_value |= ((value & SDIO_IDLE_STOP_JUMP_SEL_MASK) << SDIO_IDLE_STOP_JUMP_SEL_POS);
    REG_WRITE(SDIO_REG0XE_ADDR,reg_value);
}

/* If slave read multi-block from host side complete, set it whether enable interrupt. */
static inline void sdio_ll_slave_set_read_block_int_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_WRSTS_ERR_INT_MASK_MASK << SDIO_WRSTS_ERR_INT_MASK_POS);
    reg_value |= ((value & SDIO_WRSTS_ERR_INT_MASK_MASK) << SDIO_WRSTS_ERR_INT_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR, reg_value);
}

/* SDIO enable multi-block transfer feature */
static inline void sdio_ll_set_read_mul_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SLAVE_READ_MUL_SEL_MASK << SDIO_SLAVE_READ_MUL_SEL_POS);
    reg_value |= ((value & SDIO_SLAVE_READ_MUL_SEL_MASK) << SDIO_SLAVE_READ_MUL_SEL_POS);
    REG_WRITE(SDIO_REG0XE_ADDR, reg_value);
}

/* If slave writes multi-block to host side complete, set it whether enable interrupt. */
static inline void sdio_ll_slave_set_write_block_int_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SLAVE_WRITE_BLOCK_INT_MASK_MASK << SDIO_SLAVE_WRITE_BLOCK_INT_MASK_POS);
    reg_value |= ((value & SDIO_SLAVE_WRITE_BLOCK_INT_MASK_MASK) << SDIO_SLAVE_WRITE_BLOCK_INT_MASK_POS);
    REG_WRITE(SDIO_REG0XE_ADDR, reg_value);
}

/* SDIO enable multi-block transfer feature */
static inline void sdio_ll_set_write_mul_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0XE_ADDR);
    reg_value &= ~(SDIO_SLAVE_WRITE_MUL_SEL_MASK << SDIO_SLAVE_WRITE_MUL_SEL_POS);
    reg_value |= ((value & SDIO_SLAVE_WRITE_MUL_SEL_MASK) << SDIO_SLAVE_WRITE_MUL_SEL_POS);
    REG_WRITE(SDIO_REG0XE_ADDR, reg_value);
}

/*write only reg:reg0xb:default value:0x0*/
static inline void sdio_ll_set_reg0xf_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0XF_ADDR,value);
}

static inline uint32_t sdio_ll_get_reg0x10_value(void)
{
    return REG_READ(SDIO_REG0X10_ADDR);
}

/* REG_0x0c:reg0xc->RX_FIFO_DOUT:0xc[31:0],sd host fifo read data,0x0,R*/
static inline uint32_t sdio_ll_get_rx_fifo_dout(void)
{
    return REG_READ(SDIO_REG0X10_ADDR);
}

static inline uint32_t sdio_ll_get_reg0x11_value(void)
{
    return REG_READ(SDIO_REG0X11_ADDR);
}

static inline void sdio_ll_set_reg0x11_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X11_ADDR,value);
}

/* REG_0x11:reg0xd->RX_FIFO_THRESHOLD:0xd[7:0],sd host fifo threshold for read,0x0,RW*/
static inline uint32_t sdio_ll_get_rx_fifo_threshold(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_RX_FIFO_THRESHOLD_POS) & SDIO_RX_FIFO_THRESHOLD_MASK);
    return reg_value;
}

static inline void sdio_ll_set_rx_fifo_threshold(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_RX_FIFO_THRESHOLD_MASK << SDIO_RX_FIFO_THRESHOLD_POS);
    reg_value |= ((value & SDIO_RX_FIFO_THRESHOLD_MASK) << SDIO_RX_FIFO_THRESHOLD_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->TX_FIFO_THRESHOLD:0xd[15:8],sd host fifo threshold for write,0x0,RW*/
static inline uint32_t sdio_ll_get_tx_fifo_threshold(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_TX_FIFO_THRESHOLD_POS) & SDIO_TX_FIFO_THRESHOLD_MASK);
    return reg_value;
}

static inline void sdio_ll_set_tx_fifo_threshold(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_TX_FIFO_THRESHOLD_MASK << SDIO_TX_FIFO_THRESHOLD_POS);
    reg_value |= ((value & SDIO_TX_FIFO_THRESHOLD_MASK) << SDIO_TX_FIFO_THRESHOLD_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->RX_FIFO_RST:0xd[16],read fifo reset, high active; Host/Slave:Reset self state,0x1,R/W1C*/
static inline uint32_t sdio_ll_get_rx_fifo_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_RX_FIFO_RST_POS) & SDIO_RX_FIFO_RST_MASK);
    return reg_value;
}

//low active:0 reset(read value is 1, write 0 to reset, asic auto set it to 1)
static inline void sdio_ll_set_rx_fifo_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_RX_FIFO_RST_MASK << SDIO_RX_FIFO_RST_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->TX_FIFO_RST:0xd[17],write fifo reset, high active; Host/Slave:Reset self state,0x1,R/W1C*/
static inline uint32_t sdio_ll_get_tx_fifo_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_TX_FIFO_RST_POS) & SDIO_TX_FIFO_RST_MASK);
    return reg_value;
}

//low active:0 reset(read value is 1, write 0 to reset, asic auto set it to 1)
static inline void sdio_ll_set_tx_fifo_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_TX_FIFO_RST_MASK << SDIO_TX_FIFO_RST_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->RXFIFO_RD_READY:0xd[18],host/slave rx fifo not empty,0x0,R*/
static inline uint32_t sdio_ll_get_rxfifo_rd_ready(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_RXFIFO_RD_READY_POS)&SDIO_RXFIFO_RD_READY_MASK);
    return reg_value;
}

/* REG_0x11:reg0xd->TXFIFO_WR_READY:0xd[19],host/slave tx fifo not full(data is valid, not used for CMD),0x1,R*/
static inline uint32_t sdio_ll_get_txfifo_wr_ready(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_TXFIFO_WR_READY_POS)&SDIO_TXFIFO_WR_READY_MASK);
    return reg_value;
}

/* REG_0x11:reg0xd->SD_STA_RST:0xd[20],sdcard command and data state reset, low active; Host/Slave:Reset self state; It's better to reset after one round transfer.,0x1,R/W1C*/
static inline uint32_t sdio_ll_get_sd_sta_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_SD_STA_RST_POS) & SDIO_SD_STA_RST_MASK);
    return reg_value;
}

//low active:0 reset(read value is 1, write 0 to reset, asic auto set it to 1)
static inline void sdio_ll_set_sd_sta_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_SD_STA_RST_MASK << SDIO_SD_STA_RST_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->SD_CLK_SEL:0xd[22:21],sdcard clock div sel, used or not depend on project; Invalid.,0x3,RW*/
static inline uint32_t sdio_ll_get_sd_clk_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_SD_CLK_SEL_POS) & SDIO_SD_CLK_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_clk_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_SD_CLK_SEL_MASK << SDIO_SD_CLK_SEL_POS);
    reg_value |= ((value & SDIO_SD_CLK_SEL_MASK) << SDIO_SD_CLK_SEL_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->SD_RD_WAIT_SEL:0xd[23],read data after command ?; Use default value.,0x1,RW*/
static inline uint32_t sdio_ll_get_sd_rd_wait_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_SD_RD_WAIT_SEL_POS) & SDIO_SD_RD_WAIT_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_rd_wait_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_SD_RD_WAIT_SEL_MASK << SDIO_SD_RD_WAIT_SEL_POS);
    reg_value |= ((value & SDIO_SD_RD_WAIT_SEL_MASK) << SDIO_SD_RD_WAIT_SEL_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->SD_WR_WAIT_SEL:0xd[24],write data after command ?; Use default value.,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_wr_wait_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_SD_WR_WAIT_SEL_POS) & SDIO_SD_WR_WAIT_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_wr_wait_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_SD_WR_WAIT_SEL_MASK << SDIO_SD_WR_WAIT_SEL_POS);
    reg_value |= ((value & SDIO_SD_WR_WAIT_SEL_MASK) << SDIO_SD_WR_WAIT_SEL_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->CLK_REC_SEL:0xd[25],write clock recovery selection; when write data, should be set to 1(always 1,SW not use it),0x0,RW*/
static inline uint32_t sdio_ll_get_clk_rec_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_CLK_REC_SEL_POS) & SDIO_CLK_REC_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_clk_rec_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_CLK_REC_SEL_MASK << SDIO_CLK_REC_SEL_POS);
    reg_value |= ((value & SDIO_CLK_REC_SEL_MASK) << SDIO_CLK_REC_SEL_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->SAMP_SEL:0xd[26],sample egde of data 0：neg 1：pos,RW*/
static inline uint32_t sdio_ll_get_samp_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_SAMP_SEL_POS) & SDIO_SAMP_SEL_MASK);
    return reg_value;
}

static inline void sdio_ll_set_samp_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_SAMP_SEL_MASK << SDIO_SAMP_SEL_POS);
    reg_value |= ((value & SDIO_SAMP_SEL_MASK) << SDIO_SAMP_SEL_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->CLK_GATE_ON:0xd[27],module clockgate on control 1：always on,RW*/
static inline uint32_t sdio_ll_get_clk_gate_on(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_CLK_GATE_ON_POS) & SDIO_CLK_GATE_ON_MASK);
    return reg_value;
}

static inline void sdio_ll_set_clk_gate_on(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_CLK_GATE_ON_MASK << SDIO_CLK_GATE_ON_POS);
    reg_value |= ((value & SDIO_CLK_GATE_ON_MASK) << SDIO_CLK_GATE_ON_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->HOST_WR_BLK_EN:0xd[28],new multi-block write method enable,RW*/
static inline uint32_t sdio_ll_get_host_wr_blk_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_HOST_WR_BLK_EN_POS) & SDIO_HOST_WR_BLK_EN_MASK);
    return reg_value;
}

static inline void sdio_ll_set_host_wr_blk_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_HOST_WR_BLK_EN_MASK << SDIO_HOST_WR_BLK_EN_POS);
    reg_value |= ((value & SDIO_HOST_WR_BLK_EN_MASK) << SDIO_HOST_WR_BLK_EN_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

/* REG_0x11:reg0xd->HOST_RD_BLK_EN:0xd[29],new multi-block read  method enable,RW*/
static inline uint32_t sdio_ll_get_host_rd_blk_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value = ((reg_value >> SDIO_HOST_RD_BLK_EN_POS) & SDIO_HOST_RD_BLK_EN_MASK);
    return reg_value;
}

static inline void sdio_ll_set_host_rd_blk_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X11_ADDR);
    reg_value &= ~(SDIO_HOST_RD_BLK_EN_MASK << SDIO_HOST_RD_BLK_EN_POS);
    reg_value |= ((value & SDIO_HOST_RD_BLK_EN_MASK) << SDIO_HOST_RD_BLK_EN_POS);
    REG_WRITE(SDIO_REG0X11_ADDR,reg_value);
}

static inline uint32_t sdio_ll_get_reg0x12_value(void)
{
    return REG_READ(SDIO_REG0X12_ADDR);
}

static inline void sdio_ll_set_reg0x12_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X12_ADDR,value);
}

/* REG_0x10:reg0x10->SD_SLAVE:0x10[0],0:Host 1:Slave,0x0,RW*/
static inline uint32_t sdio_ll_get_sd_slave(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_POS) & SDIO_SD_SLAVE_MASK);
    return reg_value;
}

static inline void sdio_ll_set_sd_slave(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value &= ~(SDIO_SD_SLAVE_MASK << SDIO_SD_SLAVE_POS);
    reg_value |= ((value & SDIO_SD_SLAVE_MASK) << SDIO_SD_SLAVE_POS);
    REG_WRITE(SDIO_REG0X12_ADDR,reg_value);
}

/* REG_0x10:reg0x10->DAT_S_RD_MUL_BLK:0x10[1],Host write slave read; Slave:0:Read signle block 1:Read multi-blocks; If one block transfer finish, the sdio slave will stop the bus to transfer data again.Once set this bit to 1(set to 0 and then to 1), slave will release the sdio bus to notify host can transfer next block data.,0x0,RW*/
static inline uint32_t sdio_ll_get_dat_s_rd_mul_blk(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value = ((reg_value >> SDIO_DAT_S_RD_MUL_BLK_POS) & SDIO_DAT_S_RD_MUL_BLK_MASK);
    return reg_value;
}

static inline void sdio_ll_set_dat_s_rd_mul_blk(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value &= ~(SDIO_DAT_S_RD_MUL_BLK_MASK << SDIO_DAT_S_RD_MUL_BLK_POS);
    reg_value |= ((value & SDIO_DAT_S_RD_MUL_BLK_MASK) << SDIO_DAT_S_RD_MUL_BLK_POS);
    REG_WRITE(SDIO_REG0X12_ADDR,reg_value);
}

/* REG_0x10:reg0x10->IO_CUR_STA_REG:0x10[3:2],io state register reponse to host,0x0,RW*/
static inline uint32_t sdio_ll_get_io_cur_sta_reg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value = ((reg_value >> SDIO_IO_CUR_STA_REG_POS) & SDIO_IO_CUR_STA_REG_MASK);
    return reg_value;
}

static inline void sdio_ll_set_io_cur_sta_reg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value &= ~(SDIO_IO_CUR_STA_REG_MASK << SDIO_IO_CUR_STA_REG_POS);
    reg_value |= ((value & SDIO_IO_CUR_STA_REG_MASK) << SDIO_IO_CUR_STA_REG_POS);
    REG_WRITE(SDIO_REG0X12_ADDR,reg_value);
}

/* REG_0x10:reg0x10->CMD_52_STOP_CLR:0x10[4],cmd52 send stop command(cmd_s_rec_wr_dat_0[0]), write 1 clear cmd_s_rec_wr_dat_0; Host send CMD52 with Address-0,Bit[0] = 1 to notify slave stop,slave check this value to stop;clear this bit to start again,0x0,WO*/
static inline void sdio_ll_set_cmd_52_stop_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value &= ~(SDIO_CMD_52_STOP_CLR_MASK << SDIO_CMD_52_STOP_CLR_POS);
    reg_value |= ((value & SDIO_CMD_52_STOP_CLR_MASK) << SDIO_CMD_52_STOP_CLR_POS);
    REG_WRITE(SDIO_REG0X12_ADDR,reg_value);
}

/* REG_0x10:reg0x10->CMD_KEEP_DET:0x10[5],1:keep detecting if last cmd fail 0:not keep detecting,RW*/
static inline uint32_t sdio_ll_get_cmd_keep_det(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value = ((reg_value >> SDIO_CMD_KEEP_DET_POS) & SDIO_CMD_KEEP_DET_MASK);
    return reg_value;
}

static inline void sdio_ll_set_cmd_keep_det(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value &= ~(SDIO_CMD_KEEP_DET_MASK << SDIO_CMD_KEEP_DET_POS);
    reg_value |= ((value & SDIO_CMD_KEEP_DET_MASK) << SDIO_CMD_KEEP_DET_POS);
    REG_WRITE(SDIO_REG0X12_ADDR,reg_value);
}

/* REG_0x10:reg0x10->FIFO_SEND_CNT:0x10[15:8],when set HOST_WR_BLK_EN, tx_fifo cnt = this reg start sending,RW*/
static inline uint32_t sdio_ll_get_fifo_send_cnt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value = ((reg_value >> SDIO_FIFO_SEND_CNT_POS) & SDIO_FIFO_SEND_CNT_MASK);
    return reg_value;
}

static inline void sdio_ll_set_fifo_send_cnt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X12_ADDR);
    reg_value &= ~(SDIO_FIFO_SEND_CNT_MASK << SDIO_FIFO_SEND_CNT_POS);
    reg_value |= ((value & SDIO_FIFO_SEND_CNT_MASK) << SDIO_FIFO_SEND_CNT_POS);
    REG_WRITE(SDIO_REG0X12_ADDR,reg_value);
}

/* REG_0x13:reg0x13->SD_SLAVE_RDAT_0:0x13[31:0],Function REGs; Slave write data to this REG, and Host read it by CMD52.; cmd_s_res_rd_dat_7,cmd_s_res_rd_dat_6,cmd_s_res_rd_dat_5,cmd_s_res_rd_dat_4,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x13_value(void)
{
    return REG_READ(SDIO_REG0X13_ADDR);
}

static inline void sdio_ll_set_reg0x13_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X13_ADDR,value);
}

static inline uint32_t sdio_ll_get_sd_slave_rdat_0(void)
{
    return REG_READ(SDIO_REG0X13_ADDR);
}

static inline void sdio_ll_set_sd_slave_rdat_0(uint32_t value)
{
    REG_WRITE(SDIO_REG0X13_ADDR,value);
}

/* REG_0x14:reg0x14->SD_SLAVE_RDAT_1:0x14[31:0],Function REGs; Slave write data to this REG, and Host read it by CMD52.; cmd_s_res_rd_dat_7,cmd_s_res_rd_dat_6,cmd_s_res_rd_dat_5,cmd_s_res_rd_dat_4,0x0,RW*/
static inline uint32_t sdio_ll_get_reg0x14_value(void)
{
    return REG_READ(SDIO_REG0X14_ADDR);
}

static inline void sdio_ll_set_reg0x14_value(uint32_t value)
{
    REG_WRITE(SDIO_REG0X14_ADDR,value);
}

static inline uint32_t sdio_ll_get_sd_slave_rdat_1(void)
{
    return REG_READ(SDIO_REG0X14_ADDR);
}

static inline void sdio_ll_set_sd_slave_rdat_1(uint32_t value)
{
    REG_WRITE(SDIO_REG0X14_ADDR,value);
}

static inline uint32_t sdio_ll_get_reg0x15_value(void)
{
    return REG_READ(SDIO_REG0X15_ADDR);
}

/* REG_0x15:reg0x15->SD_SLAVE_WDAT_0:0x13[31:0],Function REGs; Slave read data from this REG, and Host write value with address by CMD52.; cmd_s_rec_wr_dat_3,cmd_s_rec_wr_dat_2,cmd_s_rec_wr_dat_1,cmd_s_rec_wr_dat_0,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_wdat_0(void)
{
    return REG_READ(SDIO_REG0X15_ADDR);
}

static inline uint32_t sdio_ll_get_reg0x16_value(void)
{
    return REG_READ(SDIO_REG0X16_ADDR);
}

/* REG_0x16:reg0x14->SD_SLAVE_WDAT_1:0x14[31:0],Function REGs; Slave read data from this REG, and Host write value with address by CMD52.; cmd_s_rec_wr_dat_7,cmd_s_rec_wr_dat_6,cmd_s_rec_wr_dat_5,cmd_s_rec_wr_dat_4,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_wdat_1(void)
{
    return REG_READ(SDIO_REG0X16_ADDR);
}

/* REG_0x17:sd_slave_status R*/
static inline uint32_t sdio_ll_get_sd_slave_status_value(void)
{
    return REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
}

/* REG_0x17:sd_slave_status->cmd_s_res_dat_rd:0x15[0],cmd_s_res_dat_rd  //0; ignore this bit ,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_res_dat_rd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_CMD_S_RES_DAT_RD_POS)&SDIO_SD_SLAVE_STATUS_CMD_S_RES_DAT_RD_MASK);
    return reg_value;
}

/* REG_0x17:sd_slave_status->cmd_s_res_dat_wr:0x15[1],cmd_s_res_dat_wr ,//1 ; ignore this bit ,0x1,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_res_dat_wr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_CMD_S_RES_DAT_WR_POS)&SDIO_SD_SLAVE_STATUS_CMD_S_RES_DAT_WR_MASK);
    return reg_value;
}

/* REG_0x17:sd_slave_status->cmd_s_rec_bb_cnt:0x15[10:2],cmd_s_rec_bb_cnt ,//10:2 ; The length which host write/read.,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_rec_bb_cnt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_CMD_S_REC_BB_CNT_POS)&SDIO_SD_SLAVE_STATUS_CMD_S_REC_BB_CNT_MASK);
    return reg_value;
}

/* REG_0x17:sd_slave_status->cmd_s_rec_op_code:0x15[11],cmd_s_rec_op_code,//11 ; Invalid,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_rec_op_code(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_CMD_S_REC_OP_CODE_POS)&SDIO_SD_SLAVE_STATUS_CMD_S_REC_OP_CODE_MASK);
    return reg_value;
}

/* REG_0x17:sd_slave_status->cmd_s_rec_blk_mod:0x15[12],cmd_s_rec_blk_mod,//12,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_rec_blk_mod(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_CMD_S_REC_BLK_MOD_POS)&SDIO_SD_SLAVE_STATUS_CMD_S_REC_BLK_MOD_MASK);
    return reg_value;
}

/* REG_0x17:sd_slave_status->sd_start_wr_en_r3:0x15[13],sd_start_wr_en_r3,//13; slave write data to line need set SD_START_WR_EN, before set SD_START_WR_EN, ensure this bit is 0, if 1, wait till 0,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_sd_start_wr_en_r3(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_SD_START_WR_EN_R3_POS)&SDIO_SD_SLAVE_STATUS_SD_START_WR_EN_R3_MASK);
    return reg_value;
}

/* REG_0x17:sd_slave_status->dat_s_rd_bus_4rd:0x15[14],dat_s_rd_bus_4rd ,//14; Slave:1:Slave is in busy status,CPU should deal the received data.,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_dat_s_rd_bus_4rd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_DAT_S_RD_BUS_4RD_POS)&SDIO_SD_SLAVE_STATUS_DAT_S_RD_BUS_4RD_MASK);
    return reg_value;
}

/* REG_0x17:sd_slave_status->cmd_s_res_end_4rd:0x15[15],cmd_s_res_end_4rd,//15,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_cmd_s_res_end_4rd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_CMD_S_RES_END_4RD_POS)&SDIO_SD_SLAVE_STATUS_CMD_S_RES_END_4RD_MASK);
    return reg_value;
}

/* REG_0x17:sd_slave_status->dat_s_wr_wai_4rd:0x15[16],dat_s_wr_wai_4rd, //16; wait slave write data finish; 1:write finish,0x0,R*/
static inline uint32_t sdio_ll_get_sd_slave_status_dat_s_wr_wai_4rd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_SD_SLAVE_STATUS_ADDR);
    reg_value = ((reg_value >> SDIO_SD_SLAVE_STATUS_DAT_S_WR_WAI_4RD_POS)&SDIO_SD_SLAVE_STATUS_DAT_S_WR_WAI_4RD_MASK);
    return reg_value;
}

/* sdio cmd auto response by hardware*/
static inline void sdio_ll_cmd_sst_sel_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SDIO_REG0X1A_ADDR);
    reg_value &= ~(SDIO_SD_CMD_IRQ_EX_OP_CMD_SST_SEL_MASK << SDIO_SD_CMD_IRQ_EX_OP_CMD_SST_SEL_POS);
    reg_value |= ((value & SDIO_SD_CMD_IRQ_EX_OP_CMD_SST_SEL_MASK) << SDIO_SD_CMD_IRQ_EX_OP_CMD_SST_SEL_POS);
    REG_WRITE(SDIO_REG0X1A_ADDR,reg_value);
}

static inline void sdio_ll_dat1_int_en(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(SDIO_REG0X1A_ADDR);
	reg_value &= ~(SDIO_SD_CMD_IRQ_EX_OP_DAT_S_DAT1_INT_EN_MASK << SDIO_SD_CMD_IRQ_EX_OP_DAT_S_DAT1_INT_EN_POS);
	reg_value |= ((value & SDIO_SD_CMD_IRQ_EX_OP_DAT_S_DAT1_INT_EN_MASK) << SDIO_SD_CMD_IRQ_EX_OP_DAT_S_DAT1_INT_EN_POS);
	REG_WRITE(SDIO_REG0X1A_ADDR,reg_value);
}

static inline void sdio_ll_cccr_intx_en(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(SDIO_REG0X1E_ADDR);
	reg_value &= ~(SDIO_SD_CCCR_2_INTX_MASK << SDIO_SD_CCCR_2_INTX_POS);
	reg_value |= ((value & SDIO_SD_CCCR_2_INTX_MASK) << SDIO_SD_CCCR_2_INTX_POS);
	REG_WRITE(SDIO_REG0X1E_ADDR,reg_value);
}

static inline void sdio_ll_cccr_s4mi_en(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(SDIO_REG0X1F_ADDR);
	reg_value &= ~(SDIO_SD_CCCR_3_S4MI_MASK << SDIO_SD_CCCR_3_S4MI_POS);
	reg_value |= ((value & SDIO_SD_CCCR_3_S4MI_MASK) << SDIO_SD_CCCR_3_S4MI_POS);
	REG_WRITE(SDIO_REG0X1F_ADDR,reg_value);
}

static inline void sdio_ll_cmd_s_buf_cnt_clr_en(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(SDIO_REG0X1F_ADDR);
	reg_value &= ~(SDIO_SD_CCCR_3_S4MI_MASK << SDIO_SD_CCCR_3_S4MI_POS);
	reg_value |= ((value & SDIO_SD_CCCR_3_S4MI_MASK) << SDIO_SD_CCCR_3_S4MI_POS);
	REG_WRITE(SDIO_REG0X1F_ADDR,reg_value);
}

	/* REG_0x3F */
#define SDIO_REG0X3F_ADDR  (SOC_SDIO_REG_BASE  + 0x3F*4) //REG ADDR :0x458d00fc
#define SDIO_REG0X3F_CMD_S_BUF_CNT_CLR_POS (0)
#define SDIO_REG0X3F_CMD_S_BUF_CNT_CLR_MASK (0x1)

#define SDIO_REG0X3F_CMD_S_BUF_CNT_NUM_POS (1)
#define SDIO_REG0X3F_CMD_S_BUF_CNT_NUM_MASK (0x7)

#define SDIO_REG0X3F_CMD_S_BUF_CNT_POS (4)
#define SDIO_REG0X3F_CMD_S_BUF_CNT_MASK (0x7)

#define SDIO_REG0X3F_CMD_S_BUF_COVER_CLR_POS (7)
#define SDIO_REG0X3F_CMD_S_BUF_COVER_CLR_MASK (0x1)

#define SDIO_REG0X3F_CMD_S_BUF_COVER_POS (8)
#define SDIO_REG0X3F_CMD_S_BUF_COVER_MASK (0x1)

#define SDIO_REG0X3F_CMD_S_BUF_EN_POS (9)
#define SDIO_REG0X3F_CMD_S_BUF_EN_MASK (0x1)

#define SDIO_REG0X3F_CMD_S_CNT_CLR_POS (10)
#define SDIO_REG0X3F_CMD_S_CNT_CLR_MASK (0x1)

#define SDIO_REG0X3F_RESERVED_POS (11)
#define SDIO_REG0X3F_RESERVED_MASK (0x1FFFFF)

	static inline uint32_t sdio_ll_get_reg0x3f_value(void)
	{
		return REG_READ(SDIO_REG0X3F_ADDR);
	}

	static inline void sdio_ll_set_reg0x3f_value(uint32_t value)
	{
		REG_WRITE(SDIO_REG0X3F_ADDR,value);
	}

	/* REG_0x3f:reg0x3f->cmd_s_buf_cnt_clr:0x3f[0],cmd buffer 计数器清零,0x0,RW*/
	static inline uint32_t sdio_ll_get_reg0x3f_cmd_s_buf_cnt_clr(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X3F_CMD_S_BUF_CNT_CLR_POS) & SDIO_REG0X3F_CMD_S_BUF_CNT_CLR_MASK);
		return reg_value;
	}

	static inline void sdio_ll_set_reg0x3f_cmd_s_buf_cnt_clr(uint32_t value)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value &= ~(SDIO_REG0X3F_CMD_S_BUF_CNT_CLR_MASK << SDIO_REG0X3F_CMD_S_BUF_CNT_CLR_POS);
		reg_value |= ((value & SDIO_REG0X3F_CMD_S_BUF_CNT_CLR_MASK) << SDIO_REG0X3F_CMD_S_BUF_CNT_CLR_POS);
		REG_WRITE(SDIO_REG0X3F_ADDR,reg_value);
	}

	/* REG_0x3f:reg0x3f->cmd_s_buf_cnt_num:0x3f[3:1],设置cmd buffer 缓存深度，最大深度值为5; 缓存深度=cmd_s_buf_cnt_num+1,0x0,RW*/
	static inline uint32_t sdio_ll_get_reg0x3f_cmd_s_buf_cnt_num(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X3F_CMD_S_BUF_CNT_NUM_POS) & SDIO_REG0X3F_CMD_S_BUF_CNT_NUM_MASK);
		return reg_value;
	}

	static inline void sdio_ll_set_reg0x3f_cmd_s_buf_cnt_num(uint32_t value)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value &= ~(SDIO_REG0X3F_CMD_S_BUF_CNT_NUM_MASK << SDIO_REG0X3F_CMD_S_BUF_CNT_NUM_POS);
		reg_value |= ((value & SDIO_REG0X3F_CMD_S_BUF_CNT_NUM_MASK) << SDIO_REG0X3F_CMD_S_BUF_CNT_NUM_POS);
		REG_WRITE(SDIO_REG0X3F_ADDR,reg_value);
	}

	/* REG_0x3f:reg0x3f->cmd_s_buf_cnt:0x3f[6:4],cmd buffer 缓存个数; 0：1个缓存的CMD; 1：2个缓存的CMD; 2：3个缓存的CMD; 3：4个缓存的CMD; 4：5个缓存的CMD ,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x3f_cmd_s_buf_cnt(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X3F_CMD_S_BUF_CNT_POS)&SDIO_REG0X3F_CMD_S_BUF_CNT_MASK);
		return reg_value;
	}

	/* REG_0x3f:reg0x3f->cmd_s_buf_cover_clr:0x3f[7],cmd buffer 溢出标志位清零,0x0,RW*/
	static inline uint32_t sdio_ll_get_reg0x3f_cmd_s_buf_cover_clr(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X3F_CMD_S_BUF_COVER_CLR_POS) & SDIO_REG0X3F_CMD_S_BUF_COVER_CLR_MASK);
		return reg_value;
	}

	static inline void sdio_ll_set_reg0x3f_cmd_s_buf_cover_clr(uint32_t value)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value &= ~(SDIO_REG0X3F_CMD_S_BUF_COVER_CLR_MASK << SDIO_REG0X3F_CMD_S_BUF_COVER_CLR_POS);
		reg_value |= ((value & SDIO_REG0X3F_CMD_S_BUF_COVER_CLR_MASK) << SDIO_REG0X3F_CMD_S_BUF_COVER_CLR_POS);
		REG_WRITE(SDIO_REG0X3F_ADDR,reg_value);
	}

	/* REG_0x3f:reg0x3f->cmd_s_buf_cover:0x3f[8],cmd buffer 溢出标志位,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x3f_cmd_s_buf_cover(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X3F_CMD_S_BUF_COVER_POS)&SDIO_REG0X3F_CMD_S_BUF_COVER_MASK);
		return reg_value;
	}

	/* REG_0x3f:reg0x3f->cmd_s_buf_en:0x3f[9],cmd buffer enable; 注：先把cmd_s_buf_cnt_num深度值设置完成，再使能cmd_s_buf_en,0x0,RW*/
	static inline uint32_t sdio_ll_get_reg0x3f_cmd_s_buf_en(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X3F_CMD_S_BUF_EN_POS) & SDIO_REG0X3F_CMD_S_BUF_EN_MASK);
		return reg_value;
	}

	static inline void sdio_ll_set_reg0x3f_cmd_s_buf_en(uint32_t value)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value &= ~(SDIO_REG0X3F_CMD_S_BUF_EN_MASK << SDIO_REG0X3F_CMD_S_BUF_EN_POS);
		reg_value |= ((value & SDIO_REG0X3F_CMD_S_BUF_EN_MASK) << SDIO_REG0X3F_CMD_S_BUF_EN_POS);
		REG_WRITE(SDIO_REG0X3F_ADDR,reg_value);
	}

	/* REG_0x3f:reg0x3f->cmd_s_cnt_clr:0x3f[10],cmd计数器清零,0x0,RW*/
	static inline uint32_t sdio_ll_get_reg0x3f_cmd_s_cnt_clr(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X3F_CMD_S_CNT_CLR_POS) & SDIO_REG0X3F_CMD_S_CNT_CLR_MASK);
		return reg_value;
	}

	static inline void sdio_ll_set_reg0x3f_cmd_s_cnt_clr(uint32_t value)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X3F_ADDR);
		reg_value &= ~(SDIO_REG0X3F_CMD_S_CNT_CLR_MASK << SDIO_REG0X3F_CMD_S_CNT_CLR_POS);
		reg_value |= ((value & SDIO_REG0X3F_CMD_S_CNT_CLR_MASK) << SDIO_REG0X3F_CMD_S_CNT_CLR_POS);
		REG_WRITE(SDIO_REG0X3F_ADDR,reg_value);
	}

	/* REG_0x40 */
#define SDIO_REG0X40_ADDR  (SOC_SDIO_REG_BASE  + 0x40*4) //REG ADDR :0x458d0100
#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF0_POS (0)
#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF0_MASK (0x3F)

#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF1_POS (6)
#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF1_MASK (0x3F)

#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF2_POS (12)
#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF2_MASK (0x3F)

#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF3_POS (18)
#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF3_MASK (0x3F)

#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF4_POS (24)
#define SDIO_REG0X40_CMD_S_REC_INDEX_BUF4_MASK (0x3F)

#define SDIO_REG0X40_RESERVED_POS (30)
#define SDIO_REG0X40_RESERVED_MASK (0x3)

	static inline uint32_t sdio_ll_get_reg0x40_value(void)
	{
		return REG_READ(SDIO_REG0X40_ADDR);
	}

	/* REG_0x40:reg0x40->cmd_s_rec_index_buf0:0x40[5:0],缓存接收到的第1个CMD index,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x40_cmd_s_rec_index_buf0(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X40_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X40_CMD_S_REC_INDEX_BUF0_POS)&SDIO_REG0X40_CMD_S_REC_INDEX_BUF0_MASK);
		return reg_value;
	}

	/* REG_0x40:reg0x40->cmd_s_rec_index_buf1:0x40[11:6],缓存接收到的第2个CMD index,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x40_cmd_s_rec_index_buf1(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X40_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X40_CMD_S_REC_INDEX_BUF1_POS)&SDIO_REG0X40_CMD_S_REC_INDEX_BUF1_MASK);
		return reg_value;
	}

	/* REG_0x40:reg0x40->cmd_s_rec_index_buf2:0x40[17:12],缓存接收到的第3个CMD index,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x40_cmd_s_rec_index_buf2(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X40_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X40_CMD_S_REC_INDEX_BUF2_POS)&SDIO_REG0X40_CMD_S_REC_INDEX_BUF2_MASK);
		return reg_value;
	}

	/* REG_0x40:reg0x40->cmd_s_rec_index_buf3:0x40[23:18],缓存接收到的第4个CMD index,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x40_cmd_s_rec_index_buf3(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X40_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X40_CMD_S_REC_INDEX_BUF3_POS)&SDIO_REG0X40_CMD_S_REC_INDEX_BUF3_MASK);
		return reg_value;
	}

	/* REG_0x40:reg0x40->cmd_s_rec_index_buf4:0x40[29:24],缓存接收到的第5个CMD index,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x40_cmd_s_rec_index_buf4(void)
	{
		uint32_t reg_value;
		reg_value = REG_READ(SDIO_REG0X40_ADDR);
		reg_value = ((reg_value >> SDIO_REG0X40_CMD_S_REC_INDEX_BUF4_POS)&SDIO_REG0X40_CMD_S_REC_INDEX_BUF4_MASK);
		return reg_value;
	}

	/* REG_0x41 */
#define SDIO_REG0X41_ADDR  (SOC_SDIO_REG_BASE  + 0x41*4) //REG ADDR :0x458d0104
#define SDIO_REG0X41_CMD_S_REC_ARGUMENT_BUF0_POS (0)
#define SDIO_REG0X41_CMD_S_REC_ARGUMENT_BUF0_MASK (0xFFFFFFFF)

	static inline uint32_t sdio_ll_get_reg0x41_value(void)
	{
		return REG_READ(SDIO_REG0X41_ADDR);
	}

	/* REG_0x41:reg0x41->cmd_s_rec_argument_buf0:0x41[31:0],缓存接收到的第1个CMD argument,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x41_cmd_s_rec_argument_buf0(void)
	{
		return REG_READ(SDIO_REG0X41_ADDR);
	}

	/* REG_0x42 */
#define SDIO_REG0X42_ADDR  (SOC_SDIO_REG_BASE  + 0x42*4) //REG ADDR :0x458d0108
#define SDIO_REG0X42_CMD_S_REC_ARGUMENT_BUF1_POS (0)
#define SDIO_REG0X42_CMD_S_REC_ARGUMENT_BUF1_MASK (0xFFFFFFFF)

	static inline uint32_t sdio_ll_get_reg0x42_value(void)
	{
		return REG_READ(SDIO_REG0X42_ADDR);
	}

	/* REG_0x42:reg0x42->cmd_s_rec_argument_buf1:0x42[31:0],缓存接收到的第2个CMD argument,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x42_cmd_s_rec_argument_buf1(void)
	{
		return REG_READ(SDIO_REG0X42_ADDR);
	}

	/* REG_0x43 */
#define SDIO_REG0X43_ADDR  (SOC_SDIO_REG_BASE  + 0x43*4) //REG ADDR :0x458d010c
#define SDIO_REG0X43_CMD_S_REC_ARGUMENT_BUF2_POS (0)
#define SDIO_REG0X43_CMD_S_REC_ARGUMENT_BUF2_MASK (0xFFFFFFFF)

	static inline uint32_t sdio_ll_get_reg0x43_value(void)
	{
		return REG_READ(SDIO_REG0X43_ADDR);
	}

	/* REG_0x43:reg0x43->cmd_s_rec_argument_buf2:0x43[31:0],缓存接收到的第3个CMD argument,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x43_cmd_s_rec_argument_buf2(void)
	{
		return REG_READ(SDIO_REG0X43_ADDR);
	}

	/* REG_0x44 */
#define SDIO_REG0X44_ADDR  (SOC_SDIO_REG_BASE  + 0x44*4) //REG ADDR :0x458d0110
#define SDIO_REG0X44_CMD_S_REC_ARGUMENT_BUF3_POS (0)
#define SDIO_REG0X44_CMD_S_REC_ARGUMENT_BUF3_MASK (0xFFFFFFFF)

	static inline uint32_t sdio_ll_get_reg0x44_value(void)
	{
		return REG_READ(SDIO_REG0X44_ADDR);
	}

	/* REG_0x44:reg0x44->cmd_s_rec_argument_buf3:0x44[31:0],缓存接收到的第4个CMD argument,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x44_cmd_s_rec_argument_buf3(void)
	{
		return REG_READ(SDIO_REG0X44_ADDR);
	}

	/* REG_0x45 */
#define SDIO_REG0X45_ADDR  (SOC_SDIO_REG_BASE  + 0x45*4) //REG ADDR :0x458d0114
#define SDIO_REG0X45_CMD_S_REC_ARGUMENT_BUF4_POS (0)
#define SDIO_REG0X45_CMD_S_REC_ARGUMENT_BUF4_MASK (0xFFFFFFFF)

	static inline uint32_t sdio_ll_get_reg0x45_value(void)
	{
		return REG_READ(SDIO_REG0X45_ADDR);
	}

	/* REG_0x45:reg0x45->cmd_s_rec_argument_buf4:0x45[31:0],缓存接收到的第5个CMD argument,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x45_cmd_s_rec_argument_buf4(void)
	{
		return REG_READ(SDIO_REG0X45_ADDR);
	}

	/* REG_0x46 */
#define SDIO_REG0X46_ADDR  (SOC_SDIO_REG_BASE  + 0x46*4) //REG ADDR :0x458d0118
#define SDIO_REG0X46_CMD_S_CNT_POS (0)
#define SDIO_REG0X46_CMD_S_CNT_MASK (0xFFFFFFFF)

	static inline uint32_t sdio_ll_get_reg0x46_value(void)
	{
		return REG_READ(SDIO_REG0X46_ADDR);
	}

	/* REG_0x46:reg0x46->cmd_s_cnt:0x46[31:0],cmd 接收数量计数器,0x0,R*/
	static inline uint32_t sdio_ll_get_reg0x46_cmd_s_cnt(void)
	{
		return REG_READ(SDIO_REG0X46_ADDR);
	}

#ifdef __cplusplus
}
#endif
