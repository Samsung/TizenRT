#!/usr/bin/env python3
"""
libc++ Test Conversion Script for TizenRT
Converts libcxx-17.0.6 test files to TizenRT-compatible format

This script:
1. Adds Samsung/Apache license headers
2. Converts int main(int, char**) to int tc_libcxx_<test_name>(void)
3. Replaces assert() with TC_ASSERT_EXPR()
4. Wraps exception code with #ifndef _LIBCPP_NO_EXCEPTIONS
5. Adds TC_SUCCESS_RESULT() at test end
6. Generates Makefile entries automatically
"""

import os
import re
import sys
import argparse
from pathlib import Path
from typing import List, Tuple, Set

# TizenRT license header
TIZENRT_LICENSE = '''/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
'''

# Features to exclude (based on __config_site)
EXCLUDED_FEATURES = [
    '_LIBCPP_HAS_NO_FILESYSTEM',
    '_LIBCPP_HAS_NO_WIDE_CHARACTERS',
    '_LIBCPP_HAS_NO_RANDOM_DEVICE',
    '_LIBCPP_HAS_NO_SYSTEM_ERROR',
    '_LIBCPP_HAS_NO_UNICODE',
    '_LIBCPP_HAS_NO_STDIN',
    '_LIBCPP_HAS_NO_STDOUT',
    '_LIBCPP_HAS_NO_TIME_UTC',
    '_LIBCPP_HAS_NO_RANGES',
]

# Patterns to identify tests that should be excluded
EXCLUSION_PATTERNS = [
    r'wchar_t',
    r'wchar',
    r'wint_t',
    r'wstream',
    r'wcout',
    r'wcin',
    r'wcerr',
    r'wclog',
    r'wfstream',
    r'wifstream',
    r'wofstream',
    r'wstringstream',
    r'wistringstream',
    r'wostringstream',
    r'wstring',
    r'filesystem',
    r'random_device',
    r'stderr',
    r'stdin',
    r'stdout',
    r'fopen',
    r'fread',
    r'fwrite',
    r'fseek',
    r'ftell',
    r'fgetc',
    r'fgets',
    r'fputc',
    r'fputs',
    r'fscanf',
    r'fprintf',
    r'printf\s*\(',
    r'scanf\s*\(',
    r'getchar',
    r'putchar',
    r'gets\s*\(',
    r'puts\s*\(',
    r'cout',
    r'cin',
    r'cerr',
    r'clog',
    r'istream',
    r'ostream',
    r'ifstream',
    r'ofstream',
    r'fstream',
    r'streambuf',
    r'stringstream',
    r'istringstream',
    r'ostringstream',
    r'filebuf',
    r'std::ranges::',
    r'ranges::',
    r'view::',
    r'views::',
]

def should_exclude_test(content: str, filepath: str) -> Tuple[bool, str]:
    """Check if a test should be excluded based on content or path."""
    # Check for wide character tests
    if 'wchar_t' in filepath or 'wchar' in filepath.lower():
        return True, "Wide character test"
    
    # Check for filesystem tests
    if 'filesystem' in filepath:
        return True, "Filesystem test"
    
    # Check for input.output category (most I/O tests excluded)
    if 'input.output' in filepath:
        # Allow some specific I/O tests
        allowed_io = ['sstream', 'strstream']
        if not any(x in filepath for x in allowed_io):
            return True, "I/O test (stdin/stdout disabled)"
    
    # Check for random device tests
    if 'rand.device' in filepath or 'random_device' in content:
        return True, "Random device test"
    
    # Check for ranges tests
    if 'ranges' in filepath and 'std/ranges' in filepath:
        # Check if it uses ranges features
        if re.search(r'std::ranges::|ranges::|view::|views::', content):
            return True, "Ranges test"
    
    # Check for wide character usage in content
    if re.search(r'\bwchar_t\b', content):
        # Check if it's a primary focus (not just in comments)
        lines_with_wchar = [l for l in content.split('\n') if 'wchar_t' in l and not l.strip().startswith('//')]
        if len(lines_with_wchar) > 2:
            return True, "Wide character test"
    
    return False, ""

def convert_main_function(content: str, test_name: str) -> str:
    """Convert main function to TizenRT test function."""
    # Pattern for main function
    main_pattern = r'int\s+main\s*\(\s*(?:int\s*,\s*char\s*\*\*\s*)?\)\s*\{'
    
    # Generate test function name
    func_name = f"tc_{test_name}"
    
    # Replace main with test function
    content = re.sub(main_pattern, f'int {func_name}(void) {{', content)
    
    return content, func_name

def convert_asserts(content: str) -> str:
    """Convert assert() calls to TC_ASSERT_EXPR()."""
    # Pattern for assert
    assert_pattern = r'\bassert\s*\('
    
    # Replace assert with TC_ASSERT_EXPR
    content = re.sub(assert_pattern, 'TC_ASSERT_EXPR(', content)
    
    return content

