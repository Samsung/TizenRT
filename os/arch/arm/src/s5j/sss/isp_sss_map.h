/*!
 *  @file       isp_sss_map.h
 *  @brief      Headerfile : sss sfr address map for isp
 *  @author     jinsu.hyun
 *  @version	v0.01 : 2016.7.7
 *  @version	v0.10 : 2016.7.15 pre-beta release
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef ISP_SSS_MAP_H_
#define ISP_SSS_MAP_H_

#define MB_STATUS						(*(volatile u32 *)(MB_REG_BASE + 0x0000))

#ifdef CONFIG_S5J_SILICON
#define MB_REG_BASE						(0x800E0000)
#define MB_CM0_HRESET                                           (*(volatile u32 *)(MB_REG_BASE + 0x0004))
#define MB_CM0_SRAM_ACC_CON                                     (*(volatile u32 *)(MB_REG_BASE + 0x0008))
#define MB_CM0_DBGCON                                           (*(volatile u32 *)(MB_REG_BASE + 0x0034))
#else
#define MB_REG_BASE						(0x70020000)
#endif

#define CTRL_FIELD_BASE					(MB_REG_BASE + 0x0100)
#define DATA_FIELD_BASE					(MB_REG_BASE + 0x0110)

#define CTRL_FIELD_ADDR(val)			(CTRL_FIELD_BASE+(val<<2))
#define DATA_FIELD_ADDR(val)			(DATA_FIELD_BASE+(val<<2))

#define CTRL_FIELD(val)					(*(volatile u32 *)(CTRL_FIELD_ADDR(val)))
#define DATA_FIELD(val)					(*(volatile u32 *)(DATA_FIELD_ADDR(val)))

#define ISP_DATA_FIELD_SET(index, value)	DATA_FIELD(index) = value
#define ISP_DATA_FIELD_GET(index, value)	value = DATA_FIELD(index)
#define ISP_CTRL_FIELD_SET(index, value)	CTRL_FIELD(index) = value
#define ISP_CTRL_FIELD_GET(index, value)	value = CTRL_FIELD(index)

#endif							/* ISP_SSS_MAP_H_ */
