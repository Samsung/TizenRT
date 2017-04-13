examples/kernel_sample
^^^^^^^^^^^^^^^^^^^^^^

  This is the TinyAra 'qualification' suite.  It attempts to exercise
  a broad set of OS functionality.  Its coverage is not very extensive
  as of this writing, but it is used to qualify each TinyAra release.

  The behavior of the kernel_sample can be modified with the following
  settings.

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_LOOPS
      Used to control the number of executions of the test.  If
      undefined, the test executes one time.  If defined to be
      zero, the test runs forever.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_STACKSIZE
      Used to create the kernel_sample task.  Default is 8192.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_NBARRIER_THREADS
      Specifies the number of threads to create in the barrier
      test.  The default is 8 but a smaller number may be needed on
      systems without sufficient memory to start so many threads.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_AIO
      Enables test of asynchronous I/O.  You must have a mounted, writable
		  directory in place to use this test.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_AIOPATH
      This is the location of a directory in a mounted file system that
		  the AIO test can write into.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_RR_RANGE
      During round-robin scheduling test two threads are created. Each of the threads
      searches for prime numbers in the configurable range, doing that configurable
      number of times.
      This value specifies the end of search range and together with number of runs
      allows to configure the length of this test - it should last at least a few
      tens of seconds. Allowed values [1; 32767], default 10000
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_RR_RUNS
      During round-robin scheduling test two threads are created. Each of the threads
      searches for prime numbers in the configurable range, doing that configurable
      number of times.

