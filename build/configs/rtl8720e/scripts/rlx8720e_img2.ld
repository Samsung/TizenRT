/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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

OUTPUT_ARCH(arm)
ENTRY(app_start)

MEMORY
{
	/* KM4 ROM */
	KM4_IROM (rx) :                         ORIGIN = 0x00000000, LENGTH = 0x0001E000 - 0x00000000	/* KM4 IROM_S: 120k */
	KM4_IROM_NS (rx) :                      ORIGIN = 0x0001E000, LENGTH = 0x00034000 - 0x0001E000	/* KM4 IROM_NS: 88k */
	KM4_IROM_STDLIB_NS (rx) :               ORIGIN = 0x00034000, LENGTH = 0x00041000 - 0x00034000	/* KM4 STDLIB ROM: 52k */
	KM4_DROM_NS (rx) :                      ORIGIN = 0x00041000, LENGTH = 0x00045000 - 0x00041000	/* KM4 DROM_NS: 16k */
	KM4_DROM (rx) :                         ORIGIN = 0x00045000, LENGTH = 0x00048000 - 0x00045000	/* KM4 DROM_S: 12k */

	/* KM4 Secure RAM, BIT(28)=1 */
	KM4_ROMBSS_RAM_S (rw) :                 ORIGIN = 0x30007000, LENGTH = 0x30008000 - 0x30007000	/* KM4 ROM BSS RAM S: 4k */
	KM4_MSP_RAM_S (rw) :                    ORIGIN = 0x30008000, LENGTH = 0x3000A000 - 0x30008000	/* KM4 MSP_S RAM: 8k */
	KM4_BOOTLOADER_RAM_S (rwx) :            ORIGIN = 0x3000A020, LENGTH = 0x30012000 - 0x3000A020	/* KM4 BOOT Loader RAM: 32k */
	KM4_FLOADER_RAM_S (rwx) :               ORIGIN = 0x3000A020, LENGTH = 0x30020000 - 0x3000A020	/* KM4 ImgTool Flash Loader RAM: 88k */

	/* KM4 Non-secure RAM */
	KM4_MSP_RAM_NS (rw) :                   ORIGIN = 0x20003000, LENGTH = 0x20004000 - 0x20003000	/* KM4  MSP_NS RAM: 4k */
	KM4_ROMBSS_RAM_COM (rw) :               ORIGIN = 0x20004000, LENGTH = 0x20005000 - 0x20004000	/* KM4  ROM BSS COMMON(S & NS both used) RAM: 4k */
	KM4_ROMBSS_RAM_NS (rw) :                ORIGIN = 0x20005000, LENGTH = 0x20006000 - 0x20005000	/* KM4 ROM BSS NS RAM: 4k */
	KM4_STDLIB_HEAP_RAM_NS (rw) :           ORIGIN = 0x20006000, LENGTH = 0x20007000 - 0x20006000	/* KM4 ROM STDLIB HEAP: 4k */

	/* PMC RAM, fixed in memory layout, do not changed */
	KR4_IPC_RAM (rwx) :                     ORIGIN = 0x20004900, LENGTH = 0x20004C00 - 0x20004900	/* KR4 IPC RAM: 768B */
	KR4_BOOT_RAM_BACKUP (rwx) :             ORIGIN = 0x20004C00, LENGTH = 0x20004C20 - 0x20004C00	/* KR4 BOOT RAM: 32B, Backup */
	KR4_RAM_ONLY (rwx) :                    ORIGIN = 0x20004C40, LENGTH = 0x20004E00 - 0x20004C40	/* KR4 PMC RAM: 0.5k */
	KM4_RAM_ONLY (rwx) :                    ORIGIN = 0x20004E20, LENGTH = 0x20005000 - 0x20004E20	/* KM4 PMC RAM: 0.5K */
	KR4_BOOT_RAM (rwx) :                    ORIGIN = 0x20040020, LENGTH = 0x20040040 - 0x20040020	/* KR4 BOOT RAM: 64B, Can be ignore When DSP use this addr */

	/* IMG2 RAM */
	KM4_BD_RAM (rwx) :                      ORIGIN = 0x20012020, LENGTH = 0x20040000 - 0x20012020	/* KM4 BD RAM: 184k */
	KR4_BD_RAM (rwx) :                      ORIGIN = 0x20040060, LENGTH = 0x2007A000 - 0x20040060	/* KR4 BD RAM: 232k */
	KM4_HEAP_EXT (rwx) :                    ORIGIN = 0x2007A000, LENGTH = 0x200C0000 - 0x2007A000	/* KM4 HEAP EXT: 280k */

	/* KM4 Non-secure PSRAM */
	KM4_BD_PSRAM (rwx) :                    ORIGIN = 0x60000020, LENGTH = 0x6037E000 - 0x60000020	/* KM4 MAIN RAM NS : 3575 */
	KR4_BD_PSRAM (rwx) :                    ORIGIN = 0x603CE020, LENGTH = 0x60400000 - 0x603CE020	/* KR4 BD PSRAM: 200k */

	KM4_BD_PSRAM_TZ_NSC (rwx) :             ORIGIN = 0x6037E020, LENGTH = 0x6037F000 - 0x6037E020	/* KM4 BD RAM NSC: 4k */
	KM4_BD_PSRAM_TZ_ENTRY (rwx) :           ORIGIN = 0x6037F000, LENGTH = 0x60383000 - 0x6037F000	/* KM4 BD RAM ENTRY: 16k */
	KM4_BD_PSRAM_TZ_S (rwx) :               ORIGIN = 0x70383020, LENGTH = 0x703CE000 - 0x70383020	/* KM4 BD RAM S: 300k */

	/* Retention RAM */
	RETENTION_RAM (rwx) :                   ORIGIN = 0x4100CE00, LENGTH = 0x4100D000 - 0x4100CE00	/* KR4 Retention SRAM: 512B: 0k */

	/* KR4 ROM */
	KR4_IROM (rx) :                         ORIGIN = 0x00000000, LENGTH = 0x0001C000 - 0x00000000	/* KR4 IROM: 112k */
	KR4_IROM_STDLIB (rx) :                  ORIGIN = 0x0001C000, LENGTH = 0x0002D000 - 0x0001C000	/* KR4 STDLIB ROM: 68k */
	KR4_DROM (rx) :                         ORIGIN = 0x0002D000 , LENGTH = 0x00030000 - 0x0002D000 	/* KR4 DROM: 12k */

	/* KR4 RAM */
	KR4_ROMBSS_RAM (rw) :                   ORIGIN = 0x20000000, LENGTH = 0x20001000 - 0x20000000	/* KR4 ROM BSS RAM: 4k */
	KR4_MSP_RAM (rw) :                      ORIGIN = 0x20001000, LENGTH = 0x20002000 - 0x20001000	/* KR4 MSP RAM: 4k */
	KR4_STDLIB_HEAP_RAM (rw) :              ORIGIN = 0x20002000, LENGTH = 0x20003000 - 0x20002000	/* KR4 ROM STDLIB HEAP: 4k */

	/* Flash */
	PHY_IMG1 (rx) :                         ORIGIN = 0x08001020, LENGTH = 0x08009000 - 0x08001020	/* KM4 Bootloader Physical address: 32k */
	KM4_BOOT_XIP (rx) :                     ORIGIN = 0x0F800020, LENGTH = 0x10000000 - 0x0F800020	/* KM4 Bootloader Virtual address: 8192k */
	PHY_IMG2 (rx) :                         ORIGIN = 0x08014000, LENGTH = 0x081E0000 - 0x08014000	/* KR4/KM4 IMG2 Physical address: 1840k */
	KM4_IMG3 (rx) :                         ORIGIN = 0x081E0000, LENGTH = 0x08200000 - 0x081E0000	/* KM4 IMG3: 128k */
	PHY_DSP (rx) :                          ORIGIN = 0x08400000, LENGTH = 0x08600000 - 0x08400000	/* DSP IMG Physical address: 2048k */
	KR4_IMG2_XIP (rx) :                     ORIGIN = 0x0C000020, LENGTH = 0x0E000000 - 0x0C000020	/* KR4 IMG2 Virtual address: 32768k */
	KM4_IMG2_XIP (rx) :                     ORIGIN = 0x0E000020, LENGTH = 0x0F800000 - 0x0E000020	/* KM4 IMG2 Virtual address: 24576k */
	DSP_IMG2_XIP (rx) :                     ORIGIN = 0x0D000020, LENGTH = 0x0E000000 - 0x0D000020	/* DSP IMG2 Virtual address: 24576k */
	BTRACE (rx) :                           ORIGIN = 0xC8800000, LENGTH = 0xC9000000 - 0xC8800000	/* BT LOG Virtual address: 8192k */

	/* DSP ROM */
	DSP_IROM (rx) :                         ORIGIN = 0x00000000, LENGTH = 0x00000400 - 0x00000000	/* KM0 IROM: 1k */
}

