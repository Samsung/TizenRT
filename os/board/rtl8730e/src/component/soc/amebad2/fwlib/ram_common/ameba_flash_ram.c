/**
  ******************************************************************************
  * @file    rtl8721d_flash_ram.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the flash RAM functions.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"
#include <arch/barriers.h>
#include <arch/irq.h>
#include <debug.h>
#include <assert.h>

FLASH_InitTypeDef flash_init_para;
static uint32_t PrevIrqStatus;

/* Flash write-gate diagnostics: the KM4/NP handshake of
 * FLASH_Write_Lock() and the SPIC/flash busy waits used to be
 * unbounded.  They run with interrupts masked and the peer core
 * paused, so a missed NP acknowledge (IPC channel not up yet during
 * boot, KM4 not servicing the channel) or a flash that never leaves
 * busy wedges the AP in total silence.  Bound every one of them and
 * report through the assert machinery; the bound is far above the
 * longest legitimate wait (sector erase is in the 100ms range).
 */

#define FLASHGATE_SPIN_LIMIT   0x04000000

/* Latched fault code / offending register value of a bounded wait
 * that ran out inside the user-mode window: such a wait cannot
 * report inline (the flash may be stuck busy and the SPIC owned by
 * user mode), so it is reported from FLASH_Write_Unlock() once the
 * window is closed.
 */
static volatile u8 g_flash_busy_timeout;
static volatile u32 g_flash_busy_timeout_val;

/* Write-window milestone flags (file scope so the unlock side can see
 * the open side): gate_open_done is set when FLASH_UserMode_Enter()
 * first reached the command phase; gate_closed_logged arms the one-shot
 * "closed" milestone in FLASH_Write_Unlock().
 */
static volatile u8 gate_open_done;
static volatile u8 gate_closed_logged;

/* Deferred "open" milestone: SPIC sits in USER_MODE between
 * FLASH_UserMode_Enter() and FLASH_UserMode_Exit(), and nothing in
 * this file may touch the XIP flash inside that window -- neither
 * code nor data: an instruction fetch faults, and so does a load of
 * a .rodata string or const table, with vectors and handlers
 * themselves XIP-resident making either fault a silent lockup.  The
 * milestone is therefore only latched inside the window; the line is
 * printed from FLASH_Write_Unlock() once auto mode and interrupts
 * are back.
 */
static volatile u8 gate_open_pending;
static volatile u32 gate_open_cpustatus;

/* Timeout of a pre-window wait (stage 1: KM4 doorbell TX bit never
 * freed; stage 2: KM4 acknowledgement never arrived).  Both run
 * before the user-mode window opens, with the flash controller idle
 * and in auto mode, so the XIP console and assert machinery are safe
 * to use here.  Report the raw state and panic: continuing with a
 * half-open write window (peer cores paused, interrupts masked, KM4
 * unaware) would let a flash command go out unsynchronized.
 */
static void flashgate_timeout(u8 stage, u32 val)
{
	lowsyslog("FLASHGATE TIMEOUT: stage=%d val=0x%08X\n",
	          stage, (unsigned int)val);
	PANIC();

	/* Not reached while assertions are linked in (PANIC halts in
	 * up_assert).  Must never return anyway.
	 */
	for (;;) {
	}
}

/* Faults from bounded waits inside the user-mode window are latched,
 * never printed: every console path (lowsyslog, up_lowputc,
 * LOGUART_PutChar, even the RAM-resident LOGUART_PutChar_RAM which
 * reads its FIFO index flags from an XIP const table) would fetch
 * from the flash the window owns or that may be stuck busy, and with
 * XIP-resident vectors such a fault is a silent lockup.
 * FLASH_Write_Unlock() prints the latch once auto mode and
 * interrupts are back.
 *
 * Latched codes:
 * 1/3/4 = FLASH_WaitBusy_InUserMode() WaitType (flash busy, write
 * enable, transfer), 100 = same helper with WaitType 0 (SPIC-side
 * busy), 11 = peer core never entered the WFE pause, 12 = SPIC
 * refused user-mode entry, 13 = SPIC stuck in user mode, 14 = RX FIFO
 * never filled, 15 = TX FIFO never drained.
 */
SRAMDRAM_ONLY_TEXT_SECTION
static void flashgate_fail(u8 code, u32 regval)
{
	if (g_flash_busy_timeout == 0) {
		g_flash_busy_timeout = (code == 0) ? 100 : code;
		g_flash_busy_timeout_val = regval;
	}
}

void FLASH_WaitBusy_InUserMode(u32 WaitType);
void FLASH_TxCmd_InUserMode(u8 cmd, u8 DataPhaseLen, u8 *pData);
void FLASH_UserMode_Enter(void);
void FLASH_UserMode_Exit(void);

#define FLASH_GATE_USE_CKE	DISABLE

#ifdef ARM_CORE_CM4
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_Write_IPC_Int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	__disable_irq();

	PIPC_MSG_STRUCT ipc_msg = (PIPC_MSG_STRUCT)ipc_get_message(IPC_AP_TO_NP, IPC_A2N_FLASHPG_REQ);
	u8 *pflag = (u8 *)ipc_msg->msg;
	/* Sent Flag to let ca32 know */
	pflag[0] = 1;
	DCache_Clean((u32)pflag, sizeof(pflag));

	/* Wait ca32 program done */
	while (1) {
		DCache_Invalidate((u32)pflag, sizeof(pflag));
		if (pflag[0] == 0) {
			break;
		}
	}

	__enable_irq();
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE ipc_flashpg_table[] = {
	{
		.USER_MSG_TYPE = IPC_USER_DATA,
		.Rxfunc = FLASH_Write_IPC_Int,
		.RxIrqData = (VOID *) NULL,
		.Txfunc = NULL,
		.TxIrqData = (VOID *) NULL,
		.IPC_Direction = IPC_AP_TO_NP,
		.IPC_Channel = IPC_A2N_FLASHPG_REQ
	}
};
#endif

