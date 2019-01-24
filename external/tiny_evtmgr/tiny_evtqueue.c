/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifdef __TIZENRT__
#include <tinyara/config.h>
#endif
#include <stdio.h>
#include <time.h>
#ifdef LINUX
#include <malloc.h>
#endif
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#ifdef LINUX
#include <unistd.h>
#include <assert.h>
#endif
#include "tiny_evtqueue.h"

#define TQ_SIG_TERM "tr"
#define TQ_SIG_NEWMSG "nm"
#define TQ_SIG_LEN 3

#ifdef TQ_ENABLE_LOG
#define TQ_LOG printf
#else
#define TQ_LOG(...)
#endif
#ifdef LINUX
#define TQ_ERR TQ_LOG("T%lx [ERR] %s\t%s:%d\n", pthread_self(), __FUNCTION__, __FILE__, __LINE__)
#define TQ_ENTRY TQ_LOG("T%lx --> %s\t%s:%d\n", pthread_self(), __FUNCTION__, __FILE__, __LINE__)
#define TQ_OUT TQ_LOG("T%lx <-- %s\t%s:%d\n", pthread_self(), __FUNCTION__, __FILE__, __LINE__)
#else
#define TQ_ERR TQ_LOG("T%x [ERR] %s\t%s:%d\n", getpid(), __FUNCTION__, __FILE__, __LINE__)
#define TQ_ENTRY TQ_LOG("T%x --> %s\t%s:%d\n", getpid(), __FUNCTION__, __FILE__, __LINE__)
#define TQ_OUT TQ_LOG("T%x <-- %s\t%s:%d\n", getpid(), __FUNCTION__, __FILE__, __LINE__)
#endif

#define TQ_ASSERT								\
	do {										\
		assert(0);								\
	} while (0)

#define TQ_LOCK(eq)								\
	do {										\
		pthread_mutex_lock(&eq->lock);			\
	} while (0)

#define TQ_UNLOCK(eq)							\
	do {										\
		pthread_mutex_unlock(&eq->lock);		\
	} while (0)

#define TQ_GET_WAITTIME(cur, wake)										\
	(wake.tv_usec > cur.tv_usec ? (wake.tv_usec - cur.tv_sec) : (1000000 + wake.tv_usec - cur.tv_usec))

#define TQ_RETURN(res)							\
	do {										\
		if (res != 0) {							\
			TQ_ERR;								\
		} else {								\
			TQ_OUT;								\
		}										\
		return res;								\
	} while (0)

#define TQ_ISSET(tmp) ((tmp->time.tv_sec > 0) || (tmp->time.tv_usec > 0))

#define TQ_SEND_SIGNAL(eq)						\
	do {										\
		pthread_mutex_lock(&eq->lock);			\
		pthread_cond_signal(&eq->cond);			\
		pthread_mutex_unlock(&eq->lock);		\
	} while (0)

#define TQ_WAIT_SIGNAL(eq)							\
	do {											\
		pthread_cond_wait(&eq->cond, &eq->lock);	\
	} while (0)

#define TQ_SET_STATE(eq, s)						\
	do {										\
		eq->state = s;							\
	} while (0)

struct tiny_qlist {
	struct tiny_qlist *next;
	struct tiny_qlist *prev;
	struct timeval time;
	teq_node *item;
};

typedef enum _tiny_evtq_state {
	TQ_STATE_RUN,
	TQ_STATE_WAIT,
	TQ_STATE_TERMINATING,
} tiny_evtq_state;

struct tiny_evtq {
	struct tiny_qlist *front;
	struct tiny_qlist *rear;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	int fds[2];
	tiny_evtq_state state;
	tq_delete_item rfunc;
};

/**
 * Internal Functions
 */
void _tq_add_queue_front(struct tiny_evtq *eq, struct tiny_qlist *it);
void _tq_rem_queue(struct tiny_evtq *eq, struct tiny_qlist *it);
void _tq_add_queue_next(struct tiny_evtq *eq, struct tiny_qlist *n1, struct tiny_qlist *n2);
int _tq_create_item(struct tiny_qlist **item, uint16_t delay);
int _tq_send_signal(struct tiny_evtq *eq, const char *type);
int _tq_calc_time(struct timeval *wait, struct timeval *t1, struct timeval *t2);

int _tq_create_item(struct tiny_qlist **item, uint16_t delay)
{
	struct tiny_qlist *it = (struct tiny_qlist *)malloc(sizeof(struct tiny_qlist));
	if (!it) {
		return -1;
	}

	it->prev = it->next = 0;
	it->item = 0;

	if (delay != 0) {
		if (gettimeofday(&it->time, 0) == -1) {
			TQ_ASSERT;
		}
		it->time.tv_sec += delay;
	} else {
		it->time.tv_sec = it->time.tv_usec = 0;
	}

	*item = it;

	return 0;
}

