/*****************************************************************************
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

#ifndef PANIC_RECORD_R4_DEFS_H__
#define PANIC_RECORD_R4_DEFS_H__
/*
 * R4 Panic Record Definitions.
 *
 * This record is used to pass summary information about the context of
 * Maxwell R4 firmware panics to the host.
 *
 * The record location, relative to shared DRAM memory, is defined by the
 * R4_PANIC_RECORD_OFFSET field in the firmware header [see SC-505846-SW].
 *
 * Notes:-
 * - The host panic handler should _not_ expect the R4 record to be
 * written prior to a panic indication from Maxwell, and it may never
 * be written at all. The checksum should indicate a valid record.
 *
 * N.B. Defined in this standalone header for inclusion in .s and .c.
 */

/*
 * The current version of the PANIC_RECORD_R4 structure defined below.
 * Written to version field by firmware, checked by host.
 * This also serves as a rudimentary endianess check.
 */
#define PANIC_RECORD_R4_VERSION_1 1

/*
 * Total number of R4 registers saved.
 */
#define PANIC_RECORD_R4_REGISTER_COUNT  18

/*
 * Number of panic info arguments.
 */
#define PANIC_RECORD_R4_INFO_COUNT  4

/*
 * Checksum seed to prevent false match on all zeros or ones.
 */
#define PANIC_RECORD_R4_CKSUM_SEED  0xa5a5a5a5

/*****************************************************************************
 * R4 Panic Record 32bit field indices.
 *****************************************************************************/

/*
 * Version of this structure.
 */
#define PANIC_RECORD_R4_VERSION_INDEX   0

/*
 * Clock counters at time of the R4 panic.
 *
 * The 1M clock is generally the most useful but there is period
 * after IP wake-up when it is not monotonic. The 32K count
 * is included in-case of a panic during wake-up.
 */
#define PANIC_RECORD_R4_TIMESTAMP_1M_INDEX      (PANIC_RECORD_R4_VERSION_INDEX + 1)
#define PANIC_RECORD_R4_TIMESTAMP_32K_INDEX     (PANIC_RECORD_R4_TIMESTAMP_1M_INDEX + 1)

/*
 * Snapshot of main r4 CPU registers.
 */
#define PANIC_RECORD_R4_REGISTERS_INDEX (PANIC_RECORD_R4_TIMESTAMP_32K_INDEX + 1)

/*
 * Panic info.
 *
 * 1st field is key/index of panic_string.
 */
#define PANIC_RECORD_R4_INFO_INDEX              (PANIC_RECORD_R4_REGISTERS_INDEX + PANIC_RECORD_R4_REGISTER_COUNT)

/*
 * 32bit XOR of all the fields above + PANIC_RECORD_R4_CKSUM_SEED
 *
 * Written by firmware on panic, checked by host.
 */
#define PANIC_RECORD_R4_CKSUM_INDEX             (PANIC_RECORD_R4_INFO_INDEX + PANIC_RECORD_R4_INFO_COUNT)

/*
 * Length of the r4 panic record (uint32s).
 */
#define PANIC_RECORD_R4_LEN                     (PANIC_RECORD_R4_CKSUM_INDEX + 1)

/*****************************************************************************
 * R4 uint32 Register indices relative to PANIC_RECORD_R4_REGISTERS_INDEX
 *****************************************************************************/

#define PANIC_RECORD_R4_REGISTER_R0   0
#define PANIC_RECORD_R4_REGISTER_R1   1
#define PANIC_RECORD_R4_REGISTER_R2   2
#define PANIC_RECORD_R4_REGISTER_R3   3
#define PANIC_RECORD_R4_REGISTER_R4   4
#define PANIC_RECORD_R4_REGISTER_R5   5
#define PANIC_RECORD_R4_REGISTER_R6   6
#define PANIC_RECORD_R4_REGISTER_R7   7
#define PANIC_RECORD_R4_REGISTER_R8   8
#define PANIC_RECORD_R4_REGISTER_R9   9
#define PANIC_RECORD_R4_REGISTER_R10  10
#define PANIC_RECORD_R4_REGISTER_R11  11
#define PANIC_RECORD_R4_REGISTER_R12  12
#define PANIC_RECORD_R4_REGISTER_SP   13
#define PANIC_RECORD_R4_REGISTER_LR   14
#define PANIC_RECORD_R4_REGISTER_SPSR 15
#define PANIC_RECORD_R4_REGISTER_PC   16
#define PANIC_RECORD_R4_REGISTER_CPSR 17

/*****************************************************************************
 * R4 Register octet offsets relative to PANIC_RECORD_R4_REGISTERS_INDEX
 *****************************************************************************/

#define PANIC_RECORD_R4_REGISTER_OFFSET_R0   (PANIC_RECORD_R4_REGISTER_R0 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R1   (PANIC_RECORD_R4_REGISTER_R1 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R2   (PANIC_RECORD_R4_REGISTER_R2 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R3   (PANIC_RECORD_R4_REGISTER_R3 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R4   (PANIC_RECORD_R4_REGISTER_R4 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R5   (PANIC_RECORD_R4_REGISTER_R5 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R6   (PANIC_RECORD_R4_REGISTER_R6 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R7   (PANIC_RECORD_R4_REGISTER_R7 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R8   (PANIC_RECORD_R4_REGISTER_R8 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R9   (PANIC_RECORD_R4_REGISTER_R9 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R10  (PANIC_RECORD_R4_REGISTER_R10 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R11  (PANIC_RECORD_R4_REGISTER_R11 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_R12  (PANIC_RECORD_R4_REGISTER_R12 * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_SP   (PANIC_RECORD_R4_REGISTER_SP * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_LR   (PANIC_RECORD_R4_REGISTER_LR * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_SPSR (PANIC_RECORD_R4_REGISTER_SPSR * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_PC   (PANIC_RECORD_R4_REGISTER_PC * 4)
#define PANIC_RECORD_R4_REGISTER_OFFSET_CPSR (PANIC_RECORD_R4_REGISTER_CPSR * 4)

#endif							/* PANIC_RECORD_R4_DEFS_H__ */
