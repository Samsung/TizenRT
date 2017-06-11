/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * aws/sample/TizenRT/aws_sample_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <apps/shell/tash.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
struct pthread_arg
{
    int argc;
    char **argv;
};

extern int aws_shadow(int argc, char **argv);
extern int aws_shadow_console_echo(int argc, char **argv);
extern int aws_subscribe_publish_cpp(int argc, char **argv);
extern int aws_subscribe_publish_library(int argc, char **argv);
extern int aws_subscribe_publish(int argc, char **argv);

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int aws_sample_cb(void *args)
{
    int argc;
    char **argv;
    int num = 0;

    argc = ((struct pthread_arg *)args)->argc;
    argv = ((struct pthread_arg *)args)->argv;

    if( argc == 1 )
    {
        printf("usage : aws_sample <example_no> \n");
        printf("    ex) aws_sample 5 ==> runs aws_susbcribe_publish\n\n");
        printf("    1. aws_shadow\n");
        printf("    2. aws_shadow_console_echo\n");
        printf("    3. aws_subscribe_publish_cpp\n");
        printf("    4. aws_subscribe_publish_library\n");
        printf("    5. aws_subscribe_publish\n");
    }
    else
    {
        num = atoi(argv[1]);

        switch(num)
        {
            case 1:
                aws_shadow(0, NULL);
                break;
            case 2:
                aws_shadow_console_echo(0, NULL);
                break;
            case 3:
                aws_subscribe_publish_cpp(0, NULL);
                break;
            case 4:
                aws_subscribe_publish_library(0, NULL);
                break;
            case 5:
                aws_subscribe_publish(0, NULL);
                break;
            default:
                printf("Wrong number : %d\n",num);
                break;
        }
    }

    return 0;
}

/****************************************************************************
 * aws_sample_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int aws_sample_main(int argc, char **argv)
#endif
{
    pthread_t tid;
    pthread_attr_t attr;
    //struct sched_param sparam;
    int ret;

    struct pthread_arg args;
    args.argc = argc;
    args.argv = argv;


    /* Initialize the attribute variable */
    if ((ret = pthread_attr_init(&attr)) != 0)
        printf("%s: pthread_attr_init failed, status=%d\n", __func__, ret);

    /* set a stacksize */
    if ((ret = pthread_attr_setstacksize(&attr, CONFIG_EXAMPLES_AWS_STACKSIZE)) != 0)
        printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, ret);

    /* create pthread with entry function */
    if ((ret = pthread_create(&tid, &attr,
                            (pthread_startroutine_t)aws_sample_cb, (void *)&args)) != 0)
        printf("%s: pthread_create failed, status=%d\n", __func__, ret);

    /* Wait for the threads to stop */
    pthread_join(tid, NULL);


    return 0;
}
