

# Pthread Cancellation and Cleanup Stress Test Suite

## Overview

This test suite provides comprehensive stress tests for pthread cancellation and cleanup functionality in TizenRT. It is designed to work with both **FLAT** and **LOADABLE** builds.

## Configuration

### Required Options

Enable the test suite in your defconfig:

```
CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST=y
CONFIG_PTHREAD_CLEANUP=y
CONFIG_CANCELLATION_POINTS=y
```

### Optional Configuration

```
# Stack size for test threads (default: 8192)
CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_STACKSIZE=8192

# Number of threads for stress tests (default: 10, range: 1-100)
CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_THREAD_COUNT=10

# Nesting depth for stress tests (default: 50, range: 1-500)
CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_NESTING_DEPTH=50

# Iterations per stress test (default: 1000)
CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_ITERATIONS=1000

# Number of test loops (default: 1, 0 = infinite)
CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_LOOPS=1
```

## Test Categories

### 1. Basic Functionality Tests (TC01-TC05)

| Test ID | Description |
|---------|-------------|
| TC01 | Single cleanup handler with pthread_exit |
| TC02 | Multiple cleanup handlers with pthread_exit |
| TC03 | Cleanup handler with pthread_cleanup_pop(execute=1) |
| TC04 | Cleanup handler with pthread_cleanup_pop(execute=0) |
| TC05 | Nested push/pop pairs |

### 2. Cancellation Tests (TC06-TC11)

| Test ID | Description |
|---------|-------------|
| TC06 | Deferred cancellation at cancellation points |
| TC07 | Asynchronous cancellation |
| TC08 | Cancellation with cleanup handlers |
| TC09 | Cancel disabled thread |
| TC10 | Cancel enabled/disabled state transitions |
| TC11 | Detached thread cancellation |

### 3. Cancellation Point Tests (TC12-TC16)

| Test ID | Description |
|---------|-------------|
| TC12 | Cancellation during pthread_cond_wait |
| TC13 | Cancellation during pthread_join |
| TC14 | Cancellation during sem_wait |
| TC15 | Cancellation during sleep/usleep |
| TC16 | Multiple cancellation points in sequence |

### 4. Stress Tests (TC17-TC21)

| Test ID | Description |
|---------|-------------|
| TC17 | High-frequency push/pop (1000+ iterations) |
| TC18 | Multiple threads (10+) with cleanup handlers |
| TC19 | Deep nesting (50+ levels) |
| TC20 | Rapid create/cancel cycles |
| TC21 | Long-running cleanup handlers |

### 5. Resource Cleanup Tests (TC22-TC27)

| Test ID | Description |
|---------|-------------|
| TC22 | Memory allocation/deallocation cleanup |
| TC23 | Mutex lock/unlock cleanup |
| TC24 | Semaphore post/wait cleanup |
| TC25 | File descriptor close cleanup |
| TC26 | Multiple resource types in single handler |
| TC27 | Cleanup handler ordering verification |

### 6. Edge Case Tests (TC28-TC33)

| Test ID | Description |
|---------|-------------|
| TC28 | NULL argument to cleanup handler |
| TC29 | Cleanup handler that calls pthread_exit |
| TC30 | Cleanup during thread cancellation |
| TC31 | Pop without matching push (error case) |
| TC32 | Cleanup with CANCEL_ASYNCHRONOUS type |
| TC33 | Mixed cancellation types in same thread |

## Usage

### Running the Tests

1. **Build TizenRT** with the stress test enabled:
   ```bash
   cd TizenRT-master
   make
   ```

2. **Run the tests** from the TASH shell:
   ```
   pthread_cleanup> h    # Show help menu
   pthread_cleanup> 1    # Run specific test (e.g., TC01)
   pthread_cleanup> b    # Run basic tests (TC01-TC05)
   pthread_cleanup> c    # Run cancellation tests (TC06-TC11)
   pthread_cleanup> p    # Run cancellation point tests (TC12-TC16)
   pthread_cleanup> s    # Run stress tests (TC17-TC21)
   pthread_cleanup> r    # Run resource cleanup tests (TC22-TC27)
   pthread_cleanup> e    # Run edge case tests (TC28-TC33)
   pthread_cleanup> a    # Run all tests
   pthread_cleanup> q    # Quit
   ```

### Test Output Format

```
[TEST] TC01: Single cleanup handler with pthread_exit
[INFO] Starting test...
[INFO] Cleanup handler called with arg=1
[PASS] Test completed successfully
```

Or on failure:
```
[TEST] TC02: Multiple cleanup handlers
[INFO] Starting test...
[FAIL] Expected 5 cleanup handlers, got 3
```

## Building for Different Configurations

### Flat Build
```bash
cd build/configs/<your_board>
# Ensure CONFIG_BUILD_FLAT=y
make
```

### Loadable Build (Binary Separation)
```bash
cd build/configs/<your_board>
# Ensure CONFIG_BUILD_2PASS=y and CONFIG_APP_BINARY_SEPARATION=y
make
```

## Architecture

The test suite uses a modular design:

```
pthread_cleanup_test/
├── Kconfig              # Configuration options
├── Kconfig_ENTRY        # Entry point configuration
├── Make.defs            # Build configuration
├── Makefile             # Make rules
├── pthread_cleanup.h     # Common header
├── pthread_cleanup_main.c # Main entry with menu
├── test_basic.c         # Basic functionality tests
├── test_cancel.c        # Cancellation tests
├── test_cancel_points.c # Cancellation point tests
├── test_stress.c        # Stress tests
├── test_resources.c     # Resource cleanup tests
├── test_edge_cases.c    # Edge case tests
└── README.md            # This file
```

## Notes

1. **CONFIG_PTHREAD_CLEANUP**: Required for cleanup handler tests. When disabled, tests are skipped gracefully.

2. **CONFIG_CANCELLATION_POINTS**: Required for cancellation tests. When disabled, cancellation-related tests are skipped.

3. **Stack Size**: Some stress tests (especially deep nesting) may require larger stack sizes. Increase `CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_STACKSIZE` if tests fail with stack overflow.

4. **Loadable Builds**: The test suite uses syscall wrappers automatically when running in loadable mode. No special configuration is needed.

## Troubleshooting

### Test crashes or hangs
- Increase stack size: `CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_STACKSIZE=16384`
- Reduce nesting depth: `CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_NESTING_DEPTH=10`
- Reduce thread count: `CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST_THREAD_COUNT=5`

### Cleanup handlers not called
- Verify `CONFIG_PTHREAD_CLEANUP=y` is set
- Check that pthread_cleanup_push/pop are properly paired
- Ensure pthread_exit or pthread_cancel is used (not return)

### Cancellation not working
- Verify `CONFIG_CANCELLATION_POINTS=y` is set
- Check that the thread is at a cancellation point
- Verify cancellation state is enabled (not disabled)

## License

Copyright (C) 2026 Samsung Electronics. All rights reserved.

Licensed under the Apache License, Version 2.0.
