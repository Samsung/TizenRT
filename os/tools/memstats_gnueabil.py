#!/usr/bin/env python
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
                    if re.search('\*\(.*\)', nextline_stripped) != None or re.search('\**.o\(', nextline_stripped) != None:
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
            if re.search('\*\(.*\)', line.strip()) != None or re.search('\**.o\(', lsplit[0]) != None:
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
            elif len(lsplit) > 0 and re.search('\*fill\*', lsplit[0]) != None:
                if currentLibObj:
                    try:
                        if int(lsplit[1], 16) != 0:
                            size_value = int(lsplit[2], 16)
                            addLibSize(currentSection, currentLibObj, size_value)
                    except ValueError:
                        continue

infile.close()

# ---- Sections to display (skip debug) ----
SKIP_SECTIONS = set([
    '.debug_line', '.debug_info', '.debug_abbrev', '.debug_aranges',
    '.debug_str', '.debug_ranges', '.debug_frame', '.debug_loc',
    '.comment', '.ARM.attributes', '.rel.dyn', '.ARM.exidx'
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
                   '.data.rel.ro', '.got', '.got.plt', '.plt', '.plt.got',
                   '.eh_frame', '.eh_frame_hdr', '.ARM.extab', '.vectors']
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
    ram_patterns = ['.data', '.bss', '.ram', '.stack', '.heap',
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


def printMemorySummary():
    """Print SRAM, PSRAM/DRAM, and Flash usage summary per library/binary with section breakdown."""
    # First, detect the board to determine column labels
    activeSections = getActiveSections()
    for secName in activeSections:
        detectBoard(secName)
    
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
        category = categorizeSection(secName)
        if category == 'FLASH':
            flashSecs.append(secName)
        elif category == 'SRAM':
            sramSecs.append(secName)
        elif category in ['DRAM', 'PSRAM']:
            ramSecs.append(secName)
        else:
            otherSecs.append(secName)
    
    # Print legend with actual sections found in map file
    if flashSecs:
        print("  FLASH: " + ", ".join(sorted(flashSecs)))
    if sramSecs:
        print("  SRAM:  " + ", ".join(sorted(sramSecs)))
    if ramSecs:
        print("  %s:  " % ramColName + ", ".join(sorted(ramSecs)))
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
            category = categorizeSection(secName)
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
        other_size = 0
        sectionSizes = {}
        
        for secName in activeSections:
            size = sectionLibSizes.get(secName, {}).get(lib, 0)
            sectionSizes[secName] = size
            category = categorizeSection(secName)
            if category == 'SRAM':
                sram_size += size
            elif category in ['DRAM', 'PSRAM']:
                ram_size += size
            elif category == 'FLASH':
                flash_size += size
            else:
                other_size += size
        
        libMemory[lib] = {
            'sram': sram_size,
            'ram': ram_size,  # DRAM or PSRAM
            'flash': flash_size,
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
    header += "Flash".rjust(summaryWidth)
    header += "Other".rjust(summaryWidth)
    header += "Total".rjust(totalWidth)
    print(header)
    print("-" * len(header))
    
    # Print each library's memory usage with section breakdown
    totalSram = 0
    totalRam = 0  # DRAM or PSRAM
    totalFlash = 0
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
        line += str(mem['other']).rjust(summaryWidth)
        line += str(mem['total']).rjust(totalWidth)
        print(line)
        
        totalSram += mem['sram']
        totalRam += mem['ram']
        totalFlash += mem['flash']
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
    totalLine += str(totalOther).rjust(summaryWidth)
    totalLine += str(grandTotal).rjust(totalWidth)
    print(totalLine)
    print()
    
    # Print summary percentages
    if grandTotal > 0:
        print("Memory Distribution:")
        print("  SRAM:   %d bytes (%.2f%%)" % (totalSram, (totalSram * 100.0 / grandTotal)))
        print("  %s:  %d bytes (%.2f%%)" % (ramColName, totalRam, (totalRam * 100.0 / grandTotal)))
        print("  Flash:  %d bytes (%.2f%%)" % (totalFlash, (totalFlash * 100.0 / grandTotal)))
        if totalOther > 0:
            print("  Other:  %d bytes (%.2f%%)" % (totalOther, (totalOther * 100.0 / grandTotal)))
    print()


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

if options.memory:
    printMemorySummary()
