# PowerShell script to convert libcxx-17.0.6 algorithm tests to TizenRT format
# Run on Windows where the source files are accessible

$ErrorActionPreference = "Stop"

$LIBCXX_SRC = "W:\libcxx-17.0.6.src\test\std\algorithms"
$OUTPUT_DIR = "external\libcxx-test\std\algorithms"

# TizenRT license header
$LICENSE = @"
/****************************************************************************
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
"@

# Exclusion patterns
$EXCLUDE_PATTERNS = @(
    'ranges.',
    'pstl.',
    'compile.pass.cpp',
    'assert.',
    'tested_elsewhere',
    'nothing_to_do',
    'no_unique_address',
    'contiguous_trivial_optimization',
    'complexity.pass.cpp',
    'verify.cpp'
)

# Function to check if a file should be excluded
function Should-Exclude($filename) {
    foreach ($pattern in $EXCLUDE_PATTERNS) {
        if ($filename -match $pattern) { return $true }
    }
    return $false
}

# Function to generate test name from relative path
function Get-TestName($relPath) {
    # Remove .pass.cpp extension
    $name = $relPath -replace '\.pass\.cpp$', ''
    # Replace path separators and dots with underscores
    $name = $name -replace '[\\/.]', '_'
    # Replace hyphens with underscores
    $name = $name -replace '-', '_'
    # Sanitize operator symbols
    $name = $name -replace '==', '_eq_eq'
    $name = $name -replace '!=', '_neq'
    $name = $name -replace '<=', '_leq'
    $name = $name -replace '>=', '_geq'
    $name = $name -replace '\+\+', '_plus_plus'
    $name = $name -replace '--', '_minus_minus'
    # Clean up duplicate underscores
    while ($name -match '__') { $name = $name -replace '__', '_' }
    $name = $name.Trim('_')
    return "tc_libcxx_algorithms_$name"
}

# Function to determine config guard
function Get-ConfigGuard($relPath) {
    if ($relPath -match 'alg\.nonmodifying') { return 'CONFIG_LIBCXX_UTC_ALGORITHMS_NONMOD' }
    if ($relPath -match 'alg\.modifying\.operations') { return 'CONFIG_LIBCXX_UTC_ALGORITHMS_MOD' }
    if ($relPath -match 'alg\.sorting') { return 'CONFIG_LIBCXX_UTC_ALGORITHMS_SORT' }
    if ($relPath -match 'alg\.c\.library') { return 'CONFIG_LIBCXX_UTC_ALGORITHMS_CLIB' }
    if ($relPath -match 'numeric\.ops') { return 'CONFIG_LIBCXX_UTC_ALGORITHMS_NUMERIC' }
    if ($relPath -match 'algorithms\.results') { return 'CONFIG_LIBCXX_UTC_ALGORITHMS' }
    if ($relPath -match 'algorithms\.general') { return 'CONFIG_LIBCXX_UTC_ALGORITHMS' }
    return 'CONFIG_LIBCXX_UTC_ALGORITHMS'
}

# Collect all .pass.cpp files
$allFiles = Get-ChildItem -Path $LIBCXX_SRC -Filter "*.pass.cpp" -Recurse
$convertedFiles = @()
$excludedCount = 0

foreach ($file in $allFiles) {
    $filename = $file.Name
    
    if (Should-Exclude $filename) {
        $excludedCount++
        Write-Host "  Skipped: $($file.FullName | Split-Path -Leaf)" -ForegroundColor DarkGray
        continue
    }
    
    # Read content
    $content = Get-Content $file.FullName -Raw -Encoding UTF8
    
    # Check for excluded includes
    $excludeForContent = $false
    $excludedIncludes = @('<iostream>', '<fstream>', '<filesystem>', '<locale>', '<codecvt>', '<regex>', '<stop_token>', '<coroutine>', '<semaphore>', '<latch>', '<barrier>')
    foreach ($inc in $excludedIncludes) {
        if ($content -match [regex]::Escape("#include $inc")) {
            $excludeForContent = $true
            break
        }
    }
    
    if ($excludeForContent) {
        $excludedCount++
        Write-Host "  Skipped (excluded include): $($file.FullName | Split-Path -Leaf)" -ForegroundColor DarkGray
        continue
    }
    
    # Get relative path from source root
    $relPath = $file.FullName.Substring($LIBCXX_SRC.Length + 1)
    
    # Generate test name
    $testName = Get-TestName $relPath
    
    # Get config guard
    $guard = Get-ConfigGuard $relPath
    
    # Convert content
    # 1. Remove LLVM license headers
    $content = $content -replace '//===---.*?---===//\s*\n', ''
    $content = $content -replace '//\s*Part of the LLVM Project.*?\n', ''
    $content = $content -replace '//\s*SPDX-License-Identifier.*?\n', ''
    $content = $content -replace '//\s*See https://llvm.org/LICENSE.*?\n', ''
    $content = $content -replace '//\s*Dual licensed under the MIT.*?\n', ''
    $content = $content -replace '//\s*University of Illinois Open.*?\n', ''
    $content = $content -replace '//\s*Source Licenses\. See LICENSE\.TXT.*?\n', ''
    
    # 2. Add TizenRT license
    $content = $LICENSE + $content
    
    # 3. Convert main function
    $content = $content -replace 'int\s+main\s*\(\s*int\s+argc\s*,\s*char\s*\*\*\s*argv\s*\)\s*\{', "int $testName(void) {"
    $content = $content -replace 'int\s+main\s*\(\s*int\s*,\s*char\s*\*\*\s*\)\s*\{', "int $testName(void) {"
    $content = $content -replace 'int\s+main\s*\(\s*\)\s*\{', "int $testName(void) {"
    
    # 4. Convert assert to TC_ASSERT_EXPR
    $content = $content -replace '\bassert\s*\(', 'TC_ASSERT_EXPR('
    
    # 5. Add TizenRT include (after last #include)
    $lastIncludeIdx = -1
    $lines = $content -split "`n"
    for ($i = 0; $i -lt $lines.Length; $i++) {
        if ($lines[$i] -match '^\s*#include') { $lastIncludeIdx = $i }
    }
    if ($lastIncludeIdx -ge 0) {
        $lines = $lines[0..$lastIncludeIdx] + '#include "libcxx_tc_common.h"' + $lines[($lastIncludeIdx + 1)..($lines.Length - 1)]
    }
    $content = $lines -join "`n"
    
    # 6. Add TC_SUCCESS_RESULT before return 0
    $content = $content -replace '(\s+)return 0;', "`$1TC_SUCCESS_RESULT();`n`$1return 0;"
    
    # Write output file
    $outputPath = Join-Path $OUTPUT_DIR $relPath
    $outputDir = Split-Path $outputPath -Parent
    if (-not (Test-Path $outputDir)) { New-Item -ItemType Directory -Path $outputDir -Force | Out-Null }
    
    Set-Content -Path $outputPath -Value $content -Encoding UTF8 -NoNewline
    
    $convertedFiles += @{
        RelPath = $relPath
        TestName = $testName
        Guard = $guard
    }
    
    Write-Host "  Converted: $relPath -> $testName" -ForegroundColor Green
}

Write-Host "`nTotal: $($convertedFiles.Count) converted, $excludedCount excluded" -ForegroundColor Cyan

# Generate tc_algorithms.hpp
$hppContent = $LICENSE
$hppContent += "#ifndef __EXTERNAL_LIBCXX_TEST_TC_ALGORITHMS_H`n"
$hppContent += "#define __EXTERNAL_LIBCXX_TEST_TC_ALGORITHMS_H`n`n"

# Group by guard
$guardGroups = $convertedFiles | Group-Object { $_.Guard }
foreach ($group in $guardGroups) {
    $guard = $group.Name
    $hppContent += "#ifdef $guard`n"
    foreach ($item in $group.Group) {
        $hppContent += "int $($item.TestName)(void);`n"
    }
    $hppContent += "#endif`n`n"
}
$hppContent += "#endif`n"

Set-Content -Path "$OUTPUT_DIR\tc_algorithms.hpp" -Value $hppContent -Encoding UTF8

# Generate tc_algorithms.cpp
$cppContent = $LICENSE
$cppContent += '#include "libcxx_tc_common.h"' + "`n"
$cppContent += '#include "tc_algorithms.hpp"' + "`n`n"
$cppContent += 'extern "C"' + "`n"
$cppContent += "{`n"
$cppContent += "`tint tc_algorithms_main(void)`n"
$cppContent += "`t{`n"

foreach ($group in $guardGroups) {
    $guard = $group.Name
    $cppContent += "#ifdef $guard`n"
    foreach ($item in $group.Group) {
        $cppContent += "`t`t$($item.TestName)();`n"
    }
    $cppContent += "#endif`n`n"
}

$cppContent += "`t`treturn 0;`n"
$cppContent += "`t}`n"
$cppContent += "}`n"

Set-Content -Path "$OUTPUT_DIR\tc_algorithms.cpp" -Value $cppContent -Encoding UTF8

# Generate Makefile snippet
$mfContent = "`n# algorithms test sources`n"
$mfContent += "ifeq (`$(CONFIG_LIBCXX_UTC_ALGORITHMS),y)`n"

foreach ($group in $guardGroups) {
    $guard = $group.Name
    if ($guard -ne 'CONFIG_LIBCXX_UTC_ALGORITHMS') {
        $mfContent += "ifeq (`$($guard),y)`n"
    }
    foreach ($item in $group.Group) {
        $mfContent += "CXXSRCS += std/algorithms/$($item.RelPath -replace '\\','/')`n"
    }
    if ($guard -ne 'CONFIG_LIBCXX_UTC_ALGORITHMS') {
        $mfContent += "endif`n"
    }
}

$mfContent += "CXXSRCS += std/algorithms/tc_algorithms.cpp`n"
$mfContent += "endif`n"

Set-Content -Path "external\libcxx-test\makefile_algorithms.snippet" -Value $mfContent -Encoding UTF8

Write-Host "`nGenerated files:" -ForegroundColor Yellow
Write-Host "  $OUTPUT_DIR\tc_algorithms.hpp"
Write-Host "  $OUTPUT_DIR\tc_algorithms.cpp"
Write-Host "  external\libcxx-test\makefile_algorithms.snippet"
Write-Host "`nDone! Next steps:"
Write-Host "1. Review the generated files"
Write-Host "2. Add Makefile entries to external/libcxx-test/Makefile"
Write-Host "3. Build and fix compilation issues"