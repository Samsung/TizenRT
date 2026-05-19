#!/usr/bin/env python
# -*- coding: utf-8 -*-
###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################

from __future__ import print_function
from optparse import OptionParser
import sys
import re
if sys.version_info[0] < 3:
    from sets import Set

parser = OptionParser(
    usage="Usage: %prog [options] -f <mapfile>\n"
          "\n"
          "Examples:\n"
          "  %prog -m -f build/output/bin/tinyara.map\n"
          "      Print RAM/Flash usage summary per library\n"
          "\n"
          "  %prog -l -f build/output/bin/tinyara.map\n"
          "      Print library sizes with section breakdown\n"
          "\n"
          "  %prog -s -f build/output/bin/tinyara.map\n"
          "      Print section summary (address and size)\n"
          "\n"
          "  %prog -d -f build/output/bin/tinyara.map\n"
          "      Print detailed object file sizes per library\n"
          "\n"
          "  %prog -a -f build/output/bin/tinyara.map\n"
          "      Print all information (equivalent to -s -l -d -m)\n"
          "\n"
          "  %prog -m -f build/output/bin/tinyara.map -o memory.txt\n"
          "      Save output to file instead of stdout\n"
          "\n"
          "  %prog -m -f build/output/bin/tinyara.map -L build/configs/bk7239n/scripts/bk7239n_bsp_ns.ld\n"
          "      Use linker script for accurate section region classification\n"
)
parser.add_option("-f", "--file", dest="infilename",
                  help="Map file to parse (e.g., build/output/bin/tinyara.map)", metavar="INPUT_FILE")
parser.add_option("-o", "--output", dest="output",
                  help="Output file (default: stdout)", metavar="OUTPUT_FILE")
parser.add_option("-a", "--all", action="store_true", dest="all",
                  help="Print all information (section summary + library sizes + details + memory)", default=False)
parser.add_option("-s", "--size", action="store_true", dest="totsize",
                  help="Print section summary (address and size for each section)", default=False)
parser.add_option("-l", "--libsize", action="store_true",
                  dest="libsize", help="Print library sizes with section breakdown", default=False)
parser.add_option("-d", "--details", action="store_true", dest="details",
                  help="Print object file details grouped by library", default=False)
parser.add_option("-m", "--memory", action="store_true", dest="memory",
                  help="Print SRAM/DRAM/PSRAM/Flash usage summary per library (board-aware)", default=False)
parser.add_option("-L", "--ldscript", dest="ldscript",
                  help="Linker script (.ld) to parse for section region info (enables LD-driven classification)", metavar="LDSCRIPT")

(options, args) = parser.parse_args()
if not options.infilename:
    parser.print_help()
    sys.exit(1)

if (not options.all) and (not options.totsize) and (not options.libsize) and (not options.details) and (not options.memory):
    parser.print_help()
    sys.exit(1)

infile = open(options.infilename, 'r')
if options.output:
    sys.stdout = open(options.output, 'w')

# ---- Dynamic section discovery ----
sectionInfo = {}
sectionLibSizes = {}
sectionObjDetails = {}
sectionPatterns = {}
sectionOrder = []

currentSection = ""
currentLibObj = ""
action = 0

# ---- Board detection ----
detectedBoard = "unknown"
def detectBoard(secName):
    """Detect board based on section names."""
    global detectedBoard
    if detectedBoard != "unknown":
        return detectedBoard
    
    sec_lower = secName.lower()
    # rtl8730e specific sections
    if '.xip_image2' in sec_lower or '.bluetooth_trace' in sec_lower:
        detectedBoard = "rtl8730e"
    # bk7239n specific sections
    elif '.psram_data' in sec_lower or '.psram_bss' in sec_lower or '.psram_heap' in sec_lower:
        detectedBoard = "bk7239n"
    # rtl8721csm specific sections
    elif '.ram_image2' in sec_lower or '.rom.' in sec_lower or '.hal.' in sec_lower:
        detectedBoard = "rtl8721csm"
    
    return detectedBoard


def transform(k):
    import re
    # Escape all regex special characters except '*'
    result = ''
    for char in k:
        if char == '*':
            result += '.*'
        elif char in '.^$+?{}[]\\|()':
            result += '\\' + char
        else:
            result += char
    return '^' + result + '$'


def isValidSubSymbol(field1, dictKeys):
    for k in dictKeys:
        tk = transform(k)
        if re.search(tk, field1) != None:
            return k
    return None


def getListfromString(s):
    res = []
    s = s[s.find('(')+1:s.rfind(')')]
    word = ''
    for x in s:
        if (x == '(' or x == ')' or x == ' '):
            if word != '':
                res.append(word)
            word = ''
        else:
            word = word + x
    if word != '':
        res.append(word)
    return res


def addLibSize(secName, libObj, size):
    """Track library size within a top-level section."""
    libObj = libObj.strip().split('/')
    libObj = libObj[len(libObj)-1]
    if '(' in libObj:
        library = libObj.split('(')[0]
        dotO = libObj.split('(')[1][:-1]
    else:
        library = "NOLIB"
        dotO = libObj

    if secName not in sectionLibSizes:
        sectionLibSizes[secName] = {}
    if library not in sectionLibSizes[secName]:
        sectionLibSizes[secName][library] = 0
    sectionLibSizes[secName][library] += size

    if secName not in sectionObjDetails:
        sectionObjDetails[secName] = {}
    if library not in sectionObjDetails[secName]:
        sectionObjDetails[secName][library] = {}
    if dotO not in sectionObjDetails[secName][library]:
        sectionObjDetails[secName][library][dotO] = 0
    sectionObjDetails[secName][library][dotO] += size