__kr4_image2_entry_func__ = ORIGIN(KR4_BD_RAM);
__kr4_ipc_memory_start__ = ORIGIN(KR4_IPC_RAM);
__kr4_flash_text_start__ = ORIGIN(KR4_IMG2_XIP);
__km4_flash_text_start__ = ORIGIN(KM4_IMG2_XIP);
__dsp_flash_text_start__ = ORIGIN(DSP_IMG2_XIP);
__kr4_psram_text_start__ = ORIGIN(KR4_BD_PSRAM);
__km4_boot_text_start__ = ORIGIN(KM4_BOOT_XIP);
__km4_image3_flash_start__ = ORIGIN(KM4_IMG3);
__nspe_phy_addr__ = ORIGIN(PHY_IMG2);

__kr4_start_table_backup_addr__ = ORIGIN(KR4_BOOT_RAM_BACKUP);
/*__audio_buffer_start__ = ORIGIN(AUDIO_BUFFER);*/

_stext_flash = __km4_flash_text_start__;
_etext_flash = LENGTH(KM4_IMG2_XIP);
__flash_text_start__ = _stext_flash;
__flash_text_end__ = _etext_flash;

SECTIONS
{
	.rom.text (NOLOAD): { } > KM4_IROM_NS
	.rom.rodata (NOLOAD): { } > KM4_DROM_NS
	.hal.rom.bss (NOLOAD): { } > KM4_ROMBSS_RAM_COM
	.hal.ns_rom.bss (NOLOAD): { } > KM4_ROMBSS_RAM_NS

	/* fixed region. do not add any code to this region */
	.sram_only.text.data :
	{
		__sram_only_start__ = .;
		*(.sram.only.text*)
		. = ALIGN(32);

		*(.sram.only.data*)
	} > KM4_RAM_ONLY

	/* xip part */
	.xip_image2.text :
	{
		__flash_text_start__ = .;
		_stext_ram = ABSOLUTE(.);
		*(.non.dram.text*)
		*_udivmoddi4.o(.text* .rodata*)
		__flash_text_end__ = .;
	} > KM4_IMG2_XIP

	/* sram part */
	.sram_image2.text.data :
	{
		__sram_image2_start__ = .;
		__sram_text_start__ = .;
		_sdata = ABSOLUTE(.);
		__image2_entry_func__ = .;
		KEEP(*(SORT(.image2.entry.data*)))
		__image2_validate_code__ = .;
		KEEP(*(.image2.validate.rodata*))

		*(.sramdram.only.text*)
		__sram_text_end__ = .;
		__sram_image2_end__ = .;
		. = ALIGN(32);
	} > KM4_BD_RAM

	.xip_image2.text :
	{
		__image2_backtrace_start__ = .;
		*(.img2_custom_signature*)
		*(.image2.net.ram.text*)
		*(.timesensitive.text*)

		*(.text*)
		*(.rodata*)

		. = ALIGN (4);
		__cmd_table_start__ = .;
		KEEP(*(.cmd.table.data*))
		__cmd_table_end__ = .;
		. = ALIGN (4);
		__ipc_table_start__ = .;
		KEEP(*(*.ipc.table.data*))
		__ipc_table_end__ = .;
		. = ALIGN (4);
		__git_ver_table_start__ = .;
		KEEP(*(.git.ver.data))
		__git_ver_table_end__ = .;

		/* Add This for C++ support */
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP(*(.preinit_array))
		__preinit_array_end = .;
		. = ALIGN(4);
		__init_array_start = .;
		_sinit = ABSOLUTE(.);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		_einit = ABSOLUTE(.);
		__init_array_end = .;
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		__fini_array_end = .;	

		/*-----------------*/	
		_etext_ram = ABSOLUTE(.);	
		__image2_backtrace_end__ = .;
	
		. = ALIGN (32);
	} > KM4_IMG2_XIP

	/* fixed region. do not change */
	.ARM.extab  : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*) 

	} > KM4_IMG2_XIP

	/* fixed region. do not change */
	.ARM.exidx :
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > KM4_IMG2_XIP
	
	.sram_image2.text.data :
	{
		*(.timesensitive.data*)
		*(.data*)
		_edata = ABSOLUTE(.);
		/*cache line size aligned in order to avoid MMU remap err*/
		. = ALIGN(32);
	} > KM4_BD_RAM

	/*-----------------*/
	.ram_image2.bss (NOLOAD):
	{
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		__bss_end__ = .;
	} > KM4_BD_RAM

	.ram_image2.bd.data (NOLOAD):
	{
		. = ALIGN (32);
		__ram_bd_start__ = .;
		*(.bdsram.data*)
		. = ALIGN (32);
		__ram_bd_end__ = .;
	} > KM4_BD_RAM

	.ram_image2.nocache.data (NOLOAD):
	{
		. = ALIGN (32);
		__ram_nocache_start__ = .;
		*(.ncsram.data*)
		. = ALIGN (32);
		__ram_nocache_end__ = .;
	} > KM4_BD_RAM

	.ram_heap.data (NOLOAD):
	{
		. = ALIGN (32);
		/*reserved heap to 8k for sbrk*/
		end = .;
		*(.heap.stdlib*)
		. = . + 8192;
	} > KM4_BD_RAM

	.bdram_heap.start (NOLOAD):
	{
		/* For TizenRT to locate idle thread stack */
		_sbss = ABSOLUTE(.);
		__bdram_heap_buffer_start__ = .;
		/* For TizenRT to locate idle thread stack */
		_ebss = ABSOLUTE(.);
	} > KM4_BD_RAM

	/* psram part */
	.psram_image2.text.data :
	{
		*(.timesensitive.data*)
		*(.data*)

		/*cache line size aligned in order to avoid MMU remap err*/
		. = ALIGN(32);
	} > KM4_BD_PSRAM

	.psram_heap.start (NOLOAD):
	{
		/* Heap start address in external RAM */
		_sext_heap_start = ABSOLUTE(.);
	} > KM4_BD_PSRAM

	.bluetooth_trace.text :
	{
		__btrace_start__ = .;
		*(.BTTRACE)
		__btrace_end__ = .;
	} > BTRACE
	/* Heap start address in internal RAM */
	_sint_heap_start = ABSOLUTE(ORIGIN(KM4_HEAP_EXT));
	_sidle_stack = ABSOLUTE(ORIGIN(KM4_MSP_RAM_NS) + LENGTH(KM4_MSP_RAM_NS)) - CONFIG_IDLETHREAD_STACKSIZE;
	__StackLimit = ABSOLUTE(ORIGIN(KM4_HEAP_EXT) + LENGTH(KM4_HEAP_EXT));
	__PsramStackLimit = ABSOLUTE(ORIGIN(KM4_BD_PSRAM) + LENGTH(KM4_BD_PSRAM));
}

ASSERT(__sram_only_start__ == ORIGIN(KM4_RAM_ONLY), "__sram_only_start__ is changed.")
ASSERT(__flash_text_start__ == ORIGIN(KM4_IMG2_XIP), "__flash_text_start__ is changed.")
ASSERT(__sram_image2_start__ == ORIGIN(KM4_BD_RAM), "__sram_image2_start__ is changed.")
ASSERT(__image2_entry_func__ == ORIGIN(KM4_BD_RAM), "__image2_entry_func__ is not KM4_BD_RAM.")

__psram_image2_start__ = .;
__bdram_heap_buffer_size__ = ORIGIN(KM4_BD_RAM) + LENGTH(KM4_BD_RAM) - __bdram_heap_buffer_start__;

