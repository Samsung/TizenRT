#include <stdio.h>

#ifndef LINUX
extern int sl_register_linux(void);
#endif
#define API_TEST 0

#if API_TEST
extern int security_api_test_main(int argc, char *argv[]);
#else
extern int utc_security_main(int argc, char *argv[]);
#endif
extern int sl_register_linux(void);


int main(int argc, char *argv[])
{
	int res = sl_register_linux();
	if (res < 0) {
		printf("register driver fail\n");
		return -1;
	}
#if API_TEST
	security_api_test_main(argc, argv);
#else
	utc_security_main(argc, argv);
#endif

	return 0;
}