# ---- Parse the map file ----
for line in infile:
    if line not in ['\n', '\r\n']:
        if line.strip() == "Linker script and memory map":
            action = 1
            continue
        if line.strip() == "/DISCARD/":
            action = 0

        if action == 0:
            continue

        if re.search("__ebase = ORIGIN", line) != None:
            continue

        lsplit = line.split()

        # Top-level section: line starts with '.' at column 0
        if (line[0] == '.'):
            currentSection = lsplit[0]
            currentLibObj = ""
            if currentSection == ".comment":
                action = 0
                continue

            # Parse address and size if present on the same line
            addr = 0
            size = 0
            if len(lsplit) >= 3:
                try:
                    addr = int(lsplit[1], 16)
                    size = int(lsplit[2], 16)
                except ValueError:
                    pass
            elif len(lsplit) >= 2:
                try:
                    addr = int(lsplit[1], 16)
                except ValueError:
                    pass

            # If address/size not on same line, check next line
            # but skip wildcard directives - don't consume them
            if addr == 0 and size == 0:
                nextline = next(infile, None)
                if nextline is not None:
                    nextline_stripped = nextline.strip()
                    # Don't consume wildcard directive lines
                    if re.search(r'\*\(.*\)', nextline_stripped) != None or re.search(r'\**.o\(', nextline_stripped) != None:

                        # Put it back by processing it as a wildcard line
                        patterns = getListfromString(nextline_stripped)
                        for p in patterns:
                            if p not in sectionPatterns.get(currentSection, []):
                                sectionPatterns.setdefault(currentSection, []).append(p)
                    else:
                        nlsplit = nextline_stripped.split()
                        if len(nlsplit) >= 2:
                            try:
                                addr = int(nlsplit[0], 16)
                                size = int(nlsplit[1], 16)
                            except ValueError:
                                pass

            sectionInfo[currentSection] = {
                'address': addr,
                'size': size
            }
            sectionPatterns[currentSection] = []
            sectionOrder.append(currentSection)
            continue
        else:
            if currentSection == '':
                continue

            # Wildcard pattern line
            if re.search(r'\*\(.*\)', line.strip()) != None or re.search(r'\**.o\(', lsplit[0]) != None:

                patterns = getListfromString(line.strip())
                for p in patterns:
                    if p not in sectionPatterns.get(currentSection, []):
                        sectionPatterns.setdefault(currentSection, []).append(p)
                continue

            # Subsection line (starts with '.')
            if len(lsplit) > 0 and len(lsplit[0]) > 0 and lsplit[0][0] == '.':
                matchedPattern = isValidSubSymbol(
                    lsplit[0], sectionPatterns.get(currentSection, []))

                if matchedPattern is not None:
                    if len(lsplit) > 2:
                        try:
                            size_value = int(lsplit[2], 16)
                            if size_value != 0:
                                libObj = lsplit[len(lsplit)-1]
                                currentLibObj = libObj
                                addLibSize(currentSection, libObj, size_value)
                        except ValueError:
                            continue
                    else:
                        nextline = next(infile, None)
                        if nextline is None:
                            continue
                        nextline = nextline.strip()
                        nlsplit = nextline.split()
                        try:
                            size_value = int(nlsplit[1], 16)
                            if size_value != 0:
                                libObj = nlsplit[len(nlsplit)-1]
                                currentLibObj = libObj
                                addLibSize(currentSection, libObj, size_value)
                        except ValueError:
                            continue

            # *fill* alignment padding line
            elif len(lsplit) > 0 and re.search(r'\*fill\*', lsplit[0]) != None:

                if currentLibObj:
                    try:
                        if int(lsplit[1], 16) != 0:
                            size_value = int(lsplit[2], 16)
                            addLibSize(currentSection, currentLibObj, size_value)
                    except ValueError:
                        continue

infile.close()

# ---- Sections to display (skip only debug/metadata sections) ----
# Only debug and metadata sections are skipped - they are not loaded
# to the target and don't consume flash/RAM. All runtime sections
# (.text, .data, .bss, .ARM.exidx, .rel.dyn, etc.) are shown.
SKIP_SECTIONS = set([
    '.debug_line', '.debug_info', '.debug_abbrev', '.debug_aranges',
    '.debug_str', '.debug_ranges', '.debug_frame', '.debug_loc',
    '.comment', '.ARM.attributes'
])

displaySections = [s for s in sectionOrder if s not in SKIP_SECTIONS]


def getActiveSections():
    """Return sections that have at least one library with non-zero size."""
    active = []
    for secName in displaySections:
        if secName in sectionLibSizes:
            for lib in sectionLibSizes[secName]:
                if sectionLibSizes[secName][lib] > 0:
                    active.append(secName)
                    break
    return active


def printSectionSummary():
    """Print section summary similar to arm-none-eabi-size -A."""
    print("######################################")
    print("##        Section Summary           ##")
    print("######################################")
    print("%-40s  %-18s  %10s" % ("Section", "Address", "Size"))
    print("-" * 72)

    totalSize = 0
    for secName in displaySections:
        info = sectionInfo[secName]
        calcSize = info['size']
        if calcSize == 0 and secName not in ['.psram_heap.start']:
            continue
        addrStr = "0x%08x" % info['address'] if info['address'] != 0 else "0x00000000"
        sizeStr = "0x%x" % calcSize
        print("%-40s  %-18s  %10s" % (secName, addrStr, sizeStr))
        totalSize += calcSize

    print("-" * 72)
    sizeStr = "0x%x" % totalSize
    print("%-40s  %-18s  %10s" % ("Total", "", sizeStr))
    print("")


