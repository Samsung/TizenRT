#include <stdio.h>

extern int hal_keymgr_test(void);
extern int hal_auth_test(void);
extern int hal_ss_test(void);
extern int hal_crypto_test(void);

int main(void)
{
	int res = 0;
	res = hal_keymgr_test();
	res = hal_auth_test();
	res = hal_ss_test();
	res = hal_crypto_test();

	return 0;
}
