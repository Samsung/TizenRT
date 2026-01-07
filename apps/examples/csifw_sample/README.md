# CSI Framework Sample Application

## Overview

The CSI (Channel State Information) Framework Sample Application demonstrates how to use the CSI framework to collect and process wireless channel state information. This application provides a simple interface to register callbacks for raw and/or parsed CSI data, configure collection parameters, and run the collection for a specified duration.

## Features

- Support for different CSI data callback types:
  - Raw data only
  - Parsed data only (Not yet implemented)
  - Both raw and parsed data
- Configurable CSI collection parameters
- Simple command-line interface
- Error handling and validation
- Clean resource management

## Usage

### Command Syntax

```bash
csifw_sample <Configuration> <Interval> <App_run_duration> <Callback_type>
```

### Parameters

| Parameter | Description | Valid Range |
|-----------|-------------|-------------|
| Configuration | CSI configuration type | 0-3 (framework dependent) |
| Interval | Data collection interval in milliseconds | ≥30 ms |
| App_run_duration | Application run time in seconds | >0 |
| Callback_type | Type of data callbacks to register | 0=Raw only, 1=Parsed only, 2=Both |

### Examples

```bash
# Run with raw data callback only
csifw_sample 0 64 100 0

# Run with parsed data callback only
csifw_sample 1 100 60 1

# Run with both raw and parsed data callbacks
csifw_sample 2 50 120 2
```

## Application Flow

1. **Initialization**: Register service with specified callbacks
2. **Start**: Begin CSI data collection
3. **Run**: Collect data for specified duration
4. **Stop**: Stop CSI data collection
5. **Deinitialization**: Unregister service and clean up resources

## Code Structure

### Main Components

- `csifw_sample_app_main()`: Entry point and main control flow
- `initialize_csi_service()`: Service initialization with callback configuration
- `start_csi_collection()`: Start CSI data collection
- `stop_csi_collection()`: Stop CSI data collection
- `deinitialize_csi_service()`: Service deinitialization
- Callback functions:
  - `raw_data_callback()`: Handles raw CSI data
  - `parsed_data_callback()`: Handles parsed CSI data

### Key Improvements

1. **Reduced Code Duplication**: Single initialization function handles all callback types
2. **Enhanced Error Handling**: Comprehensive input validation and error checking
3. **Improved Readability**: Enumerations and descriptive variable names
4. **Better Documentation**: Doxygen-style comments for all functions
5. **Robust Resource Management**: Proper cleanup even in error cases

## API Reference

### Data Structures

```c
typedef enum {
    CB_TYPE_RAW_ONLY = 0,
    CB_TYPE_PARSED_ONLY = 1,
    CB_TYPE_BOTH = 2
} callback_type_t;
```

### Functions

#### `csifw_sample_app_main()`

Main initialization function that orchestrates the entire application flow.

#### `initialize_csi_service()`

Initializes the CSI service with the specified callback configuration.

#### `start_csi_collection()`, `stop_csi_collection()`, `deinitialize_csi_service()`

Control functions for managing the CSI service lifecycle.

#### Callback Functions

- `raw_data_callback()`: Processes raw CSI data
- `parsed_data_callback()`: Processes parsed CSI data

## Configuration Notes

- Minimum supported interval is 30ms
- Run time must be positive
- Callback type must be 0, 1, or 2
- Configuration values depend on the underlying CSI framework implementation
