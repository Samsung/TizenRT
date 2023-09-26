/*
FUNCTION
	<<memset>>---set an area of memory

INDEX
	memset

ANSI_SYNOPSIS
	#include <string.h>
	void *memset(void *<[dst]>, int <[c]>, size_t <[length]>);

TRAD_SYNOPSIS
	#include <string.h>
	void *memset(<[dst]>, <[c]>, <[length]>)
	void *<[dst]>;
	int <[c]>;
	size_t <[length]>;

DESCRIPTION
	This function converts the argument <[c]> into an unsigned
	char and fills the first <[length]> characters of the array
	pointed to by <[dst]> to the value.

RETURNS
	<<memset>> returns the value of <[dst]>.

PORTABILITY
<<memset>> is ANSI C.

    <<memset>> requires no supporting OS subroutines.

QUICKREF
	memset ansi pure
*/

#include <section_config.h>
#include <basic_types.h>

#include <string.h>
//#include "local.h"

#ifdef _HAVE_CC_INHIBIT_LOOP_TO_LIBCALL
# define __inhibit_loop_to_libcall __attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns")))
#else
# define __inhibit_loop_to_libcall
#endif

#define LBLOCKSIZE (sizeof(long))
#define UNALIGNED(X)   ((long)X & (LBLOCKSIZE - 1))
#define TOO_SMALL(LEN) ((LEN) < LBLOCKSIZE)

LIBC_ROM_TEXT_SECTION
_LONG_CALL_
void *__inhibit_loop_to_libcall _memset(void *m, int c, size_t n)
{
	char *s = (char *) m;

//#if !defined(PREFER_SIZE_OVER_SPEED)
	u32 i;
	unsigned long buffer;
	unsigned long *aligned_addr;
	uint32_t d = c & 0xff;	/* To avoid sign extension, copy C to an
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
		/*
		* SVACE-UNREACHABLE_CODE : It was depends on the chip architecture is 32bit or 64bit and the #define LBLOCKSIZE (sizeof(long)) value set by the user
		* Realtek: To cover all architecture RTK solution
		*/
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