ALIGNMTO(CACHE_LINE_SIZE) u8 Flash_Sync_Flag[CACHE_LINE_ALIGMENT(64)];

/**
  * @brief  This function is used to lock CPU when write or erase flash under XIP.
  * @note
  *		- all interrupt include systick will be stopped.
  * @retval none
  */
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_Write_Lock(void)
{
	/* Mask this core's interrupts only.  enter_critical_section() must
	 * NOT be used here: it holds the kernel-wide g_cpu_irqlock across
	 * the up_cpu_pause_all() handshake below.  Any work on the other
	 * core that needs a critical section (timer tick, workqueue, FTL)
	 * then spins on that lock with interrupts disabled, deaf to the
	 * pause SGI, while this core waits for that core's pause
	 * acknowledgement: a guaranteed deadlock (the old TizenRT kernel
	 * broke it with pause-aware spin-waits in irq_csection.c, the
	 * NuttX 12.13 kernel does not).  A local mask is sufficient: the
	 * other core is frozen by up_cpu_pause_all() before the flash is
	 * touched, and concurrent flash users are serialized by the
	 * RT_DEV_LOCK_FLASH device mutex.
	 */
	PrevIrqStatus = up_irq_save();

	/* Add This Code For XIP when ca32 Program Flah */
#if (defined(ARM_CORE_CA32) && defined(CONFIG_XIP_FLASH))
#if (defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1)
	/*1. All cores other than the lock holder will be paused to prevent access while XIP */
	up_cpu_pause_all();
#endif
#if FLASH_GATE_USE_CKE
	/*2. Disable KM4 clock */
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0, HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0) & ~APBPeriph_NP_CLOCK);
#else
	/*2. Sent IPC to KM4 */
	IPC_MSG_STRUCT ipc_msg_temp;
	u32 gate_n;
	static u8 s_gate_armed;

	if (!s_gate_armed) {
		s_gate_armed = 1;
		/* First write window of this boot: no flash command is in
		 * flight yet, so the XIP console output is safe here.  If
		 * this line is ever missing from a boot log, the AP wedged
		 * before its first flash write and the gate is innocent.
		 */
		lowsyslog("FLASHGATE: armed, first write-lock window opening\n");
	}

	memset(Flash_Sync_Flag, 0, sizeof(Flash_Sync_Flag));
	DCache_Clean((u32)Flash_Sync_Flag, sizeof(Flash_Sync_Flag));

	ipc_msg_temp.msg_type = IPC_USER_POINT;
	ipc_msg_temp.msg = (u32)Flash_Sync_Flag;
	ipc_msg_temp.msg_len = 1;
	ipc_msg_temp.rsvd = 0; /* for coverity init issue */

	/* Stage 1: ring the KM4 doorbell directly.  ipc_send_message()
	 * must NOT be called from this IRQ-masked window: when the
	 * channel TX bit is busy it calls IPC_wait_idle(), whose
	 * task-context branch blocks on a semaphore for
	 * IPC_SEMA_MAX_DELAY (0xFFFFFFFF ms) waiting for an IPC
	 * completion interrupt this section can never service: a
	 * forever-block no caller-side bound can see.  Replicate the
	 * driver's exact wire protocol instead (same message-table slot
	 * at __km0_ipc_memory_start__, same DCache_Clean, same
	 * IPC_TX_DATA trigger write), with a bounded local wait for the
	 * TX bit: expiry re-arms up to ten times, then reports stage=1
	 * through the assert machinery while the flash is still idle
	 * (XIP readable).
	 */
	{
		extern u8 __km0_ipc_memory_start__[];
		PIPC_MSG_STRUCT msg_entry = (PIPC_MSG_STRUCT)__km0_ipc_memory_start__ +
			(IPC_TX_CHANNEL_NUM * IPC_TX_CHANNEL_SWITCH(IPC_AP_TO_NP) +
			 IPC_TX0_CHANNEL_NUM * IPC_TX0_CHANNEL_SWITCH(IPC_AP_TO_NP) +
			 IPC_A2N_FLASHPG_REQ);
		const u32 db_bit = (u32)BIT(IPC_A2N_FLASHPG_REQ);
		u32 db_wait;

		gate_n = 10;
		db_wait = FLASHGATE_SPIN_LIMIT;
		while (IPCAP_DEV->IPC_TX_DATA & db_bit) {
			if (--db_wait == 0) {
				if (--gate_n == 0) {
					flashgate_timeout(1, IPCAP_DEV->IPC_TX_DATA);
				}
				db_wait = FLASHGATE_SPIN_LIMIT;
			}
		}

		*msg_entry = ipc_msg_temp;
		DCache_Clean((u32)msg_entry, sizeof(IPC_MSG_STRUCT));
		IPCAP_DEV->IPC_TX_DATA = db_bit;
	}

	/* Stage 2: KM4 took the request but must set
	 * Flash_Sync_Flag[0] from its channel handler before the AP may
	 * touch the flash.  Bounded for the same reason.
	 */
	gate_n = FLASHGATE_SPIN_LIMIT;
	while (1) {
		DCache_Invalidate((u32)Flash_Sync_Flag, sizeof(Flash_Sync_Flag));
		if (Flash_Sync_Flag[0]) {
			break;
		}
		if (--gate_n == 0) {
			flashgate_timeout(2, Flash_Sync_Flag[0]);
		}
	}
