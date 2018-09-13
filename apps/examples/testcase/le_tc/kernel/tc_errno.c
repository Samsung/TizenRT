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

/// @file tc_errno.c

/// @brief Test Case Example for Errno API
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "tc_internal.h"

#define ERROR_NUM_START 1
#define ERROR_NUM_END   124

/**
 * @fn                  :tc_errno_set_get_errno
 * @brief               :set error code , get error code
 * @scenario            :set error code and get error code, get value should be equal to set value
 * API's covered        :set_errno, get_errno
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_errno_set_get_errno(void)
{
	int ret_chk = 0;
	int errcode;
#ifdef CONFIG_LIBC_STRERROR
	char *ret_err = NULL;
	const char *errstr_arr[ERROR_NUM_END] =	{ "Operation not permitted", "No such file or directory", "No such process", "Interrupted system call", "I/O error", "No such device or address", "Arg list too long", "Exec format error", "Bad file number", "No child processes", "Try again", "Out of memory", "Permission denied", "Bad address", "Block device required", "Device or resource busy", "File exists", "Cross-device link", "No such device", "Not a directory", "Is a directory", "Invalid argument", "File table overflow", "Too many open files", "Not a typewriter", "Text file busy", "File too large", "No space left on device", "Illegal seek", "Read-only file system", "Too many links", "Broken pipe", "Math argument out of domain of func", "Math result not representable", "Resource deadlock would occur", "File name too long", "No record locks available", "Function not implemented", "Directory not empty", "Too many symbolic links encountered", "Operation would block", "No message of desired type", "Identifier removed",
"Channel number out of range", "Level 2 not synchronized", "Level 3 halted", "Level 3 reset", "Link number out of range", "Protocol driver not attached", "No CSI structure available", "Level 2 halted", "Invalid exchange", "Invalid request descriptor", "Exchange full", "No anode", "Invalid request code", "Invalid slot", "Unknown", "Bad font file format", "Device not a stream", "No data available", "Timer expired", "Out of streams resources", "Machine is not on the network", "Package not installed", "Object is remote", "Link has been severed", "Advertise error", "Srmount error", "Communication error on send", "Protocol error", "Multihop attempted", "RFS specific error", "Not a data message", "Value too large for defined data type", "Name not unique on network", "File descriptor in bad state", "Remote address changed", "Can not access a needed shared library", "Accessing a corrupted shared library", ".lib section in a.out corrupted", "Attempting to link in too many shared libraries",
"Cannot exec a shared library directly", "Illegal byte sequence", "Interrupted system call should be restarted", "Streams pipe error", "Too many users", "Socket operation on non-socket", "Destination address required", "Message too long", "Protocol wrong type for socket", "Protocol not available", "Protocol not supported", "Socket type not supported", "Operation not supported on transport endpoint", "Protocol family not supported", "Address family not supported by protocol", "Address already in use", "Cannot assign requested address", "Network is down", "Network is unreachable", "Network dropped connection because of reset",
		"Software caused connection abort", "Connection reset by peer", "No buffer space available", "Transport endpoint is already connected", "Transport endpoint is not connected", "Cannot send after transport endpoint shutdown", "Too many references: cannot splice", "Connection timed out", "Connection refused", "Host is down", "No route to host", "Operation already in progress", "Operation now in progress", "Stale NFS file handle", "Structure needs cleaning", "Not a XENIX named type file", "No XENIX semaphores available", "Is a named type file", "Remote I/O error", "Quota exceeded", "No medium found", "Wrong medium type"
	};
#endif

	for (errcode = ERROR_NUM_START; errcode <= ERROR_NUM_END; errcode++) {
		//error is not defined for error number 41 and 58
		if (errcode != 41 && errcode != 58) {
			set_errno(errcode);

			ret_chk = get_errno();
			TC_ASSERT_EQ("get_errno", ret_chk, errcode);

#ifdef CONFIG_LIBC_STRERROR
			ret_err = (char *)strerror(ret_chk);
			TC_ASSERT_NEQ("strerror", ret_err, NULL);
			TC_ASSERT_EQ("strerror", strncmp(ret_err, errstr_arr[errcode - 1], strlen(ret_err)), 0);
#endif
		}
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: errno
 ****************************************************************************/

int errno_main(void)
{
	tc_errno_set_get_errno();

	return 0;
}
