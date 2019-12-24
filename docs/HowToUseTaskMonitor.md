# How to use Task Monitor

Task Monitor is a kernel thread which monitors the tasks/pthreads.  
It checks whether registered tasks/pthreads are alive.  

If any task/pthread does not update its active status within registered interval, then Task Monitor will reset the board.

## Contents
- [Enable Task Monitor](#enable-task-monitor)
- [Use Task Monitor](#use-task-monitor)


## Enable Task Monitor
Please find four steps to enable Task monitor as shown below:

### 1. Enable CONFIG TASK MONITOR

In menuconfig, enable **CONFIG_TASK_MONITOR**.  
```
Task Monitor -> Enable Task Monitor
```

### 2. Set priority of Task Monitor

In menuconfig, Set **CONFIG_TASK_MONITOR_PRIORITY** value which indicates priority of Task Monitor.  
```
Task Monitor -> Priority of Task Monitor
```

We recommend setting the Task Monitor's priority higher than the registered task/pthread.  
Otherwise, the Task Monitor may not work.  

For example, if a task/pthread has a priority of 100 and a Task Monitor has a priority of 90, then the Task Monitor may not get scheduled and the deadlock can persist.

### 3. Set interval for checking alive(sec)

In menuconfig, Set **CONFIG_TASK_MONITOR_INTERVAL** value which indicates interval for checking alive(sec)  
```
Task Monitor -> Interval for checking alive(sec) -> change a number over 0
```

Task Monitor checks the status of registered tasks/pthreads at every this interval.  

For instance if user set CONFIG_TASK_MONITOR_INTERVAL to 5 seconds and registers a task/pthread with interval 7 seconds, then the Task Monitor checks the status of the registered task/pthread after 10 seconds instead of 7 seconds.
	
### 4. Set max interval for checking alive(sec)

In menuconfig, Set **CONFIG_TASK_MONITOR_MAX_INTERVAL** value which indicates max interval for checking alive(sec)  
```
Task Monitor -> Max interval for checking alive(sec) -> change a number over CONFIG_TASK_MONITOR_INTERVAL
```

It is maximum interval that a user can register and should be set to a multiple of TASK_MONITOR_INTERVAL.  
If it is not a multiple of TASK_MONITOR_INTERVAL, it is set to the previous multiple.  

For example, If TASK_MONITOR_INTERVAL set by 5 seconds and TASK_MONITOR_MAX_INTERVAL set by 17 seconds, then TASK_MONITOR_MAX_INTERVAL will be changed 15 seconds.

## Use Task Monitor
Please find an explanation of APIs to use Task Monitor as shown below:

### Supported APIs
A header file [task_monitor.h](../os/include/tinyara/task_monitor.h) provides following APIs which support to Task Monitor management as shown below:
```
int task_monitor_register(int interval);
void task_monitor_update_status(void);
```

There are two APIs that register with the Task Monitor and update the status of the task/pthread.  

**task_monitor_register(int interval)** function registers current task/pthread with specific interval.  
**task_monitor_update_status(void)** function updates the active status of the current task/pthread.

### Example of Task Monitor
User can use task_monitor_register(INTERVAL) to register task/pthread and update its status using task_monitor_update_status() before the time interval expired.
```
#include <tinyara/task_monitor.h>
#define INTERVAL 5
int main
{
    task_monitor_register(INTERVAL);
    ...
    task_monitor_update_status();
    ...
}
```