#endif
#endif
}
/**
  * @brief  This function is used to unlock CPU after write or erase flash under XIP.
  * @note
  *		- all interrupt will be restored.
  * @retval none
  */
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_Write_Unlock(void)
{
#if (defined(ARM_CORE_CA32) && defined(CONFIG_XIP_FLASH))
	/*1. Let KM4 Go */
#if FLASH_GATE_USE_CKE
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0, HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0) | APBPeriph_NP_CLOCK);
#else
	Flash_Sync_Flag[0] = 0;
	DCache_Clean((u32)Flash_Sync_Flag, sizeof(Flash_Sync_Flag));
#endif

#if (defined(CONFIG_SMP) && CONFIG_SMP_NCPUS > 1)
	/*2. Resume all secondary cores */
	up_cpu_resume_all();
#endif
#endif

	/* restore irq */
	up_irq_restore(PrevIrqStatus);

	/* Deferred "open" line: safe now, auto mode and interrupts are
	 * back (see gate_open_pending comment).
	 */
	if (gate_open_pending) {
		gate_open_pending = 0;
		lowsyslog("FLASHGATE: open (cpu_status=0x%08lx), first flash command phase started\n",
			  (unsigned long)gate_open_cpustatus);
	}

	/* Deferred report for a FLASH_WaitBusy_InUserMode() that ran out
	 * of patience inside the closed window: latch first, then speak
	 * once interrupts are back and the flash is expected idle again.
	 */
	if (g_flash_busy_timeout) {
		u8 waittype = g_flash_busy_timeout;
		u32 waitval = g_flash_busy_timeout_val;

		g_flash_busy_timeout = 0;
		lowsyslog("FLASH BUSY TIMEOUT: type=%d val=0x%08X\n",
		          waittype, (unsigned int)waitval);
		PANIC();
	}

	/* One-shot milestone: the complete lock -> user-mode -> command
	 * -> busy -> unlock cycle ran through once without latching any
	 * bounded-wait fault.  If a boot log shows "open" but never
	 * "closed", the wedge is inside the first command sequence
	 * itself (erase/write phases, bounded reports cover them).
	 */
	if (gate_open_done && !gate_closed_logged) {
		gate_closed_logged = 1;
		lowsyslog("FLASHGATE: closed, first write window completed OK\n");
	}
}

/**
* @brief  This function is used to send Rx command to flash to get status register or flash id, and lock CPU when Rx
* @param  cmd: command that need to be sent.
* @param  read_len: the number of bytes that will be read by SPIC after sending cmd.
* @param  read_data: pointer to a byte array which is used to save received data.
* @note This function is only used for rx status/flash id ,not used for read flash data.
*		Only work in OneBitMode.
* @retval none
*/
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_RxCmdXIP(u8 cmd, u32 read_len, u8 *read_data)
{
	FLASH_Write_Lock();

	FLASH_RxCmd(cmd, read_len, read_data);

	FLASH_Write_Unlock();
}
#endif

/**
  * @brief  FLASH_SetStatus used to set register status. FLASH_WriteEn & FLASH_WaitBusy, and lock CPU when set
  *  		are included in this function to avoid hardfault when TxCmd in XIP
  * @param    Cmd: command to be sent
  * @param    Len: the number of bytes to be sent after Cmd
  * @param    Status: pointer to byte array to be sent
  * @retval     none
  */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_SetStatusXIP(u8 Cmd, u32 Len, u8 *Status)
{
	FLASH_Write_Lock();

	FLASH_SetStatus(Cmd, Len, Status);

	FLASH_Write_Unlock();
}
#endif

/**
  * @brief  FLASH_SetStatusBits set or clear status bits., used to set protect bit or quad enable bit, and lock CPU when set
  * @param    SetBits: 16bits valid, SetBits[7:0] is status1 & SetBits[15:8] is status2
  * @param    NewState: ENABLE/DISABLE
  * @retval none
  */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_SetStatusBitsXIP(u32 SetBits, u32 NewState)
{
	FLASH_Write_Lock();

	FLASH_SetStatusBits(SetBits, NewState);

	FLASH_Write_Unlock();
}
#endif

/**
  * @brief  This function is used to write data to flash in OneBitMode and User Mode, and lock CPU when write.
  * @param  StartAddr: Start address in flash from which SPIC writes.
  * @param  DataPhaseLen: the number of bytes that SPIC sends in Data Phase.
  * @param  pData: pointer to a byte array that is to be sent.
  * @note
  *		- page program(256B) time typical is 0.7ms: BaudRate=2.9Mbps, so one bit mode is enough.
  *		- page program(12B) time typical is 20+2.5*11= 47.5us BaudRate = 2.02M bps, so program 12B once is enough.
  *		- for compatibility with amebaz, which has 16-byte TX FIFO is 16 byte and max len is 16-cmdlen = 12 byte
  * @retval none
  */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_TxData12BXIP(u32 StartAddr, u8 DataPhaseLen, u8 *pData)
{
	FLASH_Write_Lock();

	FLASH_TxData(StartAddr, DataPhaseLen, pData);
	DCache_Invalidate(SPI_FLASH_BASE + StartAddr, DataPhaseLen);

	FLASH_Write_Unlock();
}
#endif

