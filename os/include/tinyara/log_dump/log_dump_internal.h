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

#ifndef __LOG_DUMP_INTERNAL_H
#define __LOG_DUMP_INTERNAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
/****************************************************************************
 * Description:
 *   This is used to save each character to log buffer
 *
 ****************************************************************************/
int log_dump_save(char ch);

/****************************************************************************
 * Description:
 *   This is used to read the logs saved in the log buffer
 *
 ****************************************************************************/
size_t log_dump_read(FAR char *buffer, size_t buflen);

/****************************************************************************
 * Description:
 *   This is used to wake up the compression thread to compress current data
 *
 ****************************************************************************/
int log_dump_read_wake(void);

/****************************************************************************
 * Description:
 *   This is used to START and STOP storing the log dump
 *   buffer contents either LOGDUMP_SAVE_START or LOGDUMP_SAVE_STOP
 ****************************************************************************/
int log_dump_set(FAR const char *buffer, size_t buflen);

/****************************************************************************
 * Description:
 *   Thread used to receive log data and compress the same
 *
 ****************************************************************************/
int log_dump(int argc, char *argv[]);

#endif							/* __LOG_DUMP_INTERNAL_H */
