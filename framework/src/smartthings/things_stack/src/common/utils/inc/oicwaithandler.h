/*
 * oicwaithandler.h
 *
 *  Created on: 2016. 10. 31.
 *      Author: eunseok
 */

#ifndef OICWAITHANDLER_H_
#define OICWAITHANDLER_H_

#include <octypes.h>

#define DEFAULT_TIMEOUT     60      // default 20 second wait.

typedef struct timeout_s
{
    int cur_counter;        // current time-out counter
    int cur_num;            // current time-out number
    int ori_num;            // initial time-out number
}TimeOut_s;

typedef void*(*CheckTimeOutCallFunc)(TimeOut_s* param);

OCDoHandle GetRequestHandle(void);

int CASRequestHandle(OCDoHandle EqualVal, OCDoHandle writeHandleVal);

bool isEmptyRequestHandle(void);

OCDoHandle AddRequestHandle(OCDoHandle HandleVal);

void DelAllRequestHandle(void);

long long nextTimeOut( const long long ori_num , const long long cur_num );  // PDF Gaussian Distribution Time-Out Calculator.

unsigned long int CreateTimeOutProcess( OCDoHandle hadler, CheckTimeOutCallFunc CallFunc, TimeOut_s* timeOut );


#endif /* OICWAITHANDLER_H_ */
