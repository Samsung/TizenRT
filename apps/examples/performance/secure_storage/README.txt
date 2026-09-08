examples/performance/secure_storage
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  usage:
    perf_ss read 100
    perf_ss write 100
    perf_ss erase 100
    perf_ss composite 100
    perf_ss all 100

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_SS_PERFORMANCE_TEST

  Depends on:
  * CONFIG_SE

  The benchmark uses secure-storage slot 5 and a 4000-byte payload. Slot 5
  is deleted and rewritten during the test, so reserve it for benchmarking.
  The slot reset needed between write measurements is intentionally outside
  the timed region. The composite test measures write -> read -> erase as one
  operation.
