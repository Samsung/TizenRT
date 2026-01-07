# CSI Framework Test Application

## Overview

The CSI (Channel State Information) Framework Test Application is a comprehensive testing suite designed to validate the functionality, robustness, and performance of the CSI framework. This application provides extensive test cases covering basic operations, complex scenarios, multi-threading, and various configuration options.

## Features

- **Basic Operations Testing**: Initialization, start, stop, and deinitialization of CSI services
- **Configuration Management**: Get/set configuration parameters, interval adjustments
- **Multi-Handle Support**: Testing with up to 4 concurrent CSI handles
- **Multi-Threading**: Concurrent execution with multiple threads
- **Extended Sequences**: Complex operation sequences with timed delays
- **User-Defined Parameters**: Customizable test scenarios with runtime parameters
- **Callback Variants**: Support for raw data

## Test Cases

### Basic Operations (Test Cases 1-6)

1. **Initialize CSI Service Only**: Register CSI service with specified configuration and interval without starting collection
2. **Start CSI Data Collection Only**: Begin CSI data collection on a previously initialized handle
3. **Stop CSI Data Collection Only**: Stop CSI data collection on an active handle
4. **Deinitialize CSI Service Only**: Unregister a previously initialized CSI service handle
5. **Get Configuration Information**: Retrieve and display current CSI configuration, collection interval, and AP MAC address
6. **Set Collection Interval**: Update the CSI collection interval for an initialized handle

### Basic Sequences (Test Cases 7-8)

7. **Primary Handle Basic Sequence**: Execute a complete sequence (Init->Start->Stop->Deinit) on the primary handle
8. **Secondary Handle Basic Sequence**: Execute a complete sequence (Init->Start->Stop->Deinit) on the secondary handle

### Extended Sequences (Test Cases 9-11)

9. **Extended Sequence with Restart**: Complex sequence involving handle restart and multi-handle operations
10. **Interleaved Handle Operations**: Complex sequence with interleaved operations on multiple handles
11. **Timed Delays Sequence**: Sequence with specific timed delays between operations for stress testing

### Complex Scenarios (Test Cases 12-13)

12. **Multi-Handle Concurrent**: Runs four CSI handles simultaneously to test concurrent operation capabilities
13. **Multi-Threaded Execution**: Executes CSI sequences in separate threads to test thread safety

### User-Defined Parameters (Test Case 14)

14. **Basic Sequence with Parameters**: Runs a basic sequence with user-defined configuration, interval, and runtime

## Building the Application

The application is automatically built as part of the TizenRT build system when `CONFIG_CSIFW_TEST=y` is set. The build process includes:

1. All source files in the `src/` directory:
   - `csifw_test.c` (main application)
   - `csifw_tc_core_operations.c` (basic operations and core functionality)
   - `csifw_tc_complex_workflows.c` (complex scenarios and workflows)
   - `csifw_tc_concurrency_stress.c` (concurrency and stress testing)
2. Header files in the `include/` directory

## Usage

### Command Syntax

```bash
csifw_app {option} [config_type] [interval] [run_time]
```

### Parameters

| Parameter | Description | Valid Range |
|-----------|-------------|-------------|
| option | Test case number to execute | 1-17 |
| config_type | CSI configuration type | 0-3 (framework dependent) |
| interval | Data collection interval in milliseconds | ≥30 ms |
| run_time | Application run time in seconds | >0 |

### Configuration Types

| Value | Type | Description |
|-------|------|-------------|
| 0 | HT_CSI_DATA | High Throughput CSI data |
| 1 | NON_HT_CSI_DATA | Non-High Throughput CSI data |
| 2 | HT_CSI_DATA_ACC1 | HT CSI data with ACC1 |
| 3 | NON_HT_CSI_DATA_ACC1 | Non-HT CSI data with ACC1 |

### Complete Test Case Examples