/**
  * @brief  This function is used to erase flash, and lock CPU when erase.
  * @param EraseType: can be one of the following  parameters:
  		@arg EraseChip: Erase the whole chip.
  		@arg EraseBlock: Erase specified block(64KB)
  		@arg EraseSector: Erase specified sector(4KB)
  * @param    Address should 4 byte align.The block/sector which
  * 		the address in will be erased.
  * @retval none
  */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_EraseXIP(u32 EraseType, u32 Address)
{
	FLASH_Write_Lock();

	FLASH_Erase(EraseType, Address);
	if (EraseType == EraseSector) {
		DCache_Invalidate(SPI_FLASH_BASE + Address, 0x1000);
	} else if (EraseType == EraseBlock) {
		DCache_Invalidate(SPI_FLASH_BASE + Address, 0x10000);
	} else {
		DCache_Invalidate(SPI_FLASH_BASE, 0x0FFFFFFF - SPI_FLASH_BASE);
	}

	FLASH_Write_Unlock();
}
#endif

/**
  * @brief  This function is used to write data to flash in OneBitMode and User Mode, and lock CPU when write.
  * @param  StartAddr: Start address in flash from which SPIC writes.
  * @param  DataPhaseLen: the number of bytes that SPIC sends in Data Phase.
  * @param  pData: pointer to a byte array that is to be sent.
  * @note
  *		- page program(256B) time typical is 0.7ms: BaudRate=2.9Mbps, so one bit mode is enough.
  *		- should use FLASH_SW_CS_Control to protect flash write
  * @retval none
  */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_TxData256BXIP(u32 StartAddr, u32 DataPhaseLen, u8 *pData)
{
	FLASH_Write_Lock();

	FLASH_TxData(StartAddr, DataPhaseLen, pData);
	DCache_Invalidate(SPI_FLASH_BASE + StartAddr, DataPhaseLen);

	FLASH_Write_Unlock();
}
#endif

/**
  * @brief  Read a stream of data from specified address
  * @param obj: Flash object define in application software.
  * @param  address: Specifies the starting address to read from.
  * @param  len: Specifies the length of the data to read.
  * @param  data: Specified the address to save the readback data.
  * @retval   status: Success:1 or Failure: Others.
  * @note auto mode is ok, because we have flash cache
  */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
int  FLASH_ReadStream(u32 address, u32 len, u8 *pbuf)
{
	assert_param(pbuf != NULL);

	_memcpy(pbuf, (const void *)(SPI_FLASH_BASE + address), len);

	return 1;
}
#endif

/**
  * @brief  Write a stream of data to specified address
  * @param  address: Specifies the starting address to write to.
  * @param  len: Specifies the length of the data to write.
  * @param  data: Pointer to a byte array that is to be written.
  * @retval   status: Success:1 or Failure: Others.
  */
SRAMDRAM_ONLY_TEXT_SECTION
int  FLASH_WriteStream(u32 address, u32 len, u8 *pbuf)
{
	// Check address: 4byte aligned & page(256bytes) aligned
	u32 page_begin = address & (~0xff);
	u32 page_end = (address + len - 1) & (~0xff);
	u32 page_cnt = ((page_end - page_begin) >> 8) + 1;

	u32 addr_begin = address;
	u32 addr_end = (page_cnt == 1) ? (address + len) : (page_begin + 0x100);
	u32 size = addr_end - addr_begin;

	FLASH_Write_Lock();
	while (page_cnt) {
		FLASH_TxData(addr_begin, size, pbuf);
		pbuf += size;

		page_cnt--;
		addr_begin = addr_end;
		addr_end = (page_cnt == 1) ? (address + len) : (addr_begin + 0x100);
		size = addr_end - addr_begin;
	}

	DCache_Invalidate(SPI_FLASH_BASE + address, len);
	FLASH_Write_Unlock();

	return 1;
}


/**
  * @brief    Configure SPIC IP Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg BIT_SHIFT_FLASH_CLK_XTAL
  *                            @arg BIT_SHIFT_FLASH_CLK_PLL
  * @param  Protection:  if disable interrupt when switch clock:
  * @retval   None
  */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_ClockSwitch(u32 Source, u32 Protection)
{
	/* To avoid gcc warnings */
	(void) Source;
	(void) Protection;
#if defined (ARM_CORE_CM0)
	u32 Temp = 0;
	u32 timeout = 20;

	if (Protection) {
		asm volatile("cpsid i" : : : "memory");
		//asm volatile ("cpsid f" : : : "memory");
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	}

	/* sequence should be followed strickly */
	if (Source == BIT_LSYS_CKSL_SPIC_XTAL) {
		/* 1. clock source switch to XTAL */
		RCC_PeriphClockSource_SPIC(Source);
		DelayUs(10);

		/* 2. disable clock ps div & disable clock div */
		Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_HSYS_SPIC_CTRL);
		Temp &= ~(HSYS_BIT_FLASH_DIV_EN | HSYS_BIT_FLASH_PS_DIV_EN);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HSYS_SPIC_CTRL, Temp);
		FLASH_CalibrationNewCmd(DISABLE);

		/* 3. Disable PLL_PS, SMART does not use PLL_PS */

		/* 4. SPIC Dummy to low speed dummy */
		flash_init_para.FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
		FLASH_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);
	} else if (Source == BIT_SHIFT_FLASH_CLK_PLL) {
		/* 1. enable PLL_PS, SMART does not use PLL_PS */

		/* 2. enable clock ps div & disable clock div */
		Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_HSYS_SPIC_CTRL);
		Temp |= (HSYS_BIT_FLASH_DIV_EN | HSYS_BIT_FLASH_PS_DIV_EN); /* enable clock ps div & enable clock div*/
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_HSYS_SPIC_CTRL, Temp);
		FLASH_CalibrationNewCmd(ENABLE);

		/* wait clock ready about 40us */
		while (timeout > 0) {
			timeout--;
			Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_HSYS_SPIC_CTRL);
			if ((Temp & HSYS_BIT_FLASH_DIV_RDY) && (Temp & HSYS_BIT_FLASH_PS_DIV_RDY)) {
				break;
			}
		}

		/* 3. clock source switch */
		RCC_PeriphClockSource_SPIC(Source);
		DelayUs(10);

		/* 4. SPIC Dummy to high speed dummy */
		flash_init_para.FLASH_rd_sample_phase = flash_init_para.FLASH_rd_sample_phase_cal;
		FLASH_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);
	}

	if (Protection) {
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
		asm volatile("cpsie i" : : : "memory");
		//asm volatile ("cpsie f" : : : "memory");
	}
