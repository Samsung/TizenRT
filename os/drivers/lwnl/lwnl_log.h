#ifndef _LWNL_LOG_H__
#define _LWNL_LOG_H__

#define LWNL_TAG "[LWNL]"

#define LWNL_ERR										\
	do {												\
		nldbg(LWNL_TAG"[ERR] %s: %d line err(%s)\n",	\
			  __FILE__, __LINE__, strerror(errno));		\
	} while (0)

#define LWNL_ENTER														\
	do {																\
		nlvdbg(LWNL_TAG"[T%d] --->%s:%d\n", getpid(), __FILE__, __LINE__); \
	} while (0)

#define LWNL_LEAVE														\
	do {																\
		nlvdbg(LWNL_TAG"[T%d]<---%s:%d\n", getpid(),  __FILE__, __LINE__); \
	} while (0)

/*  lwnl event queue */
#define LWQ_ENTRY										\
		do {											\
			nlvdbg("[LWQ][T%d] %s\t%s:%d \n", getpid(),	\
				   __FUNCTION__, __FILE__, __LINE__);	\
		} while (0)

#define LWQ_ERR												\
		do {												\
			nldbg("[LWQ][ERR][T%d] %s\t%s:%d \n", getpid(),	\
				  __FUNCTION__, __FILE__, __LINE__);		\
		} while (0)

#define LWQ_LOG nlvdbg
#endif // #define _LWNL_LOG_H__
