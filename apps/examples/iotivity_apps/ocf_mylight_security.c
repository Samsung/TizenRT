#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "iotivity_common.h"

#include <pinoxmcommon.h>
#include <octypes.h>


static FILE *on_open(const char *path, const char *mode)
{
	FILE *fp;

	if (!path)
		return NULL;

	if (!strcmp(path, "oic_svr_db.dat"))
		fp = fopen(SVR_DB_PATH, mode);
	else if (!strcmp(path, "device_properties.dat"))
		fp = fopen(DEVICE_DB_PATH, mode);
	else if (!strcmp(path, "introspection.dat"))
		fp = fopen(INTROSPECTION_DB_PATH, mode);
	else {
		DBG("unknown storage file '%s'", path);
		fp = fopen(path, mode);
	}

	if (!fp) {
		DBG("fopen() failed. path=%s, mode=%s", path, mode);
		return NULL;
	}

#ifdef TRACE_STORAGE_OPERATION
	DBG("open: path=%s, mode=%s", path, mode);
#endif

	return fp;
}

static size_t on_read(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
#ifdef TRACE_STORAGE_OPERATION
	DBG("read: size=%zd, nmemb=%zd, fp=0x%p", size, nmemb, fp);
#endif

	return fread(ptr, size, nmemb, fp);
}

static size_t on_write(const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
#ifdef TRACE_STORAGE_OPERATION
	DBG("write: size=%zd, nmemb=%zd, fp=0x%p", size, nmemb, fp);
#endif

	return fwrite(ptr, size, nmemb, fp);
}

static int on_close(FILE *fp)
{
#ifdef TRACE_STORAGE_OPERATION
	DBG("close: fp=0x%p", fp);
#endif

	return fclose(fp);
}

static int on_unlink(const char *path)
{
#ifdef TRACE_STORAGE_OPERATION
	DBG("unlink: path=%s", path);
#endif

	return unlink(path);
}

static void on_display_pin(char *pin, size_t length, void *user_data _UNUSED_)
{
	MSG("PIN CODE: '%s' (pin length=%zd)", pin, length);
}

static void on_close_pin(void)
{
	MSG("PIN CLOSED");
}

OCPersistentStorage ps = {
	.open = on_open,
	.read = on_read,
	.write = on_write,
	.close = on_close,
	.unlink = on_unlink
};

int ocf_mylight_security_init(void)
{
	OCStackResult ret;

	OCRegisterPersistentStorageHandler(&ps);

	ret = SetDisplayPinWithContextCB(on_display_pin, NULL);
	if (ret != OC_STACK_OK) {
		DBG("SetDisplayPinWithContextCB failed! (ret=%d)", ret);
		return -1;
	}

	SetClosePinDisplayCB(on_close_pin);

	ret = SetRandomPinPolicy(8, NUM_PIN);
	if (ret != OC_STACK_OK) {
		DBG("SetRandomPinPolicy failed! (ret=%d)", ret);
		return -1;
	}

	return 0;
}
