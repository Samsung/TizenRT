# Heap Fail Analyzer Script - Usage Guide

This document explains how to use the `heapfailanalyzer.sh` script to analyze heap failure logs in TizenRT.  
This utility simplifies usage of the `heapfailanalyzer.py` parser through Docker.

---

## Overview

The script wraps the Python-based heap log analyzer (`heapfailanalyzer.py`) and runs it inside a Docker container.  
It automatically mounts paths, handles input/output, and generates an Excel report summarizing heap failures or leaks.

Key features:
- Runs from the tools/memory folder
- Supports both absolute and relative paths for input log files
- Outputs analysis results to the tools/memory folder by default
- Allows specifying a custom output directory

---

## Prerequisites

- TizenRT Docker version 2.0.1 (Python version 3.8 supported) must be installed and running
- A valid heap log file generated from TizenRT's heap tracking feature
- No need to install Python or any dependencies on the host — all handled inside Docker

---

## Usage

### Command Line Mode

The script should be run from the tools/memory directory:

```bash
cd tizenrt/tools/memory

# Basic usage with relative path
./heapfailanalyzer.sh analyze logs/system.log

# Using absolute path for log file
./heapfailanalyzer.sh analyze /absolute/path/to/logfile.log

# Specify custom output directory (relative to tools/memory folder)
./heapfailanalyzer.sh analyze logs/system.log --output-dir=my_results

# Specify custom output directory (absolute path)
./heapfailanalyzer.sh analyze logs/system.log --output-dir=/home/user/results

```

### Interactive Menu Mode

```bash
# Launch the interactive menu
cd tizenrt/tools/memory
./heapfailanalyzer.sh  
         or
./heapfailanalyzer.sh menu 

======================================================
  "Select Option"
======================================================
  "1. Analyze Log File"
  "h. Help"
  "x. Exit"
======================================================
1

======================================================
  "Log Analyzer Tool"
======================================================
  "Enter the log file path (absolute or relative to tools/memory folder):"
logs/system.log

  "Enter output directory path (leave empty for default, absolute or relative to tools/memory folder):"
my_results
```

## Output Details

The script generates two types of output files:

### Excel Report
A detailed `.xlsx` file that summarizes the heap failure analysis with sheets for:
- Overall summary statistics
- Per-PID memory usage
- Per-Owner memory usage

### Text Log
A `.txt` file containing the console output from the analysis process.

### Default Output Location

By default, output files are generated in the `./heapfailanalyzer_output/` directory within the tools/memory folder:

- Excel file: `summary_yyyymmdd_hhmmss.xlsx`
- Text log: `loganalyzer_output_analyze_yyyy.mm.dd-hh.mm.ss.txt`

### Custom Output Location

You can specify a custom output location using the `--output-dir` parameter or through the interactive menu. The specified path can be:
- An absolute path (e.g., `/home/user/my_results`)
- A path relative to the tools/memory folder (e.g., `my_results`)

---

## Help Menu

Run the help command to see all available options:

```bash
./heapfailanalyzer.sh help
```

Output:
```
Usage: heapfailanalyzer.sh [menu] | [help] | [analyze log_file [options]]

menu      Display interactive menu
help      Display this help menu
analyze   Analyze the specified log file
          Ex: heapfailanalyzer.sh analyze logs/system.log
          Ex: heapfailanalyzer.sh analyze /absolute/path/to/system.log

Options:
  --output-dir=PATH    Specify a custom output directory (absolute or relative to tools/memory folder)

Notes:
  - Default output location is ./heapfailanalyzer_output/ in tools/memory folder
  - Relative paths are interpreted as relative to the tools/memory folder
```

---

## Path Interpretation Rules

- **Input Log File**: 
  - Absolute path: Used as-is (e.g., `/home/user/logs/system.log`)
  - Relative path: Relative to tools/memory folder (e.g., `logs/system.log` → `tizenrt/tools/memory/logs/system.log`)

- **Output Directory**:
  - Default: `tizenrt/tools/memory/heapfailanalyzer_output/`
  - Absolute path: Used as-is (e.g., `/home/user/results`)
  - Relative path: Relative to tools/memory folder (e.g., `my_results` → `tizenrt/tools/memory/my_results`)

---

## Example Workflow

1. Run TizenRT with heap tracking enabled
2. Collect log file containing heap failure information
3. Navigate to the tools/memory directory: `cd tizenrt/tools/memory`
4. Run the analyzer: `./heapfailanalyzer.sh analyze ../../logs/system.log`
5. Check the output in `./heapfailanalyzer_output/`
6. Open the Excel file to view the detailed memory analysis

---
