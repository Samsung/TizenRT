#include "ameba_soc.h"

static const char *TAG = "SECURE";
#if defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) && (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 1)

const char *client_key_s = \
						   "-----BEGIN EC PARAMETERS-----\r\n" \
						   "BggqhkjOPQMBBw==\r\n" \
						   "-----END EC PARAMETERS-----\r\n" \
						   "-----BEGIN EC PRIVATE KEY-----\r\n" \
						   "MHcCAQEEILTklp7lSQO5jw+mReFdg/rZzFwaATQhip5qlF5eUigLoAoGCCqGSM49\r\n" \
						   "AwEHoUQDQgAE0nMJjk4NMTa63GQ/N1GRWLph8L+lXzwoloV/fJFc57r4G6NvKq2C\r\n" \
						   "hWfeUFu8bhpDJGa9P7O8OkuPTMz7yn8yaw==\r\n" \
						   "-----END EC PRIVATE KEY-----\r\n";

#endif

/*
 * The following example shows how to call secure entry function from NS functions.
 *	 (1) In NS code, users can't call internal_secure_func() directly, but can call secure_entry() instead.
 */

/* Internal secure function that is not available for NS functions.  */
int internal_secure_func(void)
{
	RTK_LOGI(TAG, "Print log from secure code\n");
	return 0;
}

/* Use attribute `cmse_nonsecure_entry' to make `secure_entry' available for NS functions.  */
IMAGE3_ENTRY_SECTION
NS_ENTRY int secure_entry(void)
{
	return internal_secure_func();
}



/*
 * The following example shows how to call  NS function from secure functions.
 *	(1) Users should call entry() in NS function to pass a non-secure functioin pointer to fp.
 *	(2) Then in secure code, users can call call_callback() to execute the non-secure function.
 */

void default_callback(void) {}

// fp can point to a secure function or a non-secure function
nsfunc *fp = (nsfunc *) default_callback;        // secure function pointer

IMAGE3_ENTRY_SECTION
void NS_ENTRY entry(nsfunc *callback)
{
	fp = cmse_nsfptr_create(callback);  // non-secure function pointer
}

void call_callback(void)
{
	if (cmse_is_nsfptr(fp)) {
		fp();    // non-secure function call
	} else {
		((void (*)(void)) fp)();    // normal function call
	}
}

int _write(int file, char *ptr, int len)
{
	int nChars = 0;

	/* Check for stdout and stderr
	(only necessary if file descriptors are enabled.) */
	if (file != 1 && file != 2) {
		return -1;
	}
	for (/*Empty */; len > 0; --len) {
		LOGUART_PutChar(*ptr++);
		++nChars;
	}
	return nChars;
}

int _read(int file, char *ptr, int len)
{
	int nChars = 0;

	/* Check for stdin
	(only necessary if FILE descriptors are enabled) */
	if (file != 0) {
		return -1;
	}
	for (/*Empty*/; len > 0; --len) {
		int c = LOGUART_GetChar(_TRUE);
		if ((c < 0) || (c == '\r')) {
			*ptr = '\0';
			break;
		}
		*(ptr++) = c;
		++nChars;
	}
	return nChars;
}
