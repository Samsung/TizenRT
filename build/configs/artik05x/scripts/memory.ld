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
    /* 2MB FLASH */

    kflash (rx)      : ORIGIN = 0x040c8000, LENGTH = 1200K
    uflash (rx)      : ORIGIN = 0x041F4000, LENGTH = 1200K

    /* 112Kb of contiguous SRAM */

    ksram (rwx)      : ORIGIN = 0x02023800, LENGTH = 242K
    usram (rwx)      : ORIGIN = 0x02060000, LENGTH = 128K
}
__kflash_segment_start__  = ORIGIN(kflash);
__kflash_segment_size__   = LENGTH(kflash);
__uflash_segment_start__  = ORIGIN(uflash);
__uflash_segment_size__   = LENGTH(uflash);
__ksram_segment_start__   = ORIGIN(ksram);
__ksram_segment_size__    = LENGTH(ksram);
__usram_segment_start__   = ORIGIN(usram);
__usram_segment_size__    = LENGTH(usram);
