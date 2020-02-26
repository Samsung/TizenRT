/****************************************************************************
+ *
+ * Copyright 2019 Samsung Electronics All Rights Reserved.
+ *
+ * Licensed under the Apache License, Version 2.0 (the "License");
+ * you may not use this file except in compliance with the License.
+ * You may obtain a copy of the License at
+ *
+ * http://www.apache.org/licenses/LICENSE-2.0
+ *
+ * Unless required by applicable law or agreed to in writing,
+ * software distributed under the License is distributed on an
+ * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
+ * either express or implied. See the License for the specific
+ * language governing permissions and limitations under the License.
+ *
+ ****************************************************************************/
/****************************************************************************
+ * configs/artik050/scripts/memory.ld
+ *
+ *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
+ *   Author: Gregory Nutt <gnutt@nuttx.org>
+ *
+ * Redistribution and use in source and binary forms, with or without
+ * modification, are permitted provided that the following conditions
+ * are met:
+ *
+ * 1. Redistributions of source code must retain the above copyright
+ *    notice, this list of conditions and the following disclaimer.
+ * 2. Redistributions in binary form must reproduce the above copyright
+ *    notice, this list of conditions and the following disclaimer in
+ *    the documentation and/or other materials provided with the
+ *    distribution.
+ * 3. Neither the name NuttX nor the names of its contributors may be
+ *    used to endorse or promote products derived from this software
+ *    without specific prior written permission.
+ *
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

MEMORY
{
	IROM (rx) : 			ORIGIN = 0x10100000, LENGTH = 0x1010A000 - 0x10100000	/* ROM: 40k */
	IROM_NS (rx) : 			ORIGIN = 0x1010A000, LENGTH = 0x10140000 - 0x1010A000	/* ROM: 216k */
	DROM_NS (rx) : 			ORIGIN = 0x101C0000, LENGTH = 0x101D4000 - 0x101C0000	/* ROM: 80k */
	DROM (rx) : 			ORIGIN = 0x101D4000, LENGTH = 0x101D8000 - 0x101D4000	/* ROM: 16k */
	ROMBSS_RAM_COM (rw)  : 		ORIGIN = 0x10000000, LENGTH = 0x10001000 - 0x10000000	/* ROM BSS COMMON(S & NS both used) RAM: 4K */
	ROMBSS_RAM_NS (rw)  : 		ORIGIN = 0x10001000, LENGTH = 0x10002000 - 0x10001000	/* ROM BSS NS RAM: 4K */
	RSVD_RAM_NS (rw)  : 		ORIGIN = 0x10002000, LENGTH = 0x10004000 - 0x10002000	/* RSVD RAM: 8K */
	MSP_RAM_NS (rw)  : 		ORIGIN = 0x10004000, LENGTH = 0x10005000 - 0x10004000	/* MSP_NS RAM: 4K */

	BD_RAM_NS (rwx)  : 		ORIGIN = 0x10005000, LENGTH = 0x1007C000 - 0x10005000	/* MAIN RAM NS: 456K */
	ROMBSS_RAM_S (rwx)  : 		ORIGIN = 0x1007C000, LENGTH = 0x1007D000 - 0x1007C000	/* ROM BSS RAM S: 4K */
	BOOTLOADER_RAM_S (rwx)  : 	ORIGIN = 0x1007D000, LENGTH = 0x1007F000 - 0x1007D000	/* BOOT Loader RAM: 8K */
	MSP_RAM_S (rwx)  : 		ORIGIN = 0x1007F000, LENGTH = 0x10080000 - 0x1007F000	/* MSP_S RAM: 4k */
	EXTENTION_SRAM (rwx)  : 		ORIGIN = 0x100E0000, LENGTH = 0x10100000 - 0x100E0000	/* EXTENTION SRAM: 128k */

	PSRAM_NS (rwx)  : 		ORIGIN = 0x02000000, LENGTH = 0x02400000 - 0x02000000	/* PSRAM_NS: 4M */

	/* Flash */
	KM0_BOOT (rx)  :		ORIGIN = 0x08000000+0x20, LENGTH = 0x02000-0x20	/* XIPBOOT: 8k, 32 Bytes resvd for header*/
	BACKUP (r) :			ORIGIN = 0x08002000, LENGTH = 0x1000	/* BACKUP: 4K system data in flash */
	XIPSYS (r) :			ORIGIN = 0x08003000, LENGTH = 0x1000	/* XIPSYS: 4K system data in flash */
	KM4_BOOT (rx)  :		ORIGIN = 0x08004000+0x20, LENGTH = 0x02000-0x20	/* XIPBOOT: 8k, 32 Bytes resvd for header*/
	KM0_IMG2 (rx)  : 		ORIGIN = 0x0C000000+0x20, LENGTH = 0x02000000-0x20	/* KM0_IMG2: 32MB, 32 Bytes resvd for header, virtual address */
	KM4_IMG2 (rx)  : 		ORIGIN = 0x0E000000+0x20, LENGTH = 0x02000000-0x20	/* KM4_IMG2 OTA1: 32MB, 32 Bytes resvd for header,  virtual address */
	BTRACE (rx) :			ORIGIN = 0x00800000, LENGTH = 0x00C00000 -0x00800000	/* Bluetooth Trace */
	
	/* KM0 RAM*/
	KM0_SRAM (rwx)  : 		ORIGIN = 0x00080000, LENGTH = 0x00090000 - 0x00080000	/* KM0 SRAM: 64k */
	RETENTION_RAM (rwx)  : 		ORIGIN = 0x000C0000, LENGTH = 0x000C0400 - 0x000C0000	/* KM0 Retention SRAM: 1k */
}
