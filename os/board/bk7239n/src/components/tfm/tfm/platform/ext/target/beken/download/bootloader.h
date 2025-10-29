#ifndef _BOOT_LOADER_H_
#define _BOOT_LOADER_H_



#define  CRC_LEN_32     1


#ifndef MIN
# define MIN(a,b) ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
# define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif


void control_set_to_flash(void);

void bl_init(void) ;

void legacy_boot_main(void);
void PMU_uninit( void ) ;

#endif // _BOOT_LOADER_H_

