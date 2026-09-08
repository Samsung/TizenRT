#!/bin/bash
#
# Script to convert libcxx-17.0.6 tests to TizenRT format
# Run this on the Docker build environment where the source files are available
#
# Usage:
#   bash run_conversion.sh [category]
#
# Categories: utilities, containers, algorithms, all
#

set -e

# Configuration - adjust these paths to your Docker environment
LIBCXX_SRC="/root/tizenrt/external/libcxx-17.0.6.src"
TEST_SRC="${LIBCXX_SRC}/test"
OUTPUT_DIR="/root/tizenrt/external/libcxx-test"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Check source directory exists
if [ ! -d "$TEST_SRC" ]; then
    echo "ERROR: Source directory not found: $TEST_SRC"
    echo "Please adjust LIBCXX_SRC path in this script"
    exit 1
fi

# Check Python3 is available
if ! command -v python3 &> /dev/null; then
    echo "ERROR: python3 is required but not found"
    exit 1
fi

CATEGORY="${1:-all}"

convert_category() {
    local cat="$1"
    echo ""
    echo "================================================================"
    echo "Converting category: $cat"
    echo "================================================================"
    python3 "${SCRIPT_DIR}/convert_tests.py" \
        --src "$TEST_SRC" \
        --output "$OUTPUT_DIR" \
        --category "$cat" \
        --generate-makefile \
        --verbose
}

case "$CATEGORY" in
    utilities)
        convert_category "utilities"
        ;;
    containers)
        convert_category "containers"
        ;;
    algorithms)
        convert_category "algorithms"
        ;;
    all)
        # Priority 1: Utilities (tuple, optional, variant, any, etc.)
        convert_category "utilities"

        # Priority 2: Containers (set, multiset, deque, adaptors, etc.)
        convert_category "containers"

        # Priority 3: Algorithms
        convert_category "algorithms"
        ;;
    *)
        echo "Unknown category: $CATEGORY"
        echo "Usage: $0 [utilities|containers|algorithms|all]"
        exit 1
        ;;
esac

echo ""
echo "================================================================"
echo "Conversion complete!"
echo "================================================================"
echo ""
echo "Next steps:"
echo "1. Review generated files in $OUTPUT_DIR"
echo "2. Copy support headers (see copy_support_headers.sh)"
echo "3. Integrate Makefile snippets into $OUTPUT_DIR/Makefile"
echo "4. Add Kconfig entries for new sub-categories"
echo "5. Build and fix compilation issues"