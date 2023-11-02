/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 *
 *   Copyright (C) 2008, 2009, 2011-2014 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_FS_IOCTL_H
#define __INCLUDE_FS_IOCTL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* General ioctl definitions ************************************************/
/* Each TinyAra ioctl commands are uint16_t's consisting of an 8-bit type
 * identifier and an 8-bit command number.  All command type identifiers are
 * defined below:
 */

#define _TIOCBASE       (0x0100)	/* Terminal I/O ioctl commands */
#define _WDIOCBASE      (0x0200)	/* Watchdog driver ioctl commands */
#define _FIOCBASE       (0x0300)	/* File system ioctl commands */
#define _DIOCBASE       (0x0400)	/* Character driver ioctl commands */
#define _BIOCBASE       (0x0500)	/* Block driver ioctl commands */
#define _MTDIOCBASE     (0x0600)	/* MTD ioctl commands */
#define _SIOCBASE       (0x0700)	/* Socket ioctl commands */
#define _ARPIOCBASE     (0x0800)	/* ARP ioctl commands */
#define _TSIOCBASE      (0x0900)	/* Touchscreen ioctl commands */
#define _SNIOCBASE      (0x0a00)	/* Sensor ioctl commands */
#define _ANIOCBASE      (0x0b00)	/* Analog (DAC/ADC) ioctl commands */
#define _PWMIOCBASE     (0x0c00)	/* PWM ioctl commands */
#define _CAIOCBASE      (0x0d00)	/* CDC/ACM ioctl commands */
#define _BATIOCBASE     (0x0e00)	/* Battery driver ioctl commands */
#define _QEIOCBASE      (0x0f00)	/* Quadrature encoder ioctl commands */
#define _AUDIOIOCBASE   (0x1000)	/* Audio ioctl commands */
#define _SLCDIOCBASE    (0x1100)	/* Segment LCD ioctl commands */
#define _WLIOCBASE      (0x1200)	/* Wireless modules ioctl commands */
#define _CFGDIOCBASE    (0x1300)	/* Config Data device (app config) ioctl commands */
#define _TCIOCBASE      (0x1400)	/* Timer ioctl commands */
#define _DJOYBASE       (0x1500)	/* Discrete joystick ioctl commands */
#define _AJOYBASE       (0x1600)	/* Analog joystick ioctl commands */
#define _PIPEBASE       (0x1700)	/* FIFO/pipe ioctl commands */
#define _RTCBASE        (0x1800)	/* RTC ioctl commands */
#define _FOTABASE       (0x1900)	/* FOTA ioctl commands */
#define _GPIOBASE       (0x2000)	/* GPIO ioctl commands */
#define _TMBASE         (0x2100)	/* Task Management ioctl commands */
#define _MMINFOBASE     (0x2200)	/* Mminfo ioctl commands */
#define _SPIBASE        (0x2300)	/* SPI ioctl commands */
#define _LWNLIOCBASE    (0x2400)	/* LWNL ioctl commands */
#define _SECLINKBASE    (0x2500)	/* seclink ioctl commands */
#define _IOTBUSBASE     (0x2600)	/* iotbus ioctl commands */
#define _FBIOCBASE      (0x2700)	/* Frame buffer character driver ioctl commands */
#define _CPULOADBASE    (0x2800)	/* cpuload ioctl commands */
#define _COMPBASE       (0x2900)	/* compress ioctl commands */
#define _TESTIOCBASE    (0xfe00)	/* KERNEL TEST DRV module ioctl commands */



/* boardctl() commands share the same number space */
#define _BOARDBASE      (0xff00)	/* boardctl commands */

/* Macros used to manage ioctl commands */

#define _IOC_MASK       (0x00ff)
#define _IOC_TYPE(cmd)  ((cmd)&~_IOC_MASK)
#define _IOC_NR(cmd)    ((cmd)&_IOC_MASK)

#define _IOC(type, nr)  ((type)|(nr))

/* Terminal I/O ioctl commands **********************************************/

#define _TIOCVALID(c)   (_IOC_TYPE(c) == _TIOCBASE)
#define _TIOC(nr)       _IOC(_TIOCBASE, nr)

