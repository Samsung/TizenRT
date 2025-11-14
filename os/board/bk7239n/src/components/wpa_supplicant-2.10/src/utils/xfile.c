#include "xfile.h"
#include <string.h>

static xFILE sysfiles[MAXFILENUM];

int register_xfile(char *name, char *base, int size)
{
	int i;
	for (i = 0; i < MAXFILENUM; i++) {
		if (!sysfiles[i]._tmpfname) {
			sysfiles[i]._tmpfname = name;
			sysfiles[i]._base = base;
			sysfiles[i]._ptr = base;
			sysfiles[i]._flag = _XFIXSIZE;
			sysfiles[i]._bufsiz = size;
			sysfiles[i]._cnt = size;
			return 0;
		}
	}

	return -1;
}

/* find the file handle */
xFILE *xfindfile(const char *name)
{
	int i;

	if ((name == 0) || (strlen(name) == 0)) {
		WPA_LOGI("xfile: invalid filename.\n");
		return NULL;
	}

	for (i = 0; i < MAXFILENUM - 1; i++) {
		if (sysfiles[i]._tmpfname && !strcmp(name, sysfiles[i]._tmpfname))
			return &sysfiles[i];
	}
	WPA_LOGI("xfile: %s not found.\n", name);
	return NULL;
}


xFILE *xfopen(const char *filename, const char *mode)
{
	xFILE *f;

	f = xfindfile(filename);
	WPA_LOGI("xfile open: %s\n", filename);

	if (f->_flag & _XFINUSE)
		return NULL; /* file is already opened */

	f->_ptr = f->_base;
	f->_flag |= _XFINUSE;

	return f;
}


int xfread(void *buffer, uint32 size, uint32 count, xFILE *f)
{
	uint32 bound, cnt = 0, i = 0;
	char *buf = buffer;

	if (!f)
		return -1;

	if (f->_flag & _XFIXSIZE) {
		bound = (uint32)f->_base + f->_cnt;
		while (cnt < count) {
			if ((uint32)f->_ptr + size > bound)
				break;
			memcpy(&buf[i], f->_ptr, size);
			i += size;
			f->_ptr += size;
			cnt++;
		}
		return cnt;
	}

	return -1;
}

int xfwrite(const void *buffer, uint32 size, uint32 count, xFILE *f)
{
	uint32 bound, cnt = 0, i = 0;
	const char *buf = (const char *)buffer;

	if (!f)
		return -1;

	if (f->_flag & _XFIXSIZE) {
		bound = (uint32)f->_base + f->_bufsiz;

		while (cnt < count) {
			if ((uint32)f->_ptr + size > bound)
				break;
			memcpy(f->_ptr, &buf[i], size);
			f->_ptr += size;
			cnt++;
			i += size;
			if (f->_ptr >= f->_base + f->_cnt)
				f->_cnt = (int)(f->_ptr - f->_base);
		}

		return cnt;
	}

	return -1;
}

int xfclose(xFILE *f)
{
	if (!f)
		return -1;

	f->_flag &= ~_XFINUSE;

	WPA_LOGI("Closing file: %s\n", f->_tmpfname);
	return 0;
}

int xfseek(xFILE *f, int offset, int whence)
{
	if (!f)
		return -1;

	if (f->_flag & _XFIXSIZE) {
		uint32_t base;
 		uint32 bound = (uint32)f->_base + f->_cnt;

		switch (whence) {
		case xSEEK_SET:
			base = (uint32)f->_base;
			break;
		case xSEEK_CUR:
			base = (uint32)f->_ptr;
			break;
		case xSEEK_END:
			base = (uint32)f->_base + f->_cnt;
			break;
		default:
			return -1;
		}

		if (base + offset > bound)
			return -1;
		if (base + offset < (uint32)f->_base)
			return -1;

		f->_ptr = (char *)(base + offset);

		return 0;
	}

	return -1;
}

int xfsize(xFILE *f)
{
	if (!f)
		return -1;

	if (f->_flag & _XFIXSIZE)
		return f->_cnt;

	return 0;
}

char *xfgets(char *string, int n, xFILE *f)
{
	uint32 bound;
	int i;
	char *p = string; /* assign input string to p */

	if (!f)
		return NULL;

	if (f->_flag & _XFIXSIZE) {
		bound = (uint32)f->_base + f->_cnt;
		if ((uint32)f->_ptr >= bound)
			return NULL;

		for (i = 0; i < (n - 1); i++) {
			if ((uint32)f->_ptr >= bound)
				break;
			if (*f->_ptr == '\n') {
				f->_ptr++;
				break;
			}
			*p++ = *f->_ptr++;
		}
		*p = '\0';
		return (string);
	}

	return NULL;
}

int xfrewind(xFILE *f)
{
	return xfseek(f, 0, xSEEK_SET);
}

long xftell(xFILE *f)
{
	if (!f)
		return -1;

	if (!f->_ptr || f->_ptr < f->_base)
		return -1;

	return (uint32_t)f->_ptr - (uint32_t)f->_base;
}


int xfsetstate(char *name, int *value, int type)
{
	xFILE *f = 0;

	f = xfindfile(name);
	if (f == 0)
		return -1; /* file not found */

	switch (type) {
	case xFILEFLAG:
		f->_flag = (int)value;
		break;

	case xFILEBUFF:
		f->_bufsiz = (int)value;
		break;

	case xFILEBASE:
		f->_base = (char *)value;
		break;
	}

	return 0;
}


