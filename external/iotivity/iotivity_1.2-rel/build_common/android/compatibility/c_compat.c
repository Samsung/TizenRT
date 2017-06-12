#include <stdlib.h>
#include <sys/socket.h>

/* from stdlib.h */
float strtof(const char *nptr, char **endptr)
{
    return (float)strtod(nptr, endptr);
}

double atof(const char *nptr)
{
    return strtod(nptr, NULL);
}

int abs(int __n)
{
    return (__n < 0) ? -__n : __n;
}

long labs(long __n)
{
    return (__n < 0L) ? -__n : __n;
}

long long llabs(long long __n)
{
    return (__n < 0LL) ? -__n : __n;
}

int rand(void)
{
    return (int)lrand48();
}

void srand(unsigned int __s)
{
    srand48(__s);
}

long random(void)
{
    return lrand48();
}

void srandom(unsigned int __s)
{
    srand48(__s);
}

/* from __cmsg_nxthdr.cpp */
/*
 * The function __cmsg_nxthd() is missing in Android 4.4, but the Android NDK
 * header files in the version we are using are referencing it and we use it in
 * our code, this functions was added in version 5.0. To make IoTivity
 * dynamically loadable at load time on Android KitKat 4.4 add this functions
 * as a weak symbol, so it will be used if the c lib does not provide it, like
 * on Android < 5.0 This code was taken from these two resources:
 * https://raw.githubusercontent.com/android/platform_bionic/master/libc/bionic/__cmsg_nxthdr.cpp
 * https://github.com/android/platform_bionic/commit/ff64831b0965c16c95c9f81a148f30a6ef3a6c64
 */
struct cmsghdr* __attribute__((weak)) __cmsg_nxthdr(struct msghdr* msg, struct cmsghdr* cmsg)
{
    struct cmsghdr* ptr;
    ptr = (struct cmsghdr*)(((unsigned char*) cmsg) + CMSG_ALIGN(cmsg->cmsg_len));
    size_t len = (unsigned long)((char*)(ptr+1) - (char*) msg->msg_control);
    if (len > msg->msg_controllen) {
        return NULL;
    }
    return ptr;
}
