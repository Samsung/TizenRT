#ifndef _AMEBAD2_CHIPINFO_LIMIT_H_
#define _AMEBAD2_CHIPINFO_LIMIT_H_

/* This File shall hidden for customer */
/************* 0x7FC**************/

/**********[7:4]***********/
/*C = bit[5] xor bit[7]*/
/*D = bit[4] xor bit[6]*/
/*CD 00: 1.4G*/
/*CD 01: 1.2G*/
/*CD 10: 1G*/
/*CD 11: 800M*/

/**********[3:0]***********/
/*A = bit[1] xor bit[3]*/
/*B = bit[0] xor bit[2]*/
/*AB 00: A32*2*/
/*AB 01: A32*1*/
/*AB 10: A32*0*/
/*AB 11: RSVD*/

#define CHIP_FUNCTION_LIMIT_GROUP1				0x7FC
#define SYSCFG_GET_CPU_CORENUM_CFG(x)			((u32)(((x >> 4) & 0x0000000F)))
#define SYSCFG_GET_CPU_CLOCKRATE_CFG(x)			((u32)(((x >> 0) & 0x0000000F)))

#define CHIP_CORE_NUM2							0x2
#define CHIP_CORE_NUM1							0x1
#define CHIP_CORE_NUM0							0x0
#define CHIP_CORE_NUMERROR						0xFF

#define CHIP_CPU_1400							0x3
#define CHIP_CPU_1200							0x2
#define CHIP_CPU_1000							0x1
#define CHIP_CPU_800							0x0

/************* 0x7FD**************/

/***********[7:6]************/
/*RSVD*/

/**********[5:4]***********/
/*11 or 00 Original size*/
/*01 or 10 reduce size to half*/

/**********[3:2]***********/
/*11 or 00 WIFI 11AX En*/
/*01 or 10 WIFI 11n only*/

/**********[1:0]***********/
/*11 or 00 dual band:2.4G+5G*/
/*01 or 10 single band:2.4G*/

#define CHIP_FUNCTION_LIMIT_GROUP2				0x7FD
#define SYSCFG_GET_MEMSIZE_CFG(x)				((u32)(((x >> 4) & 0x00000003)))
#define SYSCFG_GET_WIFI_CFG(x)					((u32)(((x >> 2) & 0x00000003)))
#define SYSCFG_GET_WIFIBAND_CFG(x)				((u32)(((x >> 0) & 0x00000003)))


/*
  * @brief  Check whether DTCM in DSP is shared in OTP
  * @retval core num
  */
__STATIC_INLINE u32 ChipInfo_CpuCoreNum(void)
{
	u8 OTPinfo;
	u8 Atemp, Btemp;

	OTP_Read8(CHIP_FUNCTION_LIMIT_GROUP1, &OTPinfo);

	/*A = bit[1] xor bit[3]*/
	/*B = bit[0] xor bit[2]*/
	/*AB 00: A32*2*/
	/*AB 01: A32*1*/
	/*AB 10: A32*0*/
	/*AB 11: RSVD*/
	OTPinfo = SYSCFG_GET_CPU_CORENUM_CFG(OTPinfo);
	Atemp = ((OTPinfo >> 3) & 0x1) ^ ((OTPinfo >> 1) & 0x1);
	Btemp = ((OTPinfo >> 2) & 0x1) ^ ((OTPinfo >> 0) & 0x1);

	OTPinfo = Atemp << 1 | Btemp;
	if (OTPinfo == 0) {
		return CHIP_CORE_NUM2;
	} else if (OTPinfo == 1) {
		return CHIP_CORE_NUM1;
	} else if (OTPinfo == 2) {
		return CHIP_CORE_NUM0;
	} else {
		return CHIP_CORE_NUMERROR;
	}
}

/**
  * @brief  Cpu CLock rate
  * @retval Clock Rate
  */
__STATIC_INLINE u32 ChipInfo_CpuClock(void)
{
	u8 OTPinfo;
	u8 Atemp, Btemp;

	OTP_Read8(CHIP_FUNCTION_LIMIT_GROUP1, &OTPinfo);

	/*C = bit[5] xor bit[7]*/
	/*D = bit[4] xor bit[6]*/
	/*CD 00: 1.4G*/
	/*CD 01: 1.2G*/
	/*CD 10: 1G*/
	/*CD 11: 800M*/
	OTPinfo = SYSCFG_GET_CPU_CLOCKRATE_CFG(OTPinfo);
	Atemp = ((OTPinfo >> 3) & 0x1) ^ ((OTPinfo >> 1) & 0x1);
	Btemp = ((OTPinfo >> 2) & 0x1) ^ ((OTPinfo >> 0) & 0x1);

	OTPinfo = Atemp << 1 | Btemp;
	if (OTPinfo == 0) {
		return CHIP_CPU_1400;
	} else if (OTPinfo == 1) {
		return CHIP_CPU_1200;
	} else if (OTPinfo == 2) {
		return CHIP_CPU_1000;
	} else if (OTPinfo == 3) {
		return CHIP_CPU_800;
	}

}

/**
  * @brief  Check DDR Memory Size
  * @retval
  * TRUE: Origin
  * FALSE: HALF
  */
__STATIC_INLINE u32 ChipInfo_MemorySizeOrigin(void)
{
	u8 OTPinfo;

	OTP_Read8(CHIP_FUNCTION_LIMIT_GROUP2, &OTPinfo);

	// Bit[5] xor Bit[4] = 1'b 0:  origin size
	// Bit[5] xor Bit[4] = 1'b 1:  half size
	if ((SYSCFG_GET_MEMSIZE_CFG(OTPinfo) == 3) || (SYSCFG_GET_MEMSIZE_CFG(OTPinfo) == 0)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief  Check WIFI protocol in OTP
  * @retval
  * TRUE: Support
  * FALSE: Do not support
  */
__STATIC_INLINE u32 ChipInfo_WIFI6Support(void)
{
	u8 OTPinfo;

	OTP_Read8(CHIP_FUNCTION_LIMIT_GROUP2, &OTPinfo);

	// Bit[3] xor Bit[2] = 1'b 0:  11ax
	// Bit[3] xor Bit[2] = 1'b 1:  11n only
	if ((SYSCFG_GET_WIFI_CFG(OTPinfo) == 3) || (SYSCFG_GET_WIFI_CFG(OTPinfo) == 0)) {
		return TRUE;
	} else {
		return FALSE;
	}
}


/**
  * @brief  Check WIFI 5G band support
  * @retval
  * TRUE: Support
  * FALSE: Do not support
  */
__STATIC_INLINE u32 ChipInfo_WIFI5GSupport(void)
{
	u8 OTPinfo;

	OTP_Read8(CHIP_FUNCTION_LIMIT_GROUP2, &OTPinfo);

	// Bit[1] xor Bit[0] = 1'b 0:  5G support
	// Bit[1] xor Bit[0] = 1'b 1:  2.4G only
	if ((SYSCFG_GET_WIFIBAND_CFG(OTPinfo) == 3) || (SYSCFG_GET_WIFIBAND_CFG(OTPinfo) == 0)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

#endif
