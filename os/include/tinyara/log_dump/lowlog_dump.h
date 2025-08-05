/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#ifndef __LOWLOG_DUMP_H__
#define __LOWLOG_DUMP_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
/****************************************************************************
 * Description:
 *   This is used to store logs that are output via lldbg() during a user crash situation into a buffer.
 *
 ****************************************************************************/
void lowlog_dump_save_ch(char ch);

/****************************************************************************
 * Description:
 *   If the flag is set to 1, crash logs start being stored in the buffer; if
 * it is set to 0, the storage stops.
 *
 ****************************************************************************/
void lowlog_dump_set_flag(int flag); 

/****************************************************************************
 * Description: 
 *   Saves the crash log to a file. 
 *
 ****************************************************************************/
int lowlog_dump_save();

/*************************************************************************************
* Description: 
* 	Reads a log from a file and stores it in a buffer. If the log is compresed, it is
*	decompressed before being stored.
*************************************************************************************/
int lowlog_dump_read(char *filename, char *buf, int buf_size);

/*************************************************************************************
* Description: 
* 	Reads the most recent log from a file and stores it in a buffer. If the log is compresed, it is
*	decompressed before being stored.
*************************************************************************************/
int lowlog_dump_read_recent(char *buf, int buf_size);
#endif
