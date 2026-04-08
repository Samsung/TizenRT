/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

#include "s1_printf.h"

/*
 * UART PL011 Register offsets and constants.
 * These are minimal definitions for bootloader printf.
 */
#define UART_DR_OFFSET   0x00
#define UART_FR_OFFSET   0x18
#define PL011_FR_TXFF    (1 << 5) /* Transmit FIFO full */

/*
 * A simple up_putc implementation for the bootloader.
 * This writes a character directly to the UART.
 * It assumes CONFIG_UART0_BASE is the base address of the UART.
 */
int up_putc(int ch)
{
	/* Wait until the UART transmit FIFO is not full */
	while (*(volatile uint32_t *)(CONFIG_UART0_BASE + UART_FR_OFFSET) & PL011_FR_TXFF)
	{
		/* busy-wait */
	}

	/* Write the character to the UART data register */
	*(volatile uint32_t *)(CONFIG_UART0_BASE + UART_DR_OFFSET) = ch;

	return ch;
}

int putchar(int c)
{
	return up_putc(c);
}

static size_t local_strlen(const char *s)
{
    size_t len = 0;
    while (*s++) {
        len++;
    }
    return len;
}

static void print_number(unsigned long n, int base, int is_signed, int width, char pad, int uppercase)
{
	char buf[24];
	char *ptr = &buf[23];
	*ptr = '\0';

	int is_negative = 0;
	const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

	if (is_signed && (long)n < 0) {
		is_negative = 1;
		n = -(long)n;
	}

	if (n == 0) {
		*--ptr = '0';
	} else {
		while (n > 0) {
			*--ptr = digits[n % base];
			n /= base;
		}
	}

	int len = &buf[23] - ptr;
	if (is_negative) {
		len++;
	}

	if (pad == ' ') {
		while (width > len) {
			up_putc(' ');
			width--;
		}
	}

	if (is_negative) {
		up_putc('-');
	}

	if (pad == '0') {
		while (width > len) {
			up_putc('0');
			width--;
		}
	}

	while (*ptr) {
		up_putc(*ptr++);
	}
}

static int _s1_vprintf(const char *format, va_list ap)
{
	for (; *format != '\0'; format++) {
		if (*format == '%') {
			format++;
			int width = 0;
			char pad = ' ';
			int is_long = 0;

			if (*format == '0') {
				pad = '0';
				format++;
			}

			while (*format >= '0' && *format <= '9') {
				width = width * 10 + (*format - '0');
				format++;
			}

			if (*format == 'l') {
				is_long = 1;
				format++;
			}

			unsigned long val;
			int is_signed = 0;
			int uppercase = 0;

			switch (*format) {
			case 'd':
			case 'i':
				is_signed = 1;
				if (is_long) {
					val = va_arg(ap, long);
				} else {
					val = va_arg(ap, int);
				}
				print_number(val, 10, is_signed, width, pad, 0);
				break;
			case 'u':
				if (is_long) {
					val = va_arg(ap, unsigned long);
				} else {
					val = va_arg(ap, unsigned int);
				}
				print_number(val, 10, 0, width, pad, 0);
				break;
			case 'X':
				uppercase = 1;
				/* fallthrough */
			case 'x':
				if (is_long) {
					val = va_arg(ap, unsigned long);
				} else {
					val = va_arg(ap, unsigned int);
				}
				print_number(val, 16, 0, width, pad, uppercase);
				break;
			case 'p':
				val = (unsigned long)va_arg(ap, void *);
				print_number(val, 16, 0, width, pad, 0);
				break;
			case 's':
			{
				char *s = va_arg(ap, char *);
				if (!s) {
					s = "(null)";
				}
				int len = local_strlen(s);
				if (width > len) {
					int i;
					for (i = 0; i < width - len; i++) {
						up_putc(' ');
					}
				}
				for (; *s != '\0'; s++) {
					up_putc(*s);
				}
			}
				break;
			case 'c':
				up_putc(va_arg(ap, int));
				break;
			case '%':
				up_putc('%');
				break;
			default:
				up_putc('%');
				up_putc(*format);
				break;
			}
		} else {
			up_putc(*format);
		}
	}
	return 0; /* Simplified return */
}

int printf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	int ret = _s1_vprintf(format, ap);
	va_end(ap);
	return ret;
}

const char *s1_boot_prefix = "[S1-BOOT] ";

int s1_boot_printf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	for (const char *p = s1_boot_prefix; *p != '\0'; p++) {
		up_putc(*p);
	}

	int ret = _s1_vprintf(format, ap);
	va_end(ap);
	return ret;
}

int puts(const char *s)
{
	for (; *s != '\0'; s++)
	{
		up_putc(*s);
	}
	up_putc('\n');
	return 0; /* Simplified return */
}
