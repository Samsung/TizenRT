# TizenRT License Manager

The **license_manager.py** tool manages and verifies Samsung Electronics Apache 2.0 license headers across TizenRT source code files.

## Features
1. **License Verification (`check`)**:
   - Checks whether the official Samsung Electronics Apache 2.0 license is present.
   - Verifies whether the copyright year matches the file's initial creation year (tracked via Git commit history).
   - Identifies third-party licenses (e.g. BSD, GPL) without modifying them.
2. **License Addition (`add`)**:
   - Automatically inserts the properly formatted license header according to the file type.
   - Preserves script shebangs (`#!/bin/bash`, `#!/usr/bin/env python3`) and encoding declarations.
3. **Year & License Correction (`fix`)**:
   - Fixes mismatched copyright years to match the file's Git creation year.
   - Adds missing license headers.
4. **Flexible Path & Bulk Processing**:
   - Supports single file paths as well as recursive directory processing.
   - Supports both relative paths and absolute paths.
   - Ignores build artifacts and `.git` by default, with custom `--exclude` pattern support.
5. **Safety Mode (`--dry-run`)**:
   - Simulates additions and fixes without making any changes to files on disk.

---

## Supported File Types

| Type | Extensions / Names | Comment Style |
| :--- | :--- | :--- |
| **C / C++** | `.c`, `.h`, `.cpp`, `.cxx`, `.cc`, `.hpp`, `.hxx` | Block `/* ... */` |
| **Scripts & Makefiles** | `.sh`, `.bash`, `.py`, `.mk`, `.cmake`, `Makefile`, `Make.defs`, `Kconfig`, `CMakeLists.txt` | Hash `# ...` |
| **Batch Files** | `.bat`, `.cmd` | `REM ...` |

---

## Usage

You can use either subcommands (`check`, `add`, `fix`) or flag shortcuts (`-c`, `-a`, `-f`).

### 1. Check License & Creation Year
```bash
# Check a single file (relative or absolute path)
./tools/license/license_manager.py check os/compression/compress.c
./tools/license/license_manager.py -c /path/to/TizenRT/tools/cformatter.sh

# Check a folder recursively
./tools/license/license_manager.py check apps/

# Detailed verbose output (displays OK files as well)
./tools/license/license_manager.py check -v os/compression/
```

Exit codes:
- `0`: All files have valid license headers with matching creation years.
- `1`: One or more files have missing licenses or year mismatches (useful for CI/CD).

### 2. Add Missing License
```bash
# Add license to a new file
./tools/license/license_manager.py add apps/my_app/my_app_main.c

# Add license to all files in a folder missing license headers
./tools/license/license_manager.py add apps/my_app/

# Manually specify copyright year instead of Git history / fallback
./tools/license/license_manager.py add --year 2026 apps/my_app/
```

### 3. Fix License & Year Mismatch
```bash
# Automatically fix year mismatch or insert missing license
./tools/license/license_manager.py fix os/

# Using the -f flag shortcut
./tools/license/license_manager.py -f os/compression/compress.c
```

### 4. Dry-Run (Simulation)
```bash
# Preview what would be changed without touching disk files
./tools/license/license_manager.py fix --dry-run apps/
```

### 5. Custom Exclusions
```bash
# Exclude specific directories or patterns
./tools/license/license_manager.py check -e "external/*" -e "build/*" .
```

---

## Running Unit Tests

Unit tests are written using Python's standard `unittest` framework:

```bash
# From workspace root:
python3 -m unittest tools/license/test_license_manager.py -v

# Or from inside tools/license directory:
cd tools/license && python3 -m unittest test_license_manager.py -v
```
