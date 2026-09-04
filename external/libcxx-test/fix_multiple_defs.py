#!/usr/bin/env python3
"""
Fix multiple definition linker errors in libcxx test files by wrapping
file-scope code in anonymous namespaces.

Usage: python3 fix_multiple_defs.py

Two types of fixes:
1. Algorithm .pass.cpp files: wrap all code (except #includes and entry point) 
   in anonymous namespace
2. support/count_new.h: wrap global-scope definitions in anonymous namespace
"""

import os
import re

def find_entry_point(lines):
    """Find the entry point function definition line index."""
    pattern = re.compile(r'^int\s+tc_libcxx_\w+\s*\(')
    for i, line in enumerate(lines):
        if pattern.match(line.strip()):
            return i
    return -1

def find_last_include(lines):
    """Find the last #include line index (including those inside #if blocks)."""
    last = -1
    for i, line in enumerate(lines):
        if line.strip().startswith('#include'):
            last = i
    return last

def find_entry_point_end(lines, start):
    """Find the closing brace of the entry point function."""
    brace_count = 0
    started = False
    for i in range(start, len(lines)):
        for ch in lines[i]:
            if ch == '{':
                brace_count += 1
                started = True
            elif ch == '}':
                brace_count -= 1
            if started and brace_count == 0:
                return i
    return len(lines) - 1

def fix_pass_cpp_file(filepath):
    """Wrap file-scope code in anonymous namespace to prevent ODR violations."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    
    # Skip if already has file-level anonymous namespace wrapping
    # Check for "namespace {" at file scope (not indented, not inside a function)
    for line in content.split('\n'):
        stripped = line.strip()
        if stripped == 'namespace {' or stripped.startswith('namespace { //'):
            print(f"  SKIP (already has anonymous namespace): {filepath}")
            return False

    lines = content.split('\n')
    
    entry_start = find_entry_point(lines)
    if entry_start == -1:
        print(f"  SKIP (no entry point): {filepath}")
        return False
    
    last_include = find_last_include(lines)
    if last_include == -1:
        print(f"  SKIP (no includes): {filepath}")
        return False
    
    entry_end = find_entry_point_end(lines, entry_start)
    
    # Find first non-blank line after last include (start of code to wrap)
    code_start = last_include + 1
    while code_start < len(lines) and lines[code_start].strip() == '':
        code_start += 1
    
    # Check if there's code between includes and entry point
    has_code_before = code_start < entry_start
    
    # Check if there's code after entry point (non-blank, non-comment)
    has_code_after = False
    for i in range(entry_end + 1, len(lines)):
        stripped = lines[i].strip()
        if stripped and not stripped.startswith('//'):
            has_code_after = True
            break
    
    if not has_code_before and not has_code_after:
        print(f"  SKIP (no code to wrap): {filepath}")
        return False
    
    result = []
    
    if has_code_before:
        # Add lines up to and including blank lines after includes
        result.extend(lines[:code_start])
        # Open anonymous namespace
        result.append('namespace {')
        # Add code between includes and entry point
        result.extend(lines[code_start:entry_start])
        # Close anonymous namespace
        result.append('} // namespace')
        result.append('')
    else:
        result.extend(lines[:entry_start])
    
    # Add entry point function (at top level)
    result.extend(lines[entry_start:entry_end + 1])
    
    if has_code_after:
        result.append('')
        result.append('namespace {')
        result.extend(lines[entry_end + 1:])
        result.append('} // namespace')
    else:
        result.extend(lines[entry_end + 1:])
    
    new_content = '\n'.join(result)
    if new_content == content:
        print(f"  SKIP (no change): {filepath}")
        return False
    
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(new_content)
    print(f"  FIXED: {filepath}")
    return True


def fix_count_new_h(filepath):
    """Fix count_new.h by wrapping global definitions in anonymous namespace."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    
    if 'namespace {' in content:
        print(f"  SKIP (already has anonymous namespace): {filepath}")
        return False
    
    # Wrap the section from MemCounter::disable_checking through the end of
    # operator replacements (#endif // DISABLE_NEW_COUNT) in anonymous namespace.
    # This prevents ODR violations when multiple .cpp files include this header.
    
    # Find the static const member definition
    pattern1 = re.compile(r'^(#ifdef DISABLE_NEW_COUNT)\s*$', re.MULTILINE)
    # Find the end of operator replacements
    pattern2 = re.compile(r'^(#endif // DISABLE_NEW_COUNT)\s*$', re.MULTILINE)
    
    match1 = pattern1.search(content)
    match2 = pattern2.search(content)
    
    if not match1 or not match2:
        print(f"  SKIP (patterns not found): {filepath}")
        return False
    
    # Insert "namespace {" before #ifdef DISABLE_NEW_COUNT
    # Insert "} // namespace" after #endif // DISABLE_NEW_COUNT
    insert_pos1 = match1.start()
    insert_pos2 = match2.end()
    
    new_content = (content[:insert_pos1] + 
                   'namespace {\n' + 
                   content[insert_pos1:insert_pos2] + 
                   '\n} // namespace\n' + 
                   content[insert_pos2:])
    
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(new_content)
    print(f"  FIXED: {filepath}")
    return True


def main():
    base_dir = os.path.dirname(os.path.abspath(__file__))
    algo_dir = os.path.join(base_dir, 'std', 'algorithms')
    
    fixed_count = 0
    
    # Fix all .pass.cpp files in the algorithms directory
    print("Fixing algorithm test files...")
    for root, dirs, files in os.walk(algo_dir):
        for fname in sorted(files):
            if fname.endswith('.pass.cpp'):
                filepath = os.path.join(root, fname)
                if fix_pass_cpp_file(filepath):
                    fixed_count += 1
    
    # Fix count_new.h
    print("\nFixing support/count_new.h...")
    count_new_path = os.path.join(base_dir, 'support', 'count_new.h')
    if os.path.exists(count_new_path):
        if fix_count_new_h(count_new_path):
            fixed_count += 1
    
    print(f"\nTotal files fixed: {fixed_count}")


if __name__ == '__main__':
    main()