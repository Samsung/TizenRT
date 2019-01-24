#ifndef __TINY_TEST_UTILS_H__
#define __TINY_TEST_UTILS_H__

#ifdef LINUX
#define TQ_TEST_ERR printf("[TQ_TEST] T%lx [ERR] %s\t%s:%d\n", pthread_self(), __FUNCTION__, __FILE__, __LINE__)
#else
#define TQ_TEST_ERR printf("[TQ_TEST] T%x [ERR] %s\t%s:%d\n", getpid(), __FUNCTION__, __FILE__, __LINE__)
#endif

#ifdef LINUX
#define TQ_TEST_SUCCESS printf("[TQ_TEST] T%lx [PASS] %s\t%s:%d\n", pthread_self(), __FUNCTION__, __FILE__, __LINE__)
#else
#define TQ_TEST_SUCCESS printf("[TQ_TEST] T%x [PASS] %s\t%s:%d\n", getpid(), __FUNCTION__, __FILE__, __LINE__)
#endif

#define TQ_CHECK_RESULT(func)					\
	do {										\
		int res = func;							\
		if (res != 0) {							\
			g_tq_fail++;						\
			TQ_TEST_ERR;						\
		} else {								\
			g_tq_success++;						\
			TQ_TEST_SUCCESS;					\
		}										\
	} while (0)

#define TQ_DECLARE								\
	static int g_tq_success = 0;				\
	static int g_tq_fail = 0

#define TQ_INITIALIZE()							\
	do {										\
		g_tq_success = g_tq_fail = 0;			\
	} while (0)

#define TQ_RESULT()														\
	do {																\
		printf("Result\tSuccess(%d)\tFail(%d)\n", g_tq_success, g_tq_fail);	\
	} while (0)

#endif // __TINY_TEST_UTILS_H__
