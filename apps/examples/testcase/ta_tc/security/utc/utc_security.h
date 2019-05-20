#ifndef _UTC_SECURITY_TEST_H__
#define _UTC_SECURITY_TEST_H__

#include <security/security_api.h>

#define UTC_CRYPTO_KEY_NAME "ss/01"
#define UTC_CERT_NAME "ss/02"

/*  Debugging */
#define US_ERROR														\
	do {																\
		printf("[USERR] %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
		return;															\
	} while (0)

#define US_DEFINE_DATA(name, text)					\
	char *name##_text = text;						\
	int name##_len = strlen(name##_text) + 1;		\
	security_data name = {name##_text, name##_len}

/**
 * Function
 */
void utc_common_main(void);
void utc_auth_main(void);
void utc_crypto_main(void);
void utc_keymgr_main(void);
void utc_ss_main(void);



#endif // _UTC_SECURITY_TEST_H__
