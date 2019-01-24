#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "tiny_evtqueue.h"
#include "tiny_test_utils.h"

TQ_DECLARE;


#define TEQ_TEST_ENTRY													\
	do {																\
		printf("\n=================================================\n"); \
		printf("%s\n", __FUNCTION__);									\
		printf("=================================================\n");	\
	} while (0)															\


void *
delete_func(teq_node *msg)
{
	int *addr = (int *)msg->data;
	printf("[TEQTC] delete item(%d)\n", *addr);
	free(addr);
}

static void *
thr_get_message(void *arg)
{
	tem_msg_queue q1 = (tem_msg_queue)arg;

	teq_node *msg;
	TQ_CHECK_RESULT(tq_pop_queue(q1, &msg, 0));

	int *data = (int *)msg->data;
	printf("[TEQTC] get data (%d)\n", *data);

	free(data);

	return 0;
}

static void *
thr_message_looper(void *arg)
{
	tem_msg_queue q = (tem_msg_queue)arg;
	while (1) {
		teq_node *msg;
		tq_result res = tq_pop_queue(q, &msg, 0);
		if (res != TQ_SUCCESS) {
			printf("[TEQTC] error pop(%d)\n", res);
			break;
		}
		int *data = (int *)msg->data;
		printf("[TEQTC] thr_msg_looper: get (%d)\n", *data);
	}
	return 0;
}


static int
test_evt_queue_basic(void)
{
	TEQ_TEST_ENTRY;
	tem_msg_queue q1;
	printf("[TEQTC] create queue\n");
	TQ_CHECK_RESULT(tq_init_queue(&q1, delete_func));

	printf("[TEQTC] make a message\n");
	teq_node msg;
	int *data = (int *)malloc(sizeof(int));
	*data = 3;
	msg.data = data;

	printf("[TEQTC] push a message to a queue\n");
	TQ_CHECK_RESULT(tq_push_queue(q1, &msg, 0));

	printf("[TEQTC] get a message from a queue\n");
	teq_node *fmsg;
	TQ_CHECK_RESULT(tq_pop_queue(q1, &fmsg, 0));

	printf("[TEQTC] close a queue\n");
	TQ_CHECK_RESULT(tq_deinit_queue(q1));

	return 0;
}

static int
test_evt_queue_multi_task(void)
{
	TEQ_TEST_ENTRY;
	pthread_t t1, t2;

	tem_msg_queue q1;
	TQ_CHECK_RESULT(tq_init_queue(&q1, delete_func));

	teq_node msg1;
	int *data1 = (int *)malloc(sizeof(int));
	if(!data1) {
		TQ_TEST_ERR;
	}
	*data1 = 1234;
	msg1.data = data1;

	teq_node msg2;
	int *data2 = (int *)malloc(sizeof(int));
	if(!data2) {
		TQ_TEST_ERR;
	}
	*data2 = 5678;
	msg2.data = data2;

	TQ_CHECK_RESULT(pthread_create(&t1, 0, thr_get_message, (void *)q1));

	TQ_CHECK_RESULT(pthread_create(&t2, 0, thr_get_message, (void *)q1));

	TQ_CHECK_RESULT(tq_push_queue(q1, &msg1, 0));

	TQ_CHECK_RESULT(tq_push_queue(q1, &msg2, 0));

	TQ_CHECK_RESULT(pthread_join(t1, 0));

	TQ_CHECK_RESULT(pthread_join(t2, 0));

	return 0;
}

static int
test_evt_queue_full(void)
{
	// to be
	return 0;
}

