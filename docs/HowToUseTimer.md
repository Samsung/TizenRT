# How to use Timer

There are two types of timer, software and hardware timers.  
They have different behavior and different usages.  
Software timer can create unlimited number of timer if it has enough memory, but it is inaccurate.  
Hardware timer provides accurate and fine time but it has a limitation in number.

## Contents

- [Software Timers](#software-timers)  
- [Hardware Timers](#hardware-timers)

## Software Timers

There are three kinds of software timers, sleep, work queue and POSIX timer.  

They work with system tick and it caues inaccurate time resolution and response.  
- Resolution
There are the sleep functions for micro-second and for nano-second but because of the tick,  
the minimum time is 1 tick. TizenRT provides the config, CONFIG_USEC_PER_TICK, to set the timer per a tick.  
If CONFIG_USEC_PER_TICK is set with 10,000 (10 ms) and the given time is less than 10ms, it will awaken after 10ms.  
- Inaccurate
The sleep may be lengthened slightly by any system activity or by the time spent processing the call or  
by the granularity of system timers.

Software timer is good to use when some work could be run anytime after given time and it could be run many times at the same time.

### Sleep

There are three sleep functions, sleep, usleep and nanosleep.  
They make running thread suspended with given time, after that it comes back to scheduling and executes the next line.  
Only difference is the time resolution as shown below:  
- sleep in second  
- usleep in micro-second  
- nanosleep in nano-second

The sleep and nanosleep are POSIX-compatible APIs and usleep is the same as Linux.  
All of their prototypes are in *unistd.h*.

> **NOTE**
> 
> By POSIX, signal could awaken the sleep before given time with EINTR return value.

### Work Queue
<To be updated>

### POSIX Timer
<To be updated>

## Hardware Timers

Because chipset runs timer and measure the time by itself, it provides high resolution and works on time  
even if there are heavy operations in SW side. But because it is HW, it has a limitation in number.  
TizenRT provides the timer driver to keep the same usages of application with all of boards through VFS functionality.

### Driver

Because it uses VFS, timer is working with *open*, *read*, *write* and *ioctl* APIs.  
Timer driver is *os/drivers/timer.c* file. 

The way of Hardware timer is:
1. Open the timer device
```
int fd = open(<TIMER_DEVICE_PATH>, O_RDONLY);
```
2. Set the notification to get time expiration
```
struct timer_notify_s notify;
notify.arg   = NULL;                  /* An argument to pass with the FIN */
notify.pid   = (pid_t)getpid();       /* The ID of the task/thread to receive the FIN */

ioctl(fd, TCIOC_NOTIFICATION, &notify);
```
3. Set the time
```
ioctl(fd, TCIOC_SETTIMEOUT, time);
```
4. Start the timer
```
ioctl(fd, TCIOC_START, 0);
```
5. Wait the notification using *fin_wait*
```
fin_wait();
```
6. Stop the timer
```
ioctl(fd, TCIOC_STOP, 0);
```
7. Close the timer device
```
close(fd);
```

Please fine [the example of timer](https://github.com/Samsung/TizenRT/blob/master/apps/examples/timer/timer_main.c) in *apps/examples/timer*.