def wrap_exceptions(content: str) -> str:
    """Wrap exception-related code with #ifndef _LIBCPP_NO_EXCEPTIONS."""
    lines = content.split('\n')
    result = []
    in_exception_block = False
    exception_start = -1
    
    i = 0
    while i < len(lines):
        line = lines[i]
        
        # Check for try block start
        if re.match(r'^\s*try\s*\{', line):
            if not in_exception_block:
                result.append('#ifndef _LIBCPP_NO_EXCEPTIONS')
                in_exception_block = True
            result.append(line)
        
        # Check for catch block
        elif re.match(r'^\s*}\s*catch\s*\(', line):
            result.append(line)
        
        # Check for throw statement
        elif re.match(r'^\s*throw\s+', line) or re.match(r'^\s*throw\s*\(', line):
            if not in_exception_block:
                result.append('#ifndef _LIBCPP_NO_EXCEPTIONS')
                in_exception_block = True
            result.append(line)
        
        # Check for end of try/catch block
        elif in_exception_block and re.match(r'^\s*}\s*$', line):
            result.append(line)
            # Check if next line is not catch
            if i + 1 < len(lines) and not re.match(r'^\s*catch\s*\(', lines[i + 1]):
                result.append('#endif // _LIBCPP_NO_EXCEPTIONS')
                in_exception_block = False
        
        # Check for exception type usage
        elif re.search(r'\bstd::exception\b|\bstd::runtime_error\b|\bstd::logic_error\b|\bstd::out_of_range\b|\bstd::invalid_argument\b|\bstd::length_error\b|\bstd::domain_error\b|\bstd::range_error\b|\bstd::overflow_error\b|\bstd::underflow_error\b', line):
            if not in_exception_block and 'throw' in line:
                if not line.strip().startswith('//'):
                    result.append('#ifndef _LIBCPP_NO_EXCEPTIONS')
                    in_exception_block = True
            result.append(line)
        
        else:
            result.append(line)
        
        i += 1
    
    # Close any remaining open exception blocks
    if in_exception_block:
        result.append('#endif // _LIBCPP_NO_EXCEPTIONS')
    
    return '\n'.join(result)

def add_success_result(content: str, func_name: str) -> str:
    """Add TC_SUCCESS_RESULT() at the end of the test function."""
    # Find the function end (last return 0 or last })
    lines = content.split('\n')
    
    # Find the function body
    func_start = -1
    for i, line in enumerate(lines):
        if f'int {func_name}' in line:
            func_start = i
            break
    
    if func_start == -1:
        return content
    
    # Find the last return statement in the function
    last_return = -1
    brace_count = 0
    for i in range(func_start, len(lines)):
        line = lines[i]
        brace_count += line.count('{') - line.count('}')
        if brace_count == 0 and 'return 0;' in line:
            last_return = i
        if brace_count == 0 and i > func_start:
            break
    
    if last_return != -1:
        # Add TC_SUCCESS_RESULT before return 0
        indent = len(lines[last_return]) - len(lines[last_return].lstrip())
        lines.insert(last_return, ' ' * indent + 'TC_SUCCESS_RESULT();')
    
    return '\n'.join(lines)

def add_includes(content: str) -> str:
    """Add necessary TizenRT includes."""
    # Check if libcxx_tc_common.h is already included
    if 'libcxx_tc_common.h' in content:
        return content
    
    # Find the position after existing includes
    lines = content.split('\n')
    include_end = 0
    
    for i, line in enumerate(lines):
        if line.startswith('#include') or line.startswith('#include'):
            include_end = i + 1
        elif include_end > 0 and not line.startswith('#include') and not line.strip().startswith('//'):
            break
    
    # Add TizenRT test header
    tizenrt_include = '#include "libcxx_tc_common.h"'
    
    # Check if test_macros.h is included
    if 'test_macros.h' in content:
        # Add after test_macros.h
        for i, line in enumerate(lines):
            if 'test_macros.h' in line:
                lines.insert(i + 1, tizenrt_include)
                break
    else:
        # Add at include_end position
        if include_end > 0:
            lines.insert(include_end, tizenrt_include)
        else:
            # Add after license header
            for i, line in enumerate(lines):
                if line.strip() == '' and i > 20:
                    lines.insert(i, tizenrt_include)
                    break
    
    return '\n'.join(lines)

def convert_test_file(filepath: str, output_dir: str, src_base: str) -> Tuple[str, bool, str]:
    """Convert a single test file to TizenRT format."""
    # Read original file
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    # Check if should be excluded
    should_exclude, reason = should_exclude_test(content, filepath)
    if should_exclude:
        return filepath, False, f"Excluded: {reason}"
    
    # Generate test name from filepath
    rel_path = os.path.relpath(filepath, src_base)
    test_name = rel_path.replace(os.sep, '_').replace('.pass.cpp', '').replace('.cpp', '').replace('.', '_')
    test_name = test_name.replace('-', '_')
    
    # Convert content
    # 1. Replace LLVM license with TizenRT license
    # Remove existing license headers
    content = re.sub(r'//===---*---===//.*?//===---*---===//\s*\n', '', content, flags=re.DOTALL)
    content = re.sub(r'//.*?Part of the LLVM Project.*?\n', '', content)
    content = re.sub(r'//.*?SPDX-License-Identifier.*?\n', '', content)
    content = re.sub(r'//.*?See https://llvm.org/LICENSE.*?\n', '', content)
    content = re.sub(r'//.*?Dual licensed under the MIT.*?\n', '', content)
    content = re.sub(r'//.*?University of Illinois Open.*?\n', '', content)
    content = re.sub(r'//.*?Source Licenses\. See LICENSE\.TXT.*?\n', '', content)
    
    # Add TizenRT license
    content = TIZENRT_LICENSE + content
    
    # 2. Convert main function
    content, func_name = convert_main_function(content, test_name)
    
    # 3. Convert asserts
    content = convert_asserts(content)
    
    # 4. Add includes
    content = add_includes(content)
    
    # 5. Wrap exceptions
    content = wrap_exceptions(content)
    
    # 6. Add success result
    content = add_success_result(content, func_name)
    
    # Write output file
    output_path = os.path.join(output_dir, rel_path)
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(content)
    
    return rel_path, True, func_name