#endif
}
#endif

SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_UserMode_Enter(void)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 gate_n;
	u32 cpu_status = 0;

#ifdef CONFIG_SMP
	long ulCoreID = (up_cpu_index() + 1) % CONFIG_SMP_NCPUS;
	CA32_TypeDef *ca32 = CA32_BASE;

	/* Bound the peer-core pause re-check: if someone resumed it
	 * between up_cpu_pause_all() and here, its next XIP fetch during
	 * our write is the bug to find, so report and continue rather
	 * than spin forever with interrupts masked.
	 *
	 * Gate on the pause handshake alone, not on the STANDBYWFE
	 * status bits: up_is_cpu_paused() asserts only after the peer's
	 * SGI handler entered up_cpu_paused(), whose park loop and
	 * spinlocks are linked into PSRAM (System.map: 0x601024ce /
	 * 0x60105748), so a handshake-paused core provably cannot fetch
	 * XIP while our write is in flight.  The STANDBYWFE bits of
	 * CA32_C0_CPU_STATUS are unreliable on this part: they read 0
	 * even while the peer genuinely sits in WFE (measured constant
	 * 0x00000003 across the full bounded wait), so gating on them
	 * wedges boot on a register that never tells the truth.  The
	 * value is still captured for the report below and the one-shot
	 * open log.
	 */
	gate_n = FLASHGATE_SPIN_LIMIT;
	cpu_status = ca32->CA32_C0_CPU_STATUS;
	while (!up_is_cpu_paused(ulCoreID)) {
		if (--gate_n == 0) {
			flashgate_fail(11, ca32->CA32_C0_CPU_STATUS);
			break;
		}
		cpu_status = ca32->CA32_C0_CPU_STATUS;
	}
#endif
	UP_DSB();
	UP_ISB();
	FLASH_WaitBusy_InUserMode(WAIT_SPIC_BUSY);
	spi_flash->CTRLR0 |= BIT_USER_MODE;

	/* user mode is entered after auto cmd is done, which means if SPIC BUSY=1, HW will not write Ctrl0 */
	gate_n = FLASHGATE_SPIN_LIMIT;
	while (!(spi_flash->CTRLR0 & BIT_USER_MODE)) {
		if (--gate_n == 0) {
			flashgate_fail(12, spi_flash->CTRLR0);
			break;
		}
	}
	if (!gate_open_done) {
		gate_open_done = 1;
		/* Only reached once the KM4 handshake completed and SPIC
		 * accepted user mode: real flash commands can start now.
		 * Latch the milestone only: SPIC is in user mode from here
		 * until FLASH_UserMode_Exit(), and any XIP access, code or
		 * .rodata data, would fault against XIP-resident vectors.
		 * The line is printed by FLASH_Write_Unlock() once auto
		 * mode is restored.
		 */
		gate_open_cpustatus = cpu_status;
		gate_open_pending = 1;
	}
}

SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_UserMode_Exit(void)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 gate_n = FLASHGATE_SPIN_LIMIT;

	spi_flash->CTRLR0 &= ~BIT_USER_MODE;

	while (spi_flash->CTRLR0 & BIT_USER_MODE) {
		if (--gate_n == 0) {
			flashgate_fail(13, spi_flash->CTRLR0);
			break;
		}
	}
	UP_ISB();
}

/**
* @brief  This function is used to send Rx command to flash to get status register or flash id.
* @param  cmd: command that need to be sent.
* @param  read_len: the number of bytes that will be read by SPIC after sending cmd.
* @param  read_data: pointer to a byte array which is used to save datas received.
* @note This function is only used for rx status/flash id ,not used for read flash data.
*		Only work in OneBitMode.
* @retval none
*/
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_RxCmd_InUserMode(u8 cmd, u32 read_len, u8 *read_data)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 rx_num = 0;
	u32 value;
	u32 gate_n;

	/* Caller should Enter user mode first */

	/* Set OneBit Mode */
	FLASH_SetSpiMode(&flash_init_para, SpicOneBitMode);

	/* Set CMD phase: 1 byte, ADDR phase: 0 byte */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = value | USER_ADDR_LENGTH(0);

	/* Set RX_NDF: frame number of receiving data. TX_NDF should be set in both transmit mode and receive mode.
		TX_NDF should be set to zero in receive mode to skip the TX_DATA phase. */
	spi_flash->RX_NDF = RX_NDF(read_len);
	spi_flash->TX_NDF = 0;

	/* set CTRLR0: RX_mode and one bit mode for data & addr*/
	spi_flash->CTRLR0 = ((spi_flash->CTRLR0 & 0xFFF0FFFF) | TMOD(3));

	/* set flash_cmd: write cmd to fifo */
	spi_flash->DR[0].BYTE = cmd;

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* read data */
	rx_num = 0;
	gate_n = FLASHGATE_SPIN_LIMIT;
	while (rx_num < read_len) {
		if (spi_flash->SR & BIT_RFNE) {
			read_data[rx_num] = spi_flash->DR[0].BYTE;
			rx_num += 1;
			gate_n = FLASHGATE_SPIN_LIMIT;
		} else if (--gate_n == 0) {
			/* RX FIFO never refilled: latch only; with a
			 * command in flight nothing may touch the XIP flash.
			 */
			flashgate_fail(14, spi_flash->SR);
			break;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	FLASH_WaitBusy_InUserMode(WAIT_TRANS_COMPLETE);

	/* Recover */
	FLASH_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);

	/* Caller decide to Exit user mode */
}

SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_RxCmd(u8 cmd, u32 read_len, u8 *read_data)
{
	/* Do Tx in user mode firstly */
	FLASH_UserMode_Enter();

	FLASH_RxCmd_InUserMode(cmd, read_len, read_data);

	/* Exit user mode and restore SPIC to auto mode */
	FLASH_UserMode_Exit();
}

/**
* @brief  This function is used to wait until SPIC/Flash Busy done or WLE bit set.
* @param  WaitType:  wait type, which can be one of the following parameters:
*			@arg WAIT_SPIC_BUSY: wait until SPIC transmission done.
*			@arg WAIT_TRANS_COMPLETE: wait until user transfer done.
*			@arg WAIT_FLASH_BUSY: wait until Flash busy done.
*			@arg WAIT_WRITE_EN: wait until flash status WLE(Write Enbale Latch) bit is set.
* @retval none
*/
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_WaitBusy_InUserMode(u32 WaitType)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 BusyCheck = 0;
	u8 status = 0;
	u32 busy_n = FLASHGATE_SPIN_LIMIT;

	do {
		if (WaitType == WAIT_SPIC_BUSY) {
			BusyCheck = (spi_flash->SR & BIT_BUSY);

		} else if (WaitType == WAIT_TRANS_COMPLETE) {
			/* When transfer completes, SSIENR.SPIC_EN are cleared by HW automatically. */
			BusyCheck = (spi_flash->SSIENR & BIT_SPIC_EN);

		} else if (WaitType == WAIT_FLASH_BUSY) {
			FLASH_RxCmd_InUserMode(flash_init_para.FLASH_cmd_rd_status, 1, &status);

			BusyCheck = (status & flash_init_para.FLASH_Busy_bit);
		} else if (WaitType == WAIT_WRITE_EN) {
			FLASH_RxCmd_InUserMode(flash_init_para.FLASH_cmd_rd_status, 1, &status);

			/* write enable ok & flash not busy */
			BusyCheck = (!(status & flash_init_para.FLASH_WLE_bit)) || (status & flash_init_para.FLASH_Busy_bit);
		}

		if (!BusyCheck) {
			break;
		}

		if (--busy_n == 0) {
			/* Raw report from RAM first (this wait routinely runs
			 * with the flash chip busy, where no XIP console is
			 * reachable), then latch and let the caller's sequence
			 * unwind; the assert comes from FLASH_Write_Unlock().
			 */
			flashgate_fail((u8)WaitType, spi_flash->SR);
			break;
		}
	} while (1);
}

/**
* @brief  This function is used to send WREN(Write Enable) cmd to set WLE bit
*		in flash status register.
* @param  none
* @note This function must be used prior to every Page Program,
	Sector/Block/Chip Erase, Write Status Register instruction.
* @retval none
*/
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_WriteEn_InUserMode(void)
{
	/* Wait for flash busy done */
	FLASH_WaitBusy_InUserMode(WAIT_FLASH_BUSY);

	/* Set flash_cmd: WREN to FIFO */
	FLASH_TxCmd_InUserMode(flash_init_para.FLASH_cmd_wr_en, 0, 0);

	/* Wait for write enable done*/
	FLASH_WaitBusy_InUserMode(WAIT_WRITE_EN);
}

/**
* @brief  This function is used to send Flash Instruction with Data Phase.
* @param  cmd: The command need to be sent.
* @param  DataPhaseLen: The length of Data phase.
* @param  pData: pointer to byte array which are to be sent in Data phase.
* @note
*		- This function works in OneBitMode and User mode.
*		- This function can only be used to tx cmd(WREN,WRSR,DP,RDP,ERASE etc.),not used to tx data.
* @retval none
*/
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_TxCmd_InUserMode(u8 cmd, u8 DataPhaseLen, u8 *pData)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 tx_num = 0;
	u32 addr_length;
	u32 ctrl0;
	u32 value;

	/* Caller should Enter user mode first */

	/* backup bitmode & addr_len */
	addr_length = spi_flash->USER_LENGTH;
	ctrl0 = spi_flash->CTRLR0;

	/* set CTRLR0: TX mode and one bit mode */
	spi_flash->CTRLR0 &= ~(TMOD(3) | CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));

	/* Set ADDR length: 0 byte, skip ADDR phase */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = value | USER_ADDR_LENGTH(0);

	/* Set TX_NDF: frame number of Tx data. */
	spi_flash->RX_NDF = 0;
	spi_flash->TX_NDF = TX_NDF(DataPhaseLen);

	/* set flash_cmd: write cmd to fifo */
	spi_flash->DR[0].BYTE = cmd;

	/* fill data */
	for (tx_num = 0; tx_num < DataPhaseLen; tx_num++) {
		spi_flash->DR[0].BYTE = (u8) * (pData + tx_num);
	}

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	FLASH_WaitBusy_InUserMode(WAIT_TRANS_COMPLETE);

	/* Restore bitmode & addr_len */
	spi_flash->USER_LENGTH = addr_length;
	spi_flash->CTRLR0 = ctrl0;

	/* Call FLASH_WaitBusy(WAIT_FLASH_BUSY) after this function:
		1) wait flash busy done (wip=0)
		2) restore SPIC to auto mode */
}

