/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/

#ifndef	__MPEG2_TS_TYPES_H
#define __MPEG2_TS_TYPES_H

#include <stdio.h>
#include <stdint.h>


// Section version number take 5 bits [0, 0x1F],
// We can regard 0xFF as invalid version number.
#define INVALID_VN      ((uint8_t)0xFF)
// Program number value 0 in PAT is used to specify NIT PID,
// We can regard 0 as invalid program number.
#define INVALID_PN      ((prog_num_t)0)
// Packet Identifier take 13 bits [0x0000, 0x1FFF],
// 0x1FFF is used to specify the empty packets,
// We can regard 0x1FFF as invalid PID.
#define INVALID_PID     ((ts_pid_t)0x1FFF)

// Types definition
typedef uint16_t        ts_pid_t;      // packet identifier
typedef uint8_t         table_id_t;    // table id
typedef uint16_t        prog_num_t;    // program number

#endif /* __MPEG2_TS_TYPES_H */