def printColumnar(activeSections, rows, rowNames):
    """Print data in aligned columns with right-aligned numbers."""
    nSecCols = len(activeSections)

    colWidths = []
    for secName in activeSections:
        header = secName
        w = len(header) + 1
        colWidths.append(w)

    totalWidth = len("Total") + 1
    nameWidth = 0
    for name in rowNames:
        if len(name) > nameWidth:
            nameWidth = len(name)
    nameWidth += 1

    for row in rows:
        for i in range(nSecCols):
            if len(str(row[i])) + 1 > colWidths[i]:
                colWidths[i] = len(str(row[i])) + 1
        if len(str(row[nSecCols])) + 1 > totalWidth:
            totalWidth = len(str(row[nSecCols])) + 1

    headerLine = " " * nameWidth
    for i, secName in enumerate(activeSections):
        headerLine += secName.rjust(colWidths[i])
    headerLine += "Total".rjust(totalWidth)
    print(headerLine)

    for idx, row in enumerate(rows):
        line = rowNames[idx].ljust(nameWidth)
        for i in range(nSecCols):
            line += str(row[i]).rjust(colWidths[i])
        line += str(row[nSecCols]).rjust(totalWidth)
        print(line)


def printLibSizes():
    """Print library sizes with sections as columns."""
    activeSections = getActiveSections()

    allLibs = set()
    for secName in activeSections:
        if secName in sectionLibSizes:
            for lib in sectionLibSizes[secName]:
                allLibs.add(lib)

    if not allLibs:
        return

    results = []
    libNames = []
    for lib in allLibs:
        row = []
        total = 0
        for secName in activeSections:
            size = sectionLibSizes.get(secName, {}).get(lib, 0)
            row.append(size)
            total += size
        row.append(total)
        results.append(row)
        libNames.append(lib)

    combined = list(zip(results, libNames))
    combined.sort(key=lambda x: x[0][-1], reverse=True)
    results = [c[0] for c in combined]
    libNames = [c[1] for c in combined]

    printColumnar(activeSections, results, libNames)


def getLibTotal(lib):
    """Get total size of a library across all sections."""
    total = 0
    for secName in displaySections:
        for objSize in sectionObjDetails.get(secName, {}).get(lib, {}).values():
            total += objSize
    return total


def printDetails():
    """Print object file details with sections as columns, grouped by library."""
    activeSections = getActiveSections()

    allLibs = set()
    for secName in activeSections:
        if secName in sectionObjDetails:
            for lib in sectionObjDetails[secName]:
                allLibs.add(lib)

    if not allLibs:
        return

    for lib in sorted(allLibs, key=lambda l: getLibTotal(l), reverse=True):
        libTotal = getLibTotal(lib)
        if libTotal == 0:
            continue

        print(lib + "\t" + str(libTotal))

        allObjs = set()
        for secName in activeSections:
            if secName in sectionObjDetails and lib in sectionObjDetails[secName]:
                for obj in sectionObjDetails[secName][lib]:
                    allObjs.add(obj)

        objRows = []
        objNames = []
        for obj in allObjs:
            row = []
            total = 0
            for secName in activeSections:
                size = sectionObjDetails.get(secName, {}).get(lib, {}).get(obj, 0)
                row.append(size)
                total += size
            row.append(total)
            objRows.append(row)
            objNames.append(obj)

        combined = list(zip(objRows, objNames))
        combined.sort(key=lambda x: x[0][-1], reverse=True)
        objRows = [c[0] for c in combined]
        objNames = [c[1] for c in combined]

        printColumnar(activeSections, objRows, objNames)


