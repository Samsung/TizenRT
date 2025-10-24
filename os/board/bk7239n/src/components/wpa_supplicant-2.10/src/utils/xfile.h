#ifndef __FILE_H_
#define __FILE_H_

#include <sys/types.h>
#include <common/bk_typedef.h>

#define MAXFILENUM      8 /* max number of file */

#define xSEEK_SET       0
#define xSEEK_CUR       1
#define xSEEK_END       2

enum file_state_ioctl {
	xFILENAME = 1,
	xFILESIZE,
	xFILEBUFF,
	xFILEFLAG,
	xFILEBASE,
	xFILETIME
};

/* embedded system simple file management */

typedef struct simple_file_structure {
	const char *_tmpfname;        /* pointer to filename */
	char *_base;                  /* always pointer to beginning of file */
	int   _flag;                  /* describe type of file */
#define  _XDELETE       0x0001    /* delete automatically when fclose called */
#define  _XFIXSIZE      0x0002    /* file size is fixed */
#define  _XFINUSE		0x80000000/* file is being used */
	int   _bufsiz;                /* size of the allocated buffer */

	char *_ptr;                   /* pointer to current location */
	int   _cnt;			          /* the file size */
} xFILE;

extern int register_xfile(char *name, char *base, int size);
extern int xfcreate(xFILE *f);
extern xFILE *xfopen(const char *filename, const char *mode);
extern int xfread(void *buffer, uint32 size, uint32 count, xFILE *stream);
extern int xfwrite(const void *buffer, uint32 size, uint32 count, xFILE *stream);
extern int xfclose(xFILE *stream);
extern int xfseek(xFILE *f, int offset, int whence);
extern char *xfgets(char *string, int n, xFILE *stream) ;
extern int xfdelete(char *name);
extern int xfsetstate(char *name, int *value, int type);
extern int xfsize(xFILE *f);
extern xFILE *xfindfile(const char *name);
extern long xftell(xFILE *f);
extern int xfrewind(xFILE *f);


#endif

