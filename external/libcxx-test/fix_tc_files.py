#!/usr/bin/env python3
"""
Fix tc_*.cpp/hpp files to only reference functions from .pass.cpp files
that are actually compiled in the Makefile (outside DISABLE_NEW_TESTS block).

This resolves linker errors where tc files reference functions whose
implementations don't exist on disk.
"""

import os
import re
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

def sanitize_name(name):
    """Convert a path component to a valid C++ identifier part."""
    # Replace dots, dashes, and other special chars with underscore
    name = name.replace('.', '_').replace('-', '_').replace(' ', '_')
    # Remove any double underscores
    while '__' in name:
        name = name.replace('__', '_')
    return name.strip('_')

def path_to_func_name(category, rel_path):
    """
    Convert a .pass.cpp relative path to a test function name.
    e.g., 'std/utilities/memory/smartptr_shared_common.cpp' -> 'tc_utilities_memory_smartptr_shared_common'
    """
    # Remove 'std/<category>/' prefix
    prefix = f'std/{category}/'
    if rel_path.startswith(prefix):
        rel_path = rel_path[len(prefix):]
    
    # Remove .pass.cpp or .cpp extension
    for ext in ['.pass.cpp', '.cpp']:
        if rel_path.endswith(ext):
            rel_path = rel_path[:-len(ext)]
            break
    
    # Split path and sanitize each part
    parts = rel_path.replace('\\', '/').split('/')
    sanitized = [sanitize_name(p) for p in parts if p]
    
    # Build function name: tc_<category>_<path_parts>
    func_name = 'tc_' + category + '_' + '_'.join(sanitized)
    
    # Clean up multiple underscores
    while '__' in func_name:
        func_name = func_name.replace('__', '_')
    
    return func_name

def parse_makefile_for_sources(makefile_path):
    """Parse Makefile to find .pass.cpp CXXSRCS entries outside DISABLE_NEW_TESTS block."""
    with open(makefile_path, 'r') as f:
        content = f.read()
    
    # Find the DISABLE_NEW_TESTS block and exclude it
    # Everything before 'ifeq ($(DISABLE_NEW_TESTS),n)' is active
    disable_marker = 'ifeq ($(DISABLE_NEW_TESTS),n)'
    if disable_marker in content:
        active_content = content[:content.index(disable_marker)]
    else:
        active_content = content
    
    # Find all CXXSRCS += lines with .pass.cpp files
    sources = []
    for line in active_content.split('\n'):
        line = line.strip()
        if 'CXXSRCS +=' in line and '.pass.cpp' in line:
            # Extract the path after CXXSRCS +=
            match = re.search(r'CXXSRCS\s*\+=\s*(\S+)', line)
            if match:
                sources.append(match.group(1))
    
    return sources

def get_existing_pass_files(category_dir):
    """Find all .pass.cpp files that exist on disk for a category."""
    pass_files = []
    for root, dirs, files in os.walk(category_dir):
        for f in files:
            if f.endswith('.pass.cpp'):
                rel = os.path.relpath(os.path.join(root, f), SCRIPT_DIR)
                rel = rel.replace('\\', '/')
                pass_files.append(rel)
    return pass_files

def generate_tc_files(category, sources, output_dir):
    """Generate tc_<category>.cpp and tc_<category>.hpp files."""
    
    # Derive function names from sources
    func_names = []
    for src in sources:
        if not src.endswith('.pass.cpp'):
            continue
        func_name = path_to_func_name(category, src)
        func_names.append(func_name)
    
    # Sort for consistency
    func_names.sort()
    
    # Remove duplicates
    seen = set()
    unique_funcs = []
    for f in func_names:
        if f not in seen:
            seen.add(f)
            unique_funcs.append(f)
    func_names = unique_funcs
    
    # Generate header
    header_guard = f'TC_{category.upper()}_HPP'
    header_lines = []
    header_lines.append(f'#ifndef {header_guard}')
    header_lines.append(f'#define {header_guard}')
    header_lines.append('')
    header_lines.append('#include <tinyara/config.h>')
    header_lines.append('')
    
    for func in func_names:
        header_lines.append(f'extern int {func}(void);')
    
    header_lines.append('')
    header_lines.append(f'#endif /* {header_guard} */')
    header_lines.append('')
    
    # Generate cpp
    cpp_lines = []
    cpp_lines.append(f'#include "tc_{category}.hpp"')
    cpp_lines.append('')
    cpp_lines.append(f'extern "C" {{')
    cpp_lines.append(f'#include <tc_common.h>')
    cpp_lines.append('}')
    cpp_lines.append('')
    cpp_lines.append(f'extern int tc_{category}_main(void)')
    cpp_lines.append('{')
    
    if func_names:
        for func in func_names:
            cpp_lines.append(f'\t{func}();')
    else:
        cpp_lines.append(f'\t/* No test files available for this category */')
    
    cpp_lines.append('}')
    cpp_lines.append('')
    
    # Write files
    hpp_path = os.path.join(output_dir, f'tc_{category}.hpp')
    cpp_path = os.path.join(output_dir, f'tc_{category}.cpp')
    
    with open(hpp_path, 'w') as f:
        f.write('\n'.join(header_lines))
    
    with open(cpp_path, 'w') as f:
        f.write('\n'.join(cpp_lines))
    
    return len(func_names)

def main():
    makefile_path = os.path.join(SCRIPT_DIR, 'Makefile')
    
    print("Parsing Makefile for active .pass.cpp sources...")
    sources = parse_makefile_for_sources(makefile_path)
    print(f"Found {len(sources)} active .pass.cpp sources in Makefile")
    
    # Categorize sources
    categories = {}
    for src in sources:
        # Source paths are like: std/algorithms/... or std/utilities/... or std/containers/...
        parts = src.split('/')
        if len(parts) >= 2 and parts[0] == 'std':
            cat = parts[1]
            if cat not in categories:
                categories[cat] = []
            categories[cat].append(src)
    
    for cat, cat_sources in sorted(categories.items()):
        print(f"  {cat}: {len(cat_sources)} .pass.cpp files")
    
    # Generate tc files for each category
    for cat in ['algorithms', 'utilities', 'containers', 'diagnostics', 'numerics', 'thread', 'iterators', 'strings']:
        cat_dir = os.path.join(SCRIPT_DIR, 'std', cat)
        if not os.path.isdir(cat_dir):
            print(f"Skipping {cat}: directory not found")
            continue
        
        cat_sources = categories.get(cat, [])
        # Filter to only .pass.cpp files
        pass_sources = [s for s in cat_sources if s.endswith('.pass.cpp')]
        
        count = generate_tc_files(cat, pass_sources, cat_dir)
        print(f"Generated tc_{cat}.cpp/hpp with {count} test functions")
    
    # Also handle the special categories (depr, language.support)
    for cat in ['depr', 'language.support']:
        cat_dir = os.path.join(SCRIPT_DIR, 'std', cat)
        if not os.path.isdir(cat_dir):
            continue
        cat_sources = categories.get(cat, [])
        pass_sources = [s for s in cat_sources if s.endswith('.pass.cpp')]
        if pass_sources:
            count = generate_tc_files(cat, pass_sources, cat_dir)
            print(f"Generated tc_{cat}.cpp/hpp with {count} test functions")
    
    print("\nDone! tc files have been regenerated to match active Makefile entries.")

if __name__ == '__main__':
    main()