def categorizeSection(secName):
    """Categorize a section as SRAM, PSRAM/DRAM, FLASH, or OTHER based on section name.
    
    Handles sections from multiple board configurations:
    - rtl8730e: .xip_image2.text (FLASH), .code/.data/.bss/.stack (DRAM), .bluetooth_trace.text (FLASH)
    - bk7239n: .vectors/.text (FLASH), .iram (SRAM), .psram_data/.psram_bss (PSRAM), .data/.bss (SRAM)
    - rtl8721csm: .rom.* (FLASH), .ram_image2.* (SRAM), .hal.* (varies)
    
    Returns category as 'SRAM', 'DRAM', 'PSRAM', 'FLASH', or 'OTHER'
    """
    sec_lower = secName.lower()
    board = detectBoard(secName)
    
    # ============================================================
    # rtl8730e: Most sections are in DRAM (DDR2)
    # ============================================================
    if board == "rtl8730e":
        # Flash sections
        if '.xip_image' in sec_lower or '.xip_text' in sec_lower or '.xipelf' in sec_lower:
            return 'FLASH'
        if '.bluetooth_trace' in sec_lower or '.bttrace' in sec_lower:
            return 'FLASH'
        
        # DRAM sections (DDR2 memory - CA32_BL3_DRAM_NS)
        # .code, .data, .bss, .stack, .heap, .psram_heap.start are all in DRAM
        dram_patterns = ['.code', '.data', '.bss', '.stack', '.heap', 
                         '.psram_heap', '.noinit', '.uninitialized']
        for pattern in dram_patterns:
            if sec_lower == pattern or sec_lower.startswith(pattern + '.') or pattern in sec_lower:
                return 'DRAM'
        
        # SRAM sections (on-chip)
        sram_specific = ['.dtcm_data', '.dtcm_bss', '.itcm_data', '.itcm_bss', 
                         '.ocram_data', '.ocram_bss', '.sram', '.iram']
        for pattern in sram_specific:
            if pattern in sec_lower:
                return 'SRAM'
        
        return 'OTHER'
    
    # ============================================================
    # bk7239n: PSRAM sections (external pseudo-SRAM)
    # ============================================================
    psram_patterns = ['.psram_data', '.psram_bss', '.psram_heap', '.psram_code']
    for pattern in psram_patterns:
        if pattern in sec_lower:
            return 'PSRAM'
    
    # ============================================================
    # SRAM sections (internal SRAM/TCM/OCRM) - bk7239n, rtl8721csm
    # ============================================================
    sram_specific = ['.dtcm_data', '.dtcm_bss', '.itcm_data', '.itcm_bss', 
                     '.ocram_data', '.ocram_bss', '.sram']
    for pattern in sram_specific:
        if pattern in sec_lower:
            return 'SRAM'
    
    # .iram is code in IRAM (on-chip SRAM) - bk7239n
    if sec_lower == '.iram' or sec_lower.startswith('.iram.'):
        return 'SRAM'
    
    # rtl8721csm: .ram_image2.*, .image2.* are RAM sections (in SRAM)
    if '.ram_image2' in sec_lower or '.image2.ram' in sec_lower or '.image2.net' in sec_lower:
        return 'SRAM'
    
    # ============================================================
    # FLASH sections (code and read-only data)
    # ============================================================
    flash_exact = ['.text', '.rodata', '.init', '.fini', '.init_array', '.fini_array',
                   '.preinit_array', '.ctors', '.dtors',
                   '.data.rel.ro', '.got', '.got.plt', '.plt', '.plt.got',
                   '.eh_frame', '.eh_frame_hdr', '.arm.extab', '.arm.exidx', '.vectors']

    for pattern in flash_exact:
        if sec_lower == pattern or sec_lower.startswith(pattern + '.'):
            return 'FLASH'
    
    # rtl8721csm: ROM sections are in flash
    rom_patterns = ['.rom.', '.hal.rom', '.hal.ns_rom']
    for pattern in rom_patterns:
        if pattern in sec_lower:
            return 'FLASH'
    
    # bk7239n: Flash-specific sections
    bk_flash_patterns = ['.gnu.sgstubs', '.a_device_null', '.a_devices', 
                         '.a_init_entries', '.copy.table', '.zero.table']
    for pattern in bk_flash_patterns:
        if pattern in sec_lower:
            return 'FLASH'
    
    # Additional flash patterns (metadata sections)
    flash_meta = ['.note', '.gnu.hash', '.dynsym', '.dynstr', '.gnu.version', 
                  '.gnu.version_r', '.dynamic', '.interp', '.strtab', '.symtab']
    for pattern in flash_meta:
        if pattern in sec_lower:
            return 'FLASH'
    
    # ============================================================
    # RAM sections (read-write data, bss, stacks, heaps) - bk7239n, rtl8721csm
    # ============================================================
    ram_patterns = ['.data', '.bss', '.ram', '.ramcode', '.stack', '.heap',
                    '.nosys', '.uninit', '.common', '.tdata', '.tbss']

    for pattern in ram_patterns:
        if sec_lower == pattern or sec_lower.startswith(pattern + '.'):
            return 'SRAM'
    
    # bk7239n: Additional RAM sections
    bk_ram_patterns = ['.uninitialized', '.persist_mem', '.reboot_tag']
    for pattern in bk_ram_patterns:
        if pattern in sec_lower:
            return 'SRAM'
    
    # rtl8721csm: HAL RAM sections
    hal_ram_patterns = ['.hal.ns_ram', '.hal.ram']
    for pattern in hal_ram_patterns:
        if pattern in sec_lower:
            return 'SRAM'
    
    return 'OTHER'


# NOLOAD / zero-init section patterns: these are zeroed at runtime and have
# NO load image stored in flash (no LMA in flash).  Everything else contributes
# bytes to the flash image (.bin burned to flash), whether it is XIP-resident
# (.text, .vectors, .rodata) or copied from flash to RAM/PSRAM/IRAM at boot
# (.data, .iram, .code, .psram_data, .psram_code).  This rule is board-agnostic:
#   - bk7239n: AT > FLASH sections have flash load images
#   - rtl8730e: no AT >, but image-tool copies .code/.data from flash to DRAM
# In both cases, only NOLOAD sections are absent from the flash image.
NOLOAD_PATTERNS = [
    '.bss', '.psram_bss', '.heap', '.stack', '.noinit', '.uninitialized',
    '.uninit', '.common', '.tbss', '.psram_heap', '.persist_mem', '.reboot_tag',
    '.ram_image2.bss', '.image2.bss', '.bdsram.bss'
]


def hasFlashImage(secName):
    """Return True if the section contributes bytes to the stored flash image.

    A section has a flash image unless it is NOLOAD / zero-initialized
    (no initial values stored in flash).  This is the complement of
    "is this a bss/heap section".
    """
    sec_lower = secName.lower()
    for pattern in NOLOAD_PATTERNS:
        if sec_lower == pattern or sec_lower.startswith(pattern + '.') or sec_lower == pattern + '.start':
            return False
    return True


# ---- Linker-script-driven classification (used when -L is provided) ----

# Parsed linker script data: { '.section_name': {'vma_region': str, 'lma_region': str,
#                                                 'noload': bool, 'copy': bool} }
ldSectionInfo = {}
ldMemoryRegions = {}  # { 'REGION_NAME': {'origin': int, 'length': int, 'attrs': str} }

