#ifndef OIC_STRING_UTIL_H
#define OIC_STRING_UTIL_H

#include <stdlib.h>
#include <unistd.h>

#define MAX_BUF_LEN 512
#define MAX_INPUT_QUERY_LEN 100
// #ifdef __cplusplus
// extern "C"
// {
// #endif // __cplusplus

int CompareStrings(const char* src, const char* dst);
int DuplicateString(const char* src, char** dst );
void ConcatString(char** target, char* attach);
int HexStringtoInt(const char* hex, int * num);
int GetIDValueFromQuery(char idvalue[], char* inputQuery, int size);
char* DAStrcat(char* dest, size_t destSize, const char* src);
char* DAStrncpy ( char * destination, const char * source, size_t num);

// #ifdef __cplusplus
// }
// #endif // __cplusplus
#endif
