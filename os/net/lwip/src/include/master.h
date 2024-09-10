
/*

    File: master.h


*/

#ifndef _DNRD_MASTER_H_
#define _DNRD_MASTER_H_

/* Interface to our master DNS */
int master_lookup(unsigned char *msg, int len);
int master_dontknow(unsigned char *msg, int len, unsigned char *answer);
int master_reinit(void);
int master_init(void);


#endif /* _DNRD_MASTER_H_ */

