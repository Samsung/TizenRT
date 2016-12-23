fs/procfs README
================

  This is a tiny procfs file system that allows read-only access to a few
  attributes of a task or thread.  This tiny procfs fs file system can be
  built into the system by enabling:

    CONFIG_FS_PROCFS=y

  It can then be mounted from the TASH command like like:

    TASH> mount -t procfs /proc

Example
=======

  TASH> mount -t procfs /proc

  TASH> ls /proc
  /proc:
   0/
   1/

  TASH> ls /proc/1
  /proc/1:
   status
   cmdline

  TASH> cat /proc/1/status
  Name:       init
  Type:       Task
  State:      Running
  Priority:   100
  Scheduler:  SCHED_FIFO
  SigMask:    00000000

  TASH> cat /proc/1/cmdline
  init

  TASH> sleep 100 &
  sleep [2:100]
  TASH> ls /proc
  ls /proc
  /proc:
   0/
   1/
   2/

  TASH> cat /proc/2/cmdline
  <pthread> 0x527420
