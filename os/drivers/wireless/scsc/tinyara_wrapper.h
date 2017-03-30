#ifndef __NUTTX_WRAPPER_H__
#define __NUTTX_WRAPPER_H__

#define ERESTARTSYS                     (512)

struct completion {
	sem_t sem;
};

unsigned long wait_for_completion_timeout(struct completion *x, unsigned long timeout_ms);
void wait_for_completion(struct completion *x);
int wait_for_completion_interruptible(struct completion *x);
void complete(struct completion *x);
void complete_all(struct completion *x);
void init_completion(struct completion *x);
void reinit_completion(struct completion *x);
void *copy_to_user(void *dest, const void *src, size_t n);
void *copy_from_user(void *dest, const void *src, size_t n);
#endif