/* Terminal I/O IOCTL definitions are retained in tioctl.h */

#ifndef NXFUSE_HOST_BUILD
#include <tinyara/serial/tioctl.h>
#endif

/* Watchdog driver ioctl commands *******************************************/

#define _WDIOCVALID(c)  (_IOC_TYPE(c) == _WDIOCBASE)
#define _WDIOC(nr)      _IOC(_WDIOCBASE, nr)

/* TinyAra file system ioctl definitions **************************************/

#define _FIOCVALID(c)   (_IOC_TYPE(c) == _FIOCBASE)
#define _FIOC(nr)       _IOC(_FIOCBASE, nr)

#define FIOC_MMAP       _FIOC(0x0001)	/* IN:  Location to return address (void **)
										 * OUT: If media is directly accessible,
										 *      return (void*) base address
										 *      of file
										 */
#define FIOC_REFORMAT   _FIOC(0x0002)	/* IN:  None
										 * OUT: None
										 */
#define FIOC_OPTIMIZE   _FIOC(0x0003)	/* IN:  None
										 * OUT: None
										 */
#define FIOC_FILENAME   _FIOC(0x0004)	/* IN:  FAR const char ** pointer
										 * OUT: Pointer to a persistent file name
										 *      (Guaranteed to persist while the file
										 *      is open).
										 */

#define FIONREAD        _FIOC(0x0005)	/* IN:  Location to return value (int *)
										 * OUT: Bytes readable from this fd
										 */
#define FIONWRITE       _FIOC(0x0006)	/* IN:  Location to return value (int *)
										 * OUT: Bytes writable to this fd
										 */

#define FIONBIO         _FIOC(0x000b)     /* IN:  Boolean option takes an int value.
										 * OUT: Origin option.
										 */

/* TinyAra file system ioctl definitions **************************************/

#define _DIOCVALID(c)   (_IOC_TYPE(c) == _DIOCBASE)
#define _DIOC(nr)       _IOC(_DIOCBASE, nr)

#define DIOC_GETPRIV    _DIOC(0x0001)	/* IN:  Location to return handle (void **)
										 * OUT: Reference to internal data
										 *      structure.  May have a reference
										 *      incremented.
										 */
#define DIOC_RELPRIV    _DIOC(0x0003)	/* IN:  None
										 * OUT: None, reference obtained by
										 *      FIOC_GETPRIV released.
										 */

#define DIOC_SETKEY     _DIOC(0X0004)	/* IN:  Encryption key
										 * OUT: None
										 */

/* TinyAra block driver ioctl definitions *************************************/

#define _BIOCVALID(c)   (_IOC_TYPE(c) == _BIOCBASE)
#define _BIOC(nr)       _IOC(_BIOCBASE, nr)

#define BIOC_XIPBASE    _BIOC(0x0001)	/* Perform mapping to random access memory.
										 * IN:  Pointer to pointer to void in
										 *      which to received the XIP base.
										 * OUT: If media is directly accessible,
										 *      return (void*) base address
										 *      of device memory */
#define BIOC_PROBE      _BIOC(0x0002)	/* Re-probe and interface; check for media
										 * in the slot
										 * IN:  None
										 * OUT: None (ioctl return value provides
										 *      success/failure indication). */
#define BIOC_EJECT      _BIOC(0x0003)	/* Eject/disable media in the slot
										 * IN:  None
										 * OUT: None (ioctl return value provides
										 *      success/failure indication). */
#define BIOC_LLFORMAT   _BIOC(0x0004)	/* Low-Level Format on SMART flash devices
										 * IN:  None
										 * OUT: None (ioctl return value provides
										 *      success/failure indication). */
#define BIOC_GETFORMAT  _BIOC(0x0005)	/* Returns SMART flash format information
										 * such as format status, logical sector
										 * size, total sectors, free sectors, etc.
										 * IN:  None
										 * OUT: Pointer to the format information. */
#define BIOC_ALLOCSECT  _BIOC(0x0006)	/* Allocate a logical sector from the block
										 * device.
										 * IN:  None
										 * OUT: Logical sector number allocated. */
