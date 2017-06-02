/** @todo: Add Microsoft license information */

#ifndef GETOPT_H__
#define GETOPT_H__

#ifdef __cplusplus
extern "C" {
#endif

extern char *optarg;
extern int optind;

int getopt(int argc, char *const argv[], const char *optstring);

#ifdef __cplusplus
}
#endif

#endif

