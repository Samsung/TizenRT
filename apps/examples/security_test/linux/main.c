#include <stdio.h>

#ifndef LINUX
extern int sl_register_linux(void);
#endif

extern int security_api_test_main(int argc, char *argv[]);
extern int sl_register_linux(void);
int main(int argc, char *argv[])
{
	int res = sl_register_linux();
	if (res < 0) {
		printf("register driver fail\n");
		return -1;
	}
	security_api_test_main(argc, argv);
	return 0;
}