# Mapping from MEMORY region names to runtime categories per board.
# This is the only per-board data needed (a few region names per board).
REGION_CATEGORY_MAP = {
    # bk7239n
    'FLASH': 'FLASH',
    'IRAM':  'SRAM',
    'RAM':   'SRAM',
    'PSRAM': 'PSRAM',
    # rtl8730e
    'CA32_IMG2_XIP':    'FLASH',
    'CA32_BL3_DRAM_NS': 'DRAM',
    'BTRACE':           'FLASH',
    'KM4_IMG2_XIP':     'FLASH',
    'KM0_IMG2_XIP':     'FLASH',
    'KM4_BOOT_XIP':     'FLASH',
    # rtl8721csm
    'KM0_IMG2':    'FLASH',
    'KM4_IMG2':    'FLASH',
    'KM0_RAM':     'SRAM',
    'KM4_RAM':     'SRAM',
    'KM4_BD_RAM':  'SRAM',
}


def parseLinkerScript(ldPath):
    """Parse a linker script (.ld) file.

    Extracts:
    1. MEMORY { } block -> region name, origin, length, attributes
    2. SECTIONS { } block -> for each output section: VMA region, LMA region,
       NOLOAD flag, COPY flag

    Populates the global ldSectionInfo and ldMemoryRegions dicts.
    """
    global ldSectionInfo, ldMemoryRegions

    try:
        ldfile = open(ldPath, 'r')
    except IOError:
        print("Warning: Could not open linker script '%s'" % ldPath, file=sys.stderr)
        return False

    content = ldfile.read()
    ldfile.close()

    # Remove C-style comments (/* ... */)
    content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
    # Remove C++-style comments (// ...)
    content = re.sub(r'//.*', '', content)

    # ---- Parse MEMORY { } block ----
    memMatch = re.search(r'MEMORY\s*\{(.*?)\}', content, re.DOTALL)
    if memMatch:
        memBlock = memMatch.group(1)
        # Each region line:  REGION_NAME (attrs) : ORIGIN = expr, LENGTH = expr
        # We try to evaluate ORIGIN and LENGTH as hex expressions.
        for line in memBlock.split('\n'):
            line = line.strip()
            if not line or line.startswith('/*'):
                continue
            # Match: NAME (attrs) : ORIGIN = 0x..., LENGTH = 0x... - 0x...
            m = re.match(r'(\w+)\s*\(([^)]*)\)\s*:\s*ORIGIN\s*=\s*([^,]+),\s*LENGTH\s*=\s*(.+)', line)
            if m:
                regionName = m.group(1)
                attrs = m.group(2).strip()
                originExpr = m.group(3).strip()
                lengthExpr = m.group(4).strip()
                try:
                    origin = _evalAddrExpr(originExpr)
                except Exception:
                    origin = 0
                try:
                    length = _evalAddrExpr(lengthExpr)
                except Exception:
                    length = 0
                ldMemoryRegions[regionName] = {
                    'origin': origin,
                    'length': length,
                    'attrs': attrs
                }

    # ---- Parse SECTIONS { } block ----
    secMatch = re.search(r'SECTIONS\s*\{(.*)(?:\n\}|\n\s*\}\s*$)', content, re.DOTALL)
    if secMatch:
        secBlock = secMatch.group(1)
        _parseSectionsBlock(secBlock)

    return True


def _evalAddrExpr(expr):
    """Evaluate a linker-script address expression like '0x60000020' or '0x60017000 - 0x60000020'."""
    expr = expr.strip()
    # Handle expressions with ORIGIN() / LENGTH() / arithmetic
    # Replace ORIGIN(REGION) and LENGTH(REGION)
    def replOrigin(m):
        rn = m.group(1)
        return str(ldMemoryRegions.get(rn, {}).get('origin', 0))
    def replLength(m):
        rn = m.group(1)
        return str(ldMemoryRegions.get(rn, {}).get('length', 0))
    expr = re.sub(r'ORIGIN\s*\(\s*(\w+)\s*\)', replOrigin, expr)
    expr = re.sub(r'LENGTH\s*\(\s*(\w+)\s*\)', replLength, expr)
    # Replace DEFINED(...) with 0 for simplicity
    expr = re.sub(r'DEFINED\s*\([^)]*\)', '0', expr)
    # Now try to evaluate as a simple arithmetic expression with hex numbers
    # Replace hex numbers and operators -- eval is safe-ish here (controlled input)
    try:
        # Python eval of hex arithmetic
        result = eval(expr, {'__builtins__': None}, {})
        return int(result)
    except Exception:
        return 0


