/*
 * Copyright (c) 2011 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_COMMON_ERROR
 * @{
 */

#define TIZEN_ERROR_MAX_PLATFORM_ERROR 0
#define TIZEN_ERROR_MIN_PLATFORM_ERROR (-1073741824LL)	/* = -2147483648 / 2 */

/* Use this constant macro as the value of the first module specific error */
#define TIZEN_ERROR_MIN_MODULE_ERROR (-2147483648LL)

/* Module errors cannot be greater than the following constant */
#define TIZEN_ERROR_MAX_MODULE_ERROR (TIZEN_ERROR_MIN_PLATFORM_ERROR - 1)

/* Check if error or not */
#define TIZEN_ERROR_IS_OK(x) (0 <= (x))

/* Check if error or not */
#define TIZEN_ERROR_IS_ERROR(x) (!TIZEN_ERROR_IS_OK(x))

/* Check if tizen error or not */
#define TIZEN_ERROR_IS_PLATFORM_ERROR(x) (TIZEN_ERROR_MIN_PLATFORM_ERROR <= (x) && (x) < 0)

/* Application Error Class */
#define TIZEN_ERROR_APPLICATION_CLASS -0x00000200
/* Base Error Class */
#define TIZEN_ERROR_BASE_CLASS -0x00000400
/* Content Error Class */
#define TIZEN_ERROR_CONTENT_CLASS -0x00000800
/* Location Error Class */
#define TIZEN_ERROR_LOCATION_CLASS -0x00001000
/* Multimedia Error Class */
#define TIZEN_ERROR_MULTIMEDIA_CLASS -0x00002000
/* Messaging Error Class */
#define TIZEN_ERROR_MESSAGING_CLASS -0x00004000
/* Network Error Class */
#define TIZEN_ERROR_NETWORK_CLASS -0x00008000
/* Social Error Class */
#define TIZEN_ERROR_SOCIAL_CLASS -0x00010000
/* System Error Class */
#define TIZEN_ERROR_SYSTEM_CLASS -0x00020000
/* Telephony Error Class */
#define TIZEN_ERROR_TELEPHONY_CLASS -0x00040000
/* UI Error class */
#define TIZEN_ERROR_UI_CLASS -0x00080000
/* UIX Error class */
#define TIZEN_ERROR_UIX_CLASS -0x00100000
/* AD Error class */
#define TIZEN_ERROR_AD_CLASS -0x00200000
/* Web Error class */
#define TIZEN_ERROR_WEB_CLASS -0x00400000
/* DRM Error class */
#define TIZEN_ERROR_DRM_CLASS -0x00800000
/* Account Error class */
#define TIZEN_ERROR_ACCOUNT_CLASS -0x01000000