```bash
# Basic Operations (Test Cases 1-6)
csifw_app 1 0 64                    # Initialize handle 1 with HT_CSI_DATA, 64ms interval
csifw_app 2                         # Start data collection on handle 1
csifw_app 3                         # Stop data collection on handle 1
csifw_app 4                         # Deinitialize handle 1
csifw_app 5                         # Get current configuration info
csifw_app 6 100                     # Set collection interval to 100ms

# Basic Sequences (Test Cases 7-8)
csifw_app 7 2 50                    # Basic sequence on handle 1: NON_HT_CSI_DATA, 50ms
csifw_app 8 1 75                    # Basic sequence on handle 2: NON_HT_CSI_DATA, 75ms

# Extended Sequences (Test Cases 9-13)
csifw_app 9 3 60                    # Extended sequence: NON_HT_CSI_DATA_ACC1, 60ms
csifw_app 10 2 90                   # Interleaved operations: HT_CSI_DATA_ACC1, 90ms
csifw_app 11 1 55                   # Timed delays sequence: NON_HT_CSI_DATA, 55ms

# Complex Scenarios (Test Cases 14-15)
csifw_app 12 2 70                   # Multi-handle concurrent: HT_CSI_DATA_ACC1, 70ms
csifw_app 13 3 65                   # Multi-threaded execution: NON_HT_CSI_DATA_ACC1, 65ms

# User-Defined Parameters (Test Cases 16-17)
csifw_app 14 0 64 100               # Custom: HT_CSI_DATA, 64ms, 100s runtime
```

### Getting Help

To display the complete usage guide with all test case descriptions:

```bash
csifw_app
```

This will show detailed information for all 17 test cases, including:
- Usage syntax for each test case
- Parameter descriptions and valid ranges
- Example commands with explanations
- Important notes and prerequisites

## Application Flow

### Test Case Execution

1. **Input Validation**: Parse and validate command-line arguments with optimized single-pass conversion
2. **Test Case Selection**: Execute the specified test case function
3. **CSI Operations**: Perform CSI framework operations as required by the test case
4. **Result Reporting**: Output operation status and results
5. **Resource Cleanup**: Properly deinitialize services and clean up resources

### Optimized Argument Processing

The application features an optimized argument parsing mechanism in the `csifw_tc_app_init` function:

- **Single-Pass Conversion**: Each command-line argument is converted from string to integer exactly once
- **Efficient Validation**: Argument count and range validation performed before any conversions
- **Special Case Handling**: Test case 6 (interval setting) handled efficiently without redundant conversions
- **Minimal Overhead**: Reduced function call overhead and improved startup performance

### Key Components

- `csifw_test.c`: Main application entry point and test case dispatcher
- `csifw_tc_core_operations.c`: Basic operations and core functionality (test cases 1-6)
- `csifw_tc_complex_workflows.c`: Complex scenarios and workflows (test cases 12-15)
- `csifw_tc_concurrency_stress.c`: Concurrency and stress testing (test cases 7-9, 14-15)
- `csifw_test.h`: Header file with function declarations and includes

## Detailed Test Case Descriptions

### Test Case 1: Initialize CSI Service Only
Initializes a CSI service handle with specified configuration and interval without starting collection.

### Test Case 2: Start CSI Data Collection Only
Starts CSI data collection on a previously initialized handle.

### Test Case 3: Stop CSI Data Collection Only
Stops CSI data collection on an active handle.

### Test Case 4: Deinitialize CSI Service Only
Unregisters a previously initialized CSI service handle.

### Test Case 5: Get Configuration Information
Retrieves and displays current CSI configuration, collection interval, and AP MAC address.

### Test Case 6: Set Collection Interval
Updates the CSI collection interval for an initialized handle.

### Test Case 7: Primary Handle Basic Sequence
Executes a complete sequence (Init->Start->Stop->Deinit) on the primary handle.

### Test Case 8: Secondary Handle Basic Sequence
Executes a complete sequence (Init->Start->Stop->Deinit) on the secondary handle.

### Test Case 9: Extended Sequence with Restart
Complex sequence involving handle restart and multi-handle operations.

### Test Case 10: Interleaved Handle Operations
Complex sequence with interleaved operations on multiple handles using config_type=2 (HT_CSI_DATA_ACC1).

### Test Case 11: Timed Delays Sequence
Sequence with specific timed delays between operations for stress testing.

### Test Case 12: Multi-Handle Concurrent
Runs four CSI handles simultaneously to test concurrent operation capabilities.

### Test Case 13: Multi-Threaded Execution
Executes CSI sequences in separate threads to test thread safety.

### Test Case 14: Basic Sequence with Parameters
Runs a basic sequence with user-defined configuration, interval, and runtime.

## Configuration Notes

- Minimum supported interval is 30ms
- All handles must be properly initialized before use
- Handles must be stopped before deinitialization
- Multi-threading requires pthread library support

## Code Structure

### Global Handles

The application maintains up to four global CSI service handles:
- `primary_handle`: Primary CSI service handle
- `secondary_handle`: Secondary CSI service handle
- `tertiary_handle`: Tertiary CSI service handle
- `quaternary_handle`: Quaternary CSI service handle

### Callback Functions

- `raw_data_cb_1` to `raw_data_cb_4`: Raw CSI data callbacks for each handle