static int
test_evt_queue_empty(void)
{
	TEQ_TEST_ENTRY;
	pthread_t t1, t2;

	tem_msg_queue q1;
	TQ_CHECK_RESULT(tq_init_queue(&q1, delete_func));

	TQ_CHECK_RESULT(pthread_create(&t1, 0, thr_get_message, (void *)q1));

	TQ_CHECK_RESULT(pthread_create(&t2, 0, thr_get_message, (void *)q1));

	printf("Check that those 2 threads are running and waiting items.\n");
	printf("To give time to start waiting to those threads, sleep 3 seconds\n");
	printf("before main thread pushes items to the queue\n");

	sleep(1);

	teq_node msg1;
	int *data1 = (int *)malloc(sizeof(int));
	if(!data1) {
		TQ_TEST_ERR;
	}
	*data1 = 1234;
	msg1.data = data1;

	teq_node msg2;
	int *data2 = (int *)malloc(sizeof(int));
	if(!data2) {
		TQ_TEST_ERR;
	}
	*data2 = 5678;
	msg2.data = data2;

	TQ_CHECK_RESULT(tq_push_queue(q1, &msg1, 0));

	TQ_CHECK_RESULT(tq_push_queue(q1, &msg2, 0));

	TQ_CHECK_RESULT(pthread_join(t1, 0));

	TQ_CHECK_RESULT(pthread_join(t2, 0));

	TQ_CHECK_RESULT(tq_deinit_queue(q1));

	return 0;
}

static int
test_evt_queue_timedwait(void)
{
	TEQ_TEST_ENTRY;

	struct timeval start, end;
	tem_msg_queue queue;
	TQ_CHECK_RESULT(tq_init_queue(&queue, delete_func));

	teq_node msg;
	int data = 5555;
	msg.data = &data;

	TQ_CHECK_RESULT(tq_push_queue(queue, &msg, 3));

	gettimeofday(&start, 0);

	teq_node *fmsg;
	TQ_CHECK_RESULT(tq_pop_queue(queue,&fmsg,0));

	gettimeofday(&end, 0);

	int *pdata= fmsg->data;
	printf("[TEQTC] elapsed time (%lu), data(%d)\n", end.tv_sec - start.tv_sec, *pdata);

	return 0;
}

static int
test_evt_queue_terminate_queue_remained_data(void)
{
	TEQ_TEST_ENTRY;

	tem_msg_queue q1;
	TQ_CHECK_RESULT(tq_init_queue(&q1, delete_func));

	teq_node msg;
	int *data = (int *)malloc(sizeof(int));
	*data = 3;
	msg.data = data;

	TQ_CHECK_RESULT(tq_push_queue(q1, &msg, 0));

	teq_node msg2;
	int *data2 = (int *)malloc(sizeof(int));
	*data2 = 18;
	msg2.data = data2;

	TQ_CHECK_RESULT(tq_push_queue(q1, &msg2, 0));

	TQ_CHECK_RESULT(tq_deinit_queue(q1));

	return 0;
}

/*
 * waiting thread can receive deinit message by return value of pop()
 * thr_message_looper should be terminated when main task calls deinit()
 */
static int
test_evt_queue_terminate_queue_while_wait(void)
{
	TEQ_TEST_ENTRY;

	tem_msg_queue q1;
	TQ_CHECK_RESULT(tq_init_queue(&q1, delete_func));

    // create thread
	pthread_t tid;
	TQ_CHECK_RESULT(pthread_create(&tid, 0, thr_message_looper, (void *)q1));

	teq_node msg;
	int *data = (int *)malloc(sizeof(int));
	*data = 3;
	msg.data = data;

	TQ_CHECK_RESULT(tq_push_queue(q1, &msg, 0));

	teq_node msg2;
	int *data2 = (int *)malloc(sizeof(int));
	*data2 = 18;
	msg2.data = data2;
	TQ_CHECK_RESULT(tq_push_queue(q1, &msg2, 0));

	sleep(1); // give time to start thr_message_looper

	TQ_CHECK_RESULT(tq_deinit_queue(q1));

    // wait thread is terminated.
	TQ_CHECK_RESULT(pthread_join(tid, 0));

	return 0;
}


int
test_evt_queue(void)
{
	TQ_INITIALIZE();
	TQ_CHECK_RESULT(test_evt_queue_basic());
	TQ_CHECK_RESULT(test_evt_queue_multi_task());
	TQ_CHECK_RESULT(test_evt_queue_full());
	TQ_CHECK_RESULT(test_evt_queue_empty());
	TQ_CHECK_RESULT(test_evt_queue_timedwait());
	TQ_CHECK_RESULT(test_evt_queue_terminate_queue_remained_data());
	TQ_CHECK_RESULT(test_evt_queue_terminate_queue_while_wait());

	TQ_RESULT();

	return 0;
}
