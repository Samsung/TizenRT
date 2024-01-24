#include <tinyara/config.h>
#include <stdio.h>

extern MIPIDemoShow(void);
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ameba_mipi_main(int argc, char *argv[])
#endif
{	
	printf("=== MIPI demo ===");
	MIPIDemoShow();

}
