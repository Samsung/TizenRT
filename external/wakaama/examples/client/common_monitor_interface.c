#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <stdint.h>
#include "common_monitor_interface.h"


int readonce(FAR const char *filename, FAR char *buffer, uint8_t buflen)
{
    int ret;

    printf("opening %s\n", filename);
    int fd = open(filename, O_RDONLY);

    memset(buffer, 0x00, buflen);
    if (fd < 0)
    {
        printf("error reading file %s\n", filename);
        return ERROR;
    }
    else
    {
        size_t nbytes;


        /* Read until we hit the end of the file, until we have exhausted the
        * buffer space, or until some irrecoverable error occurs
        */

        *buffer   = '\0';       /* NUL terminate the empty buffer */
        ret       = ERROR;      /* Assume failure */

        nbytes = read(fd, buffer, buflen);
        printf("%d bytes read\n", nbytes);
        if (nbytes < 0)
        {
            /* Read error */

            int errcode = errno;
            DEBUGASSERT(errcode > 0);

            /* EINTR is not a read error.  It simply means that a signal was
            * received while waiting for the read to complete.
            */

            if (errcode != EINTR)
            {
                /* Fatal error */

                printf("EINTR error\n");
            }
        }
        else if (nbytes >= 0)
        {
            *(buffer+nbytes) = '\0';
            ret = OK;
        }

    }

    /* Close the file and return. */

    close(fd);
    return ret;
}

int readfile(FAR const char *filename, FAR char *buffer, uint8_t buflen)
{
    FAR char *bufptr;
    size_t remaining;
    //ssize_t nread;
    ssize_t ntotal;
    int ret;

    printf("opening %s\n", filename);
    int fd = open(filename, O_RDONLY);

    if (fd < 0)
    {
        printf("error reading file %s\n", filename);
        return ERROR;
    }
    else
    {
        size_t nbytes;


        /* Read until we hit the end of the file, until we have exhausted the
        * buffer space, or until some irrecoverable error occurs
        */

        ntotal    = 0;          /* No bytes read yet */
        *buffer   = '\0';       /* NUL terminate the empty buffer */
        bufptr    = buffer;     /* Working pointer */
        remaining = buflen - 1; /* Reserve one byte for a NUL terminator */
        ret       = ERROR;      /* Assume failure */

        do {
            nbytes = read(fd, buffer, buflen);
            printf("%d bytes read\n", nbytes);
            if (nbytes < 0)
            {
                /* Read error */

                int errcode = errno;
                DEBUGASSERT(errcode > 0);

                /* EINTR is not a read error.  It simply means that a signal was
                * received while waiting for the read to complete.
                */

                if (errcode != EINTR)
                {
                    /* Fatal error */

                    printf("EINTR error\n");
                    break;
                }
            }
            else if (nbytes == 0)
            {
                /* End of file */

                ret = OK;
                break;
            }
            else
            {
                /* Successful read.  Make sure that the buffer is null terminated */

                DEBUGASSERT(nbytes <= remaining);
                ntotal += nbytes;
                buffer[ntotal] = '\0';

                /* Bump up the read count and continuing reading to the end of
                * file.
                */

                bufptr    += nbytes;
                remaining -= nbytes;
            }
        } while (buflen > 0);
    }

    /* Close the file and return. */

    close(fd);
    return ret;
}

void mount_procfs(void)
{
    int ret;
    ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
}