def generate_tc_main(tests: List[Tuple[str, str, str]], category: str, output_dir: str):
    """Generate tc_<category>.cpp file that calls all test functions."""
    tc_main_content = TIZENRT_LICENSE
    tc_main_content += f'''#include "libcxx_tc_common.h"

'''
    
    # Add extern declarations
    for rel_path, _, func_name in tests:
        tc_main_content += f'extern int {func_name}(void);\n'
    
    tc_main_content += f'''
extern "C"
{{
    int tc_{category}_main(void)
    {{
'''
    
    # Add function calls
    for rel_path, _, func_name in tests:
        tc_main_content += f'        {func_name}();\n'
    
    tc_main_content += '''        return 0;
    }
}
'''
    
    # Write tc_main file
    output_path = os.path.join(output_dir, f'std/{category}', f'tc_{category}.cpp')
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(tc_main_content)

def generate_makefile_entries(tests: List[Tuple[str, str, str]], category: str) -> str:
    """Generate Makefile entries for the tests."""
    makefile_content = f'''
ifeq ($(CONFIG_LIBCXX_UTC_{category.upper()}),y)
# {category} Source files
'''
    
    for rel_path, _, _ in tests:
        makefile_content += f'CXXSRCS += std/{category}/{rel_path}\n'
    
    makefile_content += f'''
# {category} TC Main
CXXSRCS += std/{category}/tc_{category}.cpp
endif
'''
    
    return makefile_content

def process_category(src_dir: str, output_dir: str, category: str) -> List[Tuple[str, str, str]]:
    """Process all tests in a category."""
    category_src = os.path.join(src_dir, 'std', category)
    category_output = os.path.join(output_dir, 'std', category)
    
    if not os.path.exists(category_src):
        print(f"Category directory not found: {category_src}")
        return []
    
    tests = []
    
    # Find all .pass.cpp files
    for root, dirs, files in os.walk(category_src):
        for file in files:
            if file.endswith('.pass.cpp') or (file.endswith('.cpp') and 'test' in file.lower()):
                filepath = os.path.join(root, file)
                rel_path, success, result = convert_test_file(filepath, output_dir, os.path.join(src_dir, 'std'))
                
                if success:
                    tests.append((rel_path, filepath, result))
                    print(f"Converted: {rel_path}")
                else:
                    print(f"Skipped: {filepath} - {result}")
    
    return tests

def main():
    parser = argparse.ArgumentParser(description='Convert libc++ tests to TizenRT format')
    parser.add_argument('--src', required=True, help='Source directory (libcxx-17.0.6.src/test)')
    parser.add_argument('--output', required=True, help='Output directory (TizenRT/external/libcxx-test)')
    parser.add_argument('--category', help='Process only this category (e.g., algorithms)')
    parser.add_argument('--generate-makefile', action='store_true', help='Generate Makefile entries')
    
    args = parser.parse_args()
    
    src_dir = args.src
    output_dir = args.output
    
    # List of categories to process
    categories = [
        'algorithms',
        'atomics',
        'concepts',
        'containers',
        'depr',
        'diagnostics',
        'experimental',
        'input.output',
        'iterators',
        'language.support',
        'library',
        'localization',
        'numerics',
        'ranges',
        're',
        'strings',
        'thread',
        'time',
        'utilities',
    ]
    
    if args.category:
        categories = [args.category]
    
    all_tests = {}
    
    for category in categories:
        print(f"\nProcessing category: {category}")
        tests = process_category(src_dir, output_dir, category)
        all_tests[category] = tests
        
        if tests:
            # Generate tc_main
            generate_tc_main(tests, category.replace('.', '_').replace('-', '_'), output_dir)
            
            if args.generate_makefile:
                makefile_entries = generate_makefile_entries(tests, category.replace('.', '_'))
                print(f"\nMakefile entries for {category}:")
                print(makefile_entries)
    
    # Summary
    print("\n" + "="*50)
    print("Conversion Summary")
    print("="*50)
    for category, tests in all_tests.items():
        print(f"{category}: {len(tests)} tests converted")
    print(f"\nTotal: {sum(len(t) for t in all_tests.values())} tests converted")

if __name__ == '__main__':
    main()