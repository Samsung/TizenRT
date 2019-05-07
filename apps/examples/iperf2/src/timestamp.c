/*                                                                                                                                                                      
 * A simple timestamp using realtime clock                                                                                                                              
 *                                                                                                                                                                      
 * usage: timestamp -c <count> -d <delay seconds>                                                                                                                       
 *                                                                                                                                                                      
 * compile with 'gcc -O2 -lrt  timestamp.c -o timestamp'                                                                                                                
 *                                                                                                                                                                      
 * Author Robert J. McMahon (rmcmahon)                                                                                                                                  
 * Last modified: 09/02/2016                                                                                                                                            
 *                                                                                                                                                                      
 * $Copyright Open Broadcom Corporation$                                                                                                                                
 *                                                                                                                                                                      
 */                                                                                                                                                                     
#include <stdio.h>                                                                                                                                                      
#include <stdlib.h>                                                                                                                                                     
#include <time.h>                                                                                                                                                       
#include <errno.h>                                                                                                                                                      
#include <ctype.h>                                                                                                                                                      
#include <unistd.h>                                                                                                                                                     
#include <signal.h>                                                                                                                                                     
#include <sys/time.h>                                                                                                                                                   
#include <string.h>                                                                                                                                                     
#include <sched.h>                                                                                                                                                      

static void sigalrm();
static void posttimestamp(void);
static int postcount = 0;       

int main (int argc, char **argv) {
    char *delayvalue = NULL;      
    int c, count=10;              
    double delay=1.0;             
    int err;                      
    struct itimerval it;          

    while ((c = getopt(argc, argv, "c:d:")) != -1) {
        switch (c) {                                
        case 'c':                                   
            count = atoi(optarg);                   
            break;                                  
        case 'd':                                   
            delayvalue = optarg;                    
            break;                                  
        case '?':                                   
            fprintf (stderr, "usage: -c <count> -d <delay seconds> \n", optopt);
            return 1;                                                           
        default:                                                                
            abort ();                                                           
        }                                                                       
    }
    if (delayvalue != NULL) {
        delay = atof(delayvalue);
    }
    postcount = count;
    // Should probably use sigaction()
    signal(SIGALRM, sigalrm);
    memset (&it, 0, sizeof (it));
    it.it_value.tv_sec = (int) (delay);
    it.it_value.tv_usec = (int) (1000000 * (delay - it.it_value.tv_sec ));
    it.it_interval.tv_sec = it.it_value.tv_sec;
    it.it_interval.tv_usec = it.it_value.tv_usec;
    err = setitimer( ITIMER_REAL, &it, NULL );
    if (err < 0) {
        perror("itimer");
        exit(1);
    }
    fprintf(stdout,"Timestamping %d times with %f second delay\n", count, delay);
    fflush(stdout);
    while (postcount > 0) {
        pause();
    }
}

void posttimestamp (void) {
    struct timespec t1;
    double timestamp;
    int err;

    err = clock_gettime(CLOCK_REALTIME, &t1);
    if (err < 0) {
        perror("clock_getttime");
    } else {
        timestamp = t1.tv_sec + (t1.tv_nsec / 1000000000.0);
        fprintf(stdout,"%f counter(%d)\n", timestamp, postcount);
    }
    fflush(stdout);
}

void sigalrm (void) {
    posttimestamp();
    postcount--;
}




