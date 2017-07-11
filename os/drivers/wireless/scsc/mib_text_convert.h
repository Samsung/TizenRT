/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#endif							/* MIB_TEXT_CONVERT_H__ */
