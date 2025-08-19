# Testing Strategy for Memory Management Changes

This document outlines the testing strategy for the recent changes to the memory management system, specifically related to the delayed deallocation mechanism and the background free worker.

## 1. Unit Tests

*   **Objective**: Ensure that existing memory management functionalities remain intact and that core allocation/deallocation logic is sound.
*   **Procedure**:
    *   Identify and execute any existing unit test suites for the memory management (MM) system. This might involve a specific make target or test runner.
    *   **Example Command (if applicable)**: `make test_mm` or similar, depending on the project's test infrastructure.
*   **Verification**:
    *   All existing unit tests should pass.
    *   Analyze any failures to determine if they are related to the recent changes.

## 2. Functional Tests

These tests focus on the new behavior introduced by the delayed deallocation list and the background worker.

### Test Case 2.1: Deallocation in Critical Context (Interrupt/Critical Section Simulation)

*   **Description**: This test simulates memory deallocations that occur within an interrupt handler or a critical section, where immediate freeing might be problematic.
*   **Procedure**:
    1.  Create a test application or module.
    2.  Within a simulated interrupt service routine (ISR) or a code block protected by `enter_critical_section()` / `leave_critical_section()`:
        *   Allocate a small block of memory using `mm_malloc()`.
        *   Perform some operations with the memory (optional).
        *   Free the memory using `mm_free()`.
    3.  After the critical section, attempt an allocation that is expected to fail if the delayed list wasn't processed (e.g., try to allocate almost all available heap).
*   **Verification**:
    *   **Delay List Population**: Use debug flags (e.g., `CONFIG_DEBUG_MM`) or logging within `mm_free()` (specifically where items are added to the delay list) to confirm the freed block is added to the delay list.
    *   **Freeing on Allocation Failure**: Confirm that if a subsequent `mm_malloc()` fails (or is about to fail), `mm_free_delaylist()` is called and attempts to free the delayed blocks. The previously "delayed-freed" block should now be available.
    *   **Freeing by Worker**: If the above step doesn't trigger freeing, verify that the background worker (Test Case 2.2) eventually frees the memory.

*   **Example Code Structure (Conceptual)**:
    ```c
    #include <tinyara/mm/mm.h>
    #include <tinyara/irq.h> // For critical_section (example)
    #include <stdio.h>

    void simulate_critical_free() {
        void *ptr;
        irqstate_t flags;

        flags = enter_critical_section();

        ptr = mm_malloc(100);
        if (ptr) {
            printf("Crit: Allocated ptr: %p\n", ptr);
            // Simulate usage
            mm_free(ptr);
            printf("Crit: Freed ptr: %p (should be on delay list)\n", ptr);
        } else {
            printf("Crit: Allocation failed\n");
        }

        leave_critical_section(flags);
    }

    // In a test task:
    // simulate_critical_free();
    // ... later try to allocate or wait for worker
    ```

### Test Case 2.2: Background Worker Thread Efficacy

*   **Description**: This test verifies that the background worker thread (`mm_delayed_free_worker`) correctly runs and processes the delay list.
*   **Prerequisites**:
    *   `CONFIG_MM_DELAYED_FREE_WORKER` must be enabled in Kconfig.
    *   `CONFIG_MM_DELAYED_FREE_INTERVAL` should be set to a short, observable interval (e.g., 5-10 seconds) for testing.
*   **Procedure**:
    1.  Populate the delay list by executing Test Case 2.1 (Deallocation in Critical Context) multiple times or with various block sizes.
    2.  Avoid further allocations that might trigger `mm_free_delaylist()` via the allocation failure path.
    3.  Wait for a period longer than `CONFIG_MM_DELAYED_FREE_INTERVAL`.
*   **Verification**:
    *   **Worker Execution**: Monitor system logs (ensure `CONFIG_DEBUG_MM` and `CONFIG_DEBUG_VERBOSE` are enabled for `mllvdbg` messages from the worker) to confirm the `mm_delayed_free_worker` thread executes. Look for messages like "Calling mm_free_delaylist for heap...".
    *   **Delay List Cleared**: After the worker is expected to have run, use debugging tools or heap information utilities (if available) to inspect the state of the delay list. It should be empty or significantly reduced.
    *   **Memory Availability**: Attempt to allocate memory that was previously on the delay list. The allocation should now succeed.

### Test Case 2.3: System Load and Stability Test

*   **Description**: This test subjects the system to high memory allocation/deallocation rates and varied task scheduling scenarios to uncover potential race conditions or stability issues.
*   **Procedure**:
    1.  Create multiple tasks (threads) with different priorities.
    2.  These tasks should perform a mix of:
        *   Allocating and freeing various sized memory blocks frequently.
        *   Some tasks performing deallocations within simulated critical sections (as in Test Case 2.1) to populate the delay list.
        *   Holding allocated memory for varying durations.
    3.  Run this test for an extended period.
