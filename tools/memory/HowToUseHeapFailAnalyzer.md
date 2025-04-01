# heapfailanalyzer User Guide

## Overview

heapfailanalyzer is a powerful Python tool designed to parse and analyze TizenRT heap allocation failure logs. It provides detailed insights into memory usage patterns, identifies problematic PIDs and memory owners, and helps diagnose memory-related issues in TizenRT systems.

## Features

- Detailed memory allocation statistics by PID and owner
- Identification of top memory consumers
- Human-readable size formatting
- Excel export functionality
- Progress tracking with ETA estimation

## Requirements

- Python 3.8 or higher
- Standard Python libraries: `re`, `sys`, `argparse`, `collections`, `datetime`, `os`, `time`
- TizenRT memory allocation failure logs

## Basic Usage

To analyze a log file with default settings:

```bash
python3 ./heapfailanalyzer.py /path/to/your/logfile.log --export summary.xlsx
```

This will perform a two-pass analysis of the log file and display a summary of memory allocation failures.

## Command Line Arguments

| Argument | Description |
|----------|-------------|
| `logfile` | Path to the log file (required) |
| `--export FILENAME` | Export summary to a excel file |

## Examples

### Basic Analysis

```bash
./heapfailanalyzer.py device_log.txt
```

### Export Results to excel

```bash
./heapfailanalyzer.py device_log.txt --export memory_analysis.xlsx
```

## Understanding the Output

### Memory Allocation Failure Summary

The main output includes:

- Total number of memory allocation failures
- Total and average requested memory sizes
- Total allocated memory
- Details of the largest allocation

Example:
```
===== MEMORY ALLOCATION FAILURE SUMMARY =====

Total failures analyzed: 1245
Total requested memory: 4567890 bytes (4.36 MB)
Average request size: 3669.79 bytes (3.58 KB)
Total allocated memory: 16789012 bytes (16.01 MB)

Largest allocation:
Size: 524288 bytes (512.00 KB)
PID: 38
Owner: 0x0800f234
Address: 0x02035678
```

### PID Summary

This section shows memory allocation statistics grouped by Process ID (PID):

```
----- PID SUMMARY -----
PID Count Total Size % of Memory
------------------------------------------------------------
23 356 8765432 (8.36 MB) 52.21
38 245 4567890 (4.36 MB) 27.21
17 102 2345678 (2.24 MB) 13.97
```

### Owner Summary

This section shows memory allocation statistics grouped by owner address:

```
----- OWNER SUMMARY -----
Owner Count Total Size % of Memory PIDs
------------------------------------------------------------------------------------------
0x0800f234 178 6789012 (6.47 MB) 40.44 23, 38
0x0800a123 156 4567890 (4.36 MB) 27.21 17, 23
0x08003456 102 2345678 (2.24 MB) 13.97 38
```

### Critical Components

This section highlights the top memory-intensive PIDs and owners:

```
----- CRITICAL COMPONENTS -----
Top memory-intensive PIDs:
1. PID 23 with 8765432 bytes allocated (8.36 MB)
2. PID 38 with 4567890 bytes allocated (4.36 MB)
3. PID 17 with 2345678 bytes allocated (2.24 MB)

Top memory-intensive Owners:
1. 0x0800f234 with 6789012 bytes allocated (6.47 MB) across 2 PIDs
2. 0x0800a123 with 4567890 bytes allocated (4.36 MB) across 2 PIDs
3. 0x08003456 with 2345678 bytes allocated (2.24 MB) across 1 PID
```

## Excel Export Format

When using the `--export` option, the tool creates a xlsx file with the following sections:

1. Overall Summary Top 5 Memory-Intensive (PID, Owners) & Largest Allocation Details
2. PID Summary
3. Owner Summary

This format makes it easy to import the data into spreadsheet applications for further analysis or reporting.

## Troubleshooting

### Common Issues

1. **"File not found" error**
- Verify the log file path is correct
- Check file permissions

2. **Unicode decode errors**
- The script uses error='replace' for UTF-8 decoding to handle invalid characters

### Debugging Tips

- The script provides detailed progress updates during processing
- Check the ETA estimates to determine if processing is proceeding as expected
- For unexpectedly slow processing, check if the log file format matches the expected TizenRT format

## Conclusion

heapfailanalyzer is an essential tool for TizenRT developers and system administrators who need to diagnose and resolve memory allocation issues. By providing detailed insights into memory usage patterns, it helps identify problematic components and optimize system performance.