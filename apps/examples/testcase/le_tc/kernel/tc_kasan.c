#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <tinyara/mm/kasan_error.h>
#include <tinyara/config.h>

#include "tc_internal.h"

static int global;

static void glbls_over(void)
{
	int *global_p, readed_val;
	unsigned int kasan_error;

	printf("\n=== GLOBALS OVERFLOW ===\n");

	global_p = &global;
	*global_p = 42;

	global_p++;

	printf("incorrect read\n");

	kasan_reset_error();
	readed_val = *global_p;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_GLOBAL_READ);

	printf("incorrect write\n");

	kasan_reset_error();
	*global_p = readed_val;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_GLOBAL_WRITE);

	TC_SUCCESS_RESULT();
}

static void glbls_under(void)
{
	int *global_p, readed_val;
	unsigned int kasan_error;

	printf("\n=== GLOBALS UDERFLOW ===\n");

	global_p = &global;
	*global_p = 42;

	global_p--;

	printf("incorrect read\n");

	kasan_reset_error();
	readed_val = *global_p;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_GLOBAL_READ);

	printf("incorrect write\n");

	kasan_reset_error();
	*global_p = readed_val;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_GLOBAL_WRITE);

	TC_SUCCESS_RESULT();
}

#define SIZE 42

static void hp_over(void)
{
	char *mem_ref, orig_char;
	int i;
	unsigned int kasan_error;

	printf("\n=== HEAP OVERFLOW ===\n");

	mem_ref = malloc(SIZE);

	for (i = 0; i < SIZE; ++i)
		mem_ref[i] = i;

	mem_ref += SIZE;

	printf("incorrect read\n");

	kasan_reset_error();
	orig_char = *mem_ref;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_HEAP_READ);

	printf("incorrect write\n");

	kasan_reset_error();
	*mem_ref = orig_char;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_HEAP_WRITE);

	mem_ref -= SIZE;
	free(mem_ref);

	TC_SUCCESS_RESULT();
}

static void hp_under(void)
{
	char *mem_ref, orig_char;
	int i;
	unsigned int kasan_error;

	printf("\n=== HEAP UNDERFLOW ===\n");

	mem_ref = malloc(SIZE);

	for (i = 0; i < SIZE; ++i)
		mem_ref[i] = i;

	mem_ref--;

	printf("incorrect read\n");

	kasan_reset_error();
	orig_char = *mem_ref;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_HEAP_READ);

	printf("incorrect write\n");

	kasan_reset_error();
	*mem_ref = orig_char;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_HEAP_WRITE);

	mem_ref++;
	free(mem_ref);

	TC_SUCCESS_RESULT();
}

static void hp_uaf(void)
{
	int *mem_ref, readed_val;
	unsigned int kasan_error;

	printf("\n=== HEAP USE-AFTER-FREE ===\n");

	mem_ref = malloc(SIZE);

	*mem_ref = SIZE;

	free(mem_ref);

	printf("incorrect read\n");

	kasan_reset_error();
	readed_val = *mem_ref;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_HEAP_UAF_READ);

	printf("incorrect write\n");

	kasan_reset_error();
	*mem_ref = readed_val;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_HEAP_UAF_WRITE);

	TC_SUCCESS_RESULT();
}

static void stck_over(void)
{
	int *var_p, var, readed_val;
	unsigned int kasan_error;

	printf("\n=== STACK OVERFLOW ===\n");

	var = 10;
	var_p = &var;

	*var_p = 12;

	var_p++;

	printf("incorrect read\n");

	kasan_reset_error();
	readed_val = *var_p;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_STACK_READ);

	printf("incorrect write\n");

	kasan_reset_error();
	*var_p = readed_val;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_STACK_WRITE);

	TC_SUCCESS_RESULT();
}

static void stck_under(void)
{
	int *var_p, var, readed_val;
	unsigned int kasan_error;

	printf("\n=== STACK UNDERFLOW ===\n");

	var = 10;
	var_p = &var;

	*var_p = 12;

	var_p--;

	printf("incorrect read\n");

	kasan_reset_error();
	readed_val = *var_p;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_STACK_READ);

	printf("incorrect write\n");

	kasan_reset_error();
	*var_p = readed_val;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_STACK_WRITE);

	TC_SUCCESS_RESULT();
}

static void stck_uas(void)
{
	int *var_p, readed_val;
	unsigned int kasan_error;

	printf("\n=== STACK USE-AFTER-SCOPE ===\n");

	{
		int var;

		var = 10;
		var_p = &var;
	}

	printf("incorrect read\n");

	kasan_reset_error();
	readed_val = *var_p;

	/* just for avoiding optimizing out */
	printf("*var_p %d\n", readed_val);

	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_STACK_UAS_READ);

	printf("incorrect write\n");

	kasan_reset_error();
	*var_p = readed_val;
	kasan_error = kasan_get_error();
	TC_ASSERT_EQ("kasan", kasan_error, KASAN_STACK_UAS_WRITE);

	TC_SUCCESS_RESULT();
}

typedef void (*test_func)(void);

static test_func test[] = {
#ifdef CONFIG_KASAN_HEAP
	&hp_over,
	&hp_under,
#endif

#ifdef CONFIG_KASAN_QRNT
	&hp_uaf,
#endif

#ifdef CONFIG_KASAN_GLOBALS
	&glbls_over,
	&glbls_under,
#endif

#ifdef CONFIG_KASAN_STACK
	&stck_over,
	&stck_under,
	&stck_uas,
#endif

	/* termination item */
	NULL
};

int kasan_main(void)
{
	int i;

	printf("kasan tests\n");

	for (i = 0; test[i] != NULL; ++i)
		(*test[i])();

	return 0;
}