/**
  * @brief  This function is used to write data to flash in OneBitMode and User Mode.
  * @param  StartAddr: Start address in flash from which SPIC writes.
  * @param  DataPhaseLen: the number of bytes that SPIC sends in Data Phase.
  * @param  pData: pointer to a byte array that is to be sent.
  * @note
  *		- RXI312_SPIC Tx data frames number can be 0~2^24-1 bytes.
  *			TX_NDF can prevent TX FIFO underflow, when TX FIFO empty and Tx num<TX_NDF, clock
  *			would be gated and CS keep low.
  *			TX_NDF can't prevent TX FIFO overflow, so driver should check SR.TFNF before push data to FIFO.
  * @retval none
  */
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_TxData(u32 StartAddr, u32 DataPhaseLen, u8 *pData)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 tx_num = 0;
	u32 ctrl0;
	u32 value;
	u32 gate_n;

	assert_param(((StartAddr & 0xFF) + DataPhaseLen) <= 0x100); /* addr should page(256) align */

	/* Do Tx in user mode firstly */
	FLASH_UserMode_Enter();

	/* write enable cmd */
	FLASH_WriteEn_InUserMode();

	/* set CTRLR0: TX mode and one bit mode */
	ctrl0 = spi_flash->CTRLR0;
	spi_flash->CTRLR0 &= ~(TMOD(3) | CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));

	/* Set ADDR length */
	value = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = value | USER_ADDR_LENGTH(flash_init_para.FLASH_addr_phase_len);

	/* Set TX_NDF: frame number of Tx data. */
	spi_flash->RX_NDF = 0;
	spi_flash->TX_NDF = TX_NDF(DataPhaseLen);

	/* set flash_cmd: write cmd & address to fifo & addr is MSB */
	spi_flash->DR[0].BYTE = FLASH_CMD_PP;
	if (flash_init_para.FLASH_addr_phase_len == ADDR_4_BYTE) {
		spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF000000) >> 24);
	}

	spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF0000) >> 16);
	spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF00) >> 8);
	spi_flash->DR[0].BYTE = (u8)(StartAddr & 0xFF);

	/* write some data to fill the fifo before set SSIENR. */
	tx_num = 0;
	while (tx_num < DataPhaseLen) {
		if (spi_flash->SR & BIT_TFNF) {
			spi_flash->DR[0].BYTE = (u8) * (pData + tx_num);
			tx_num += 1;
		} else {
			break;
		}
	}

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* write the remaining data into fifo */
	gate_n = FLASHGATE_SPIN_LIMIT;
	while (tx_num < DataPhaseLen) {
		if (spi_flash->SR & BIT_TFNF) {
			spi_flash->DR[0].BYTE = (u8) * (pData + tx_num);
			tx_num += 1;
			gate_n = FLASHGATE_SPIN_LIMIT;
		} else if (--gate_n == 0) {
			flashgate_fail(15, spi_flash->SR);
			break;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	FLASH_WaitBusy_InUserMode(WAIT_TRANS_COMPLETE);

	/* Restore bitmode */
	spi_flash->CTRLR0 = ctrl0;

	FLASH_WaitBusy_InUserMode(WAIT_FLASH_BUSY);

	/* Exit user mode and restore SPIC to auto mode */
	FLASH_UserMode_Exit();
}

/**
  * @brief  FLASH_SetStatus used to set register status. FLASH_WriteEn & FLASH_WaitBusy
  *  		are included in this function to avoid hardfault when TxCmd in XIP
  * @param    Cmd: command to be sent
  * @param    Len: the number of bytes to be sent after Cmd
  * @param    Status: pointer to byte array to be sent
  * @retval     none
  */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_SetStatus(u8 Cmd, u32 Len, u8 *Status)
{
	/* Do Tx in user mode firstly */
	FLASH_UserMode_Enter();

	FLASH_WriteEn_InUserMode();
	FLASH_TxCmd_InUserMode(Cmd, Len, Status);
	FLASH_WaitBusy_InUserMode(WAIT_FLASH_BUSY);

	/* Exit user mode and restore SPIC to auto mode */
	FLASH_UserMode_Exit();
}
#endif

/**
  * @brief  FLASH_SetStatusBits set or clear status bits., used to set protect bit or quad enable bit
  * @param    SetBits: 16bits valid, SetBits[7:0] is status1 & SetBits[15:8] is status2
  * @param    NewState: ENABLE/DISABLE
  * @retval none
  */
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_SetStatusBits(u32 SetBits, u32 NewState)
{
	u8 status[2];
	u8 status1_mask = (u8)(SetBits & 0xFF);
	u8 status2_mask = (u8)((SetBits >> 8) & 0xFF);
	u8 StatusLen = 1;

	/* Do Set in user mode firstly */
	FLASH_UserMode_Enter();

	/* status1 */
	FLASH_RxCmd_InUserMode(flash_init_para.FLASH_cmd_rd_status, 1, &status[0]);
	if (NewState == ENABLE) {
		status[0] |=  status1_mask;
	} else {
		status[0] &=  ~status1_mask;
	}

	/* check if status2 */
	if (flash_init_para.FLASH_Status2_exist) {
		StatusLen = 2;
		FLASH_RxCmd_InUserMode(flash_init_para.FLASH_cmd_rd_status2, 1, &status[1]);

		if (NewState == ENABLE) {
			status[1] |=  status2_mask;
		} else {
			status[1] &=  ~status2_mask;
		}
	}

	if (!flash_init_para.FLASH_cmd_wr_status2) {
		FLASH_WriteEn_InUserMode();
		FLASH_TxCmd_InUserMode(flash_init_para.FLASH_cmd_wr_status, StatusLen, status);
		FLASH_WaitBusy_InUserMode(WAIT_FLASH_BUSY);
	} else {
		FLASH_WriteEn_InUserMode();
		FLASH_TxCmd_InUserMode(flash_init_para.FLASH_cmd_wr_status, 1, &status[0]);
		FLASH_WaitBusy_InUserMode(WAIT_FLASH_BUSY);

		FLASH_WriteEn_InUserMode();
		FLASH_TxCmd_InUserMode(flash_init_para.FLASH_cmd_wr_status2, 1, &status[1]);
		FLASH_WaitBusy_InUserMode(WAIT_FLASH_BUSY);
	}

	/* Exit user mode and restore SPIC to auto mode */
	FLASH_UserMode_Exit();
}

/**
  * @brief  This function is used to erase flash.
  * @param EraseType: can be one of the following  parameters:
  		@arg EraseChip: Erase the whole chip.
  		@arg EraseBlock: Erase specified block(64KB)
  		@arg EraseSector: Erase specified sector(4KB)
  * @param    Address should 4 byte align.The block/sector which
  * 		the address in will be erased.
  * @retval none
  */
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_Erase(u32 EraseType, u32 Address)
{
	u8 Addr[4];
	u8 len = 0;

	assert_param(EraseType <= EraseSector); /* max len is 2 */

	if (flash_init_para.FLASH_addr_phase_len == ADDR_4_BYTE) {
		len = 4;
		Addr[0] = (Address >> 24) & 0xFF;
		Addr[1] = (Address >> 16) & 0xFF;
		Addr[2] = (Address >> 8) & 0xFF;
		Addr[3] = Address & 0xFF;
	} else {
		len = 3;
		Addr[0] = (Address >> 16) & 0xFF;
		Addr[1] = (Address >> 8) & 0xFF;
		Addr[2] = Address & 0xFF;
	}

	/* Do Erase in user mode firstly */
	FLASH_UserMode_Enter();

	FLASH_WriteEn_InUserMode();

	if (EraseType == EraseChip) {
		FLASH_TxCmd_InUserMode(flash_init_para.FLASH_cmd_chip_e, 0, 0);
	} else if (EraseType == EraseBlock) {
		FLASH_TxCmd_InUserMode(flash_init_para.FLASH_cmd_block_e, len, Addr);
	} else if (EraseType == EraseSector) {
		FLASH_TxCmd_InUserMode(flash_init_para.FLASH_cmd_sector_e, len, Addr);
	}

	FLASH_WaitBusy_InUserMode(WAIT_FLASH_BUSY);

	/* Exit user mode and restore SPIC to auto mode */
	FLASH_UserMode_Exit();
}

/* Flash related API should be located at the PSRAM */
SRAMDRAM_ONLY_TEXT_SECTION
void FLASH_SetSpiMode(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 Value32;
	u32 ctrl0 = 0;
	u16 dclen = FLASH_InitStruct->FLASH_rd_dummy_cyle[SpicBitMode] * 2 * FLASH_InitStruct->FLASH_baud_rate;

	/* Disable SPI_FLASH User Mode */
	spi_flash->SSIENR = 0;

	/* set auto read dummy cycle and line delay */
	Value32 = spi_flash->AUTO_LENGTH & ~(MASK_RD_DUMMY_LENGTH | MASK_IN_PHYSICAL_CYC);
	spi_flash->AUTO_LENGTH = Value32 | RD_DUMMY_LENGTH(dclen) | IN_PHYSICAL_CYC(FLASH_InitStruct->FLASH_rd_sample_phase);

	/* set user read dummy cycle.
		If DUM_EN, push one dummy byte in addr phase manually and reduce one byte in dummy phase */
	if (FLASH_InitStruct->FLASH_dum_en) {
		if (SpicBitMode == SpicQuadIOBitMode) {
			dclen -= QUAD_DUM_CYCLE_NUM * 2 * FLASH_InitStruct->FLASH_baud_rate;
		} else if (SpicBitMode == SpicDualIOBitMode) {
			dclen -= DUAL_DUM_CYCLE_NUM * 2 * FLASH_InitStruct->FLASH_baud_rate;
		}
	}

	Value32 = spi_flash->USER_LENGTH & ~MASK_USER_RD_DUMMY_LENGTH;
	spi_flash->USER_LENGTH = Value32 | USER_RD_DUMMY_LENGTH(dclen);

	/* clear multi channel first */
	ctrl0 = spi_flash->CTRLR0 & ~(CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));

	/* clear dual & quad bit mode */
	Value32 = spi_flash->VALID_CMD & ~(SPIC_VALID_CMD_MASK);

	switch (SpicBitMode) {
	case SpicDualOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_dual_o_valid_cmd;
		ctrl0 |= (ADDR_CH(0) | DATA_CH(1));
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_DREAD;
		break;
	case SpicDualIOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_dual_io_valid_cmd;
		ctrl0 |= (ADDR_CH(1) | DATA_CH(1));
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_2READ;
		break;
	case SpicQuadOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_quad_o_valid_cmd;
		ctrl0 |= (ADDR_CH(0) | DATA_CH(2));
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_QREAD;
		break;
	case SpicQuadIOBitMode:
		Value32 |= FLASH_InitStruct->FALSH_quad_io_valid_cmd;
		ctrl0 |= (ADDR_CH(2) | DATA_CH(2));
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_4READ;
		break;
	default:
		FLASH_InitStruct->FLASH_cur_cmd = FLASH_CMD_READ;
	}

	if (FLASH_InitStruct->FLASH_dum_en) {
		Value32 |= BIT_DUM_EN;
	}

	spi_flash->VALID_CMD = Value32;
	spi_flash->CTRLR0 = ctrl0;
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/

