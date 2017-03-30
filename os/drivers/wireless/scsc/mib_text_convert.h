/******************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd and its Licensors.
 * All rights reserved.
 *
 *****************************************************************************/

#ifndef MIB_TEXT_CONVERT_H__
#define MIB_TEXT_CONVERT_H__

#include "mib.h"

/*******************************************************************************
 *
 * NAME
 *  CsrWifiMibConvertText
 *
 * DESCRIPTION
 *  Converts a simple text mib file format into an encoded mib data buffer
 *
 * TEXT FILE FORMAT
 # Comment
 #  <psid><indexes>=<value>
 #
 #  123 = 0x01          # Unsigned Integer
 #  123 = 1             # Unsigned Integer
 #  123 = -1            # Integer
 #  123 = true          # Boolean (Case Sensitive)
 #  123 = false         # Boolean (Case Sensitive)
 #  123 = [12183984]    # Octet String (Hex Format)
 #  123 = "ABCDEFGH"    # Octet String  (Null Terminated Ascii)
 #
 # Indexes (Max 2 indexes supported)
 #  123.1   = 1         # Single Index
 #  123.1.2 = 1         # Multi Index
 #
 # Hex Format (All Unsigned Integer values can use the alternative 0x[0-F] format)
 #  0xF12E.0x1.0x2 = 0x01          # Unsigned Integer
 #
 # RETURN
 #  bool: true = No errors in conversion
 #
 *******************************************************************************/
bool CsrWifiMibConvertText(const char *mibText, struct slsi_mib_data *mibDataSet, struct slsi_mib_data *mibDataGet);

#endif /* MIB_TEXT_CONVERT_H__ */
