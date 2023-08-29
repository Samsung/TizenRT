#include <sys/types.h>

size_t
strlcpy(char *dst, const char *src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;

	/* Copy as many bytes as will fit. */
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0') {
				break;
			}
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src. */
	if (nleft == 0) {
		if (dsize != 0) {
			*dst = '\0';    /* NUL-terminate dst */
		}
		while (*src++)
			;
	}

	return (src - osrc - 1); /* count does not include NUL */
}