/* Tizen Account Error */
#define TIZEN_ERROR_ACCOUNT             -0x01000000
/* Tizen Account Oauth Error */
#define TIZEN_ERROR_ACCOUNT_OAUTH       -0x01010000
/* Tizen SYNC MANAGER Error */
#define TIZEN_ERROR_SYNC_MANAGER        -0x01020000
/* Tizen FIDO Error */
#define TIZEN_ERROR_FIDO		-0x01030000
/* Tizen Application Error */
#define TIZEN_ERROR_APPLICATION         -0x01100000
/* Tizen Application Manager Error */
#define TIZEN_ERROR_APPLICATION_MANAGER -0x01110000
/* Tizen Badge Error */
#define TIZEN_ERROR_BADGE               -0x01120000
/* Tizen Message Port Error */
#define TIZEN_ERROR_MESSAGE_PORT        -0x01130000
/* Tizen Notification Error */
#define TIZEN_ERROR_NOTIFICATION        -0x01140000
/* Tizen Package Manager Error */
#define TIZEN_ERROR_PACKAGE_MANAGER	-0x01150000
/* Tizen Shortcut Error */
#define TIZEN_ERROR_SHORTCUT		-0x01160000
/* Tizen Bundle Error */
#define TIZEN_ERROR_BUNDLE		-0x01180000
/* Tizen Data Control Error */
#define TIZEN_ERROR_DATA_CONTROL        -0x01190000
/* Tizen Service Application Error */
#define TIZEN_ERROR_SERVICE_APPLICATION -0x01200000
/* Tizen MINICONTROL Error */
#define TIZEN_ERROR_MINICONTROL         -0x01210000
/* Tizen Utility ICU Error */
#define TIZEN_ERROR_UTILITY_ICU		-0x01500000
/* Tizen Mime Type Error */
#define TIZEN_ERROR_MIME_TYPE		-0x01600000
/* Tizen Media Content Error */
#define TIZEN_ERROR_MEDIA_CONTENT	-0x01610000
/* Tizen Email Service Error */
#define TIZEN_ERROR_EMAIL_SERVICE	-0x01700000
/* Tizen Messaging Service Error */
#define TIZEN_ERROR_MESSAGING_SERVICE	-0x01710000
/* Tizen Push Error */
#define TIZEN_ERROR_PUSH		-0x01720000
/* Tizen Audio IO Error */
#define TIZEN_ERROR_AUDIO_IO		-0x01900000
/* Tizen Camera Error */
#define TIZEN_ERROR_CAMERA		-0x01910000
/* Tizen Image Util Error */
#define TIZEN_ERROR_IMAGE_UTIL		-0x01920000
/* Tizen Metadata Extractor Error */
#define TIZEN_ERROR_METADATA_EXTRACTOR	-0x01930000
/* Tizen Player Error */
#define TIZEN_ERROR_PLAYER		-0x01940000
/* Tizen Recorder Error */
#define TIZEN_ERROR_RECORDER		-0x01950000
/* Tizen Sound Manager Error */
#define TIZEN_ERROR_SOUND_MANAGER	-0x01960000
/* Tizen Tone Player Error */
#define TIZEN_ERROR_TONE_PLAYER		-0x01970000
/* Tizen Video Util Error */
#define TIZEN_ERROR_VIDEO_UTIL		-0x01980000
/* Tizen Wav Player Error */
#define TIZEN_ERROR_WAV_PLAYER		-0x01990000
/* Tizen Radio Error */
#define TIZEN_ERROR_RADIO	        -0x019A0000
/* Tizen MEDIA CODEC Error */
#define TIZEN_ERROR_MEDIACODEC          -0x019B0000
/* Tizen Metadata Editor Error */
#define TIZEN_ERROR_METADATA_EDITOR     -0x019C0000
/* Tizen Vision Error */
#define TIZEN_ERROR_MEDIA_VISION        -0x019D0000
/* Tizen MEDIA DEMUXER Error */
#define TIZEN_ERROR_MEDIA_DEMUXER        -0x019E0000
/* Tizen MEDIA MUXER Error */
#define TIZEN_ERROR_MEDIA_MUXER        -0x019F0000
/* Tizen MEDIA STREAMER Error */
#define TIZEN_ERROR_MEDIA_STREAMER        -0x01A00000
/* Tizen MEDIA STREAMRECORDER Error */
#define TIZEN_ERROR_STREAMRECORDER		-0x01A10000
/* Tizen MEDIA TOOL Error */
#define TIZEN_ERROR_MEDIA_TOOL		-0x01A20000
/* Tizen Bluetooth Error */
#define TIZEN_ERROR_BLUETOOTH		-0x01C00000
/* Tizen Connection Error */
#define TIZEN_ERROR_CONNECTION		-0x01C10000
/* Tizen NFC Error */
#define TIZEN_ERROR_NFC			-0x01C20000
/* Tizen Serial Error */
#define TIZEN_ERROR_SERIAL		-0x01C30000
/* Tizen Tethering Error */
#define TIZEN_ERROR_TETHERING		-0x01C40000
/* Tizen Wifi Error */
#define TIZEN_ERROR_WIFI		-0x01C50000
/* Tizen Wifi Direct Error */
#define TIZEN_ERROR_WIFI_DIRECT		-0x01C60000
/* Tizen Smart Card Error */
#define TIZEN_ERROR_SMARTCARD          -0x01C70000
/* Tizen IotCon Error */
#define TIZEN_ERROR_IOTCON	        -0x01C80000
/* Tizen SSDP Error */
#define TIZEN_ERROR_SSDP		-0x01C90000
/* Tizen DNSSD Error */
#define TIZEN_ERROR_DNSSD		-0x01CA0000
/* Tizen VPNSVC Error */
#define TIZEN_ERROR_VPNSVC		-0x01CB0000
/* Tizen MTP Error */
#define TIZEN_ERROR_MTP		-0x01CC0000
/* Tizen Http Error */
#define TIZEN_ERROR_HTTP		-0x01CD0000
/* Tizen Wifi Manager Error */
#define TIZEN_ERROR_WIFI_MANAGER	-0x01CE0000
/* Tizen Privacy Manager Error */
#define TIZEN_ERROR_PRIVACY_MANAGER	-0x01E00000
/* Tizen Key Manager Error */
#define TIZEN_ERROR_KEY_MANAGER		-0x01E10000
/* Tizen PRIVILEGE INFO Error */
#define TIZEN_ERROR_PRIVILEGE_INFORMATION -0x01E20000
/* Tizen CSR Error */
#define TIZEN_ERROR_CSR -0x01E30000
/* Tizen Calendar Error */
#define TIZEN_ERROR_CALENDAR		-0x02000000
/* Tizen Contacts Error */
#define TIZEN_ERROR_CONTACTS		-0x02010000
/* Tizen Phonenumber utils Error */
#define TIZEN_ERROR_PHONENUMBER_UTILS		-0x02020000
/* Tizen Storage Error */
#define TIZEN_ERROR_STORAGE		-0x02200000
/* Tizen Vconf Error */
#define TIZEN_ERROR_VCONF		-0x02210000
/* Tizen Device Error */
#define TIZEN_ERROR_DEVICE		-0x02400000
/* Tizen Dlog Error */
#define TIZEN_ERROR_DLOG		-0x02410000
/* Tizen Media Key Error */
#define TIZEN_ERROR_MEDIA_KEY		-0x02420000
/* Tizen Runtime Info Error */
#define TIZEN_ERROR_RUNTIME_INFORMATION	-0x02430000
/* Tizen Sensor Error */
#define TIZEN_ERROR_SENSOR		-0x02440000
/* Tizen System Info Error */
#define TIZEN_ERROR_SYSTEM_INFORMATION	-0x02450000
/* Tizen System Setting Error */
#define TIZEN_ERROR_SYSTEM_SETTING	-0x02460000
/* Tizen System Resource Error */
#define TIZEN_ERROR_SYSTEM_RESOURCE	-0x02470000
/* Tizen Context Error */
#define TIZEN_ERROR_CONTEXT		-0x02480000
/* Tizen Health Error */
#define TIZEN_ERROR_HEALTH		-0x02490000
/* Tizen Feedback Error */
#define TIZEN_ERROR_FEEDBACK		-0x024A0000
/* Tizen USB Host Error */
#define TIZEN_ERROR_USB_HOST		-0x024B0000
/* Tizen Telephony Error */
#define TIZEN_ERROR_TELEPHONY		-0x02600000
/* Tizen EFL Util Error */
#define TIZEN_ERROR_EFL_UTIL		-0x02800000
/* Tizen UtilX Error */
#define TIZEN_ERROR_UTILX		-0x02810000
/* Tizen Native Buffer Error */
#define TIZEN_ERROR_NATIVE_BUFFER	-0x02820000
/* Tizen Buffer Manager Error */
#define TIZEN_ERROR_TBM			-0x02830000
/* Tizen EOM Error */
#define TIZEN_ERROR_EOM			-0x02840000
/* Tizen PANEL Error */
#define TIZEN_ERROR_PANEL		-0x02850000
/* Tizen TZSH Error */
#define TIZEN_ERROR_TZSH		-0x02860000
/* Tizen Download Error */
#define TIZEN_ERROR_DOWNLOAD            -0x02A00000
/* Tizen WebView Error */
#define TIZEN_ERROR_WEBVIEW		-0x02A10000
/* Tizen Location Manager Error */
#define TIZEN_ERROR_LOCATION_MANAGER	-0x02C00000
/* Tizen Geofence Manager Error */
#define TIZEN_ERROR_GEOFENCE_MANAGER	-0x02C10000
/* Tizen Maps Service Error */
#define TIZEN_ERROR_MAPS_SERVICE        -0x02C20000
/* Tizen STT Error */
#define TIZEN_ERROR_STT			-0x02F00000
/* Tizen TTS Error */
#define TIZEN_ERROR_TTS			-0x02F10000
/* Tizen IME Error */
#define TIZEN_ERROR_IME                 -0x02F20000
/* Tizen Service Adaptor Error */
#define TIZEN_ERROR_SERVICE_ADAPTOR     -0x02F30000
/* Tizen Widget Error */
#define TIZEN_ERROR_WIDGET              -0x02F40000
/* Tizen Voice Control Error */
#define TIZEN_ERROR_VOICE_CONTROL       -0x02F50000
/* Tizen Application Service Platform (ASP) Error */
#define TIZEN_ERROR_ASP                 -0x02F60000
/* Tizen Zigbee Error */
#define TIZEN_ERROR_ZIGBEE              -0x02F70000
/* Tizen Smart Traffic Control (STC) Error */
#define TIZEN_ERROR_STC                 -0x02F80000
/* Tizen Thumbnail Util Error */
#define TIZEN_ERROR_THUMBNAIL_UTIL      -0x02F90000
/* Tizen Job Scheduler Error */
#define TIZEN_ERROR_JOB_SCHEDULER       -0x02FA0000
/* Tizen SoftAP Error */
#define TIZEN_ERROR_SOFTAP              -0x02FB0000
/* Tizen Watchface Complication Error */
#define TIZEN_ERROR_COMPLICATION        -0x02FC0000
/* Tizen Device Certificate Manager Error */
#define TIZEN_ERROR_DEVICE_CERTIFICATE_MANAGER -0x02FD0000
/* Tizen Intelligent Network Monitoring (INM) Error */
#define TIZEN_ERROR_INM                 -0x02FE0000
/* Update Control Error */
#define TIZEN_ERROR_UPDATE_CONTROL      -0x02FF0000
/* Multi Assistant Error */
#define TIZEN_ERROR_MULTI_ASSISTANT     -0x03000000
/* Autofill Error */
#define TIZEN_ERROR_AUTOFILL            -0x03010000
/* Media Controller Error */
#define TIZEN_ERROR_MEDIA_CONTROLLER    -0x03020000
/* Component Manager */
#define TIZEN_ERROR_COMPONENT_MANAGER   -0x03030000


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
//	TIZEN_ERROR_KEY_NOT_AVAILABLE = -ENOKEY, /**< Required key not available */
//	TIZEN_ERROR_KEY_EXPIRED = -EKEYEXPIRED, /**< Key has expired */
//	TIZEN_ERROR_KEY_REVOKED = -EKEYREVOKED, /**< Key has been revoked */
//	TIZEN_ERROR_KEY_REJECTED = -EKEYREJECTED, /**< Key was rejected by service */
	TIZEN_ERROR_OWNER_DEAD = -EOWNERDEAD, /**< Owner died (for robust mutexes) */
	TIZEN_ERROR_UNKNOWN = TIZEN_ERROR_MIN_PLATFORM_ERROR, /**< Unknown error */
	/* This is a place to add new errors here.
	 * Do not assign integer values explicitly. Values are auto-assigned.
	 */
	TIZEN_ERROR_TIMED_OUT, /**< Time out */
	TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
	TIZEN_ERROR_USER_NOT_CONSENTED, /**< Not Consented */
	TIZEN_ERROR_DEVICE_POLICY_RESTRICTION, /**< Restriction by Device Policy */
	TIZEN_ERROR_END_OF_COLLECTION,
	} tizen_error_e;


/**
 * @brief Gets the last error code in the thread.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @details This function retrieves the last error code which is set by set_last_result()
 * @return One of #tizen_error_e
 * @see #tizen_error_e
 * @see set_last_result()
*/
int get_last_result(void);


/**
 * @brief Sets the last error code to be returned in the thread.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] err The error code to be returned
 * @see #tizen_error_e
 * @see get_last_result()
*/
void set_last_result(int err);


/**
 * @brief Gets the message for given the error code.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remark This function returns a static pointer; subsequent calls will overwrite it.
 * @param[in] err The error value to be returned
 * @return The error's message
 * @see #tizen_error_e
 *
 * @code
 *  char* errMsg;
 *  location_manager_h location_handle;
 *  int result = location_manager_create(LOCATION_METHOD_GPS, &location_handle);
 *
 *  if (LOCATIONS_ERROR_NONE != result) {
 *      errMsg = get_error_message(result);
 *      dlog_print(DLOG_INFO, "MyTag", "%s", errMsg);
 *  }
 * @endcode
 *
 */
char *get_error_message(int err);


/**
 * @}
 */


#ifdef __cplusplus
}


#endif
#endif		   /**<__TIZEN_COMMON_ERROR_H__ */
