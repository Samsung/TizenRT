/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */
#ifndef _CSTUB_H
#define _CSTUB_H

#if defined( __ICCARM__)
#ifndef __caddr_t_defined
typedef	char 	*caddr_t;
#define __caddr_t_defined
#endif

#ifndef _OFF_T_DECLARED
typedef	long	off_t;		/* file offset */
#define	_OFF_T_DECLARED
#endif

struct	stat {
	int		st_dev;
	int		st_ino;
	int	st_mode;
	int	st_nlink;
	int		st_uid;
	int		st_gid;
	int		st_rdev;
	off_t		st_size;
#if defined(__svr4__) && !defined(__PPC__) && !defined(__sun__)
	time_t	st_atime;
	time_t	st_mtime;
	time_t	st_ctime;
#else
// struct timespec st_atim;
// struct timespec st_mtim;
// struct timespec st_ctim;
// blksize_t     st_blksize;
// blkcnt_t	st_blocks;
#if !defined(__rtems__)
	long		st_spare4[2];
#endif
#endif
};

#define		S_IFCHR	0020000	/* character special */
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <errno.h>

#define HEAP_START		__HeapBase
#define HEAP_END		__HeapLimit

#ifndef _ASMLANGUAGE

/* HEAP_START, HEAP_END are set in the linker command file */
extern caddr_t HEAP_START, HEAP_END;

#endif /* !_ASMLANGUAGE */

#endif