SECTIONS
{
    PROVIDE(rtl_crypto_aes_cbc_init = 0x1fc01);
    PROVIDE(rtl_crypto_aes_cbc_encrypt = 0x1fc55);
    PROVIDE(rtl_crypto_aes_cbc_decrypt = 0x1fca5);
    PROVIDE(rtl_crypto_aes_ecb_init = 0x1fcf5);
    PROVIDE(rtl_crypto_aes_ecb_encrypt = 0x1fd49);
    PROVIDE(rtl_crypto_aes_ecb_decrypt = 0x1fd85);
    PROVIDE(rtl_crypto_aes_ctr_init = 0x1fdc1);
    PROVIDE(rtl_crypto_aes_ctr_encrypt = 0x1fe15);
    PROVIDE(rtl_crypto_aes_ctr_decrypt = 0x1fe5d);
    PROVIDE(rtl_crypto_aes_cfb_init = 0x1fea5);
    PROVIDE(rtl_crypto_aes_cfb_encrypt = 0x1fef9);
    PROVIDE(rtl_crypto_aes_cfb_decrypt = 0x1ff41);
    PROVIDE(rtl_crypto_aes_ofb_init = 0x1ff89);
    PROVIDE(rtl_crypto_aes_ofb_encrypt = 0x1ffdd);
    PROVIDE(rtl_crypto_aes_ofb_decrypt = 0x20025);
    PROVIDE(rtl_crypto_aes_gcm_init = 0x2006d);
    PROVIDE(rtl_crypto_aes_gcm_encrypt = 0x200c1);
    PROVIDE(rtl_crypto_aes_gcm_decrypt = 0x2013d);
    PROVIDE(io_assert_failed = 0x201b9);
    PROVIDE(BKUP_Write = 0x201d9);
    PROVIDE(BKUP_Read = 0x20205);
    PROVIDE(BKUP_Set = 0x2022d);
    PROVIDE(BKUP_Clear = 0x20255);
    PROVIDE(XTAL_ClkGet = 0x20281);
    PROVIDE(CPU_ClkSrc_Set = 0x2029d);
    PROVIDE(CPU_ClkSrc_Get = 0x202b9);
    PROVIDE(PLL_ClkGet = 0x202c9);
    PROVIDE(CPU_ClkGet = 0x2031d);
    PROVIDE(CRYPTO_MemDump = 0x203a5);
    PROVIDE(CRYPTO_SetSrcDesc = 0x204b5);
    PROVIDE(CRYPTO_SetDstDesc = 0x20545);
    PROVIDE(CRYPTO_CleanCmdOk = 0x205d1);
    PROVIDE(CRYPTO_ClearAllINT = 0x20625);
    PROVIDE(CRYPTO_SetCmdBuf = 0x20675);
    PROVIDE(CRYPTO_SrcDescKeyIv = 0x2091d);
    PROVIDE(CRYPTO_Reset = 0x20d99);
    PROVIDE(CRYPTO_SHA_Reset = 0x20de1);
    PROVIDE(CRYPTO_SetSecurityModeAD = 0x20e29);
    PROVIDE(CRYPTO_Init = 0x20fed);
    PROVIDE(CRYPTO_SHA_Init = 0x21011);
    PROVIDE(CRYPTO_OTPKey_Init = 0x21035);
    PROVIDE(CRYPTO_OTPKey_SHA_Init = 0x21095);
    PROVIDE(CRYPTO_Cache_Sync = 0x21109);
    PROVIDE(CRYPTO_ProcessAD = 0x212bd);
    PROVIDE(CRYPTO_SendSeqBuf = 0x21a7d);
    PROVIDE(CRYPTO_CipherInit = 0x21b85);
    PROVIDE(CRYPTO_CipherEncryptAD = 0x21ba5);
    PROVIDE(CRYPTO_CipherDecryptAD = 0x21bd9);
    PROVIDE(DelayNop = 0x21c11);
    PROVIDE(DelayUs = 0x21c1d);
    PROVIDE(DelayMs = 0x21c95);
    PROVIDE(ECDSA_BufSub = 0x21ca9);
    PROVIDE(ECDSA_BufCmp = 0x21d11);
    PROVIDE(ECDSA_HashBuf_Setting = 0x21d79);
    PROVIDE(ECDSA_CurveSel = 0x21ee9);
    PROVIDE(ECDSA_EngRst = 0x22005);
    PROVIDE(ECDSA_Cmd = 0x22019);
    PROVIDE(ECDSA_KeyGen = 0x2203d);
    PROVIDE(ECDSA_Sign = 0x22111);
    PROVIDE(ECDSA_Verify = 0x22239);
    PROVIDE(ECDSA_ECDH_Compute_Shared = 0x22315);
    PROVIDE(ECDSA_Basic_Func = 0x223f5);
    PROVIDE(FLASH_SetSpiMode = 0x22579);
    PROVIDE(FLASH_RxCmd = 0x22691);
    PROVIDE(FLASH_WaitBusy = 0x22711);
    PROVIDE(FLASH_RxData = 0x227bd);
    PROVIDE(FLASH_TxCmd = 0x22891);
    PROVIDE(FLASH_WriteEn = 0x228f9);
    PROVIDE(FLASH_TxData = 0x2291d);
    PROVIDE(FLASH_SetStatus = 0x22a15);
    PROVIDE(FLASH_Erase = 0x22a39);
    PROVIDE(FLASH_DeepPowerDown = 0x22af1);
    PROVIDE(FLASH_WaitBusy_Timeout = 0x22b45);
    PROVIDE(FLASH_WriteEn_Timeout = 0x22bcd);
    PROVIDE(FLASH_SetStatusBits = 0x22bf1);
    PROVIDE(FLASH_StructInit_Micron = 0x22cbd);
    PROVIDE(FLASH_StructInit_MXIC = 0x22d39);
    PROVIDE(FLASH_StructInit_GD = 0x22db5);
    PROVIDE(FLASH_StructInit = 0x22e31);
    PROVIDE(FLASH_Init = 0x22ead);
    PROVIDE(FLASH_Wait_Boot_Finish = 0x22ec9);
    PROVIDE(FLASH_CalibrationPLLSel = 0x22ed9);
    PROVIDE(FLASH_CalibrationPLL_GetSrc = 0x22ee5);
    PROVIDE(FLASH_Calibration_PSPLL_Open = 0x22ef1);
    PROVIDE(FLASH_Calibration_PSPLL_Close = 0x22f41);
    PROVIDE(FLASH_CalibrationPLLPSCmd = 0x22f79);
    PROVIDE(FLASH_CalibrationPLLPS_Shift = 0x22fb5);
    PROVIDE(FLASH_PLLInit_ClockDiv = 0x22fe1);
    PROVIDE(FLASH_CalibrationPhase = 0x23065);
    PROVIDE(FLASH_CalibrationPhaseIdx = 0x230f5);
    PROVIDE(FLASH_CalibrationNewCmd = 0x23101);
    PROVIDE(FLASH_CalibrationNew = 0x23129);
    PROVIDE(FLASH_Read_HandShake_Cmd = 0x232e5);
    PROVIDE(FLASH_Read_HandShake = 0x2331d);
    PROVIDE(FLASH_Calibration = 0x233ed);
    PROVIDE(GPIO_INTMode = 0x23505);
    PROVIDE(GPIO_INTConfig = 0x23591);
    PROVIDE(GPIO_INTHandler = 0x235d5);
    PROVIDE(GPIO_Direction = 0x2368d);
    PROVIDE(GPIO_Init = 0x236d9);
    PROVIDE(GPIO_DeInit = 0x23769);
    PROVIDE(GPIO_ReadDataBit = 0x237dd);
    PROVIDE(GPIO_WriteBit = 0x23815);
    PROVIDE(GPIO_PortDirection = 0x23861);
    PROVIDE(GPIO_PortRead = 0x238b5);
    PROVIDE(GPIO_PortWrite = 0x238f9);
    PROVIDE(GPIO_DebounceClock = 0x23945);
    PROVIDE(GPIO_LevelSync = 0x23995);
    PROVIDE(GPIO_UserRegIrq = 0x239e1);
    PROVIDE(LOGUART_StructInit = 0x23a0d);
    PROVIDE(LOGUART_SetBaud = 0x23a45);
    PROVIDE(LOGUART_RxCmd = 0x23af1);
    PROVIDE(LOGUART_AGGCmd = 0x23b05);
    PROVIDE(LOGUART_AGGPathCmd = 0x23b19);
    PROVIDE(LOGUART_AGGSetTimeOut = 0x23b99);
    PROVIDE(LOGUART_AGGSetThreshold = 0x23bb5);
    PROVIDE(LOGUART_AGGSetSyncPattern = 0x23be1);
    PROVIDE(LOGUART_AGGPathAllCmd = 0x23bf1);
    PROVIDE(LOGUART_PutChar = 0x23c15);
    PROVIDE(LOGUART_GetChar = 0x23c7d);
    PROVIDE(LOGUART_Readable = 0x23c99);
    PROVIDE(LOGUART_GetRxCount = 0x23ca9);
    PROVIDE(LOGUART_GetIMR = 0x23cb5);
    PROVIDE(LOGUART_SetIMR = 0x23cc1);
    PROVIDE(LOGUART_INTConfig = 0x23ccd);
    PROVIDE(LOGUART_INTClear = 0x23cdd);
    PROVIDE(LOGUART_GetStatus = 0x23ce5);
    PROVIDE(LOGUART_ClearRxFifo = 0x23ce9);
    PROVIDE(_LOGUART_Init = 0x23d09);
    PROVIDE(LOGUART_Init = 0x23e5d);
    PROVIDE(LOGUART_WaitBusy = 0x23ec1);
    PROVIDE(LOGUART_WaitTxComplete = 0x23f01);
    PROVIDE(LOGUART_BT_SendData = 0x23f41);
    PROVIDE(LOGUART_Relay_StructInit = 0x23f79);
    PROVIDE(LOGUART_Relay_SetFormat = 0x23f8d);
    PROVIDE(LOGUART_Relay_SetBaud = 0x23fdd);
    PROVIDE(LOGUART_Relay_ClearRxFifo = 0x24035);
    PROVIDE(LOGUART_Relay_RxCmd = 0x24059);
    PROVIDE(LOGUART_MonitorParaConfig = 0x2406d);
    PROVIDE(LOGUART_LPBaudSet = 0x240b5);
    PROVIDE(LOGUART_Relay_LPSetBaud = 0x24161);
    PROVIDE(LOGUART_RxMonitorCmd = 0x241c1);
    PROVIDE(LOGUART_RxMonBaudCtrlRegGet = 0x241d5);
    PROVIDE(LOGUART_RxMonitorSatusGet = 0x241d9);
    PROVIDE(LOGUART_DiagInit = 0x241dd);
    PROVIDE(md_hash_save = 0x2426d);
    PROVIDE(md_hash_restore = 0x242d5);
    PROVIDE(rtl_crypto_md5_init = 0x24345);
    PROVIDE(rtl_crypto_md5_process = 0x243b9);
    PROVIDE(rtl_crypto_md5_update_rom = 0x24415);
    PROVIDE(rtl_crypto_md5_update = 0x24489);
    PROVIDE(rtl_crypto_md5_final = 0x244ed);
    PROVIDE(rtl_crypto_md5 = 0x24565);
    PROVIDE(rtl_crypto_hmac_md5_init = 0x2458d);
    PROVIDE(rtl_crypto_hmac_md5_process = 0x24649);
    PROVIDE(rtl_crypto_hmac_md5_update_rom = 0x24689);
    PROVIDE(rtl_crypto_hmac_md5_update = 0x24709);
    PROVIDE(rtl_crypto_hmac_md5_final = 0x2476d);
    PROVIDE(rtl_crypto_hmac_md5 = 0x247e9);
    PROVIDE(clear_ns_rom_bss = 0x24815);
    PROVIDE(RSIP_OTF_init = 0x2482d);
    PROVIDE(RSIP_IV_Set = 0x2483d);
    PROVIDE(RSIP_OTF_Cmd = 0x24859);
    PROVIDE(RSIP_OTF_Enable = 0x24871);
    PROVIDE(RSIP_MMU_Config = 0x248e9);
    PROVIDE(RSIP_MMU_Cmd = 0x24929);
    PROVIDE(RSIP_MMU_Cache_Clean = 0x24951);
    PROVIDE(PAD_DrvStrength = 0x24961);
    PROVIDE(PAD_PullCtrl = 0x24985);
    PROVIDE(PAD_SleepPullCtrl = 0x24a4d);
    PROVIDE(PAD_ResistorCtrl = 0x24b15);
    PROVIDE(PAD_CMD = 0x24b39);
    PROVIDE(Pinmux_Config = 0x24b59);
    PROVIDE(Pinmux_ConfigGet = 0x24b71);
    PROVIDE(Pinmux_UartLogCtrl = 0x24b81);
    PROVIDE(PAD_SpicCtrl = 0x24bc9);
    PROVIDE(Pinmux_SpicCtrl = 0x24c25);
    PROVIDE(Pinmux_Swdoff = 0x24ced);
    PROVIDE(RCC_PeriphClockCmd = 0x24d01);
    PROVIDE(RCC_PeriphClockSource_UART = 0x24da1);
    PROVIDE(RCC_PeriphClockSource_SPIC = 0x24e1d);
    PROVIDE(RCC_PeriphClockSource_PSRAM = 0x24e39);
    PROVIDE(RCC_PeriphClockSource_SDM = 0x24e55);
    PROVIDE(RCC_PeriphClockSource_VADMEM = 0x24e79);
    PROVIDE(RCC_PeriphClockSource_AUDIOCODEC = 0x24e95);
    PROVIDE(RCC_PeriphClockSource_CTC = 0x24eb1);
    PROVIDE(RCC_PeriphClockSource_ADC = 0x24ecd);
    PROVIDE(RCC_PeriphClockSource_GPIO = 0x24ee9);
    PROVIDE(RCC_PeriphClockSource_SPORT = 0x24f05);
    PROVIDE(RCC_PeriphClockSource_32K = 0x24f3d);
    PROVIDE(RCC_PeriphClockSource_LOGUART = 0x24f55);
    PROVIDE(hash_save = 0x24f91);
    PROVIDE(hash_restore = 0x24ff9);
    PROVIDE(rtl_crypto_sha1_init = 0x25069);
    PROVIDE(rtl_crypto_sha1_process = 0x250ed);
    PROVIDE(rtl_crypto_sha1_update_rom = 0x25125);
    PROVIDE(rtl_crypto_sha1_update = 0x25195);
    PROVIDE(rtl_crypto_sha1_final = 0x251f9);
    PROVIDE(rtl_crypto_sha1 = 0x2526d);
    PROVIDE(rtl_crypto_sha2_init = 0x25295);
    PROVIDE(rtl_crypto_sha2_process = 0x254c1);
    PROVIDE(rtl_crypto_sha2_update_rom = 0x254fd);
    PROVIDE(rtl_crypto_sha2_update = 0x25571);
    PROVIDE(rtl_crypto_sha2_final = 0x255d5);
    PROVIDE(rtl_crypto_sha2 = 0x2564d);
    PROVIDE(rtl_crypto_hmac_sha1_init = 0x25675);
    PROVIDE(rtl_crypto_hmac_sha1_process = 0x25741);
    PROVIDE(rtl_crypto_hmac_sha1_update_rom = 0x2577d);
    PROVIDE(rtl_crypto_hmac_sha1_update = 0x257f5);
    PROVIDE(rtl_crypto_hmac_sha1_final = 0x25859);
    PROVIDE(rtl_crypto_hmac_sha1 = 0x258d1);
    PROVIDE(rtl_crypto_hmac_sha2_init = 0x258fd);
    PROVIDE(rtl_crypto_hmac_sha2_process = 0x25bb5);
    PROVIDE(rtl_crypto_hmac_sha2_update_rom = 0x25bf5);
    PROVIDE(rtl_crypto_hmac_sha2_update = 0x25c6d);
    PROVIDE(rtl_crypto_hmac_sha2_final = 0x25cd1);
    PROVIDE(rtl_crypto_hmac_sha2 = 0x25d4d);
    PROVIDE(SYSCFG_RLVersion = 0x25d79);
    PROVIDE(SYSCFG_GetRLNum = 0x25d9d);
    PROVIDE(SYSCFG_TRP_UARTImage = 0x25dc1);
    PROVIDE(SYSCFG_TRP_ICFG = 0x25dd1);
    PROVIDE(SYSCFG_TRP_TestMode = 0x25de1);
    PROVIDE(SYSCFG_TRP_OTPBYP = 0x25df1);
    PROVIDE(SYSCFG_ROMINFO_Get = 0x25e01);
    PROVIDE(SYSCFG_ROMINFO_Set = 0x25e11);
    PROVIDE(SYSCFG_CHIPType_Get = 0x25e2d);
    PROVIDE(SYSCFG_OTP_RDPEn = 0x25e3d);
    PROVIDE(SYSCFG_OTP_RSIPEn = 0x25e5d);
    PROVIDE(SYSCFG_OTP_SBootEn = 0x25e7d);
    PROVIDE(SYSCFG_OTP_LowBaudLog = 0x25e9d);
    PROVIDE(SYSCFG_OTP_DisBootLog = 0x25ead);
    PROVIDE(SYSCFG_OTP_BootFlashBaud = 0x25ebd);
    PROVIDE(SYSCFG_OTP_SPICBootSpeedUpDis = 0x25ecd);
    PROVIDE(SYSCFG_OTP_SPICAddr4ByteEn = 0x25edd);
    PROVIDE(SYSCFG_OTP_FlashDSleepEn = 0x25eed);
    PROVIDE(SYSCFG_OTP_UartDownloadDis = 0x25efd);
    PROVIDE(SYSCFG_OTP_AntiRollback = 0x25f0d);
    PROVIDE(SYSCFG_OTP_FaultLogDis = 0x25f21);
    PROVIDE(SYSCFG_OTP_RSIPMode = 0x25f35);
    PROVIDE(SYSCFG_OTP_HUKDerivEn = 0x25f4d);
    PROVIDE(SYSCFG_OTP_RomPatchEn = 0x25f61);
    PROVIDE(SYSCFG_OTP_RandDelayEn = 0x25f81);
    PROVIDE(SYSCFG_OTP_RMAMode = 0x25f91);
    PROVIDE(SYSCFG_HWTrigDownload = 0x25fb5);
    PROVIDE(SYSCFG_SWTrigDownload = 0x25fc9);
    PROVIDE(SYSTIMER_Init = 0x25fed);
    PROVIDE(SYSTIMER_TickGet = 0x26039);
    PROVIDE(SYSTIMER_GetPassTick = 0x26049);
    PROVIDE(SYSTIMER_GetPassTime = 0x26069);
    PROVIDE(RTIM_TimeBaseStructInit = 0x26099);
    PROVIDE(RTIM_Cmd = 0x260b9);
    PROVIDE(RTIM_GetCount = 0x261e9);
    PROVIDE(RTIM_INTConfig = 0x262f9);
    PROVIDE(RTIM_INTClear = 0x26439);
    PROVIDE(RTIM_TimeBaseInit = 0x26559);
    PROVIDE(RTIM_DeInit = 0x2675d);
    PROVIDE(RTIM_INTClearPendingBit = 0x2687d);
    PROVIDE(RTIM_GetFlagStatus = 0x269b9);
    PROVIDE(RTIM_GetINTStatus = 0x26b2d);
    PROVIDE(TRNG_Random_Delay = 0x26cad);
    PROVIDE(TRNG_Security_Only = 0x26d2d);
    PROVIDE(TRNG_INTConfig = 0x26d79);
    PROVIDE(TRNG_ClearINT = 0x26dad);
    PROVIDE(TRNG_GetINT = 0x26dd5);
    PROVIDE(TRNG_Init = 0x26df9);
    PROVIDE(UART_DeInit = 0x26dfd);
    PROVIDE(UART_StructInit = 0x26e05);
    PROVIDE(UART_BaudParaGetFull = 0x26e21);
    PROVIDE(UART_BaudParaGet = 0x26e61);
    PROVIDE(UART_SetBaud = 0x26e89);
    PROVIDE(UART_SetBaudExt = 0x26f39);
    PROVIDE(UART_SetRxLevel = 0x26fc9);
    PROVIDE(UART_RxCmd = 0x26fd9);
    PROVIDE(UART_Writable = 0x26fed);
    PROVIDE(UART_Readable = 0x26ff5);
    PROVIDE(UART_CharPut = 0x26ffd);
    PROVIDE(UART_CharGet = 0x27001);
    PROVIDE(UART_ReceiveData = 0x27009);
    PROVIDE(UART_SendData = 0x27031);
    PROVIDE(UART_ReceiveDataTO = 0x27059);
    PROVIDE(UART_SendDataTO = 0x27095);
    PROVIDE(UART_RxByteCntClear = 0x270d1);
    PROVIDE(UART_RxByteCntGet = 0x270dd);
    PROVIDE(UART_BreakCtl = 0x270e5);
    PROVIDE(UART_ClearRxFifo = 0x270f9);
    PROVIDE(UART_Init = 0x27119);
    PROVIDE(UART_ClearTxFifo = 0x271b5);
    PROVIDE(UART_INTConfig = 0x271c1);
    PROVIDE(UART_INT_Clear = 0x271d1);
    PROVIDE(UART_IntStatus = 0x271d9);
    PROVIDE(UART_ModemStatusGet = 0x271dd);
    PROVIDE(UART_LineStatusGet = 0x271e1);
    PROVIDE(UART_WaitBusy = 0x271e5);
    PROVIDE(INT_HardFault = 0x27209);
    PROVIDE(INT_MemManage = 0x2722d);
    PROVIDE(INT_BusFault = 0x27251);
    PROVIDE(INT_UsageFault = 0x27275);
    PROVIDE(INT_SecureFault = 0x27299);
    PROVIDE(INT_HardFault_C = 0x272bd);
    PROVIDE(INT_NMI = 0x2770d);
    PROVIDE(irq_table_init = 0x278ad);
    PROVIDE(irq_enable = 0x27b4d);
    PROVIDE(irq_disable = 0x27b69);
    PROVIDE(irq_set_priority = 0x27b91);
    PROVIDE(irq_get_priority = 0x27bb9);
    PROVIDE(irq_set_pending = 0x27bd9);
    PROVIDE(irq_get_pending = 0x27bf9);
    PROVIDE(irq_clear_pending = 0x27c1d);
    PROVIDE(irq_register = 0x27c3d);
    PROVIDE(irq_unregister = 0x27c8d);
    PROVIDE(FaultPatch_register = 0x27cb1);
    PROVIDE(WDG_Wait_Busy = 0x27cc1);
    PROVIDE(WDG_StructInit = 0x27ce5);
    PROVIDE(WDG_Init = 0x27cf5);
    PROVIDE(WDG_Enable = 0x27d7d);
    PROVIDE(WDG_Timeout = 0x27dd9);
    PROVIDE(WDG_Refresh = 0x27e4d);
    PROVIDE(WDG_INTConfig = 0x27ea9);
    PROVIDE(WDG_ClearINT = 0x27f1d);
    PROVIDE(IWDG_LP_Enable = 0x27f89);
    PROVIDE(WDG_Reset_Bypass = 0x27fc9);
    PROVIDE(WDG_Reset_Timeout = 0x28059);
    PROVIDE(WDG_Reset_ClearINT = 0x28071);
    PROVIDE(DiagVSprintf = 0x28105);
    PROVIDE(DiagPrintf = 0x283cd);
    PROVIDE(DiagPrintfD = 0x28401);
    PROVIDE(DiagSPrintf = 0x28435);
    PROVIDE(DiagSnPrintf = 0x28469);
    PROVIDE(cmd_rom_table = 0x287e9);
    PROVIDE(_rand = 0x287f5);
    PROVIDE(__wrap_rand = 0x28851);
    PROVIDE(RandBytes_Get = 0x28855);
    PROVIDE(shell_array_init = 0x2891d);
    PROVIDE(shell_get_argc = 0x2892b);
    PROVIDE(shell_get_argv = 0x28957);
    PROVIDE(shell_cmd_chk = 0x28a25);
    PROVIDE(shell_uart_irq_rom = 0x28ae1);
    PROVIDE(shell_init_rom = 0x28c05);
    PROVIDE(shell_task_rom = 0x28c79);
    PROVIDE(shell_rom = 0x28ca9);
    PROVIDE(_char2num = 0x28d19);
    PROVIDE(_2char2dec = 0x28d49);
    PROVIDE(_2char2hex = 0x28dbd);
    PROVIDE(_memchr = 0x28e3d);
    PROVIDE(_memcmp = 0x28ebd);
    PROVIDE(_memcpy = 0x28f1d);
    PROVIDE(_memmove = 0x28fed);
    PROVIDE(_memset = 0x290e5);
    PROVIDE(_strcmp = 0x29189);
    PROVIDE(_stricmp = 0x29205);
    PROVIDE(_strlen = 0x29259);
    PROVIDE(_strtoul = 0x292b5);
    PROVIDE(_strupr = 0x293e1);
    PROVIDE(__wrap_memset = 0x293fd);
    PROVIDE(__wrap_memcpy = 0x29405);
    PROVIDE(__wrap_memcmp = 0x2940d);
    PROVIDE(__wrap_memchr = 0x29415);
    PROVIDE(__wrap_memmove = 0x2941d);
    PROVIDE(__wrap_strlen = 0x29425);
    PROVIDE(__wrap_strcmp = 0x2942d);
    PROVIDE(__wrap_stricmp = 0x29435);
    PROVIDE(__wrap_strupr = 0x2943d);
    PROVIDE(__wrap_strtoul = 0x29445);
    PROVIDE(mbedtls_asn1_get_len = 0x2944d);
    PROVIDE(mbedtls_asn1_get_tag = 0x294e1);
    PROVIDE(mbedtls_asn1_get_bool = 0x29509);
    PROVIDE(mbedtls_asn1_get_int = 0x29539);
    PROVIDE(mbedtls_asn1_get_mpi = 0x29581);
    PROVIDE(mbedtls_asn1_get_bitstring = 0x295a7);
    PROVIDE(mbedtls_asn1_get_bitstring_null = 0x295e7);
    PROVIDE(mbedtls_asn1_get_sequence_of = 0x2960f);
    PROVIDE(mbedtls_asn1_get_alg = 0x29691);
    PROVIDE(mbedtls_asn1_get_alg_null = 0x29727);
    PROVIDE(mbedtls_asn1_free_named_data = 0x29757);
    PROVIDE(mbedtls_asn1_free_named_data_list = 0x29781);
    PROVIDE(mbedtls_asn1_find_named_data = 0x297a5);
    PROVIDE(mbedtls_asn1_write_len = 0x297cd);
    PROVIDE(mbedtls_asn1_write_tag = 0x298b5);
    PROVIDE(mbedtls_asn1_write_raw_buffer = 0x298d1);
    PROVIDE(mbedtls_asn1_write_mpi = 0x298fb);
    PROVIDE(mbedtls_asn1_write_null = 0x2997d);
    PROVIDE(mbedtls_asn1_write_oid = 0x299a3);
    PROVIDE(mbedtls_asn1_write_algorithm_identifier = 0x299d9);
    PROVIDE(mbedtls_asn1_write_bool = 0x29a2b);
    PROVIDE(mbedtls_asn1_write_int = 0x29a75);
    PROVIDE(mbedtls_asn1_write_tagged_string = 0x29adb);
    PROVIDE(mbedtls_asn1_write_utf8_string = 0x29b1b);
    PROVIDE(mbedtls_asn1_write_printable_string = 0x29b2d);
    PROVIDE(mbedtls_asn1_write_ia5_string = 0x29b3f);
    PROVIDE(mbedtls_asn1_write_bitstring = 0x29b51);
    PROVIDE(mbedtls_asn1_write_octet_string = 0x29bdb);
    PROVIDE(mbedtls_asn1_store_named_data = 0x29c11);
    PROVIDE(mbedtls_mpi_init = 0x2a747);
    PROVIDE(mbedtls_mpi_free = 0x2a753);
    PROVIDE(mbedtls_mpi_grow = 0x2a781);
    PROVIDE(mbedtls_mpi_shrink = 0x2a7d9);
    PROVIDE(mbedtls_mpi_copy = 0x2a84d);
    PROVIDE(mbedtls_mpi_swap = 0x2a8a5);
    PROVIDE(mbedtls_mpi_safe_cond_assign = 0x2a8d1);
    PROVIDE(mbedtls_mpi_safe_cond_swap = 0x2a93b);
    PROVIDE(mbedtls_mpi_lset = 0x2a9bd);
    PROVIDE(mbedtls_mpi_get_bit = 0x2a9f5);
    PROVIDE(mbedtls_mpi_set_bit = 0x2aa17);
    PROVIDE(mbedtls_mpi_lsb = 0x2aa69);
    PROVIDE(mbedtls_mpi_bitlen = 0x2aa9d);
    PROVIDE(mbedtls_mpi_size = 0x2aad3);
    PROVIDE(mbedtls_mpi_read_binary = 0x2aadf);
    PROVIDE(mbedtls_mpi_write_binary = 0x2ab4b);
    PROVIDE(mbedtls_mpi_shift_l = 0x2abb7);
    PROVIDE(mbedtls_mpi_shift_r = 0x2ac57);
    PROVIDE(mbedtls_mpi_cmp_abs = 0x2accb);
    PROVIDE(mbedtls_mpi_cmp_mpi = 0x2ad31);
    PROVIDE(mbedtls_mpi_lt_mpi_ct = 0x2adb3);
    PROVIDE(mbedtls_mpi_cmp_int = 0x2ae43);
    PROVIDE(mbedtls_mpi_add_abs = 0x2ae6f);
    PROVIDE(mbedtls_mpi_sub_abs = 0x2af1f);
    PROVIDE(mbedtls_mpi_add_mpi = 0x2afbb);
    PROVIDE(mbedtls_mpi_sub_mpi = 0x2afff);
    PROVIDE(mbedtls_mpi_add_int = 0x2b045);
    PROVIDE(mbedtls_mpi_sub_int = 0x2b071);
    PROVIDE(mbedtls_mpi_mul_mpi = 0x2b09d);
    PROVIDE(mbedtls_mpi_mul_int = 0x2b165);
    PROVIDE(mbedtls_mpi_read_string = 0x2b17f);
    PROVIDE(mbedtls_mpi_div_mpi = 0x2b2a5);
    PROVIDE(mbedtls_mpi_div_int = 0x2b6af);
    PROVIDE(mbedtls_mpi_mod_mpi = 0x2b6d9);
    PROVIDE(mbedtls_mpi_mod_int = 0x2b73d);
    PROVIDE(mbedtls_mpi_write_string = 0x2b777);
    PROVIDE(mbedtls_mpi_exp_mod = 0x2b91d);
    PROVIDE(mbedtls_mpi_gcd = 0x2bd33);
    PROVIDE(mbedtls_mpi_fill_random = 0x2be3b);
    PROVIDE(mbedtls_mpi_inv_mod = 0x2c033);
    PROVIDE(mbedtls_mpi_is_prime_ext = 0x2c2eb);
    PROVIDE(mbedtls_mpi_is_prime = 0x2c345);
    PROVIDE(mbedtls_mpi_gen_prime = 0x2c34f);
    PROVIDE(mbedtls_ecdh_gen_public = 0x2c561);
    PROVIDE(mbedtls_ecdh_compute_shared = 0x2c595);
    PROVIDE(mbedtls_ecdh_init = 0x2c5e9);
    PROVIDE(mbedtls_ecdh_setup = 0x2c631);
    PROVIDE(mbedtls_ecdh_free = 0x2c645);
    PROVIDE(mbedtls_ecdh_make_params = 0x2c68d);
    PROVIDE(mbedtls_ecdh_read_params = 0x2c6f5);
    PROVIDE(mbedtls_ecdh_get_params = 0x2c735);
    PROVIDE(mbedtls_ecdh_make_public = 0x2c795);
    PROVIDE(mbedtls_ecdh_read_public = 0x2c7e1);
    PROVIDE(mbedtls_ecdh_calc_secret = 0x2c80d);
    PROVIDE(mbedtls_ecdsa_sign = 0x2cca5);
    PROVIDE(mbedtls_ecdsa_sign_det = 0x2ccc5);
    PROVIDE(mbedtls_ecdsa_sign_det_ext = 0x2cce5);
    PROVIDE(mbedtls_ecdsa_verify = 0x2ccf3);
    PROVIDE(mbedtls_ecdsa_write_signature_restartable = 0x2ccf5);
    PROVIDE(mbedtls_ecdsa_write_signature = 0x2cd9d);
    PROVIDE(mbedtls_ecdsa_write_signature_det = 0x2cdbd);
    PROVIDE(mbedtls_ecdsa_read_signature_restartable = 0x2cddb);
    PROVIDE(mbedtls_ecdsa_read_signature = 0x2ce79);
    PROVIDE(mbedtls_ecdsa_genkey = 0x2ce8b);
    PROVIDE(mbedtls_ecdsa_init = 0x2ceaf);
    PROVIDE(mbedtls_ecdsa_free = 0x2ceb3);
    PROVIDE(mbedtls_ecdsa_from_keypair = 0x2cebb);
    PROVIDE(mbedtls_ecp_curve_list = 0x2d999);
    PROVIDE(mbedtls_ecp_grp_id_list = 0x2d9a1);
    PROVIDE(mbedtls_ecp_curve_info_from_grp_id = 0x2d9d9);
    PROVIDE(mbedtls_ecp_curve_info_from_tls_id = 0x2d9f1);
    PROVIDE(mbedtls_ecp_curve_info_from_name = 0x2da0d);
    PROVIDE(mbedtls_ecp_point_init = 0x2da39);
    PROVIDE(mbedtls_ecp_group_init = 0x2da55);
    PROVIDE(mbedtls_ecp_keypair_init = 0x2da97);
    PROVIDE(mbedtls_ecp_point_free = 0x2dab3);
    PROVIDE(mbedtls_ecp_group_free = 0x2dab9);
    PROVIDE(mbedtls_ecp_keypair_free = 0x2db25);
    PROVIDE(mbedtls_ecp_copy = 0x2db45);
    PROVIDE(mbedtls_ecp_group_copy = 0x2db71);
    PROVIDE(mbedtls_ecp_set_zero = 0x2db77);
    PROVIDE(mbedtls_ecp_is_zero = 0x2e3b5);
    PROVIDE(mbedtls_ecp_point_cmp = 0x2e3c7);
    PROVIDE(mbedtls_ecp_point_read_string = 0x2e3fd);
    PROVIDE(mbedtls_ecp_point_write_binary = 0x2e429);
    PROVIDE(mbedtls_ecp_point_read_binary = 0x2e4c5);
    PROVIDE(mbedtls_ecp_tls_read_point = 0x2e535);
    PROVIDE(mbedtls_ecp_tls_write_point = 0x2e565);
    PROVIDE(mbedtls_ecp_tls_read_group_id = 0x2e595);
    PROVIDE(mbedtls_ecp_tls_read_group = 0x2e5d9);
    PROVIDE(mbedtls_ecp_tls_write_group = 0x2e5f5);
    PROVIDE(mbedtls_ecp_check_pubkey = 0x2e62d);
    PROVIDE(mbedtls_ecp_check_privkey = 0x2e7b5);
    PROVIDE(mbedtls_ecp_mul_restartable = 0x2e829);
    PROVIDE(mbedtls_ecp_mul = 0x2ed91);
    PROVIDE(mbedtls_ecp_muladd_restartable = 0x2ee23);
    PROVIDE(mbedtls_ecp_muladd = 0x2ee9d);
    PROVIDE(mbedtls_ecp_gen_privkey = 0x2eeb3);
    PROVIDE(mbedtls_ecp_gen_keypair_base = 0x2efc9);
    PROVIDE(mbedtls_ecp_gen_keypair = 0x2effd);
    PROVIDE(mbedtls_ecp_gen_key = 0x2f015);
    PROVIDE(mbedtls_ecp_check_pub_priv = 0x2f03d);
    PROVIDE(mbedtls_ecp_group_load = 0x2fe09);
    PROVIDE(mbedtls_hmac_drbg_init = 0x301e1);
    PROVIDE(mbedtls_hmac_drbg_update_ret = 0x301f5);
    PROVIDE(mbedtls_hmac_drbg_update = 0x30363);
    PROVIDE(mbedtls_hmac_drbg_seed_buf = 0x30367);
    PROVIDE(mbedtls_hmac_drbg_reseed = 0x303b3);
    PROVIDE(mbedtls_hmac_drbg_seed = 0x303b7);
    PROVIDE(mbedtls_hmac_drbg_set_prediction_resistance = 0x30421);
    PROVIDE(mbedtls_hmac_drbg_set_entropy_len = 0x30425);
    PROVIDE(mbedtls_hmac_drbg_set_reseed_interval = 0x30429);
    PROVIDE(mbedtls_hmac_drbg_random_with_add = 0x3042d);
    PROVIDE(mbedtls_hmac_drbg_random = 0x304ed);
    PROVIDE(mbedtls_hmac_drbg_free = 0x304fd);
    PROVIDE(mbedtls_md_list = 0x30519);
    PROVIDE(mbedtls_md_info_from_string = 0x30521);
    PROVIDE(mbedtls_md_info_from_type = 0x305c1);
    PROVIDE(mbedtls_md_init = 0x30605);
    PROVIDE(mbedtls_md_free = 0x3060f);
    PROVIDE(mbedtls_md_clone = 0x3064d);
    PROVIDE(mbedtls_md_setup = 0x30675);
    PROVIDE(mbedtls_md_init_ctx = 0x306bd);
    PROVIDE(mbedtls_md_starts = 0x306c3);
    PROVIDE(mbedtls_md_update = 0x306d9);
    PROVIDE(mbedtls_md_finish = 0x306ed);
    PROVIDE(mbedtls_md = 0x30701);
    PROVIDE(mbedtls_md_hmac_starts = 0x30721);
    PROVIDE(mbedtls_md_hmac_update = 0x307e9);
    PROVIDE(mbedtls_md_hmac_finish = 0x3080d);
    PROVIDE(mbedtls_md_hmac_reset = 0x3086d);
    PROVIDE(mbedtls_md_hmac = 0x3089d);
    PROVIDE(mbedtls_md_process = 0x308f5);
    PROVIDE(mbedtls_md_get_size = 0x30909);
    PROVIDE(mbedtls_md_get_type = 0x3090f);
    PROVIDE(mbedtls_md_get_name = 0x30915);
    PROVIDE(mbedtls_md5_init = 0x3091d);
    PROVIDE(mbedtls_md5_free = 0x30925);
    PROVIDE(mbedtls_md5_clone = 0x3092f);
    PROVIDE(mbedtls_md5_starts_ret = 0x30939);
    PROVIDE(mbedtls_md5_starts = 0x30965);
    PROVIDE(mbedtls_internal_md5_process = 0x30969);
    PROVIDE(mbedtls_md5_process = 0x310a1);
    PROVIDE(mbedtls_md5_update_ret = 0x310a5);
    PROVIDE(mbedtls_md5_update = 0x3111f);
    PROVIDE(mbedtls_md5_finish_ret = 0x31123);
    PROVIDE(mbedtls_md5_finish = 0x311d1);
    PROVIDE(mbedtls_md5_ret = 0x311d5);
    PROVIDE(mbedtls_md5 = 0x31231);
    PROVIDE(mbedtls_platform_zeroize = 0x3139d);
    PROVIDE(mbedtls_sha1_init = 0x313ad);
    PROVIDE(mbedtls_sha1_free = 0x313b5);
    PROVIDE(mbedtls_sha1_clone = 0x313bf);
    PROVIDE(mbedtls_sha1_starts_ret = 0x313c9);
    PROVIDE(mbedtls_sha1_starts = 0x313fd);
    PROVIDE(mbedtls_internal_sha1_process = 0x31401);
    PROVIDE(mbedtls_sha1_process = 0x3228d);
    PROVIDE(mbedtls_sha1_update_ret = 0x32291);
    PROVIDE(mbedtls_sha1_update = 0x3230b);
    PROVIDE(mbedtls_sha1_finish_ret = 0x3230f);
    PROVIDE(mbedtls_sha1_finish = 0x323d5);
    PROVIDE(mbedtls_sha1_ret = 0x323d9);
    PROVIDE(mbedtls_sha1 = 0x32417);
    PROVIDE(mbedtls_sha256_init = 0x3241d);
    PROVIDE(mbedtls_sha256_free = 0x32425);
    PROVIDE(mbedtls_sha256_clone = 0x3242f);
    PROVIDE(mbedtls_sha256_starts_ret = 0x32439);
    PROVIDE(mbedtls_sha256_starts = 0x324dd);
    PROVIDE(mbedtls_internal_sha256_process = 0x324e1);
    PROVIDE(mbedtls_sha256_process = 0x32be3);
    PROVIDE(mbedtls_sha256_update_ret = 0x32be7);
    PROVIDE(mbedtls_sha256_update = 0x32c61);
    PROVIDE(mbedtls_sha256_finish_ret = 0x32c65);
    PROVIDE(mbedtls_sha256_finish = 0x32d6b);
    PROVIDE(mbedtls_sha256_ret = 0x32d6f);
    PROVIDE(mbedtls_sha256 = 0x32db1);
    PROVIDE(mbedtls_sha512_init = 0x32db9);
    PROVIDE(mbedtls_sha512_free = 0x32dc1);
    PROVIDE(mbedtls_sha512_clone = 0x32dcb);
    PROVIDE(mbedtls_sha512_starts_ret = 0x32dd5);
    PROVIDE(mbedtls_sha512_starts = 0x32f09);
    PROVIDE(mbedtls_internal_sha512_process = 0x32f0d);
    PROVIDE(mbedtls_sha512_process = 0x33a9b);
    PROVIDE(mbedtls_sha512_update_ret = 0x33a9f);
    PROVIDE(mbedtls_sha512_update = 0x33b31);
    PROVIDE(mbedtls_sha512_finish_ret = 0x33b35);
    PROVIDE(mbedtls_sha512_finish = 0x33d6d);
    PROVIDE(mbedtls_sha512_ret = 0x33d71);
    PROVIDE(mbedtls_sha512 = 0x33db3);
    PROVIDE(__rom_stdlib_text_start__ = 0x34000);
    PROVIDE(__wrap_atoi = 0x34745);
    PROVIDE(__wrap_stratoi = 0x34745);
    PROVIDE(_atoi = 0x34745);
    PROVIDE(_stratoi = 0x34745);
    PROVIDE(__wrap_printf = 0x34771);
    PROVIDE(_printf = 0x34771);
    PROVIDE(__wrap_puts = 0x34821);
    PROVIDE(_puts = 0x34821);
    PROVIDE(__wrap_snprintf = 0x3488f);
    PROVIDE(_snprintf = 0x3488f);
    PROVIDE(__wrap_sprintf = 0x3492d);
    PROVIDE(_sprintf = 0x3492d);
    PROVIDE(__wrap_sscanf = 0x3496d);
    PROVIDE(_sscanf = 0x3496d);
    PROVIDE(__wrap_strcat = 0x34aa3);
    PROVIDE(_strcat = 0x34aa3);
    PROVIDE(__wrap_strchr = 0x34ad5);
    PROVIDE(_strchr = 0x34ad5);
    PROVIDE(__wrap_strcpy = 0x34b61);
    PROVIDE(_strcpy = 0x34b61);
    PROVIDE(__wrap_strncat = 0x34c79);
    PROVIDE(_strncat = 0x34c79);
    PROVIDE(__wrap_strncmp = 0x34cbb);
    PROVIDE(_strncmp = 0x34cbb);
    PROVIDE(__wrap_strncpy = 0x34d17);
    PROVIDE(_strncpy = 0x34d17);
    PROVIDE(__wrap_strpbrk = 0x34d6b);
    PROVIDE(_strpbrk = 0x34d6b);
    PROVIDE(__wrap_strsep = 0x34d9f);
    PROVIDE(_strsep = 0x34d9f);
    PROVIDE(__wrap_strstr = 0x34fd1);
    PROVIDE(_strstr = 0x34fd1);
    PROVIDE(__wrap_strtok = 0x35159);
    PROVIDE(_strtok = 0x35159);
    PROVIDE(__wrap_strtol = 0x352e9);
    PROVIDE(_strtol = 0x352e9);
    PROVIDE(__wrap_vsnprintf = 0x38173);
    PROVIDE(_vsnprintf = 0x38173);
    PROVIDE(__rom_stdlib_text_end__ = 0x40280);
    PROVIDE(__rom_entry_ns_start__ = 0x41000);
    PROVIDE(rom_sec_call_ns_entry = 0x41000);
    PROVIDE(__rom_entry_ns_end__ = 0x41010);
    PROVIDE(SPIC_CALIB_PATTERN = 0x41404);
    PROVIDE(GPIO_PORTx_S = 0x4140c);
    PROVIDE(GPIO_PORTx = 0x41414);
    PROVIDE(mbedtls_sha512_info = 0x42fcc);
    PROVIDE(mbedtls_sha384_info = 0x42ffc);
    PROVIDE(mbedtls_sha256_info = 0x4302c);
    PROVIDE(mbedtls_sha224_info = 0x4305c);
    PROVIDE(mbedtls_sha1_info = 0x4308c);
    PROVIDE(mbedtls_md5_info = 0x430bc);
    PROVIDE(__nspe_phy_addr__ = 0x8014000);
    PROVIDE(__km4_image3_flash_start__ = 0x81e0000);
    PROVIDE(__kr4_flash_text_start__ = 0xc000020);
    PROVIDE(__km4_flash_text_start__ = 0xe000020);
    PROVIDE(__km4_boot_text_start__ = 0xf800020);
    PROVIDE(__rom_bss_start__ = 0x20004000);
    PROVIDE(ConfigDebugClose = 0x20004000);
    PROVIDE(ConfigDebug = 0x20004004);
    PROVIDE(flash_init_para = 0x20004014);
    PROVIDE(shell_argv_array = 0x2000407c);
    PROVIDE(shell_ctl = 0x200040bc);
    PROVIDE(shell_rxbuf = 0x200040e0);
    PROVIDE(shell_buf = 0x20004160);
    PROVIDE(SHA_OTPKEY = 0x200041e0);
    PROVIDE(crypto_engine = 0x20004200);
    PROVIDE(crypto_sha_engine = 0x20004440);
    PROVIDE(RBSS_UDELAY_CLK = 0x200046dc);
    PROVIDE(FLASH_PLL_Source_IsCPU = 0x200046e0);
    PROVIDE(PortA_IrqHandler = 0x200046e4);
    PROVIDE(PortA_IrqData = 0x20004764);
    PROVIDE(PortB_IrqHandler = 0x200047e4);
    PROVIDE(PortB_IrqData = 0x20004864);
    PROVIDE(__rom_bss_end__ = 0x200048e4);
    PROVIDE(__rom_bss_start_ns__ = 0x20005000);
    PROVIDE(NewVectorTable = 0x20005000);
    PROVIDE(FaultPatchHandler = 0x20005180);
    PROVIDE(UserIrqFunTable = 0x20005184);
    PROVIDE(UserIrqDataTable = 0x200052c4);
    PROVIDE(p_rom_ssl_ram_map = 0x20005404);
    PROVIDE(rom_ssl_ram_map = 0x20005408);
    PROVIDE(__rom_bss_end_ns__ = 0x20005448);
    PROVIDE(__rom_stdlib_data_start__ = 0x20006000);
    PROVIDE(__rom_stdlib_data_end__ = 0x200069ac);
    PROVIDE(__rom_stdlib_bss_start__ = 0x200069c0);
    PROVIDE(__rom_stdlib_bss_end__ = 0x20006a0c);
    PROVIDE(__rom_stdlib_heap_start__ = 0x20006a20);
    PROVIDE(__kr4_image2_entry_func__ = 0x20040020);
    PROVIDE(__kr4_ipc_memory_start__ = 0x2005fd00);
    PROVIDE(__retention_entry_func__ = 0x21020000);
    PROVIDE(__rom_bss_start_s__ = 0x30007000);
    PROVIDE(__ram_start_table_start__ = 0x3000a020);
    PROVIDE(__rom_bss_end_s__ = 0x6000f000);
    PROVIDE(__kr4_psram_text_start__ = 0x60180020);
}
SECTIONS
{
    PROVIDE(__rom_stdlib_text_start__ = 0x34000);
    PROVIDE(_atoi = 0x34745);
    PROVIDE(_stratoi = 0x34745);
    PROVIDE(__wrap_atoi = 0x34745);
    PROVIDE(__wrap_stratoi = 0x34745);
    PROVIDE(__wrap_printf = 0x34771);
    PROVIDE(_printf = 0x34771);
    PROVIDE(__wrap_puts = 0x34821);
    PROVIDE(_puts = 0x34821);
    PROVIDE(__wrap_snprintf = 0x3488f);
    PROVIDE(_snprintf = 0x3488f);
    PROVIDE(__wrap_sprintf = 0x3492d);
    PROVIDE(_sprintf = 0x3492d);
    PROVIDE(__wrap_sscanf = 0x3496d);
    PROVIDE(_sscanf = 0x3496d);
    PROVIDE(__wrap_strcat = 0x34aa3);
    PROVIDE(_strcat = 0x34aa3);
    PROVIDE(__wrap_strchr = 0x34ad5);
    PROVIDE(_strchr = 0x34ad5);
    PROVIDE(__wrap_strcpy = 0x34b61);
    PROVIDE(_strcpy = 0x34b61);
    PROVIDE(__wrap_strncat = 0x34c79);
    PROVIDE(_strncat = 0x34c79);
    PROVIDE(__wrap_strncmp = 0x34cbb);
    PROVIDE(_strncmp = 0x34cbb);
    PROVIDE(__wrap_strncpy = 0x34d17);
    PROVIDE(_strncpy = 0x34d17);
    PROVIDE(__wrap_strpbrk = 0x34d6b);
    PROVIDE(_strpbrk = 0x34d6b);
    PROVIDE(__wrap_strsep = 0x34d9f);
    PROVIDE(_strsep = 0x34d9f);
    PROVIDE(__wrap_strstr = 0x34fd1);
    PROVIDE(_strstr = 0x34fd1);
    PROVIDE(__wrap_strtok = 0x35159);
    PROVIDE(_strtok = 0x35159);
    PROVIDE(__wrap_strtol = 0x352e9);
    PROVIDE(_strtol = 0x352e9);
    PROVIDE(__wrap_vsnprintf = 0x38173);
    PROVIDE(_vsnprintf = 0x38173);
    PROVIDE(__rom_stdlib_text_end__ = 0x40280);
    PROVIDE(__rom_stdlib_data_start__ = 0x20006000);
    PROVIDE(__rom_stdlib_data_end__ = 0x200069ac);
    PROVIDE(__rom_stdlib_bss_start__ = 0x200069c0);
    PROVIDE(__rom_stdlib_bss_end__ = 0x20006a0c);
    PROVIDE(__rom_stdlib_heap_start__ = 0x20006a20);
}