#define BIOC_FREESECT   _BIOC(0x0007)	/* Allocate a logical sector from the block
										 * device.
										 * IN:  None
										 * OUT: Logical sector number allocated. */
#define BIOC_READSECT   _BIOC(0x0008)	/* Read a logical sector from the block
										 * device.
										 * IN:  Pointer to sector read data (the
										 *      logical sector number, count and
										 *      read buffer address
										 * OUT: Number of bytes read or error */
#define BIOC_WRITESECT  _BIOC(0x0009)	/* Write to data to a logical sector
										 * IN:  Pointer to sector write data (the
										 *      logical sector number and write
										 *      buffer address
										 * OUT: None (ioctl return value provides
										 *      success/failure indication). */
#define BIOC_GETPROCFSD _BIOC(0x000A)	/* Get ProcFS data specific to the
										 * block device.
										 * IN:  Pointer to a struct defined for
										 *      the block to load with it's
										 *      ProcFS data.
										 * OUT: None (ioctl return value provides
										 *      success/failure indication). */
#define BIOC_BULKERASE  _BIOC(0x000B)	/* Request Bulk erase to block driver
										 * IN:	None
										 * OUT: None (ioctl return value provides
										 *      success/failure indication). */

#define BIOC_CORRUPTION _BIOC(0x000C)	/* Request corrupt blocks for testing(usually remove root sector)
										 * IN:	None
										 * OUT: None (ioctl return value provides
										 *		success/failure indication). */

#define BIOC_FIBMAP     _BIOC(0x000D)	/* Reveal Physical sector number from bitmap
										 * of block device.
										 * IN:	Logical sector number which need
										 *		to reveal physical sector.
										 * OUT: Physical sector number align with
										 *		logical sector number */
#define BIOC_DEBUGCMD   _BIOC(0x00FF)	/* Send driver specific debug command /
										 * data to the block device.
										 * IN:  Pointer to a struct defined for
										 *      the block with specific debug
										 *      command and data.
										 * OUT: None.  */

/* TinyAra MTD driver ioctl definitions ***************************************/

#define _MTDIOCVALID(c)   (_IOC_TYPE(c) == _MTDIOCBASE)
#define _MTDIOC(nr)       _IOC(_MTDIOCBASE, nr)

#define MTDIOC_GEOMETRY   _MTDIOC(0x0001)	/* IN:  Pointer to write-able struct
											 *      mtd_geometry_s in which to receive
											 *      receive geometry data (see mtd.h)
											 * OUT: Geometry structure is populated
											 *      with data for the MTD */
#define MTDIOC_XIPBASE    _MTDIOC(0x0002)	/* IN:  Pointer to pointer to void in
											 *      which to received the XIP base.
											 * OUT: If media is directly accessible,
											 *      return (void*) base address
											 *      of device memory */
#define MTDIOC_BULKERASE  _MTDIOC(0x0003)	/* IN:  None
											 * OUT: None */
#define MTDIOC_SETSPEED   _MTDIOC(0x0004)	/* IN:  New bus speed in Hz
											 * OUT: None */

/* TinyAra ARP driver ioctl definitions (see include/netinet/arp.h) *******************/

#define _ARPIOCVALID(c)   (_IOC_TYPE(c) == _ARPIOCBASE)
#define _ARPIOC(nr)       _IOC(_ARPIOCBASE, nr)

/* TinyAra touchscreen ioctl definitions (see include/tinyara/input/touchscreen.h) ******/

#define _TSIOCVALID(c)    (_IOC_TYPE(c) == _TSIOCBASE)
#define _TSIOC(nr)        _IOC(_TSIOCBASE, nr)

/* TinyAra sensor ioctl definitions (see include/tinyara/sensor/xxx.h) ******************/

#define _SNIOCVALID(c)    (_IOC_TYPE(c) == _SNIOCBASE)
#define _SNIOC(nr)        _IOC(_SNIOCBASE, nr)

/* TinyAra Analog (DAC/ADC_ ioctl commands ************************************/

#define _ANIOCVALID(c)    (_IOC_TYPE(c) == _ANIOCBASE)
#define _ANIOC(nr)        _IOC(_ANIOCBASE, nr)

#define ANIOC_TRIGGER     _ANIOC(0x0001)	/* Trigger one conversion
											 * IN: None
											 * OUT: None */

/* TinyAra PWM ioctl definitions (see include/tinyara/pwm.h) ****************************/

#define _PWMIOCVALID(c)   (_IOC_TYPE(c) == _PWMIOCBASE)
#define _PWMIOC(nr)       _IOC(_PWMIOCBASE, nr)

/* TinyAra USB CDC/ACM serial driver ioctl definitions ************************/
/* (see include/tinyara/usb/cdcacm.h) */

#define _CAIOCVALID(c)    (_IOC_TYPE(c) == _CAIOCBASE)
#define _CAIOC(nr)        _IOC(_CAIOCBASE, nr)

/* TinyAra battery driver ioctl definitions ************************/
/* (see include/tinyara/power/battery_ioctl.h) */

#define _BATIOCVALID(c)   (_IOC_TYPE(c) == _BATIOCBASE)
#define _BATIOC(nr)       _IOC(_BATIOCBASE, nr)

/* TinyAra Quadrature Encoder driver ioctl definitions ************************/
/* (see include/tinyara/sensors/qencoder.h) */

#define _QEIOCVALID(c)    (_IOC_TYPE(c) == _QEIOCBASE)
#define _QEIOC(nr)        _IOC(_QEIOCBASE, nr)

/* Audio driver ioctl definitions *************************************/
/* (see tinyara/audio/audio.h) */

#define _AUDIOIOCVALID(c) (_IOC_TYPE(c) == _AUDIOIOCBASE)
#define _AUDIOIOC(nr)     _IOC(_AUDIOIOCBASE, nr)

/* Frame buffer character drivers *******************************************/

#define _FBIOCVALID(c)    (_IOC_TYPE(c) == _FBIOCBASE)
#define _FBIOC(nr)        _IOC(_FBIOCBASE, nr)

/* Application Config Data driver ioctl definitions *************************/
/* (see include/tinyara/configdata.h */

#define _CFGDIOCVALID(c)   (_IOC_TYPE(c) == _CFGDIOCBASE)
#define _CFGDIOC(nr)       _IOC(_CFGDIOCBASE, nr)

/* Timer driver ioctl commands **********************************************/
/* (see include/tinyara/timer.h */

#define _TCIOCVALID(c)     (_IOC_TYPE(c) == _TCIOCBASE)
#define _TCIOC(nr)         _IOC(_TCIOCBASE, nr)

/* FIFOs and pipe driver ioctl definitions **********************************/

#define _PIPEIOCVALID(c)   (_IOC_TYPE(c) == _PIPEBASE)
#define _PIPEIOC(nr)       _IOC(_PIPEBASE, nr)

#define PIPEIOC_POLICY     _PIPEIOC(0x0001)	/* Set buffer policy
											 * IN: unsigned long integer
											 *     0=free on last close
											 *       (default)
											 *     1=fre when empty
											 * OUT: None */
/* RTC driver ioctl definitions *********************************************/
/* (see include/tinyara/rtc.h */

#define _RTCIOCVALID(c)   (_IOC_TYPE(c) == _RTCBASE)
#define _RTCIOC(nr)       _IOC(_RTCBASE, nr)

/* FOTA driver ioctl definitions */
#define _FOTAIOCVALID(c)   (_IOC_TYPE(c) == _FOTABASE)
#define _FOTAIOC(nr)       _IOC(_FOTABASE, nr)
#define FOTA_GET_PART      _FOTAIOC(0x0001)
#define FOTA_SET_PART      _FOTAIOC(0x0002)
#define FOTA_SET_BIN       _FOTAIOC(0x0003)
#define FOTA_SET_PARAM     _FOTAIOC(0x0004)

/* GPIO driver ioctl definitions ********************************************/
/* (see include/tinyara/gpio.h */
#define _GPIOIOCVALID(c)   (_IOC_TYPE(c) == _GPIOBASE)
#define _GPIOIOC(nr)       _IOC(_GPIOBASE, nr)

/* SPI driver ioctl definitions ********************************************/
/* (see include/tinyara/spi/spi.h */
#define _SPIIOCVALID(c)   (_IOC_TYPE(c) == _SPIBASE)
#define _SPIIOC(nr)       _IOC(_SPIBASE, nr)

/* boardctl() command definitions *******************************************/
#define _BOARDIOCVALID(c)  (_IOC_TYPE(c) == _BOARDBASE)
#define _BOARDIOC(nr)      _IOC(_BOARDBASE, nr)

/* seclink driver ioctl definitions ********************************************/
/* (see include/tinyara/seclink.h */
#define _SECLINKIOCVALID(c)   (_IOC_TYPE(c) == _SECLINKBASE)
#define _SECLINKIOC(nr)       _IOC(_SECLINKBASE, nr)

/* iotbus driver ioctl definitions ********************************************/
/* (see include/tinyara/iotbus_sig.h */
#define _IOTBUSIOCVALID(c)   (_IOC_TYPE(c) == _IOTBUSBASE)
#define _IOTBUSIOC(nr)       _IOC(_IOTBUSBASE, nr)

/* os_api_tc driver ioctl definitions *************************************/
/* (see tinyara/os_api_test_drv.h) */

#define _TESTIOCVALID(c) (_IOC_TYPE(c) == _TESTIOCBASE)
#define _TESTIOC(nr)     _IOC(_TESTIOCBASE, nr)

/* TinyAra Task Management driver ioctl definitions ************************/

#define _TMIOCVALID(c)   (_IOC_TYPE(c) == _TMBASE)
#define _TMIOC(nr)       _IOC(_TMBASE, nr)

#define TMIOC_START                _TMIOC(0x0001)
#define TMIOC_PAUSE                _TMIOC(0x0002)
#define TMIOC_RESUME               _TMIOC(0x0003)
#define TMIOC_UNICAST              _TMIOC(0x0004)
#define TMIOC_BROADCAST            _TMIOC(0x0005)
#define TMIOC_CHECK_ALIVE          _TMIOC(0x0006)
#define TMIOC_TERMINATE            _TMIOC(0x0007)
#define TMIOC_EXITCB               _TMIOC(0x0008)
#if defined(HAVE_TASK_GROUP) && !defined(CONFIG_DISABLE_PTHREAD)
#define TMIOC_PTHREAD_PARENT       _TMIOC(0x0009)
#endif

/* Mminfo driver ioctl definitions ******************************************/
#define _MMINFOIOCVALID(c)   (_IOC_TYPE(c) == _MMINFOBASE)
#define _MMINFOIOC(nr)       _IOC(_MMINFOBASE, nr)

#define MMINFOIOC_HEAP              _MMINFOIOC(0x0001)
#define MMINFOIOC_PARSE             _MMINFOIOC(0x0002)
#define MMINFOIOC_MNG_ALLOCFAIL     _MMINFOIOC(0x0003)

/* Compress driver ioctl definitions ************************/
#define _COMPIOCVALID(c)    (_IOC_TYPE(c) == _COMPBASE)
#define _COMPIOC(nr)        _IOC(_COMPBASE, nr)

#define COMPIOC_COMPRESS           _COMPIOC(0x0001)
#define COMPIOC_GET_COMP_TYPE      _COMPIOC(0x0002)
#define COMPIOC_GET_COMP_NAME      _COMPIOC(0x0003)
#define COMPIOC_DECOMPRESS	   _COMPIOC(0x0004)

/* Cpuload driver ioctl definitions ************************/

#define _CPULOADIOCVALID(c)   (_IOC_TYPE(c) == _CPULOADBASE)
#define _CPULOADIOC(nr)       _IOC(_CPULOADBASE, nr)

#define CPULOADIOC_START              _CPULOADIOC(0x0001)
#define CPULOADIOC_STOP               _CPULOADIOC(0x0002)
#define CPULOADIOC_GETVALUE           _CPULOADIOC(0x0003)

/* Audio driver ioctl definitions *************************************/
/* (see tinyara/audio/audio.h) */

#define _LWNLIOCVALID(c) (_IOC_TYPE(c) == _LWNLIOCBASE)
#define _LWNLIOC(nr)     _IOC(_LWNLIOCBASE, nr)

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_FS_IOCTL_H */