def _parseSectionsBlock(block):
    """Parse the SECTIONS block to extract section -> region mapping.

    Looks for lines like:
        .section_name ... > VMA_REGION [AT > LMA_REGION]
        .section_name ... (NOLOAD) > VMA_REGION
        .section_name ... (COPY) > VMA_REGION
    """
    lines = block.split('\n')
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        i += 1
        if not line:
            continue

        # Match a section declaration starting with '.'
        # e.g. ".iram :" or ".text :" or ".data  {" or ".bss (NOLOAD):"
        # The section name is the first token starting with '.'
        m = re.match(r'^(\.[\w.]+)', line)
        if not m:
            continue

        secName = m.group(1)
        noload = bool(re.search(r'\bNOLOAD\b', line))
        copy = bool(re.search(r'\bCOPY\b', line))

        # Find the VMA region: "> REGION" (after the section name)
        vmaRegion = None
        lmaRegion = None

        # Check for "AT > REGION" (LMA region)
        atMatch = re.search(r'AT\s*>\s*(\w+)', line)
        if atMatch:
            lmaRegion = atMatch.group(1)

        # Check for "> REGION" (VMA region) -- must come after the section name
        # We need to be careful not to match "AT >"
        # Remove the "AT > REGION" part first, then look for ">"
        lineNoAT = re.sub(r'AT\s*>\s*\w+', '', line)
        gtMatch = re.search(r'>\s*(\w+)', lineNoAT)
        if gtMatch:
            vmaRegion = gtMatch.group(1)

        # If the line doesn't have the region, check the continuation lines
        # (section declarations sometimes span multiple lines in the ld file)
        if vmaRegion is None and i < len(lines):
            # Look ahead a few lines for "> REGION"
            for j in range(i, min(i + 5, len(lines))):
                nextline = lines[j].strip()
                if nextline and nextline[0] == '.':
                    # Hit the next section declaration, stop
                    break
                atMatch2 = re.search(r'AT\s*>\s*(\w+)', nextline)
                if atMatch2 and lmaRegion is None:
                    lmaRegion = atMatch2.group(1)
                nextNoAT = re.sub(r'AT\s*>\s*\w+', '', nextline)
                gtMatch2 = re.search(r'>\s*(\w+)', nextNoAT)
                if gtMatch2 and vmaRegion is None:
                    vmaRegion = gtMatch2.group(1)
                    # Don't break -- the AT > might be on a different line
                if vmaRegion is not None:
                    break

        # Also check for ": ALIGN(...)" before "> REGION" -- e.g. ".PERSIST_MEM : ALIGN(4)"
        # The regex above should still find the "> REGION" on the same or next line.

        if secName not in ldSectionInfo:
            ldSectionInfo[secName] = {
                'vma_region': vmaRegion,
                'lma_region': lmaRegion,
                'noload': noload,
                'copy': copy
            }
        else:
            # Update if we found new info
            if vmaRegion:
                ldSectionInfo[secName]['vma_region'] = vmaRegion
            if lmaRegion:
                ldSectionInfo[secName]['lma_region'] = lmaRegion
            if noload:
                ldSectionInfo[secName]['noload'] = True
            if copy:
                ldSectionInfo[secName]['copy'] = True


def regionToCategory(regionName):
    """Map a MEMORY region name to a runtime category (FLASH, SRAM, PSRAM, DRAM)."""
    if regionName and regionName in REGION_CATEGORY_MAP:
        return REGION_CATEGORY_MAP[regionName]
    return None


def categorizeSectionFromLD(secName):
    """Categorize a section using the parsed linker script data.

    Falls back to address-range lookup and then to categorizeSection() if the
    section is not found in the linker script.
    """
    # Case-insensitive lookup in ldSectionInfo
    secInfo = None
    for ldName, info in ldSectionInfo.items():
        if ldName.lower() == secName.lower():
            secInfo = info
            break

    if secInfo and secInfo.get('vma_region'):
        cat = regionToCategory(secInfo['vma_region'])
        if cat:
            return cat

    # Fallback: try to classify by VMA address range
    if secName in sectionInfo:
        addr = sectionInfo[secName]['address']
        if addr:
            cat = _classifyByAddress(addr)
            if cat:
                return cat

    # Final fallback: hardcoded patterns
    return categorizeSection(secName)


def _classifyByAddress(addr):
    """Classify a section by checking which MEMORY region its address falls into."""
    for regionName, region in ldMemoryRegions.items():
        origin = region['origin']
        length = region['length']
        if length > 0 and origin <= addr < origin + length:
            cat = regionToCategory(regionName)
            if cat:
                return cat
    return None


def hasFlashImageFromLD(secName):
    """Determine if a section has a flash image, using linker script data.

    A section has a flash image if:
    - It is NOT (NOLOAD) and NOT (COPY)
    - AND its LMA region is a FLASH-type region, OR its VMA region is FLASH
      (for boards like rtl8730e that don't use AT >, the VMA region IS the flash)

    Falls back to hasFlashImage() if the section is not in the linker script.
    """
    secInfo = None
    for ldName, info in ldSectionInfo.items():
        if ldName.lower() == secName.lower():
            secInfo = info
            break

    if secInfo:
        if secInfo.get('noload') or secInfo.get('copy'):
            return False
        # Check LMA region first (bk7239n uses AT > FLASH)
        lmaRegion = secInfo.get('lma_region')
        if lmaRegion:
            lmaCat = regionToCategory(lmaRegion)
            if lmaCat == 'FLASH':
                return True
            # LMA is in non-flash region -- no flash image
            return False
        # No AT > directive -- check VMA region (rtl8730e style)
        vmaRegion = secInfo.get('vma_region')
        if vmaRegion:
            vmaCat = regionToCategory(vmaRegion)
            if vmaCat == 'FLASH':
                return True
            # For rtl8730e, DRAM sections are copied from flash by image-tool
            # even without AT >, so they DO have flash images
            if vmaCat == 'DRAM':
                return True
            return False

    # Fallback: check by address (is the section's LMA in a flash region?)
    # For map file sections with "load address", we could check LMA,
    # but sectionInfo only has VMA address. Use the hardcoded fallback.
    return hasFlashImage(secName)


