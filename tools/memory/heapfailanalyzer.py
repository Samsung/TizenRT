#!/usr/bin/env python3
###########################################################################
#
# Copyright 2025 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
# File : heapfailanalyzer.py
# Description: Tool to parse the memory failure log and provides a summary of memory usage by PID and owner.

"""
TizenRT Memory Allocation Failure Log Analyzer
----------------------------------------------
This script parses TizenRT memory allocation failure logs and provides a summary
of memory usage by PID and owner using a two-pass approach for better performance.
"""

import re
import sys
import argparse
from collections import defaultdict
import datetime
import os
import time
import csv
from openpyxl import Workbook

def count_failures(filename):
    """
    First pass: Count failures and measure file stats for better progress estimation
   
    Args:
        filename (str): Path to the log file
       
    Returns:
        tuple: (num_failures, file_size, line_markers)
    """
    failure_start_pattern = re.compile(r'\[(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d+)\] mm_manage_alloc_fail: Allocation failed from user heap\.')
   
    failure_count = 0
    file_size = os.path.getsize(filename)
   
    # Store positions of each 10th failure for better progress tracking
    failure_positions = []
   
    try:
        print(f"First pass: Analyzing log file structure ({format_size(file_size)})...")
        print("[" + " " * 50 + "] 0%", end="\r")
       
        with open(filename, 'r', encoding='utf-8', errors='replace') as f:
            bytes_read = 0
            last_progress = 0
            buffer_size = 4 * 1024 * 1024  # 4MB buffer
           
            for chunk in iter(lambda: f.read(buffer_size), ''):
                chunk_size = len(chunk.encode('utf-8'))  # Get encoded size
                bytes_read += chunk_size
               
                # Update progress bar
                progress = int(50 * bytes_read / file_size)
                if progress > last_progress:
                    print(f"[{'#' * progress}{' ' * (50 - progress)}] {int(100 * bytes_read / file_size)}%", end="\r")
                    last_progress = progress
               
                # Count failure starts in this chunk
                for match in failure_start_pattern.finditer(chunk):
                    failure_count += 1
                    # Store position of every 10th failure for progress tracking
                    if failure_count % 10 == 0:
                        position = bytes_read - chunk_size + match.start()
                        failure_positions.append((failure_count, position))
       
        print(f"\nFound {failure_count} memory allocation failures.")
       
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found")
        sys.exit(1)
    except Exception as e:
        print(f"Error analyzing log file: {str(e)}")
        sys.exit(1)
   
    return failure_count, file_size, failure_positions

