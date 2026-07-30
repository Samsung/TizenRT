# IBM Platform Support

This directory contains consolidated IBM platform support for both AIX and z/OS systems.

## Directory Structure

```
ibm/
├── aix/           # AIX platform support files
├── zos/           # z/OS platform support files
├── limits.h       # Unified limits header
├── locale_mgmt.h  # Unified locale management header
└── xlocale.h      # Unified extended locale header
```

## Platform Detection

The unified headers automatically route to the appropriate platform-specific implementation:

- **AIX** (`_AIX`): Uses files from `aix/` subdirectory
- **z/OS** (`__MVS__`): Uses files from `zos/` subdirectory

## File Mapping

### AIX Support Files (`aix/`)
- `limits.h` - Floating point and limit constants for AIX
- `locale_mgmt_aix.h` - Locale management for AIX 6.1 compatibility
- `support.h` - General AIX support functions
- `xlocale.h` - Extended locale functions for AIX

### z/OS Support Files (`zos/`)
- `gettod_zos.h` - Time-related functions for z/OS
- `locale_mgmt_zos.h` - Locale management for z/OS
- `nanosleep.h` - Nanosleep implementation for z/OS
- `xlocale.h` - Extended locale functions for z/OS

## Migration Notes

This consolidation maintains full backward compatibility with existing code while providing a cleaner,
more organized structure for IBM platform support.

Files are organized by platform to make it easier to understand and maintain platform-specific
implementations while providing unified headers for common usage.

⚠️ STANDARD DIRECTORY ⚠️
=====================

This __support/ directory is the new standard location for platform support files.
All new development should use this directory structure directly.