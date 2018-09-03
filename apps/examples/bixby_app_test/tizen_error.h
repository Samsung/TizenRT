/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */


#ifndef __TIZEN_COMMON_ERROR_H__
#define __TIZEN_COMMON_ERROR_H__

#include <errno.h>

#define ENOKEY 127
#define EKEYREJECTED 129
#define TIZEN_ERROR_APPLICATION         -0x01100000

/* Tizen Voice Control Error */
#define TIZEN_ERROR_VOICE_CONTROL       -0x02F50000

/**
 * @brief Enumeration for tizen errors.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
*/
typedef enum {
	TIZEN_ERROR_NONE = 0, /**< Successful */
	TIZEN_ERROR_NOT_PERMITTED = -EPERM, /**< Operation not permitted */
	TIZEN_ERROR_NO_SUCH_FILE = -ENOENT, /**< No such file or directory */
	TIZEN_ERROR_NO_SUCH_PROCESS = -ESRCH, /**< No such process */
	TIZEN_ERROR_INTERRUPTED_SYS_CALL = -EINTR, /**< Interrupted system call */
	TIZEN_ERROR_IO_ERROR = -EIO, /**< I/O error */
	TIZEN_ERROR_NO_SUCH_DEVICE = -ENXIO, /**< No such device or address */
	TIZEN_ERROR_ARGUMENT_LIST_TOO_LONG = -E2BIG, /**< Argument list too long */
	TIZEN_ERROR_EXEC_FORMAT_ERROR = -ENOEXEC, /**< Exec format error */
	TIZEN_ERROR_BAD_FILE_NUMBER = -EBADF, /**< Bad file number */
	TIZEN_ERROR_TRY_AGAIN = -EAGAIN, /**< Try again */
	TIZEN_ERROR_OUT_OF_MEMORY = -ENOMEM, /**< Out of memory */
	TIZEN_ERROR_PERMISSION_DENIED = -EACCES, /**< Permission denied */
	TIZEN_ERROR_BAD_ADDRESS = -EFAULT, /**< Bad address */
	TIZEN_ERROR_BLOCK_DEVICE_REQUIRED = -ENOTBLK, /**< Block device required */
	TIZEN_ERROR_RESOURCE_BUSY = -EBUSY, /**< Device or resource busy */
	TIZEN_ERROR_FILE_EXISTS = -EEXIST, /**< File exists */
	TIZEN_ERROR_CROSS_DEVICE_LINK = -EXDEV, /**< Cross-device link */
	TIZEN_ERROR_NOT_A_DIRECTORY = -ENOTDIR, /**< Not a directory */
	TIZEN_ERROR_IS_A_DIRECTORY = -EISDIR, /**< Is a directory */
	TIZEN_ERROR_INVALID_PARAMETER = -EINVAL, /**< Invalid function parameter */
	TIZEN_ERROR_FILE_TABLE_OVERFLOW = -ENFILE, /**< File table overflow */
	TIZEN_ERROR_TOO_MANY_OPEN_FILES = -EMFILE, /**< Too many open files */
	TIZEN_ERROR_TOO_NOT_A_TERMINAL = -ENOTTY, /**< Not a terminal */
	TIZEN_ERROR_TOO_TEXT_FILE_BUSY = -ETXTBSY, /**< Not a terminal */
	TIZEN_ERROR_FILE_TOO_LARGE = -EFBIG, /**< File too large */
	TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE = -ENOSPC, /**< No space left on device */
	TIZEN_ERROR_ILLEGAL_SEEK = -ESPIPE, /**< Illegal seek */
	TIZEN_ERROR_READ_ONLY_FILESYSTEM = -EROFS, /**< Read-only file system */
	TIZEN_ERROR_NO_DATA = -ENODATA, /**< No data available */
	TIZEN_ERROR_TOO_MANY_LINKS = -EMLINK, /**< Too many links */
	TIZEN_ERROR_BROKEN_PIPE = -EPIPE, /**< Broken pipe */
	TIZEN_ERROR_ARGUMENT_OUT_OF_DOMAIN = -EDOM, /**< Math argument out of domain of func */
	TIZEN_ERROR_RESULT_OUT_OF_RANGE = -ERANGE, /**< Math result not representable */
	TIZEN_ERROR_WOULD_CAUSE_DEADLOCK = -EDEADLK, /**< Resource deadlock would occur */
	TIZEN_ERROR_FILE_NAME_TOO_LONG = -ENAMETOOLONG, /**< File name too long */
	TIZEN_ERROR_FILE_NO_LOCKS_AVAILABLE = -ENOLCK, /**< No record locks available */
	TIZEN_ERROR_INVALID_OPERATION = -ENOSYS, /**< Function not implemented */
	TIZEN_ERROR_DIR_NOT_EMPTY = -ENOTEMPTY, /**< Directory not empty */
	TIZEN_ERROR_TOO_MANY_SYMBOLIC_LINKS = -ELOOP, /**< Too many symbolic links encountered */
	TIZEN_ERROR_WOULD_BLOCK = TIZEN_ERROR_TRY_AGAIN, /**< Operation would block */
	TIZEN_ERROR_CORRUPTED_SHARED_LIB = -ELIBBAD, /**< Accessing a corrupted shared library */
	TIZEN_ERROR_LIB_SECTION_CORRUPTED = -ELIBSCN, /**< .lib section in a.out corrupted */
	TIZEN_ERROR_LINK_TOO_MANY_SHARED_LIB = -ELIBMAX, /**< Attempting to link in too many shared libraries */
	TIZEN_ERROR_SHARED_LIB_EXEC = -ELIBEXEC, /**< Cannot exec a shared library directly */
	TIZEN_ERROR_ILLEGAL_BYTE_SEQ = -EILSEQ, /**< Illegal byte sequence */
	TIZEN_ERROR_SYSTEM_CALL_RESTART = -ERESTART, /**< Interrupted system call should be restarted */
	TIZEN_ERROR_STREAMS_PIPE = -ESTRPIPE, /**< Streams pipe error */
	TIZEN_ERROR_TOO_MANY_USERS = -EUSERS, /**< Too many users */
	TIZEN_ERROR_NON_SOCKET = -ENOTSOCK, /**< Socket operation on non-socket */
	TIZEN_ERROR_NO_DEST_ADDRESS = -EDESTADDRREQ, /**< Destination address required */
	TIZEN_ERROR_MSG_TOO_LONG = -EMSGSIZE, /**< Message too long */
	TIZEN_ERROR_PROTOCOL_WRONG_TYPE = -EPROTOTYPE, /**< Protocol wrong type for socket */
	TIZEN_ERROR_PROTOCOL_NOT_AVALIABLE = -ENOPROTOOPT, /**< Protocol not available */
	TIZEN_ERROR_PROTOCOL_NOT_SUPPORTED = -EPROTONOSUPPORT, /**< Protocol not supported */
	TIZEN_ERROR_SOCKET_TYPE_NOT_SUPPORTED = -ESOCKTNOSUPPORT, /**< Socket type not supported */
	TIZEN_ERROR_ENDPOINT_OPERATIN_NOT_SUPPORTED = -EOPNOTSUPP, /**< Operation not supported on transport endpoint */
	TIZEN_ERROR_PROTOCOL_FAMILY_NOT_SUPPORTED = -EPFNOSUPPORT, /**< Protocol family not supported */
	TIZEN_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED = -EAFNOSUPPORT, /**< Address family not supported by protocol */
	TIZEN_ERROR_ADDRES_IN_USE = -EADDRINUSE, /**< Address already in use */
	TIZEN_ERROR_CANNOT_ASSIGN_ADDRESS = -EADDRNOTAVAIL, /**< Cannot assign requested address */
	TIZEN_ERROR_NETWORK_DOWN = -ENETDOWN, /**< Network is down */
	TIZEN_ERROR_NETWORK_UNREACHABLE = -ENETUNREACH, /**< Network is unreachable */
	TIZEN_ERROR_NETWORK_RESET = -ENETRESET, /**< Network dropped connection because of reset */
	TIZEN_ERROR_CONNECTION_ABORTED = -ECONNABORTED, /**< Software caused connection abort */
	TIZEN_ERROR_CONNECTION_RESET_BY_PEER = -ECONNRESET, /**< Connection reset by peer */
	TIZEN_ERROR_BUFFER_SPACE = -ENOBUFS, /**< No buffer space available */
	TIZEN_ERROR_ENDPOINT_CONNECTED = -EISCONN, /**< Transport endpoint is already connected */
	TIZEN_ERROR_ENDPOINT_NOT_CONNECTED = -ENOTCONN, /**< Transport endpoint is not connected */
	TIZEN_ERROR_ENDPOINT_SHUTDOWN = -ESHUTDOWN, /**< Cannot send after transport endpoint shutdown */
	TIZEN_ERROR_TOO_MANY_REFERENCES = -ETOOMANYREFS, /**< Too many references: cannot splice */
	TIZEN_ERROR_CONNECTION_TIME_OUT = -ETIMEDOUT, /**< Connection timed out */
	TIZEN_ERROR_CONNECTION_REFUSED = -ECONNREFUSED, /**< Connection refused */
	TIZEN_ERROR_HOST_DOWN = -EHOSTDOWN, /**< Host is down */
	TIZEN_ERROR_NO_ROUTE_TO_HOST = -EHOSTUNREACH, /**< No route to host */
	TIZEN_ERROR_ALREADY_IN_PROGRESS = -EALREADY, /**< Operation already in progress */
	TIZEN_ERROR_NOW_IN_PROGRESS = -EINPROGRESS, /**< Operation now in progress */
	TIZEN_ERROR_STALE_NFS_FILE_HANDLE = -ESTALE, /**< Stale NFS file handle */
	TIZEN_ERROR_STRUCTURE_UNCLEAN = -EUCLEAN, /**< Structure needs cleaning */
	TIZEN_ERROR_NOT_XENIX_NAMED_TYPE_FILE = -ENOTNAM, /**< Not a XENIX named type file */
	TIZEN_ERROR_NO_XENIX_SEMAPHORES_AVAILABLE = -ENAVAIL, /**< No XENIX semaphores available */
	TIZEN_ERROR_IS_NAMED_TYPE_FILE = -EISNAM, /**< Is a named type file */
	TIZEN_ERROR_REMOTE_IO = -EREMOTEIO, /**< Remote I/O error */
	TIZEN_ERROR_QUOTA_EXCEEDED = -EDQUOT, /**< Quota exceeded */
	TIZEN_ERROR_NO_MEDIUM = -ENOMEDIUM, /**< No medium found */
	TIZEN_ERROR_WRONG_MEDIUM_TYPE = -EMEDIUMTYPE, /**< Wrong medium type */
	TIZEN_ERROR_CANCELED = -ECANCELED, /**< Operation Canceled */
	TIZEN_ERROR_KEY_NOT_AVAILABLE = -ENOKEY, /**< Required key not available */
	TIZEN_ERROR_KEY_REJECTED = -EKEYREJECTED, /**< Key was rejected by service */
	TIZEN_ERROR_OWNER_DEAD = -EOWNERDEAD, /**< Owner died (for robust mutexes) */
	/* This is a place to add new errors here.
	 * Do not assign integer values explicitly. Values are auto-assigned.
	 */
	TIZEN_ERROR_TIMED_OUT, /**< Time out */
	TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
	TIZEN_ERROR_USER_NOT_CONSENTED, /**< Not Consented */
	TIZEN_ERROR_DEVICE_POLICY_RESTRICTION, /**< Restriction by Device Policy */
	TIZEN_ERROR_END_OF_COLLECTION,
} tizen_error_e;

#endif         /**<__TIZEN_COMMON_ERROR_H__ */