void _tq_add_queue_front(struct tiny_evtq *eq, struct tiny_qlist *it)
{
	it->next = eq->front;
	it->prev = 0;
	eq->front = it;
}

void _tq_add_queue_next(struct tiny_evtq *eq, struct tiny_qlist *n1, struct tiny_qlist *n2)
{
	if (n1->next == NULL) {
		eq->rear = n2;
	}
	n2->next = n1->next;
	n1->next = n2;
	n2->prev = n1;
}

void _tq_rem_queue(struct tiny_evtq *eq, struct tiny_qlist *it)
{
	struct tiny_qlist *prev = it->prev;
	struct tiny_qlist *next = it->next;
	if (prev) {
		prev->next = next;
	} else {
		eq->front = next;
	}

	if (next) {
		next->prev = prev;
	} else {
		eq->rear = prev;
	}

	it->prev = it->next = NULL;
	free(it);
}

int _tq_send_signal(struct tiny_evtq *eq, const char *type)
{
	TQ_ENTRY;
	if (!eq) {
		TQ_RETURN(-1);
	}
	int res = write(eq->fds[1], type, TQ_SIG_LEN);
	if (res < 0) {
		TQ_RETURN(-1);
	}
	TQ_RETURN(0);
}

/*
 * Return
 * -1 : time exceed
 *  0 : time left
 */
int _tq_calc_time(struct timeval *wait, struct timeval *t1, struct timeval *t2)
{
	if (t1->tv_usec > t2->tv_usec) {
		wait->tv_usec = (1000000 + t2->tv_usec) - t1->tv_usec;
		wait->tv_sec = t2->tv_sec - 1 - t1->tv_sec;
	} else {
		wait->tv_usec = (t2->tv_usec) - t1->tv_usec;
		wait->tv_sec = t2->tv_sec - t1->tv_sec;
	}

	if ((int)wait->tv_sec < 0) {
		return -1;
	}

	return 0;
}

/*
 * Circular Queue
 */
/*
 * Public API
 */
tq_result tq_init_queue(tem_msg_queue *queue, tq_delete_item func)
{
	TQ_ENTRY;

	struct tiny_evtq *eq = (struct tiny_evtq *)malloc(sizeof(struct tiny_evtq));
	if (!eq) {
		TQ_RETURN(TQ_NOMEM);
	}

	eq->front = eq->rear = NULL;

	int res = pthread_mutex_init(&eq->lock, 0);
	if (res != 0) {
		free(eq);
		TQ_RETURN(TQ_RSCBUSY);
	}

	res = pthread_cond_init(&eq->cond, 0);
	if (res != 0) {
		free(eq);
		if (pthread_mutex_destroy(&eq->lock) != 0) {
			TQ_ERR;
		}
		TQ_RETURN(TQ_RSCBUSY);
	}
	res = pipe(eq->fds);
	if (res != 0) {
		if (pthread_mutex_destroy(&eq->lock) != 0) {
			TQ_ERR;
		}
		if (pthread_cond_destroy(&eq->cond) != 0) {
			TQ_ERR;
		}
		free(eq);
		TQ_RETURN(TQ_RSCBUSY);
	}
	eq->rfunc = func;
	eq->state = TQ_STATE_RUN;
	*queue = eq;
	TQ_RETURN(0);
}

tq_result tq_deinit_queue(tem_msg_queue queue)
{
	TQ_ENTRY;
	if (!queue) {
		TQ_RETURN(TQ_INVALID);
	}
	struct tiny_evtq *eq = queue;

	// if threads that is waiting a message are many.
	// then _tq_send_signal should be changed to broadcast.
	TQ_LOCK(eq);
	if (eq->state == TQ_STATE_TERMINATING) {
		TQ_UNLOCK(eq);
	}
	if (eq->state == TQ_STATE_WAIT) {
		TQ_SET_STATE(eq, TQ_STATE_TERMINATING);
		_tq_send_signal(eq, TQ_SIG_TERM);
		TQ_WAIT_SIGNAL(eq);
	}

	struct tiny_qlist *it = eq->front;
	while (it) {
		eq->rfunc((void *)it->item);
		struct tiny_qlist *tmp = it->next;
		it->prev = it->next = 0;
		free(it);
		it = tmp;
	}
	TQ_UNLOCK(eq);

	int res = pthread_mutex_destroy(&eq->lock);
	if (res != 0) {
		TQ_RETURN(TQ_RSCBUSY);
	}

	free(eq);

	TQ_RETURN(0);
}

