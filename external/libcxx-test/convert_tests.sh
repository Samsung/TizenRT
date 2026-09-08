#!/bin/bash
#
# libc++ Test Conversion Script for TizenRT
# Converts libcxx-17.0.6 test files to TizenRT-compatible format
#

set -e

SRCDIR=""
OUTPUTDIR=""
CATEGORY="algorithms"

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --src)
            SRCDIR="$2"
            shift 2
            ;;
        --output)
            OUTPUTDIR="$2"
            shift 2
            ;;
        --category)
            CATEGORY="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

if [ -z "$SRCDIR" ] || [ -z "$OUTPUTDIR" ]; then
    echo "Usage: $0 --src <src_dir> --output <output_dir> [--category <category>]"
    exit 1
fi

# TizenRT license header
TIZENRT_LICENSE='/****************************************************************************
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
'

# Exclusion patterns (grep regex)
EXCLUDE_PATTERNS="wchar|wchar_t|wstream|wcout|wcin|wcerr|wclog|wstring|filesystem|random_device"

# Function to check if test should be excluded
should_exclude() {
    local file="$1"
    local content="$2"
    
    # Check filepath for exclusion patterns
    if echo "$file" | grep -qE "$EXCLUDE_PATTERNS"; then
        return 0  # Exclude
    fi
    
    # Check for filesystem
    if echo "$content" | grep -q "filesystem"; then
        return 0
    fi
    
    # Check for ranges
    if echo "$content" | grep -qE "std::ranges::|ranges::|view::|views::"; then
        return 0
    fi
    
    return 1  # Don't exclude
}

# Function to convert a single test file
convert_test_file() {
    local src_file="$1"
    local src_base="$2"
    
    # Get relative path
    local rel_path="${src_file#$src_base/}"
    
    # Read content
    local content
    content=$(cat "$src_file")
    
    # Check exclusion
    if should_exclude "$src_file" "$content"; then
        echo "SKIP: $rel_path"
        return
    fi
    
    # Generate function name
    local func_name="tc_$(echo "$rel_path" | sed 's/[\/.]/_/g; s/-/_/g; s/_pass_cpp$//; s/_cpp$//')"
    
    # Create output path
    local out_file="$OUTPUTDIR/std/$rel_path"
    local out_dir
    out_dir=$(dirname "$out_file")
    mkdir -p "$out_dir"
    
    # Write license header
    echo "$TIZENRT_LICENSE" > "$out_file"
    
    # Process content
    echo "$content" | \
        # Remove LLVM license headers
        sed '/\/\/===---.*---===\/\//d' | \
        sed '/\/\/.*Part of the LLVM Project/d' | \
        sed '/\/\/.*SPDX-License-Identifier/d' | \
        sed '/\/\/.*See https:\/\/llvm.org\/LICENSE/d' | \
        sed '/\/\/.*Dual licensed under the MIT/d' | \
        sed '/\/\/.*University of Illinois Open/d' | \
        # Convert main function
        sed "s/int main(int, char\*\*)/int $func_name(void)/g" | \
        sed "s/int main(int argc, char\* argv\[\])/int $func_name(void)/g" | \
        sed "s/int main()/int $func_name(void)/g" | \
        # Convert asserts
        sed 's/\bassert(/TC_ASSERT_EXPR(/g' | \
        # Add libcxx_tc_common.h include
        sed 's/#include "test_macros.h"/#include "test_macros.h"\n#include "libcxx_tc_common.h"/g' | \
        sed 's/#include <cassert>/#include <cassert>\n#include "libcxx_tc_common.h"/g' \
        >> "$out_file"
    
    echo "CONVERT: $rel_path -> $func_name"
    
    # Return function name for later use
    echo "FUNC:$func_name" >> /tmp/converted_funcs.txt
    echo "FILE:$rel_path" >> /tmp/converted_files.txt
}

# Main processing
echo "Processing category: $CATEGORY"

# Clear temp files
rm -f /tmp/converted_funcs.txt /tmp/converted_files.txt

# Find and convert all .pass.cpp files
src_category="$SRCDIR/std/$CATEGORY"
if [ ! -d "$src_category" ]; then
    echo "Category directory not found: $src_category"
    exit 1
fi

find "$src_category" -name "*.pass.cpp" | while read -r file; do
    convert_test_file "$file" "$SRCDIR/std"
done

# Generate summary
if [ -f /tmp/converted_funcs.txt ]; then
    count=$(wc -l < /tmp/converted_funcs.txt)
    echo ""
    echo "=================================================="
    echo "Conversion Summary"
    echo "=================================================="
    echo "$CATEGORY: $count tests converted"
    echo ""
    
    echo "Function declarations for tc_${CATEGORY}.hpp:"
    while IFS= read -r line; do
        echo "int ${line#FUNC:}(void);"
    done < /tmp/converted_funcs.txt
    
    echo ""
    echo "Function calls for tc_${CATEGORY}.cpp:"
    while IFS= read -r line; do
        echo "${line#FUNC:}();"
    done < /tmp/converted_funcs.txt
    
    echo ""
    echo "Makefile entries:"
    while IFS= read -r line; do
        echo "CXXSRCS += std/${line#FILE:}"
    done < /tmp/converted_files.txt
fi

# Cleanup
rm -f /tmp/converted_funcs.txt /tmp/converted_files.txt

echo ""
echo "Conversion complete!"