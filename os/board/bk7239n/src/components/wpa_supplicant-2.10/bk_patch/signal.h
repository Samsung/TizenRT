#ifndef _SIGNAL_H_
#define _SIGNAL_H_

typedef void (*SIG_FUNC)(int);

#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGEMT		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGBUS		10
#define SIGSEGV		11
#define SIGSYS		12
#define SIGPIPE		13
#ifndef SIGALRM
#define SIGALRM		14
#endif
#ifndef SIGTERM
#define SIGTERM		15
#endif
#ifndef SIGURG
#define SIGURG		16
#endif
#ifndef SIGSTOP
#define SIGSTOP		17
#endif
#ifndef SIGTSTP
#define SIGTSTP		18
#endif
#ifndef SIGCONT
#define SIGCONT		19
#endif
#ifndef SIGCHLD
#define SIGCHLD		20
#endif
#ifndef SIGTTIN
#define SIGTTIN		21
#endif
#ifndef SIGTTOU
#define SIGTTOU		22
#endif
#ifndef SIGIO
#define SIGIO		23
#endif
#ifndef SIGXCPU
#define SIGXCPU		24
#endif
#ifndef SIGXFSZ
#define SIGXFSZ		25
#endif
#ifndef SIGVTALRM
#define SIGVTALRM	26
#endif
#ifndef SIGPROF
#define SIGPROF		27
#endif
#ifndef SIGWINCH
#define SIGWINCH	28
#endif
#ifndef SIGINFO
#define SIGINFO		29
#endif
#ifndef SIGUSR1
#define SIGUSR1		30
#endif
#ifndef SIGUSR2
#define SIGUSR2		31
#endif

#define SIGPOLL	    SIGIO
#define SIGPWR	    SIGINFO
#define SIGIOT	    SIGABRT

extern void bk_signal(int sig_num, SIG_FUNC func);
unsigned int bk_alarm(unsigned int seconds);

#endif
// eof