def parse_log_file(filename, total_failures, file_size, failure_positions):
    """
    Second pass: Parse the log file and extract memory allocation failure information
    with better progress tracking
   
    Args:
        filename (str): Path to the log file
        total_failures (int): Total failures counted in first pass
        file_size (int): Total file size
        failure_positions (list): Positions of failures for tracking
       
    Returns:
        dict: Summary dictionary (eliminates need for a separate summary generation step)
    """
    # Initialize summary directly rather than storing all failures
    summary = {
        'total_failures': total_failures,
        'by_pid': defaultdict(lambda: {'count': 0, 'total_allocated': 0}),
        'by_owner': defaultdict(lambda: {'count': 0, 'total_allocated': 0, 'pids': set()}),
        'largest_allocation': {'size': 0, 'pid': None, 'owner': None, 'address': None},
        'total_requested_size': 0,
        'requested_sizes': [],  # For calculating average later
        'total_allocated_memory': 0,
        'total_free_memory': 0,
        'top_pids': [],  # Updated to store top PIDs
        'top_owners': []  # Updated to store top owners
    }
   
    # Precompile regex patterns for better performance
    failure_start_pattern = re.compile(r'\[(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d+)\] mm_manage_alloc_fail: Allocation failed from user heap\.')
    size_pattern = re.compile(r'\[.*\] mm_manage_alloc_fail:  - requested size (\d+)')
    caller_pattern = re.compile(r'\[.*\] mm_manage_alloc_fail:  - caller address = (0x[0-9a-f]+)')
    largest_free_pattern = re.compile(r'\[.*\] mm_manage_alloc_fail:  - largest free size : (\d+)')
    total_free_pattern = re.compile(r'\[.*\] mm_manage_alloc_fail:  - total free size   : (\d+)')
    header_pattern = re.compile(r'\[.*\]   MemAddr \|   Size   \| Status \|    Owner   \|  Pid  \|')
    entry_pattern = re.compile(r'\[.*\] (0x[0-9a-f]+) \|[ ]*(\d+) \|[ ]*([A-Z])[ ]*\| (0x[ 0-9a-f]+) \|[ ]*(\d+)(?:\(([A-Z])\))?\s*\|')
   
    # Initialize processing variables
    current_failure = None
    heap_entries = []
    failures_processed = 0
    next_marker_idx = 0
   
    # Create failure marker map for better progress tracking
    failure_markers = {}
    if failure_positions:
        for count, position in failure_positions:
            failure_markers[count] = position
   
    try:
        print(f"Second pass: Processing memory allocation data...")
        print("[" + " " * 50 + "] 0%", end="\r")
       
        start_time = time.time()
        last_update_time = start_time
        bytes_read = 0
        last_progress = 0
       
        with open(filename, 'r', encoding='utf-8', errors='replace') as f:
            # Process file line by line for accurate parsing
            for line in f:
                bytes_read += len(line.encode('utf-8'))
               
                # Update progress based on failures or bytes processed
                if failure_positions and next_marker_idx < len(failure_positions):
                    next_count, next_pos = failure_positions[next_marker_idx]
                    if failures_processed >= next_count:
                        progress = int(50 * failures_processed / total_failures)
                        next_marker_idx += 1
                    else:
                        progress = int(50 * failures_processed / total_failures)
                else:
                    # Fall back to file position if markers exhausted
                    progress = int(50 * bytes_read / file_size)
               
                # Update progress display
                current_time = time.time()
                if progress > last_progress or current_time - last_update_time > 1.0:
                    elapsed_time = current_time - start_time
                   
                    # Calculate ETA based on progress
                    if failures_processed > 0:
                        estimated_total = (elapsed_time / (failures_processed / total_failures))
                        remaining_time = max(0, estimated_total - elapsed_time)
                        eta_text = f"ETA: {format_time(remaining_time)}"
                    else:
                        eta_text = "Calculating ETA..."
                   
                    print(f"[{'#' * progress}{' ' * (50 - progress)}] {failures_processed}/{total_failures} failures ({int(100 * failures_processed / total_failures)}%) - {eta_text}", end="\r")
                    last_progress = progress
                    last_update_time = current_time
               
                # Process the line
                # Check for the start of a new allocation failure
                failure_match = failure_start_pattern.match(line)
                if failure_match:
                    # If we already have a current failure, finalize it (but don't store it)
                    if current_failure:
                        failures_processed += 1
                        # Just update the requested_size info in summary
                        if current_failure.get('requested_size'):
                            summary['total_requested_size'] += current_failure['requested_size']
                            summary['requested_sizes'].append(current_failure['requested_size'])
                   
                    timestamp = failure_match.group(1)
                    # Create a new current failure
                    current_failure = {
                        'timestamp': timestamp,
                        'requested_size': None,
                        'caller_address': None,
                        'largest_free_size': None,
                        'total_free_size': None
                    }
                    continue
               
                # If we're currently processing a failure, check for details
                if current_failure:
                    # Check for requested size
                    size_match = size_pattern.match(line)
                    if size_match:
                        current_failure['requested_size'] = int(size_match.group(1))
                        continue
                   
                    # Check for caller address
                    caller_match = caller_pattern.match(line)
                    if caller_match:
                        current_failure['caller_address'] = caller_match.group(1)
                        continue
                   
                    # Check for largest free size
                    largest_free_match = largest_free_pattern.match(line)
                    if largest_free_match:
                        current_failure['largest_free_size'] = int(largest_free_match.group(1))
                        continue
                   
                    # Check for total free size
                    total_free_match = total_free_pattern.match(line)
                    if total_free_match:
                        current_failure['total_free_size'] = int(total_free_match.group(1))
                        continue
               
                # Check for memory table header
                if header_pattern.match(line):
                    # Clear heap entries for a new table
                    heap_entries = []
                    continue
               
                # Check for memory entries
                entry_match = entry_pattern.match(line)
                if entry_match:
                    address = entry_match.group(1)
                    size = int(entry_match.group(2))
                    status = entry_match.group(3)
                    owner = entry_match.group(4).strip()
                    pid = entry_match.group(5)
                    special = entry_match.group(6) if entry_match.group(6) else ""
                   
                    # Update summary directly instead of storing all entries
                    # Update PID statistics
                    summary['by_pid'][pid]['count'] += 1
                    summary['by_pid'][pid]['total_allocated'] += size
                   
                    # Update owner statistics
                    summary['by_owner'][owner]['count'] += 1
                    summary['by_owner'][owner]['total_allocated'] += size
                    summary['by_owner'][owner]['pids'].add(pid)
                   
                    # Update total allocated memory
                    summary['total_allocated_memory'] += size
                   
                    # Check for largest allocation
                    if size > summary['largest_allocation']['size']:
                        summary['largest_allocation'] = {
                            'size': size,
                            'pid': pid,
                            'owner': owner,
                            'address': address
                        }
       
        # Don't forget to process the last failure if there is one
        if current_failure:
            failures_processed += 1
            if current_failure.get('requested_size'):
                summary['total_requested_size'] += current_failure['requested_size']
                summary['requested_sizes'].append(current_failure['requested_size'])
       
        # Calculate average requested size
        if summary['requested_sizes']:
            summary['average_requested_size'] = sum(summary['requested_sizes']) / len(summary['requested_sizes'])
       
        # Find top 5 PIDs and owners by memory allocation
        sorted_pids = sorted(summary['by_pid'].items(), key=lambda x: x[1]['total_allocated'], reverse=True)
        sorted_owners = sorted(summary['by_owner'].items(), key=lambda x: x[1]['total_allocated'], reverse=True)
       
        # Get top 5 PIDs or all if less than 5
        num_pids = min(5, len(sorted_pids))
        for i in range(num_pids):
            pid, data = sorted_pids[i]
            summary['top_pids'].append({
                'pid': pid,
                'count': data['count'],
                'total_allocated': data['total_allocated']
            })
       
        # Get top 5 owners or all if less than 5
        num_owners = min(5, len(sorted_owners))
        for i in range(num_owners):
            owner, data = sorted_owners[i]
            summary['top_owners'].append({
                'owner': owner,
                'count': data['count'],
                'total_allocated': data['total_allocated'],
                'pids': data['pids']
            })
       
        total_time = time.time() - start_time
        print(f"\nProcessing completed in {format_time(total_time)}")
       
    except Exception as e:
        print(f"\nError processing log file: {str(e)}")
        sys.exit(1)
   
    return summary

