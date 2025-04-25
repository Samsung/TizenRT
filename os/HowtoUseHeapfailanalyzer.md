# Heap Fail Analyzer Script - Usage Guide

This document explains how to use the `heapfailanalyzer.sh` script to analyze heap failure logs in TizenRT.  
This utility simplifies usage of the `heapfailanalyzer.py` parser through Docker.

---

## Overview

The script wraps the Python-based heap log analyzer (`heapfailanalyzer.py`) and runs it inside a Docker container.  
It automatically mounts paths, handles input/output, and generates an Excel report summarizing heap failures or leaks.

---

## Prerequisites

- Docker version 2.0.1(python version 3.8 supported) must be installed and running.
- A valid heap log file generated from TizenRT's heap tracking feature must be available.
- No need to install Python or any dependencies on the host — all handled inside Docker.

---

## Usage

```bash
1. Without command line option 
"Example: /.tizenrt/os$ ./heapfailanalyzer.sh analyze /path/to/logfile/ (path should be relative to os folder)"

2. With menu options

/.tizenrt/os$ ./heapfailanalyzer.sh

======================================================
  "Select Option"
======================================================
  "1. Analyze Log File"
  "2. Help"
  "x. Exit"
======================================================
1
======================================================
"Log Analyzer Tool"
======================================================
"Enter the log file path relative to os folder: (ex: ../logs/system.log)".

```

## Output Details

The output of the script is an Excel file(`.xlsx`) that summarizes the heap failure analysis

### 1.Default Output Location

The output location is fixed and is located at `build/output/heapfailanalyzer_output/` directory of the TizenRT workspace.

Detailed logs will be in the Excel file name is `summary_yyyymmdd_hhmmss.xlsx`.

Summary logs will be in `loganalyzer_output_analyze_yyyy.mm.dd-hh.mm.ss.txt`.

---