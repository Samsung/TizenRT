/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "basic_types.h"
#include "ameba_soc.h"
#include "section_config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#define PI               3.14159265358979f

#undef LONG_MIN
#define LONG_MIN (-LONG_MAX-1)
#undef LONG_MAX
#define LONG_MAX __LONG_MAX__

#undef ULONG_MAX
#define ULONG_MAX (LONG_MAX * 2UL + 1)

#if 0
//#ifndef isprint
#define in_range(c, lo, up)	((u8)c >= lo && (u8)c <= up)
#define isprint(c)			in_range(c, 0x20, 0x7f)
#define isdigit(c)			in_range(c, '0', '9')
#define isxdigit(c)			(isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))
#define isspace(c)			(c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == ',')
#define isupper(c)			(((c)>='A')&&((c)<='Z'))
#define islower(c)			(((c)>='a')&&((c)<='z'))
#define isalpha(c)			(isupper(c) || islower(c))
#endif

#if defined (ARM_CORE_CM4)
#define LBLOCKSIZE (sizeof(long))
#define UNALIGNED(X)   ((long)X & (LBLOCKSIZE - 1))
#define TOO_SMALL(LEN) ((LEN) < LBLOCKSIZE)
/* Nonzero if either X or Y is not aligned on a "long" boundary.  */
#define UNALIGNED_XY(X, Y) \
  (((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE    (sizeof (long) << 2)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof (long))

void test_strcat(void)
{
	char a[20] = "st";
	char b[] = "st)";

	printf("be:%s\n", a);
	printf("af:%s\n", strcat(a, b));
	// before strcat():string(1)
	// after  strcat():string(1)string(2)
}

_OPTIMIZE_NONE_
void test_strchr(void)
{
	char *s = "01";
	char *p;

	p = strchr(s, '5');
	printf("%s\n", p);
	// 56789012345678901234567890
}

void test_strcpy(void)
{
	char a[20] = "st";
	char b[] = "st(2)";

	printf("be:%s\n", a);
	printf("af):%s\n", strcpy(a, b));
	// before strcpy():string(1)
	// after  strcpy():string(2)
}

void test_strncat(void)
{
	char a[30] = "s";
	char b[] = "string(2)";

	printf("be:%s\n", a);
	printf("af:%s\n", strncat(a, b, 10));
	// before strncat():string(1)
	// after  strncat():string(1)string
}

void test_strncmp(void)
{
	char *a = "aB";
	char *b = "Ab";

	printf("strncmp(a, b): %d\n", strncmp(a, b, 6));
	// strncmp(a, b): 32

}

void test_strncpy(void)
{
	char a[20] = "s1";
	char b[] = "s2";

	printf("be:%s\n", a);
	printf("af:%s\n", strncpy(a, b, 9));
	// before strncpy():string(1)
	// after  strncpy():string(1)
}

void test_strsep(void)
{
	char s[] = "ab-y;";
	char *delimit = " -:";
	char *p;
	char *ptr = s;

	while ((p = (char *)strsep(&ptr, delimit))) {
		printf("%s ", p);
	}
	// ab cd ef;gh i jkl mnop;qrs  tu  vwx y;z / * -: characters have been / 0 characters instead of * /
}

_OPTIMIZE_NONE_
void test_strstr(void)
{
	char *s = "01";
	char *p;

	p = strstr(s, "901");
	printf("%s\n", p);
	// 9012345678901234567890
}

void test_strtok(void)
{
	char s[] = "ab-;z";
	char *delimit = " -:";
	char *p;

	printf("%s ", (char *)strtok(s, delimit));
	while ((p = (char *) strtok(NULL, delimit))) {
		printf("%s ", p);
	}
	//printf("%s\n",p);
	// ab cd ef;gh i jkl mnop;qrs tu vwx y;z / * -: characters have been / 0 characters instead of * /
}

void test_strpbrk(void)
{
	const char str1[] = "ab";
	const char str2[] = "34";
	char *ret;

	ret = strpbrk(str1, str2);
	if (ret) {
		printf("f: %c\n", *ret);
	} else {
		printf("e\n");
	}
}

void test_stratoi(void)
{
	const char str1[] = "ab";
	u32 num = atoi(str1);
	printf("num: %d\n", (int)num);

}

void test_strtol(void)
{
	char str[30] = "20test";
	char *ptr;
	long ret;

	ret = strtol(str, &ptr, 10);
	printf("num: %ld\n", ret);
	printf("str: |%s|", ptr);
}

_OPTIMIZE_NONE_
void fadd(double *c, double a, double b)
{
	*c = a + b;
}

_OPTIMIZE_NONE_
void fsub(double *c, double a, double b)
{
	*c = a - b;
}

_OPTIMIZE_NONE_
void fmul(double *c, double a, double b)
{
	*c = a * b;
}

_OPTIMIZE_NONE_
void fdiv(double *c, double a, double b)
{
	*c = a / b;
}

_OPTIMIZE_NONE_
void acos_test(void)
{
	double x, ret, val;

	x = 0.9;

	fdiv(&val, 180.0, PI);
	//val = 180.0 / PI;

	fmul(&ret, acos(x), val);
	printf("the acos of %lf is %lf", x, ret);

}

_OPTIMIZE_NONE_
void asin_test(void)
{
	double x, ret, val;
	x = 0.9;
	fdiv(&val, 180.0, PI);

	fmul(&ret, asin(x), val);
	printf("the asin of %lf is %lf", x, ret);

}

_OPTIMIZE_NONE_
void atan_test(void)
{
	double x, ret, val;

	x = 0.9;
	fdiv(&val, 180.0, PI);
	fmul(&ret, atan(x), val);
	printf("The atan of %lf is %lf\n", x, ret);
}

_OPTIMIZE_NONE_
void atan2_test(void)
{
	double x, y, ret, val;

	x = -7.0;
	y = 7.0;
	fdiv(&val, 180.0, PI);
	fmul(&ret, atan2(y, x), val);

	printf("%lf\n", ret);
}
//x = -7.000000, y = 7.000000 的反正切是 135.000000 度

_OPTIMIZE_NONE_
void cos_test(void)
{
	double x, ret, val;

	x = 0.9;
	fdiv(&val, 180.0, PI);
	fmul(&ret, cos(x), val);

	printf("The arc cosine of %lf is %lf\n", x, ret);
}

_OPTIMIZE_NONE_
void cosh_test(void)
{
	double x = 0;
	fadd(&x, x, 0.5);

	printf("%lf 的双曲余弦是 %lf\n", x, cosh(x));
}

//0.500000 的双曲余弦是 1.127626 1.000000 的双曲余弦是 1.543081 1.500000 的双曲余弦是 2.352410

_OPTIMIZE_NONE_
void sin_test(void)
{
	double x, ret, val;

	x = 0.9;
	fdiv(&val, 180.0, PI);
	fmul(&ret, sin(x), val);
	printf("The arc cosine of %lf is %lf\n", x, ret);
}

_OPTIMIZE_NONE_
void sinh_test(void)
{
	double x, ret;
	x = 0.5;

	ret = sinh(x);
	printf("%lf 的双曲正弦是 %lf 度", x, ret);
}
//0.500000 的双曲正弦是 0.521095 度

_OPTIMIZE_NONE_
void tan_test(void)
{
	double x, ret, val;

	x = 0.9;
	val = 180.0 / PI;

	ret = tan(x) * val;
	printf("The arc cosine of %lf is %lf\n", x, ret);
}

_OPTIMIZE_NONE_
void tanh_test(void)
{
	double x, ret;
	x = 0.5;

	ret = tanh(x);
	printf("%lf 的双曲正切是 %lf 度", x, ret);
}
//0.500000 的双曲正切是 0.462117 度

_OPTIMIZE_NONE_
void exp_test(void)
{
	double x = 0;
	printf("e 的 %lf 次幂是 %lf\n", x + 2, exp(x + 2));

}
// e 的 2.000000 次幂是 7.389056

_OPTIMIZE_NONE_
void frexp_test(void)
{
	double x = 1024, fraction;
	int e;

	fsub(&x, 2048, x);
	fraction = frexp(x, &e);
	printf("x = %.2lf = %.2lf * 2^%d\n", x, fraction, e);

}

_OPTIMIZE_NONE_
void ldexp_test(void)
{
	double x, ret;
	int n;

	x = 0.65;
	n = 3;
	ret = ldexp(x, n);
	printf("%f * 2^%d = %f\n", x, n, ret);
}
//0.650000 * 2^3 = 5.200000

_OPTIMIZE_NONE_
void log_test(void)
{
	double x, ret;
	x = 2.7;

	/* 计算 log(2.7) */
	ret = log(x);
	printf("log(%lf) = %lf", x, ret);
}
//log(2.700000) = 0.993252

_OPTIMIZE_NONE_
void log10_test(void)
{
	double x, ret;
	x = 10000;

	/* 计算 log10(10000) */
	ret = log10(x);
	printf("log10(%lf) = %lf\n", x, ret);
}
//log10(10000.000000) = 4.000000

_OPTIMIZE_NONE_
void modf_test(void)
{
	double x, fractpart, intpart;

	x = 8.123456;
	fractpart = modf(x, &intpart);

	printf("intpart = %lf\n", intpart);
	printf("fractpart = %lf \n", fractpart);

}

_OPTIMIZE_NONE_
void pow_test(void)
{
	double x, ret;

	x = 8.0;
	ret = pow(x, 3);
	printf("值 8.0 ^ 3 = %lf\n", ret);

	//printf("值 3.05 ^ 1.98 = %lf", pow(3.05, 1.98));
}
//值 8.0 ^ 3 = 512.000000 值 3.05 ^ 1.98 = 9.097324

_OPTIMIZE_NONE_
void sqrt_test(void)
{
	double x, ret;

	x = 4.0;
	ret = sqrt(x);
	printf("%lf %lf\n", x, ret);
	//printf("%lf %lf\n", 5.0, sqrt(5.0) );
}
//4.000000 的平方根是 2.000000  5.000000 的平方根是 2.236068

_OPTIMIZE_NONE_
void ceil_test(void)
{
	float val1;

	val1 = 1.6;
	printf("value1 = %.1lf\n", ceil(val1));
}

_OPTIMIZE_NONE_
void fabs_test(void)
{
	float a, b;
	a = 1234;
	b = -344;

	printf("%lf %lf\n", a, fabs(a));
	printf("%lf %lf\n", b, fabs(b));
}

//1234 的绝对值是 1234.000000 -344 的绝对值是 344.000000

_OPTIMIZE_NONE_
void floor_test(void)
{
	float val1;

	val1 = 1.6;

	printf("Value1 = %.1lf\n", floor(val1));
}
//Value1 = 1.0

_OPTIMIZE_NONE_
void fmod_test(void)
{
	float a, b;
	int c;
	a = 9.2;
	b = 3.7;
	c = 2;
	printf("%f / %d 的余数是 %lf\n", a, c, fmod(a, c));
	printf("%f / %f 的余数是 %lf\n", a, b, fmod(a, b));
}
//9.200000 / 2 的余数是 1.200000  9.200000 / 3.700000 的余数是 1.800000


_OPTIMIZE_NONE_
void math_test(void)
{
	acos_test();
	asin_test();
	atan_test();
	atan2_test();
	cos_test();
	cosh_test();
	sin_test();
	sinh_test();
	tan_test();
	tanh_test();
	exp_test();
	frexp_test();
	ldexp_test();
	log_test();
	log10_test();
	modf_test();
	pow_test();
	sqrt_test();
	ceil_test();
	fabs_test();
	floor_test();
	fmod_test();
}

int rtl_string_test(void)
{
#if 0
	test_strcmp();
	test_strtoul();
	test_strlen();
	test_stricmp();
	test_strupr();
	test_char2num();
	test_2char2dec();
	test_2char2hex();
#endif
	test_strcat();
	test_strchr();
	test_strcpy();
	test_strncat();
	test_strncmp();
	test_strncpy();
	test_strsep();
	test_strstr();
	test_strtok();
	test_strpbrk();
	test_stratoi();
	test_strtol();

	math_test();
	return 0;
}


void stdlib_printf_call(u32 argc, u8 *szmac)
{
	u8 mac[6] = {0};
	char code[6];
	char buf[6];

	printf("%s\n", "rtl_string_test");
	printf("%d\n", (int)argc);

	sprintf((char *)szmac, "%02X%02X%02X%02X%02X%02X",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	snprintf(code, sizeof(code), "%03u", (unsigned int)argc);
	va_list vArgList;

	vsnprintf(buf, sizeof(buf), (char *)szmac, vArgList);
}

void stdlib_scanf_call(void)
{
	char *str = "";
	char buf1[28] = {0} ;
	char buf2[28] = {0} ;

	signed int ret = 10;
	char sc = 0;

	str = " ????1234567890ABCDEFghijklmnorst";  //the first char is white space
	ret = sscanf(str, "%c???%26c%s", &sc, buf1, buf2);
	printf("%d", ret);
}

void *stdlib_memset(void *m, int c, size_t n)
{
	char *s = (char *) m;

	//#if !defined(PREFER_SIZE_OVER_SPEED)
	u32 i;
	unsigned long buffer;
	unsigned long *aligned_addr;
	unsigned int d = c & 0xff;	/* To avoid sign extension, copy C to an
	unsigned variable.  */

	while (UNALIGNED(s)) {
		if (n--) {
			*s++ = (char) c;
		} else {
			return m;
		}
	}

	if (!TOO_SMALL(n)) {
		/* If we get this far, we know that n is large and s is word-aligned. */
		aligned_addr = (unsigned long *) s;

		/* Store D into each char sized location in BUFFER so that
		we can set large blocks quickly.  */
		buffer = (d << 8) | d;
		buffer |= (buffer << 16);
		for (i = 32; i < LBLOCKSIZE * 8; i <<= 1) {
			buffer = (buffer << i) | buffer;
		}

		/* Unroll the loop.  */
		while (n >= LBLOCKSIZE * 4) {
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			*aligned_addr++ = buffer;
			n -= 4 * LBLOCKSIZE;
		}

		while (n >= LBLOCKSIZE) {
			*aligned_addr++ = buffer;
			n -= LBLOCKSIZE;
		}
		/* Pick up the remainder with a bytewise loop.  */
		s = (char *)aligned_addr;
	}

	//#endif /* not PREFER_SIZE_OVER_SPEED */

	while (n--) {
		*s++ = (char) c;
	}

	return m;
}

void *stdlib_memcpy(void *dst0, const void *src0, size_t len0)
{
#if defined(PREFER_SIZE_OVER_SPEED)
	char *dst = (char *) dst0;
	char *src = (char *) src0;

	void *save = dst0;

	while (len0--) {
		*dst++ = *src++;
	}

	return save;
#else
	char *dst = dst0;
	const char *src = src0;
	long *aligned_dst;
	const long *aligned_src;

	/* If the size is small, or either SRC or DST is unaligned,
	then punt into the byte copy loop.  This should be rare.  */
	if (!TOO_SMALL(len0) && !UNALIGNED_XY(src, dst)) {
		aligned_dst = (long *)dst;
		aligned_src = (long *)src;

		/* Copy 4X long words at a time if possible.  */
		while (len0 >= BIGBLOCKSIZE) {
			*aligned_dst++ = *aligned_src++;
			*aligned_dst++ = *aligned_src++;
			*aligned_dst++ = *aligned_src++;
			*aligned_dst++ = *aligned_src++;
			len0 -= BIGBLOCKSIZE;
		}

		/* Copy one long word at a time if possible.  */
		while (len0 >= LITTLEBLOCKSIZE) {
			*aligned_dst++ = *aligned_src++;
			len0 -= LITTLEBLOCKSIZE;
		}

		/* Pick up any residual with a byte copier.  */
		dst = (char *)aligned_dst;
		src = (char *)aligned_src;
	}

	while (len0--) {
		*dst++ = *src++;
	}

	return dst0;
#endif /* not PREFER_SIZE_OVER_SPEED */
}


void stdlib_init(void)
{
	//ROM_SECURE_CALL_NS_ENTRY *prom_export_func = (ROM_SECURE_CALL_NS_ENTRY *)__rom_entry_ns_start__;

	//prom_export_func->diagprintf("stdlib_init\n");

	/* init stdlib rom bss section */
	stdlib_memset((void *)__rom_stdlib_bss_start__, 0,
				  __rom_stdlib_bss_end__ - __rom_stdlib_bss_start__);

	/* load stdlib rom data to sram */
	stdlib_memcpy(__rom_stdlib_data_start__, __rom_stdlib_text_end__,
				  (__rom_stdlib_data_end__ - __rom_stdlib_data_start__));
}

/* this function is used to link all stdlib and test */
/* int _rtl_printf(IN const char* fmt, ...); */
/* int _rtl_sprintf(char* str, const char* fmt, ...); */
/* int _rtl_snprintf(char* str, size_t size, const char* fmt, ...); */
/* int _rtl_vsnprintf(char *buf, size_t size, const char *fmt, va_list ap); */
/* int _rtl_sscanf(const char *buf, const char *fmt, ...); */
void stdlib_test(void)
{
	stdlib_printf_call(0, NULL);
	stdlib_scanf_call();
	rtl_string_test();
}

SECTION(".stdlib.entry.table")
STDLIB_ENTRY_TABLE stdlib_entry_table = {
	stdlib_init,
	stdlib_test,
};

//low level io called by stdlib
int _write(int file, char *ptr, int len)
{
	int nChars = 0;
	ROM_SECURE_CALL_NS_ENTRY *prom_export_func = (ROM_SECURE_CALL_NS_ENTRY *)__rom_entry_ns_start__;

	/* Check for stdout and stderr
	(only necessary if file descriptors are enabled.) */
	if (file != 1 && file != 2) {
		return -1;
	}
	for (/*Empty */; len > 0; --len) {
		prom_export_func->loguart_putchar(*ptr++);
		++nChars;
	}
	return nChars;
}

int _read(int file, char *ptr, int len)
{
	int nChars = 0;
	ROM_SECURE_CALL_NS_ENTRY *prom_export_func = (ROM_SECURE_CALL_NS_ENTRY *)__rom_entry_ns_start__;

	/* Check for stdin
	(only necessary if FILE descriptors are enabled) */
	if (file != 0) {
		return -1;
	}
	for (/*Empty*/; len > 0; --len) {
		int c = prom_export_func->loguart_getchar(_TRUE);
		if ((c < 0) || (c == '\r')) {
			*ptr = '\0';
			break;
		}
		*(ptr++) = c;
		++nChars;
	}
	return nChars;
}

#else

_OPTIMIZE_NONE_
int rtl_string_test()
{
	test_strcmp();
	test_strtoul();
	test_strlen();
	test_stricmp();
	test_strupr();
	test_char2num();
	test_2char2dec();
	test_2char2hex();

	return 0;;
}

/* this function is used to link all stdlib and test */
/* int _rtl_printf(IN const char* fmt, ...); */
/* int _rtl_sprintf(char* str, const char* fmt, ...); */
/* int _rtl_snprintf(char* str, size_t size, const char* fmt, ...); */
/* int _rtl_vsnprintf(char *buf, size_t size, const char *fmt, va_list ap); */
/* int _rtl_sscanf(const char *buf, const char *fmt, ...); */
void stdlib_test(void)
{
	rtl_string_test();
}

SECTION(".stdlib.entry.table")
STDLIB_ENTRY_TABLE stdlib_entry_table = {
	stdlib_test,
};

#endif