def format_size(size_bytes):
    """Format file size in human-readable format"""
    if size_bytes < 0:
        return "0 B"
   
    for unit in ['B', 'KB', 'MB', 'GB']:
        if size_bytes < 1024.0 or unit == 'GB':
            break
        size_bytes /= 1024.0
    return f"{size_bytes:.2f} {unit}"

def format_time(seconds):
    """Format time in human-readable format"""
    if seconds < 60:
        return f"{seconds:.1f}s"
    elif seconds < 3600:
        minutes = int(seconds // 60)
        seconds = int(seconds % 60)
        return f"{minutes}m {seconds}s"
    else:
        hours = int(seconds // 3600)
        minutes = int((seconds % 3600) // 60)
        return f"{hours}h {minutes}m"

def print_summary(summary, detailed=False):
    """
    Print a formatted summary of memory allocation failures
   
    Args:
        summary (dict): Summary statistics
        detailed (bool): Whether to print detailed information
    """
    print("\n===== MEMORY ALLOCATION FAILURE SUMMARY =====\n")
    print(f"Total failures analyzed: {summary['total_failures']}")
    print(f"Total requested memory: {summary['total_requested_size']} bytes ({format_size(summary['total_requested_size'])})")
    print(f"Average request size: {summary['average_requested_size']:.2f} bytes ({format_size(summary['average_requested_size'])})")
    print(f"Total allocated memory: {summary['total_allocated_memory']} bytes ({format_size(summary['total_allocated_memory'])})")
   
    if summary['largest_allocation']['size'] > 0:
        print(f"\nLargest allocation:")
        print(f"  Size: {summary['largest_allocation']['size']} bytes ({format_size(summary['largest_allocation']['size'])})")
        print(f"  PID: {summary['largest_allocation']['pid']}")
        print(f"  Owner: {summary['largest_allocation']['owner']}")
        print(f"  Address: {summary['largest_allocation']['address']}")
   
    print("\n----- PID SUMMARY -----")
    print(f"{'PID':<8} {'Count':<8} {'Total Size':<20} {'% of Memory':<12}")
    print("-" * 60)
   
    # Sort PIDs by total allocated memory
    sorted_pids = sorted(summary['by_pid'].items(), key=lambda x: x[1]['total_allocated'], reverse=True)
   
    for pid, data in sorted_pids:
        percent = (data['total_allocated'] / summary['total_allocated_memory'] * 100) if summary['total_allocated_memory'] else 0
        print(f"{pid:<8} {data['count']:<8} {data['total_allocated']:<12} ({format_size(data['total_allocated']):<6}) {percent:<12.2f}")
   
    print("\n----- OWNER SUMMARY -----")
    print(f"{'Owner':<20} {'Count':<8} {'Total Size':<20} {'% of Memory':<12} {'PIDs'}")
    print("-" * 90)
   
    # Sort owners by total allocated memory
    sorted_owners = sorted(summary['by_owner'].items(), key=lambda x: x[1]['total_allocated'], reverse=True)
   
    for owner, data in sorted_owners:
        percent = (data['total_allocated'] / summary['total_allocated_memory'] * 100) if summary['total_allocated_memory'] else 0
        pids = ", ".join(sorted(data['pids']))
        print(f"{owner:<20} {data['count']:<8} {data['total_allocated']:<12} ({format_size(data['total_allocated']):<6}) {percent:<12.2f} {pids}")
   
    print("\n----- CRITICAL COMPONENTS -----")
   
    # Print top 5 memory-intensive PIDs
    print("Top memory-intensive PIDs:")
    for i, pid_info in enumerate(summary['top_pids'], 1):
        print(f"{i}. PID {pid_info['pid']} with {pid_info['total_allocated']} bytes allocated ({format_size(pid_info['total_allocated'])})")
   
    # Print top 5 memory-intensive owners
    print("\nTop memory-intensive Owners:")
    for i, owner_info in enumerate(summary['top_owners'], 1):
        pids_count = len(owner_info['pids'])
        pids_text = f"across {pids_count} PID{'' if pids_count == 1 else 's'}"
        print(f"{i}. {owner_info['owner']} with {owner_info['total_allocated']} bytes allocated ({format_size(owner_info['total_allocated'])}) {pids_text}")

def export_to_excel(summary, filename):
    """
    Export summary to an Excel file with three sheets

    Args:
        summary (dict): Summary statistics
        filename (str): Output Excel file name
    """
    try:
        print(f"Exporting summary to {filename}...")
        wb = Workbook()
       
        # Create sheets
        ws_summary = wb.active
        ws_summary.title = "Summary"
       
        ws_pid_summary = wb.create_sheet("PID Summary")
       
        ws_owner_summary = wb.create_sheet("Owner Summary")
       
        # Export overall statistics, top 5 PIDs, top 5 owners, and largest allocation to the Summary sheet
        ws_summary.append(['Metric', 'Value', 'Human Readable'])
        ws_summary.append(['Total failures', summary['total_failures'], ''])
        ws_summary.append(['Total requested memory', summary['total_requested_size'], format_size(summary['total_requested_size'])])
        ws_summary.append(['Average request size', f"{summary['average_requested_size']:.2f}", format_size(summary['average_requested_size'])])
        ws_summary.append(['Total allocated memory', summary['total_allocated_memory'], format_size(summary['total_allocated_memory'])])
       
        if summary['largest_allocation']['size'] > 0:
            ws_summary.append(['', '', ''])
            ws_summary.append(['Largest Allocation', '', ''])
            ws_summary.append(['Size', summary['largest_allocation']['size'], format_size(summary['largest_allocation']['size'])])
            ws_summary.append(['PID', summary['largest_allocation']['pid'], ''])
            ws_summary.append(['Owner', summary['largest_allocation']['owner'], ''])
            ws_summary.append(['Address', summary['largest_allocation']['address'], ''])
       
        # Export top 5 memory-intensive PIDs
        ws_summary.append(['', '', ''])
        ws_summary.append(['Top 5 Memory-Intensive PIDs', '', ''])
        ws_summary.append(['#', 'PID', 'Total Allocated (bytes)', 'Human Readable Size'])
        for i, pid_info in enumerate(summary['top_pids'], 1):
            ws_summary.append([i, pid_info['pid'], pid_info['total_allocated'], format_size(pid_info['total_allocated'])])
       
        # Export top 5 memory-intensive owners
        ws_summary.append(['', '', ''])
        ws_summary.append(['Top 5 Memory-Intensive Owners', '', ''])
        ws_summary.append(['#', 'Owner', 'Total Allocated (bytes)', 'Human Readable Size', '# of PIDs'])
        for i, owner_info in enumerate(summary['top_owners'], 1):
            pids_count = len(owner_info['pids'])
            ws_summary.append([i, owner_info['owner'], owner_info['total_allocated'], format_size(owner_info['total_allocated']), pids_count])
       
        # Export PID summary to the PID Summary sheet
        ws_pid_summary.append(['PID', 'Count', 'Total Allocated (bytes)', 'Human Readable Size', '% of Memory'])
        sorted_pids = sorted(summary['by_pid'].items(), key=lambda x: x[1]['total_allocated'], reverse=True)
       
        for pid, data in sorted_pids:
            percent = (data['total_allocated'] / summary['total_allocated_memory'] * 100) if summary['total_allocated_memory'] else 0
            ws_pid_summary.append([pid, data['count'], data['total_allocated'], format_size(data['total_allocated']), f"{percent:.2f}%"])
       
        # Export Owner summary to the Owner Summary sheet
        ws_owner_summary.append(['Owner', 'Count', 'Total Allocated (bytes)', 'Human Readable Size', '% of Memory', 'PIDs'])
        sorted_owners = sorted(summary['by_owner'].items(), key=lambda x: x[1]['total_allocated'], reverse=True)
       
        for owner, data in sorted_owners:
            percent = (data['total_allocated'] / summary['total_allocated_memory'] * 100) if summary['total_allocated_memory'] else 0
            pids = ", ".join(sorted(data['pids']))
            ws_owner_summary.append([owner, data['count'], data['total_allocated'], format_size(data['total_allocated']), f"{percent:.2f}%", pids])
       
        wb.save(filename)
        print(f"Summary exported to {filename}")
    except Exception as e:
        print(f"Error exporting to Excel: {str(e)}")

def main():
    parser = argparse.ArgumentParser(description='Analyze TizenRT memory allocation failure logs')
    parser.add_argument('logfile', help='Path to the log file')
    parser.add_argument('--export', help='Export summary to CSV file')
    parser.add_argument('--detailed', action='store_true', help='Print detailed information')
    parser.add_argument('--skip-first-pass', action='store_true', help='Skip first pass analysis (faster but less accurate progress)')
    args = parser.parse_args()
   
    print(f"\nTizenRT Memory Allocation Failure Log Analyzer")
    print(f"==============================================")
   
    # Get file size
    try:
        file_size = os.path.getsize(args.logfile)
        print(f"Log file size: {format_size(file_size)}")
    except FileNotFoundError:
        print(f"Error: File '{args.logfile}' not found")
        sys.exit(1)
   
    start_time = time.time()
   
    # First pass: count failures and gather file metrics
    if not args.skip_first_pass:
        total_failures, file_size, failure_positions = count_failures(args.logfile)
    else:
        print("Skipping first pass analysis...")
        total_failures = 0  # Will be counted in second pass
        failure_positions = []
   
    # Second pass: process failures and generate summary
    summary = parse_log_file(args.logfile, total_failures, file_size, failure_positions)
   
    # Print summary
    print_summary(summary, args.detailed)
   
    # Export if requested
    if args.export:
        export_to_excel(summary, args.export)
   
    total_time = time.time() - start_time
    print(f"\nTotal analysis time: {format_time(total_time)}")
    print("Analysis complete.")

if __name__ == "__main__":
    main()
    
