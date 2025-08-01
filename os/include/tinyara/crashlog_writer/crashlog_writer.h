/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#ifndef __CRASH_LOG_H
#define __CRASH_LOG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
/****************************************************************************
 * Description:
 *   This is used to store logs that are output via lldbg() during a user crash situation into a buffer.
 *
 ****************************************************************************/
void crashlog_writer_crashlog_to_buffer(char ch);

/****************************************************************************
 * Description:
 *   If the flag is set to 1, crash logs start being stored in the buffer; if
 * it is set to 0, the storage stops.
 *
 ****************************************************************************/
void crashlog_writer_set_store_to_buffer_flag(int flag); 

/****************************************************************************
 * Description: 
 *   Saves the crash log to a file. 
 *   If the flag is 1, the log is saved in compressed form; if it is 0, it is saved uncompressed.
 *
 ****************************************************************************/
char* crashlog_writer_save_crashlog(int flag);

/*************************************************************************************
* Name: crashlog_writer_read
*
* Description: 
* 	Reads a log from a file and stores it in a buffer. If the log is compresed, it is
*	decompressed before being stored.
*************************************************************************************/
int crashlog_writer_read_crashlog(char *filename, char *buf, int buf_size);

/*************************************************************************************
* Name: crashlog_writer_read_recent
*
* Description: 
* 	Reads the most recent log from a file and stores it in a buffer. If the log is compresed, it is
*	decompressed before being stored.
*************************************************************************************/
int crashlog_writer_read_recent_crashlog(char *buf, int buf_size);
#endif
