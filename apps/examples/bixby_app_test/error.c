/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include "error.h"

const char *get_error_message(int err)
{
	switch (err) {
	case TIZEN_ERROR_NONE:
		return "Successful";
	case TIZEN_ERROR_NOT_PERMITTED:
		return "Operation not permitted";
	case TIZEN_ERROR_NO_SUCH_FILE:
		return "No such file or directory";
	case TIZEN_ERROR_NO_SUCH_PROCESS:
		return "No such process";
	case TIZEN_ERROR_INTERRUPTED_SYS_CALL:
		return "Interrupted system call";
	case TIZEN_ERROR_IO_ERROR:
		return "I/O error";
	case TIZEN_ERROR_NO_SUCH_DEVICE:
		return "No such device or address";
	case TIZEN_ERROR_ARGUMENT_LIST_TOO_LONG:
		return "Argument list too long";
	case TIZEN_ERROR_EXEC_FORMAT_ERROR:
		return "Exec format error";
	case TIZEN_ERROR_BAD_FILE_NUMBER:
		return "Bad file number";
	case TIZEN_ERROR_TRY_AGAIN:
		return "Try again";
	case TIZEN_ERROR_OUT_OF_MEMORY:
		return "Out of memory";
	case TIZEN_ERROR_PERMISSION_DENIED:
		return "Permission denied";
	case TIZEN_ERROR_BAD_ADDRESS:
		return "Bad address";
	case TIZEN_ERROR_BLOCK_DEVICE_REQUIRED:
		return "Block device required";
	case TIZEN_ERROR_RESOURCE_BUSY:
		return "Device or resource busy";
	case TIZEN_ERROR_FILE_EXISTS:
		return "File exists";
	case TIZEN_ERROR_CROSS_DEVICE_LINK:
		return "Cross-device link";
	case TIZEN_ERROR_NOT_A_DIRECTORY:
		return "Not a directory";
	case TIZEN_ERROR_IS_A_DIRECTORY:
		return "Is a directory";
	case TIZEN_ERROR_INVALID_PARAMETER:
		return "Invalid function parameter";
	case TIZEN_ERROR_FILE_TABLE_OVERFLOW:
		return "File table overflow";
	case TIZEN_ERROR_TOO_MANY_OPEN_FILES:
		return "Too many open files";
	case TIZEN_ERROR_TOO_NOT_A_TERMINAL:
		return "Not a terminal";
	case TIZEN_ERROR_TOO_TEXT_FILE_BUSY:
		return "Not a terminal";
	case TIZEN_ERROR_FILE_TOO_LARGE:
		return "File too large";
	case TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE:
		return "No space left on device";
	case TIZEN_ERROR_ILLEGAL_SEEK:
		return "Illegal seek";
	case TIZEN_ERROR_READ_ONLY_FILESYSTEM:
		return "Read-only file system";
	case TIZEN_ERROR_NO_DATA:
		return "No data available";
	case TIZEN_ERROR_TOO_MANY_LINKS:
		return "Too many links";
	case TIZEN_ERROR_BROKEN_PIPE:
		return "Broken pipe";
	case TIZEN_ERROR_ARGUMENT_OUT_OF_DOMAIN:
		return "Math argument out of domain of func";
	case TIZEN_ERROR_RESULT_OUT_OF_RANGE:
		return "Math result not representable";
	case TIZEN_ERROR_WOULD_CAUSE_DEADLOCK:
		return "Resource deadlock would occur";
	case TIZEN_ERROR_FILE_NAME_TOO_LONG:
		return "File name too long";
	case TIZEN_ERROR_FILE_NO_LOCKS_AVAILABLE:
		return "No record locks available";
	case TIZEN_ERROR_INVALID_OPERATION:
		return "Function not implemented";
	case TIZEN_ERROR_DIR_NOT_EMPTY:
		return "Directory not empty";
	case TIZEN_ERROR_TOO_MANY_SYMBOLIC_LINKS:
		return "Too many symbolic links encountered";
	case TIZEN_ERROR_CORRUPTED_SHARED_LIB:
		return "Accessing a corrupted shared library";
	case TIZEN_ERROR_LIB_SECTION_CORRUPTED:
		return ".lib section in a.out corrupted";
	case TIZEN_ERROR_LINK_TOO_MANY_SHARED_LIB:
		return "Attempting to link in too many shared libraries";
	case TIZEN_ERROR_SHARED_LIB_EXEC:
		return "Cannot exec a shared library directly";
	case TIZEN_ERROR_ILLEGAL_BYTE_SEQ:
		return "Illegal byte sequence";
	case TIZEN_ERROR_SYSTEM_CALL_RESTART:
		return "Interrupted system call should be restarted";
	case TIZEN_ERROR_STREAMS_PIPE:
		return "Streams pipe error";
	case TIZEN_ERROR_TOO_MANY_USERS:
		return "Too many users";
	case TIZEN_ERROR_NON_SOCKET:
		return "Socket operation on non-socket";
	case TIZEN_ERROR_NO_DEST_ADDRESS:
		return "Destination address required";
	case TIZEN_ERROR_MSG_TOO_LONG:
		return "Message too long";
	case TIZEN_ERROR_PROTOCOL_WRONG_TYPE:
		return "Protocol wrong type for socket";
	case TIZEN_ERROR_PROTOCOL_NOT_AVALIABLE:
		return "Protocol not available";
	case TIZEN_ERROR_PROTOCOL_NOT_SUPPORTED:
		return "Protocol not supported";
	case TIZEN_ERROR_SOCKET_TYPE_NOT_SUPPORTED:
		return "Socket type not supported";
	case TIZEN_ERROR_ENDPOINT_OPERATIN_NOT_SUPPORTED:
		return "Operation not supported on transport endpoint";
	case TIZEN_ERROR_PROTOCOL_FAMILY_NOT_SUPPORTED:
		return "Protocol family not supported";
	case TIZEN_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED:
		return "Address family not supported by protocol";
	case TIZEN_ERROR_ADDRES_IN_USE:
		return "Address already in use";
	case TIZEN_ERROR_CANNOT_ASSIGN_ADDRESS:
		return "Cannot assign requested address";
	case TIZEN_ERROR_NETWORK_DOWN:
		return "Network is down";
	case TIZEN_ERROR_NETWORK_UNREACHABLE:
		return "Network is unreachable";
	case TIZEN_ERROR_NETWORK_RESET:
		return "Network dropped connection because of reset";
	case TIZEN_ERROR_CONNECTION_ABORTED:
		return "Software caused connection abort";
	case TIZEN_ERROR_CONNECTION_RESET_BY_PEER:
		return "Connection reset by peer";
	case TIZEN_ERROR_BUFFER_SPACE:
		return "No buffer space available";
	case TIZEN_ERROR_ENDPOINT_CONNECTED:
		return "Transport endpoint is already connected";
	case TIZEN_ERROR_ENDPOINT_NOT_CONNECTED:
		return "Transport endpoint is not connected";
	case TIZEN_ERROR_ENDPOINT_SHUTDOWN:
		return "Cannot send after transport endpoint shutdown";
	case TIZEN_ERROR_TOO_MANY_REFERENCES:
		return "Too many references: cannot splice";
	case TIZEN_ERROR_CONNECTION_TIME_OUT:
		return "Connection timed out";
	case TIZEN_ERROR_CONNECTION_REFUSED:
		return "Connection refused";
	case TIZEN_ERROR_HOST_DOWN:
		return "Host is down";
	case TIZEN_ERROR_NO_ROUTE_TO_HOST:
		return "No route to host";
	case TIZEN_ERROR_ALREADY_IN_PROGRESS:
		return "Operation already in progress";
	case TIZEN_ERROR_NOW_IN_PROGRESS:
		return "Operation now in progress";
	case TIZEN_ERROR_STALE_NFS_FILE_HANDLE:
		return "Stale NFS file handle";
	case TIZEN_ERROR_STRUCTURE_UNCLEAN:
		return "Structure needs cleaning";
	case TIZEN_ERROR_NOT_XENIX_NAMED_TYPE_FILE:
		return "Not a XENIX named type file";
	case TIZEN_ERROR_NO_XENIX_SEMAPHORES_AVAILABLE:
		return "No XENIX semaphores available";
	case TIZEN_ERROR_IS_NAMED_TYPE_FILE:
		return "Is a named type file";
	case TIZEN_ERROR_REMOTE_IO:
		return "Remote I/O error";
	case TIZEN_ERROR_QUOTA_EXCEEDED:
		return "Quota exceeded";
	case TIZEN_ERROR_NO_MEDIUM:
		return "No medium found";
	case TIZEN_ERROR_WRONG_MEDIUM_TYPE:
		return "Wrong medium type";
	case TIZEN_ERROR_CANCELED:
		return "Operation Canceled";
	case TIZEN_ERROR_KEY_NOT_AVAILABLE:
		return "Required key not available";
	case TIZEN_ERROR_KEY_REJECTED:
		return "Key was rejected by service";
	case TIZEN_ERROR_OWNER_DEAD:
		return "Owner died (for robust mutexes)";
	case TIZEN_ERROR_TIMED_OUT:
		return "Time out";
	case TIZEN_ERROR_NOT_SUPPORTED:
		return "Not supported";
	case TIZEN_ERROR_USER_NOT_CONSENTED:
		return "Not Consented";
	case TIZEN_ERROR_DEVICE_POLICY_RESTRICTION:
		return "Restriction by Device Policy";
	case TIZEN_ERROR_END_OF_COLLECTION:
	default:
		return "Unknown error";
	}
}
