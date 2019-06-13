/****************************************************************************
 * net/bluetooth/bt_hcicore.c
 * HCI core Bluetooth handling.
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Ported from the Intel/Zephyr arduino101_firmware_source-v1.tar package
 * where the code was released with a compatible 3-clause BSD license:
 *
 *   Copyright (c) 2016, Intel Corporation
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/clock.h>
#include <tinyara/kthread.h>
#include <tinyara/semaphore.h>
#include <tinyara/wqueue.h>
#include <tinyara/bluetooth/bluetooth.h>
#include <tinyara/bluetooth/bt_core.h>
#include <tinyara/bluetooth/bt_hci.h>

/****************************************************************************
 * Name: bt_hci_receive
 *
 * Description:
 *   Called by the Bluetooth low-level driver when new data is received from
 *   the radio.  This may be called from the low-level driver and is part of
 *   the driver interface prototyped in include/nuttx/wireless/bluetooth/bt_driver.h
 *
 *   NOTE:  This function will defer all real work to the low or to the high
 *   priority work queues.  Therefore, this function may safely be called
 *   from interrupt handling logic.
 *
 * Input Parameters:
 *   buf - An instance of the buffer structure providing the received frame.
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/

void bt_hci_receive(FAR struct bt_buf_s *buf)
{
  /* TODO: need to implement */
  return;
}
