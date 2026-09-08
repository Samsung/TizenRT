<#
.SYNOPSIS
    libc++ Test Conversion Script for TizenRT
    Converts libcxx-17.0.6 test files to TizenRT-compatible format
#>

param(
    [Parameter(Mandatory=$true)]
    [string]$SrcDir,
    
    [Parameter(Mandatory=$true)]
    [string]$OutputDir,
    
    [string]$Category = "",
    
    [switch]$GenerateMakefile
)

# TizenRT license header
$TizenRTLicense = @"
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
$ExclusionPatterns = @(
    'wchar_t', 'wchar', 'wstream', 'wcout', 'wcin', 'wcerr', 'wclog',
    'wstring', 'filesystem', 'random_device', 'ranges::', 'view::', 'views::'
)

function Should-Exclude-Test {
    param([string]$Content, [string]$Filepath)
    
    foreach ($pattern in $ExclusionPatterns) {
        if ($Filepath -match $pattern) {
            return $true, "Matched exclusion pattern: $pattern"
        }
    }
    
    if ($Content -match '\bwchar_t\b') {
        $linesWithWchar = ($Content -split "`n" | Where-Object { $_ -match 'wchar_t' -and $_ -notmatch '^\s*//' }).Count
        if ($linesWithWchar -gt 2) {
            return $true, "Wide character test"
        }
    }
    
    if ($Filepath -match 'filesystem' -or $Content -match 'filesystem') {
        return $true, "Filesystem test"
    }
    
    if ($Content -match 'std::ranges::|ranges::|view::|views::') {
        return $true, "Ranges test"
    }
    
    return $false, ""
}

function Get-RelativePath {
    param([string]$Path, [string]$BasePath)
    
    # Simple relative path calculation for older PowerShell
    $pathParts = $Path -split '[\\/]'
    $baseParts = $BasePath -split '[\\/]'
    
    $i = 0
    while ($i -lt $pathParts.Count -and $i -lt $baseParts.Count -and $pathParts[$i] -eq $baseParts[$i]) {
        $i++
    }
    
    $relativeParts = @()
    for ($j = $i; $j -lt $pathParts.Count; $j++) {
        $relativeParts += $pathParts[$j]
    }
    
    return $relativeParts -join '/'
}

function Convert-Test-File {
    param([string]$Filepath, [string]$OutputDir, [string]$SrcBase)
    
    $content = Get-Content $Filepath -Raw -Encoding UTF8
    
    # Check exclusion
    $exclude, $reason = Should-Exclude-Test -Content $content -Filepath $Filepath
    if ($exclude) {
        return $null, $false, $reason
    }
    
    # Generate relative path manually
    $relPath = Get-RelativePath -Path $Filepath -BasePath $SrcBase
    
    # Generate test name from path
    $testName = $relPath
    $testName = $testName -replace '[\\/.]', '_'
    $testName = $testName -replace '-', '_'
    $testName = $testName -replace '_pass_cpp$', ''
    $testName = $testName -replace '_cpp$', ''
    $funcName = "tc_$testName"
    
    # Remove LLVM license headers
    $content = $content -replace '//===---.*?---===//\s*\n', ''
    $content = $content -replace '//.*?Part of the LLVM Project.*?\n', ''
    $content = $content -replace '//.*?SPDX-License-Identifier.*?\n', ''
    $content = $content -replace '//.*?See https://llvm.org/LICENSE.*?\n', ''
    $content = $content -replace '//.*?Dual licensed under the MIT.*?\n', ''
    $content = $content -replace '//.*?University of Illinois Open.*?\n', ''
    $content = $content -replace '//.*?Source Licenses\. See LICENSE\.TXT.*?\n', ''
    
    # Add TizenRT license
    $content = $TizenRTLicense + $content
    
    # Convert main function - handle various signatures
    $content = $content -replace 'int\s+main\s*\(\s*int\s*,\s*char\s*\*\*\s*\)\s*\{', "int $funcName(void) {"
    $content = $content -replace 'int\s+main\s*\(\s*int\s+argc\s*,\s*char\s*\*\s*argv\s*\[\]\s*\)\s*\{', "int $funcName(void) {"
    $content = $content -replace 'int\s+main\s*\(\s*\)\s*\{', "int $funcName(void) {"
    
    # Convert asserts
    $content = $content -replace '\bassert\s*\(', 'TC_ASSERT_EXPR('
    
    # Add TizenRT include if not present - handle separately to avoid regex issues
    if ($content -notmatch 'libcxx_tc_common\.h') {
        if ($content -match 'test_macros\.h') {
            $content = $content -replace '(#include "test_macros\.h")', "`$1`n#include ""libcxx_tc_common.h"""
        } else {
            $content = $content -replace '(#include <cassert>)', "`$1`n#include ""libcxx_tc_common.h"""
        }
    }
    
    # Write output file
    $outputPath = Join-Path $OutputDir $relPath
    $outputDirPath = Split-Path $outputPath -Parent
    if (-not (Test-Path $outputDirPath)) {
        New-Item -ItemType Directory -Path $outputDirPath -Force | Out-Null
    }
    
    Set-Content $outputPath $content -Encoding UTF8
    
    return $relPath, $true, $funcName
}

# Main processing
$categories = @(
    'algorithms'
)

if ($Category) {
    $categories = @($Category)
}

$allTests = @{}

foreach ($cat in $categories) {
    Write-Host "Processing category: $cat"
    
    $categorySrc = Join-Path $SrcDir "std\$cat"
    
    if (-not (Test-Path $categorySrc)) {
        Write-Host "Category directory not found: $categorySrc"
        continue
    }
    
    $tests = @()
    
    # Find all .pass.cpp files
    $files = Get-ChildItem -Path $categorySrc -Recurse -Include "*.pass.cpp" -ErrorAction SilentlyContinue
    
    foreach ($file in $files) {
        $relPath, $success, $result = Convert-Test-File -Filepath $file.FullName -OutputDir $OutputDir -SrcBase (Join-Path $SrcDir "std")
        
        if ($success) {
            $tests += [PSCustomObject]@{
                RelPath = $relPath
                FilePath = $file.FullName
                FuncName = $result
            }
            Write-Host "Converted: $relPath"
        } else {
            Write-Host "Skipped: $($file.Name) - $result"
        }
    }
    
    $allTests[$cat] = $tests
    Write-Host "Category $cat`: $($tests.Count) tests converted"
}

# Summary
Write-Host ""
Write-Host "=================================================="
Write-Host "Conversion Summary"
Write-Host "=================================================="
foreach ($cat in $allTests.Keys) {
    Write-Host "$cat`: $($allTests[$cat].Count) tests converted"
}
$totalConverted = ($allTests.Values | ForEach-Object { $_.Count } | Measure-Object -Sum).Sum
Write-Host ""
Write-Host "Total: $totalConverted tests converted"

# Output function names for tc_algorithms.cpp
if ($allTests['algorithms']) {
    Write-Host ""
    Write-Host "Function declarations for tc_algorithms.hpp:"
    foreach ($test in $allTests['algorithms']) {
        Write-Host "int $($test.FuncName)(void);"
    }
    
    Write-Host ""
    Write-Host "Function calls for tc_algorithms.cpp:"
    foreach ($test in $allTests['algorithms']) {
        Write-Host "$($test.FuncName)();"
    }
    
    Write-Host ""
    Write-Host "Makefile entries:"
    foreach ($test in $allTests['algorithms']) {
        Write-Host "CXXSRCS += std/$($test.RelPath)"
    }
}