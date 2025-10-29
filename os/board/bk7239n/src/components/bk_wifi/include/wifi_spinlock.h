#ifndef __SPIN_LOCK_H_
#define __SPIN_LOCK_H_

/* dummy spinlock structure */
typedef struct {} arch_spinlock_t;

typedef struct raw_spinlock {
	arch_spinlock_t raw_lock;
} raw_spinlock_t;

typedef struct spinlock {
	union {
		struct raw_spinlock rlock;
	};
} spinlock_t;

#define spin_lock_init(_lock) do { } while (0)

#define spin_lock_bh(...) 		do {} while (0)
#define spin_unlock_bh(...)		do {} while (0)

#define spin_lock(a) 			do {(void)(a);} while (0)
#define spin_unlock(a)			do {(void)(a);} while (0)

#define spin_lock_irqsave(a, flags)			do {(void)(flags);} while (0)
#define spin_unlock_irqrestore(a, flags)	do {(void)(flags);} while (0)
#endif // __SPIN_LOCK_H_