def printMemorySummary():
    """Print SRAM, PSRAM/DRAM, and Flash usage summary per library/binary with section breakdown."""
    # First, detect the board to determine column labels
    activeSections = getActiveSections()
    for secName in activeSections:
        detectBoard(secName)

    # Use LD-driven classification if linker script was provided (-L)
    useLD = bool(options.ldscript and ldSectionInfo)
    if useLD:
        classifyFn = categorizeSectionFromLD
        flashImgFn = hasFlashImageFromLD
    else:
        classifyFn = categorizeSection
        flashImgFn = hasFlashImage

    # Determine if we should show DRAM or PSRAM column based on board

    showDRAM = (detectedBoard == "rtl8730e")
    ramColName = "DRAM" if showDRAM else "PSRAM"
    
    print("######################################")
    print("##  SRAM, %s and Flash Usage  ##" % ramColName)
    print("######################################")
    print()
    
    # Print section categorization legend - dynamically generated from actual sections
    print("Detected Board: %s" % detectedBoard)
    print()
    print("Section Categories (dynamically detected from map file):")
    
    # Categorize active sections dynamically
    flashSecs = []
    sramSecs = []
    ramSecs = []  # DRAM or PSRAM
    otherSecs = []
    
    for secName in activeSections:
        category = classifyFn(secName)
        if category == 'FLASH':
            flashSecs.append(secName)
        elif category == 'SRAM':
            sramSecs.append(secName)
        elif category in ['DRAM', 'PSRAM']:
            ramSecs.append(secName)
        else:
            otherSecs.append(secName)

    # Print legend with actual sections found in map file
    source = "linker script + map file" if useLD else "map file (pattern matching)"
    print("  (Source: %s)" % source)

    if flashSecs:
        print("  FLASH (XIP, in flash .bin): " + ", ".join(sorted(flashSecs)))
    if sramSecs:
        # Distinguish SRAM sections that are also in flash .bin (copied at boot) vs NOLOAD
        sramInFlash = [s for s in sramSecs if flashImgFn(s)]
        sramNoLoad = [s for s in sramSecs if not flashImgFn(s)]
        print("  SRAM (runtime):")
        if sramInFlash:
            print("    In flash .bin (copied at boot): " + ", ".join(sorted(sramInFlash)))
        if sramNoLoad:
            print("    NOLOAD (not in flash .bin):     " + ", ".join(sorted(sramNoLoad)))
    if ramSecs:
        # Distinguish PSRAM/DRAM sections that are in flash .bin vs NOLOAD
        ramInFlash = [s for s in ramSecs if flashImgFn(s)]
        ramNoLoad = [s for s in ramSecs if not flashImgFn(s)]
        print("  %s (runtime):" % ramColName)
        if ramInFlash:
            print("    In flash .bin (copied at boot): " + ", ".join(sorted(ramInFlash)))
        if ramNoLoad:
            print("    NOLOAD (not in flash .bin):     " + ", ".join(sorted(ramNoLoad)))
    if otherSecs:
        print("  OTHER: " + ", ".join(sorted(otherSecs)))
    if not (flashSecs or sramSecs or ramSecs or otherSecs):
        print("  (no sections found)")
    print()

    
    activeSections = getActiveSections()
    
    # Categorize all active sections
    sramSections = []
    ramSections = []  # DRAM or PSRAM depending on board
    flashSections = []
    otherSections = []
    
    for secName in activeSections:
        hasData = False
        if secName in sectionLibSizes:
            for lib in sectionLibSizes[secName]:
                if sectionLibSizes[secName][lib] > 0:
                    hasData = True
                    break
        
        if hasData:
            category = classifyFn(secName)
            if category == 'SRAM':
                sramSections.append(secName)

            elif category == 'DRAM':
                ramSections.append(secName)
            elif category == 'PSRAM':
                ramSections.append(secName)
            elif category == 'FLASH':
                flashSections.append(secName)
            else:
                otherSections.append(secName)
    
    # Collect all libraries
    allLibs = set()
    for secName in activeSections:
        if secName in sectionLibSizes:
            for lib in sectionLibSizes[secName]:
                allLibs.add(lib)
    
    if not allLibs:
        print("No library data found.")
        return
    
    # Calculate SRAM, DRAM/PSRAM, Flash, Other usage and per-section breakdown for each library
    libMemory = {}
    libSectionBreakdown = {}
    
    for lib in allLibs:
        sram_size = 0
        ram_size = 0  # DRAM or PSRAM
        flash_size = 0
        flash_bin_size = 0  # actual .bin contribution (includes sections copied from flash)
        other_size = 0
        sectionSizes = {}

        for secName in activeSections:
            size = sectionLibSizes.get(secName, {}).get(lib, 0)
            sectionSizes[secName] = size
            category = classifyFn(secName)
            if category == 'SRAM':
                sram_size += size
            elif category in ['DRAM', 'PSRAM']:
                ram_size += size
            elif category == 'FLASH':
                flash_size += size
            else:
                other_size += size
            # Flash .bin: includes ALL sections with a flash image (XIP + copied to RAM)
            if flashImgFn(secName):
                flash_bin_size += size

        libMemory[lib] = {
            'sram': sram_size,
            'ram': ram_size,  # DRAM or PSRAM
            'flash': flash_size,
            'flash_bin': flash_bin_size,
            'other': other_size,
            'total': sram_size + ram_size + flash_size + other_size
        }
        libSectionBreakdown[lib] = sectionSizes

    
    # Sort libraries by total size (descending)
    sortedLibs = sorted(libMemory.keys(), key=lambda l: libMemory[l]['total'], reverse=True)
    
    # Build section order: Flash sections first, then SRAM, then DRAM/PSRAM, then Other
    orderedSections = flashSections + sramSections + ramSections + otherSections
    
    # Calculate section totals first (for column width calculation)
    sectionTotals = {}
    for sec in orderedSections:
        sectionTotals[sec] = sum(libSectionBreakdown.get(lib, {}).get(sec, 0) for lib in sortedLibs)
    
    # Calculate grand total for column width calculation
    grandTotal = sum(libMemory[lib]['total'] for lib in sortedLibs)
    
    # Calculate column widths (include totals in width calculation)
    libColWidth = max(len("Library/Binary"), max(len(lib[:40]) for lib in sortedLibs))
    sectionColWidths = {}
    for sec in orderedSections:
        maxLen = len(sec)
        for lib in sortedLibs:
            size = libSectionBreakdown.get(lib, {}).get(sec, 0)
            maxLen = max(maxLen, len(str(size)))
        # Also consider the total value for column width
        maxLen = max(maxLen, len(str(sectionTotals[sec])))
        sectionColWidths[sec] = maxLen + 1
    
    summaryWidth = 15
    totalWidth = max(len("Total"), max(len(str(libMemory[lib]['total'])) for lib in sortedLibs), len(str(grandTotal))) + 1
    
    # Print header
    header = "Library/Binary".ljust(libColWidth)
    for sec in orderedSections:
        header += sec.rjust(sectionColWidths[sec])
    header += "SRAM".rjust(summaryWidth)
    header += ramColName.rjust(summaryWidth)
    header += "Flash XIP".rjust(summaryWidth)
    header += "Flash .bin".rjust(summaryWidth)
    header += "Other".rjust(summaryWidth)

    header += "Total".rjust(totalWidth)
    print(header)
    print("-" * len(header))

    # Print each library's memory usage with section breakdown
    totalSram = 0
    totalRam = 0  # DRAM or PSRAM
    totalFlash = 0
    totalFlashBin = 0  # flash .bin (includes sections copied from flash)
    totalOther = 0

    for lib in sortedLibs:
        mem = libMemory[lib]
        if mem['total'] == 0:
            continue

        line = lib[:libColWidth].ljust(libColWidth)
        for sec in orderedSections:
            size = libSectionBreakdown.get(lib, {}).get(sec, 0)
            line += str(size).rjust(sectionColWidths[sec])

        line += str(mem['sram']).rjust(summaryWidth)
        line += str(mem['ram']).rjust(summaryWidth)
        line += str(mem['flash']).rjust(summaryWidth)
        line += str(mem['flash_bin']).rjust(summaryWidth)
        line += str(mem['other']).rjust(summaryWidth)
        line += str(mem['total']).rjust(totalWidth)
        print(line)

        totalSram += mem['sram']
        totalRam += mem['ram']
        totalFlash += mem['flash']
        totalFlashBin += mem['flash_bin']
        totalOther += mem['other']

    # Print totals
    print("-" * len(header))
    totalLine = "TOTAL".ljust(libColWidth)
    for sec in orderedSections:
        secTotal = sum(libSectionBreakdown.get(lib, {}).get(sec, 0) for lib in sortedLibs)
        # Ensure column width is enough for total value
        secTotalStr = str(secTotal)
        if len(secTotalStr) > sectionColWidths[sec]:
            sectionColWidths[sec] = len(secTotalStr) + 1
        totalLine += secTotalStr.rjust(sectionColWidths[sec])
    totalLine += str(totalSram).rjust(summaryWidth)
    totalLine += str(totalRam).rjust(summaryWidth)
    totalLine += str(totalFlash).rjust(summaryWidth)
    totalLine += str(totalFlashBin).rjust(summaryWidth)
    totalLine += str(totalOther).rjust(summaryWidth)
    totalLine += str(grandTotal).rjust(totalWidth)
    print(totalLine)

    print()
    
    # Print summary percentages
    if grandTotal > 0:
        print("Memory Distribution (runtime):")
        print("  SRAM:   %d bytes (%.2f%%)" % (totalSram, (totalSram * 100.0 / grandTotal)))
        print("  %s:  %d bytes (%.2f%%)" % (ramColName, totalRam, (totalRam * 100.0 / grandTotal)))
        print("  Flash:  %d bytes (%.2f%%)" % (totalFlash, (totalFlash * 100.0 / grandTotal)))
        if totalOther > 0:
            print("  Other:  %d bytes (%.2f%%)" % (totalOther, (totalOther * 100.0 / grandTotal)))
    print()

    # ---- Flash Image Size (actual .bin stored in flash) ----
    # The runtime "Flash" column above only counts sections resident in flash
    # (XIP code/rodata).  However the flash IMAGE also contains the initial
    # values of sections that are copied to RAM/PSRAM/IRAM at boot (.data,
    # .iram, .code, .psram_data, .psram_code, ...).  The only sections NOT in
    # the flash image are NOLOAD / zero-init ones (.bss, .psram_bss, .heap, ...).
    flashImageSize = 0
    flashImageCopiedSecs = []   # sections copied to RAM at boot (not XIP-resident)
    flashImageResidentSecs = []  # sections that live in flash (XIP/rodata)

    for secName in activeSections:
        secTotal = sum(sectionLibSizes.get(secName, {}).values())
        if secTotal == 0:
            continue
        if flashImgFn(secName):
            flashImageSize += secTotal
            category = classifyFn(secName)

            if category == 'FLASH':
                flashImageResidentSecs.append(secName)
            else:
                # This section lives in SRAM/PSRAM at runtime but its initial
                # values are stored in the flash image and copied at boot.
                flashImageCopiedSecs.append(secName)

    if flashImageSize > 0:
        print("Flash Image Size: %d bytes" % flashImageSize)
        print("  (actual .bin burned to flash; includes XIP-resident code/rodata)")
        if flashImageCopiedSecs:
            print("  Sections copied from flash to RAM/%s at boot:" % ramColName)
            for secName in sorted(flashImageCopiedSecs):
                secTotal = sum(sectionLibSizes.get(secName, {}).values())
                print("    %s: %d bytes" % (secName, secTotal))
        print("  Resident in flash (XIP): " + ", ".join(sorted(flashImageResidentSecs)))
        print("")


# ---- Output ----

if options.all:
    options.totsize = options.libsize = options.details = options.memory = True

if options.totsize:
    printSectionSummary()

if options.libsize:
    print("######################################")
    print("##          Library Sizes           ##")
    print("######################################")
    printLibSizes()

if options.details:
    print("######################################")
    print("##              Details             ##")
    print("######################################")
    printDetails()
    print("######################################\n")

# Parse linker script if provided (-L)
if options.ldscript:
    parseLinkerScript(options.ldscript)

if options.memory:
    printMemorySummary()

