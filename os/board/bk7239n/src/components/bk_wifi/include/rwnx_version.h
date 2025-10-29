/**
 ****************************************************************************************
 *
 * @file rwnx_version.h
 *
 * @brief Definition of version macros.
 *
 * This file should be included if any version related macro is needed.
 * It is modified automatically by build tool, and should seldom be
 * touched manually.
 *
 * Copyright (C) RivieraWaves 2011-2020
 *
 ****************************************************************************************
 */
#ifndef _RWNX_VERSION_H_
#define _RWNX_VERSION_H_

/**
 ****************************************************************************************
 * @addtogroup MACSW
 * @brief Versioning
 * @{
 ****************************************************************************************
 */

// Version has the form Major.minor.release.patch
// The version string is "vMM.mm.rr.pp"
// Each number should fit in a 8 bits unsigned (0 to 255)
/// Major version number
#define NX_VERSION_MAJ      6
/// Minor version number
#define NX_VERSION_MIN      8
/// Release number
#define NX_VERSION_REL      2
/// Patch number
#define NX_VERSION_PAT      0

/// Version word
#define NX_VERSION        ((NX_VERSION_MAJ << 24) | (NX_VERSION_MIN << 16) |             \
                           (NX_VERSION_REL << 8) | NX_VERSION_PAT)

#if 1
#define NX_VERSION_PACK(a, b, c, d)    (((a) << 24) | ((b) << 16) |((c) << 8) | (d))
#endif

/// Version string (global variable defined in src/build/build_version.c)
extern const char nx_version_str[];
/// Macro for previous variable. Do not use!
#define NX_VERSION_STR      "v6.8.2.0"

/// Date at which the system is built (defined in build_version.c)
extern const char nx_build_date[];

/// Version of extra fhost component (defined in build_version.c)
extern const char nx_build_fhost[];

/// @}

#endif // _RWNX_VERSION_H_
