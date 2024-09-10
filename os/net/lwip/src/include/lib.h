/*

    File: lib.h


*/

#ifndef	_LIB_INCLUDED
#define	_LIB_INCLUDED


extern char *program;
extern int verbose;


void *allocate(size_t size);
void *reallocate(void *p, size_t size);

char *strlwr(char *string);
char *strupr(char *string);

char *skip_ws(char *string);
char *noctrl(char *buffer);
char *get_word(char **from, char *to, int maxlen);
char *get_quoted(char **from, int delim, char *to, int max);
char *copy_string(char *y, char *x, unsigned int len);

unsigned int get_stringcode(char *string);
char *dnrd_strdup(const char *str);
#endif