tq_result tq_pop_queue(tem_msg_queue queue, teq_node **item, int flags)
{
	TQ_ENTRY;
	struct tiny_evtq *eq = queue;
	struct tiny_qlist *it = NULL;

	if (!eq) {
		TQ_RETURN(TQ_INVALID);
	}

	TQ_LOCK(eq);
	if (flags == TQ_DONTWAIT) {
		if (!eq->front) {
			TQ_RETURN(TQ_AGAIN);
		}
		if (eq->front->time.tv_sec == 0) {
			it = eq->front;
			*item = it->item;
			_tq_rem_queue(eq, it);
			TQ_UNLOCK(eq);
			TQ_RETURN(0);
		}
		TQ_UNLOCK(eq);
		TQ_RETURN(TQ_TIMEWAIT);
	}

	char buf[3] = { 0 };
	fd_set ofds, rfds;
	FD_ZERO(&ofds);
	FD_SET(eq->fds[0], &ofds);
	int max_fd = eq->fds[0] + 1;
	TQ_SET_STATE(eq, TQ_STATE_WAIT);
	TQ_UNLOCK(eq);

	while (1) {
		struct timeval *wait = 0;
		struct timeval wait_time, cur_time;

		rfds = ofds;
		TQ_LOCK(eq);
		if (!eq->front) {
			wait = NULL;
		} else {
			it = eq->front;

			if (it->time.tv_sec == 0 && it->time.tv_usec == 0) {
				// non-delayed message
				*item = it->item;
				_tq_rem_queue(eq, it);
				TQ_SET_STATE(eq, TQ_STATE_RUN);
				TQ_UNLOCK(eq);
				TQ_RETURN(0);
			}

			gettimeofday(&cur_time, 0);
			int chk = _tq_calc_time(&wait_time, &cur_time, &it->time);
			if (chk < 0) {
				*item = it->item;
				_tq_rem_queue(eq, it);
				TQ_SET_STATE(eq, TQ_STATE_RUN);
				TQ_UNLOCK(eq);
				TQ_RETURN(0);
			}
			wait = &wait_time;
		}
		TQ_UNLOCK(eq);

		int res = select(max_fd, &rfds, 0, 0, wait);

		if (res < 0) {
			TQ_RETURN(TQ_UNKNOWN);
		}
		if (res == 0) {
			TQ_LOG("time out\n");
			TQ_LOCK(eq);
			*item = it->item;
			_tq_rem_queue(eq, it);
			//free(it);
			TQ_SET_STATE(eq, TQ_STATE_RUN);
			TQ_UNLOCK(eq);
			TQ_RETURN(0);
		}

		if (FD_ISSET(eq->fds[0], &rfds)) {
			int len = read(eq->fds[0], buf, 3);
			if (len != 3) {
				TQ_RETURN(TQ_UNKNOWN);
			}
			if (strcmp(buf, TQ_SIG_TERM) == 0) {
				// receive terminate signal
				TQ_LOG("receive terminate message\n");
				TQ_SEND_SIGNAL(eq);
				TQ_RETURN(TQ_NOTAVAIL);
			} else if (strcmp(buf, TQ_SIG_NEWMSG) == 0) {
				// new msg is coming. so calculate queue again.
				TQ_LOG("receive new incoming message\n");
				continue;
			}
		}
	}
	TQ_RETURN(0);
}

tq_result tq_push_queue(tem_msg_queue queue, teq_node *item, uint16_t delay)
{
	TQ_ENTRY;

	struct tiny_evtq *eq = queue;

	TQ_LOCK(eq);

	struct tiny_qlist *it = NULL;
	int res = _tq_create_item(&it, delay);
	if (res != 0) {
		TQ_RETURN(TQ_NOMEM);
	}

	it->item = item;

	if (!eq->front) {
		eq->front = it;
		eq->rear = it;
	} else {
		struct tiny_qlist *tmp = eq->rear;
		if (delay == 0) {
			for (; tmp->prev && TQ_ISSET(tmp); tmp = tmp->prev) ;
			if (!tmp) {
				_tq_add_queue_front(eq, it);
			} else {
				_tq_add_queue_next(eq, tmp, it);
			}
		} else {
			for (; tmp && TQ_ISSET(tmp); tmp = tmp->prev) {
				if (tmp->time.tv_sec > it->time.tv_sec) {
					continue;
				} else {
					_tq_add_queue_next(eq, tmp, it);
					break;
				}
			}
			if (!tmp) {
				_tq_add_queue_front(eq, it);
			}
		}
	}
	_tq_send_signal(eq, TQ_SIG_NEWMSG);

	TQ_UNLOCK(eq);

	TQ_RETURN(0);
}

teq_node *tq_create_node(void *arg)
{
	TQ_ENTRY;
	teq_node *node = (teq_node *)malloc(sizeof(teq_node));
	if (!node) {
		TQ_ERR;
		return 0;
	}
	node->data = arg;

	TQ_OUT;
	return node;
}
