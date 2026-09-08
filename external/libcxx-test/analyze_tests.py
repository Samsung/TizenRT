#!/usr/bin/env python3
"""
Script to analyze and synchronize test files, tc_*.cpp, and Makefile
"""
import os
import re
import glob

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
STD_DIR = os.path.join(BASE_DIR, 'std')

def get_test_files(category):
    """Get all .pass.cpp files in a category directory"""
    category_dir = os.path.join(STD_DIR, category)
    if not os.path.exists(category_dir):
        return []
    
    files = []
    for root, dirs, filenames in os.walk(category_dir):
        for f in filenames:
            if f.endswith('.pass.cpp') and not f.endswith('.o'):
                rel_path = os.path.relpath(os.path.join(root, f), STD_DIR)
                files.append(rel_path.replace('\\', '/'))
    return sorted(files)

def get_makefile_entries(makefile_path, category):
    """Extract test file entries from Makefile for a category"""
    with open(makefile_path, 'r') as f:
        content = f.read()
    
    # Find the section for this category
    pattern = rf'# {category.title()} Tests.*?(?=# [A-Z]|\Z)'
    match = re.search(pattern, content, re.DOTALL | re.IGNORECASE)
    if not match:
        return []
    
    section = match.group(0)
    # Extract CXXSRCS entries
    entries = re.findall(r'CXXSRCS \+= std/(\S+\.pass\.cpp)', section)
    return sorted(entries)

def get_tc_functions(tc_cpp_path):
    """Extract function names from tc_*.cpp"""
    with open(tc_cpp_path, 'r') as f:
        content = f.read()
    
    # Find extern declarations
    pattern = r'extern int (tc_\w+)\(void\);'
    functions = re.findall(pattern, content)
    return sorted(set(functions))

def file_to_function_name(file_path):
    """Convert file path to expected function name"""
    # Remove .pass.cpp extension
    name = file_path.replace('.pass.cpp', '')
    # Replace path separators and special chars with underscores
    name = name.replace('/', '_').replace('.', '_').replace('-', '_')
    # Add prefix
    return f'tc_algorithms_{name}'

def main():
    categories = ['algorithms', 'containers', 'utilities', 'diagnostics', 
                  'numerics', 'thread', 'iterators', 'strings', 'depr', 
                  'language.support']
    
    print("=" * 60)
    print("Analysis of libcxx-test synchronization")
    print("=" * 60)
    
    for category in categories:
        tc_cpp = os.path.join(STD_DIR, category, f'tc_{category}.cpp')
        
        if not os.path.exists(tc_cpp):
            print(f"\n{category}: tc_{category}.cpp not found")
            continue
        
        test_files = get_test_files(category)
        tc_functions = get_tc_functions(tc_cpp)
        
        # Map files to function names
        file_to_func = {}
        for f in test_files:
            func_name = file_to_function_name(f)
            file_to_func[f] = func_name
        
        # Find functions in tc_cpp that don't have corresponding files
        all_expected_funcs = set(file_to_func.values())
        declared_funcs = set(tc_functions)
        
        missing_files = declared_funcs - all_expected_funcs
        extra_files = all_expected_funcs - declared_funcs
        
        print(f"\n{category}:")
        print(f"  Test files found: {len(test_files)}")
        print(f"  Functions declared in tc_{category}.cpp: {len(tc_functions)}")
        
        if missing_files:
            print(f"  Functions WITHOUT corresponding test files: {len(missing_files)}")
            for f in sorted(missing_files)[:5]:
                print(f"    - {f}")
            if len(missing_files) > 5:
                print(f"    ... and {len(missing_files) - 5} more")
        
        if extra_files:
            print(f"  Test files NOT called in tc_{category}.cpp: {len(extra_files)}")

if __name__ == '__main__':
    main()