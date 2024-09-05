/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
typedef enum {
	/* Memory Operations */
	CMN_LOG_ALLOC_FAIL,
	CMN_LOG_NULL_CHECK_FAIL,
	CMN_LOG_MEM_LEAK,
	CMN_LOG_BUFFER_OVERFLOW,

	/* File Operations */
	CMN_LOG_FILE_NFOUND,
	CMN_LOG_FILE_OPEN_ERROR,
	CMN_LOG_FILE_READ_ERROR,
	CMN_LOG_FILE_WRITE_ERROR,
	CMN_LOG_FILE_SEEK_ERROR,
	CMN_LOG_FILE_IOCTL_ERROR,

	/* Network Operations */
	CMN_LOG_CON_TIMEOUT,
	CMN_LOG_CON_REFUSED,
	CMN_LOG_DATA_TRANS_ERROR,
	CMN_LOG_HANDSHAKE_FAILED,
	CMN_LOG_CERTIFICATE_INVALID,

	/* Authentication and Authorization */
	CMN_LOG_INVALID_CREDENTIALS,

	/* Commonly used logs for printing information */
	CMN_LOG_VALUE_OF,
	CMN_LOG_INVALID_VAL,
	CMN_LOG_FAILED_OP,
	CMN_LOG_PERFORMING_OP,
	CMN_LOG_INIT_DONE,
	CMN_LOG_DEINIT_DONE,
	CMN_LOG_START,
	CMN_LOG_END,
	CMN_LOG_INVALID_DATA,
	CMN_LOG_VALID_DATA,
	CMN_LOG_NOT_SUPPORTED,
	CMN_LOG_MSG_MAX
} CLOG_INDEX;

static const char *clog_message_str[CMN_LOG_MSG_MAX] = {
	/* Memory Operations */
	"Memory allocation failure",	//CMN_LOG_ALLOC_FAIL,
	"Null pointer",				    //CMN_LOG_NULL_CHECK_FAIL,
	"Potential memory leak",	    //CMN_LOG_MEM_LEAK,
	"Buffer overflow detection",	//CMN_LOG_BUFFER_OVERFLOW,

	/* File Operations */
	"File not found",			//CMN_LOG_FILE_NFOUND,
	"File open failure",		//CMN_LOG_FILE_OPEN_ERROR,
	"File read failure",		//CMN_LOG_FILE_READ_ERROR,
	"File write failure",		//CMN_LOG_FILE_WRITE_ERROR,
	"File seek failure",		//CMN_LOG_FILE_SEEK_ERROR,
	"IOCTL call failure",		//CMN_LOG_FILE_IOCTL_ERROR,

	/* Network Operations */
	"Connection timeout",		//CMN_LOG_CON_TIMEOUT,
	"Connection refuse",		//CMN_LOG_CON_REFUSED,
	"Data transmission error",	//CMN_LOG_DATA_TRANS_ERROR,
	"Handshake failure",		//CMN_LOG_HANDSHAKE_FAILED,
	"Invalid certificate ",		//CMN_LOG_CERTIFICATE_INVALID,

	/* Authentication and Authorization */
	"Invalid credentials",		//CMN_LOG_INVALID_CREDENTIALS,

	/* Commonly used logs for printing information */
	"Value of",					//CMN_LOG_VALUE_OF,
	"Invalid value of",			//CMN_LOG_INVALID_VAL,
	"Operation failure",		//CMN_LOG_FAILED_OP,
	"Performing operation",		//CMN_LOG_PERFORMING_OP,
	"Init done",				//CMN_LOG_INIT_DONE,
	"Deinit done",				//CMN_LOG_DEINIT_DONE,
	"Start of",					//CMN_LOG_START,
	"End of",					//CMN_LOG_END,
	"Invalid data in",			//CMN_LOG_INVALID_DATA,
	"Valid data in :"			//CMN_LOG_VALID_DATA,
	"Not supported",		    // CMN_LOG_NOT_SUPPORTED
};