*   **Verification**:
    *   **System Stability**: The system should remain stable and not crash or hang.
    *   **No Deadlocks**: Verify that no deadlocks occur related to memory allocation or the delay list processing.
    *   **Performance**: Monitor overall system responsiveness. While not a precise performance measurement, significant degradation should be noted.
    *   **Memory Integrity**: Periodically check heap integrity if debugging tools allow.

## 3. Memory Leak Detection

*   **Objective**: Ensure that the new mechanisms do not introduce memory leaks.
*   **Procedure**:
    1.  If the TizenRT environment supports running applications under Valgrind (e.g., in a simulated environment or on a compatible target), use it.
    2.  Run a comprehensive test suite (including the functional tests above) under Valgrind's `leak-check=full` mode.
    3.  Alternatively, use any built-in heap analysis tools that can report outstanding allocations over time. The `heapinfo` utility in TizenRT could be relevant here.
*   **Example Valgrind Command (Conceptual)**:
    ```bash
    valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./your_test_application
    ```
*   **Verification**:
    *   Valgrind (or other tools) should report no "definitely lost" or "possibly lost" blocks attributable to the memory management system after tests complete and expected deallocations have occurred.
    *   Analyze any reported leaks to pinpoint their origin.

## 4. Performance Measurement

*   **Objective**: Quantify the performance impact (if any) of the changes, particularly on `mm_malloc` and `mm_free`.
*   **Procedure**:
    1.  **Baseline**: If possible, establish baseline performance metrics *before* these changes were integrated.
    2.  **Benchmarking**: Create a micro-benchmark test that performs a large number of allocations and deallocations of various common sizes.
        *   Measure the time taken for individual `mm_malloc()` and `mm_free()` calls.
        *   Calculate average, min, max, and possibly standard deviation for these times.
    3.  Run these benchmarks with:
        *   The delayed free worker disabled.
        *   The delayed free worker enabled (to see if its background activity has a noticeable impact on foreground operations).
        *   Scenarios where the delay list is populated and then freed by `mm_malloc` vs. by the worker.
*   **Example Code Structure (Conceptual for benchmarking `mm_malloc`)**:
    ```c
    #include <stdio.h>
    #include <tinyara/mm/mm.h>
    // Assume get_timestamp() returns a high-resolution timestamp (e.g., microseconds or nanoseconds)
    // This function needs to be implemented based on the target's capabilities.
    // extern uint64_t get_timestamp();

    void benchmark_malloc() {
        uint64_t start_time, end_time, total_time_malloc = 0;
        int num_ops = 10000; // Number of operations
        size_t alloc_size = 128;
        void *ptr_array[num_ops]; // To store pointers for freeing

        // Benchmark mm_malloc
        for (int i = 0; i < num_ops; ++i) {
            // start_time = get_timestamp();
            ptr_array[i] = mm_malloc(alloc_size);
            // end_time = get_timestamp();
            // if (ptr_array[i]) {
            //    total_time_malloc += (end_time - start_time);
            // } else {
            //    printf("Benchmark: malloc failed!\n");
            //    // Handle error - perhaps reduce num_ops or skip
            // }
        }

        // Free allocated memory
        for (int i = 0; i < num_ops; ++i) {
            if (ptr_array[i]) {
                mm_free(ptr_array[i]);
            }
        }
        // printf("Average mm_malloc time for %d ops of size %d: %llu units\n",
        //        num_ops, alloc_size, total_time_malloc / num_ops);
        printf("Note: Timestamping around malloc/free directly is tricky due to overhead.\n");
        printf("Focus on throughput or larger operation blocks if direct timing is noisy.\n");
    }
    ```
*   **Verification**:
    *   Compare performance numbers with the baseline (if available).
    *   Analyze any significant performance regressions. A slight overhead for managing the delay list might be acceptable, but it should be understood.
    *   Ensure that performance remains within acceptable limits for the target system.

## Note on Tooling and Environment

*   The specific commands, tools (like Valgrind, heap analyzers), and methods for obtaining high-resolution timestamps or observing thread behavior will depend heavily on the TizenRT development and debugging environment.
*   Access to JTAG debuggers, serial console logs with appropriate debug verbosity (`CONFIG_DEBUG_MM`, `CONFIG_DEBUG_VERBOSE`), and potentially custom heap inspection tools will be crucial for effective testing.
*   The `heapinfo` command in TizenRT shell can be very useful for observing heap state, total allocated size, and free space.
```
