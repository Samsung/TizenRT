#!/usr/bin/env bash
###########################################################################
#
# Copyright 2025 All Rights Reserved.
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
# heapfailanalyzer.sh

# Updated to run from tools folder and support absolute paths

# Get the directory where the script resides (tools/memory folder)
MEMORY_DIR=$(dirname "$(readlink -f "$0")")
TOOLSDIR="$(dirname "${MEMORY_DIR}")"
OSDIR="$TOOLSDIR/../os"
CONFIGFILE="${OSDIR}/.config"
TOPDIR="${TOOLSDIR}/.."

DOCKER_IMAGE=
DOCKER_PUBLIC_IMAGE="tizenrt/tizenrt"
DOCKER_VERSION="2.0.1"
OUTPUTFILE="loganalyzer_output_"

# Python script is now in the same directory as this shell script
PYTHON_SCRIPT="/root/tizenrt/tools/memory/heapfailanalyzer.py"

# Default output directory is now in the memory folder
DEFAULT_OUTPUT_DIR="${MEMORY_DIR}/heapfailanalyzer_output"
mkdir -p ${DEFAULT_OUTPUT_DIR}

# Allow write permission to the directory
chmod 777 ${DEFAULT_OUTPUT_DIR}

# Checking docker is installed
nodocker() {
cat <<EOF

You don't have docker installed in your path.
Please find our quick start to build using docker client.

https://github.com/samsung/tizenrt#quick-start

EOF
}

if ! which docker > /dev/null; then
  nodocker 1>&2
  exit 1
fi

# check docker image and pull docker image
function GET_SPECIFIC_DOCKER_IMAGE()
{
  # check existing docker image for specified version
  echo "Check Docker Image"
  DOCKER_IMAGES=`docker images | grep 'tizenrt' | awk '{print $1":"$2}'`
  for im in ${DOCKER_IMAGES}; do
    # check public image first
    if [ "$im" == "$DOCKER_PUBLIC_IMAGE:$DOCKER_VERSION" ]; then
      DOCKER_IMAGE=$DOCKER_PUBLIC_IMAGE
      DOCKER_IMAGE_EXIST="true"
      break
    fi
    # Can add other docker image
  done

  # pull the docker image with specified version
  if [ "$DOCKER_IMAGE_EXIST" != "true" ]; then
    # try to get public image first
    docker pull ${DOCKER_PUBLIC_IMAGE}:${DOCKER_VERSION}
    if [ $? -eq 0 ]; then
      echo "success to pull docker image: ${DOCKER_PUBLIC_IMAGE}:${DOCKER_VERSION}"
      DOCKER_IMAGE=$DOCKER_PUBLIC_IMAGE
      return
    fi
    echo "failed to pull docker image: ${DOCKER_PUBLIC_IMAGE}:${DOCKER_VERSION}"
    # Can add other docker image
    exit 1
  fi
}

function ANALYZE_LOG()
{
  if [ $# -lt 2 ]; then
    echo "Invalid arguments!!"
    HELP
    exit 1
  fi

  LOG_FILE="$2"
  OUTPUT_DIR="${DEFAULT_OUTPUT_DIR}"

  # Handle absolute vs relative paths for log file
  if [[ "$LOG_FILE" = /* ]]; then
    # Absolute path provided
    LOG_ABS_PATH="$LOG_FILE"
  else
    # Relative path - interpret as relative to memory directory
    LOG_ABS_PATH="${MEMORY_DIR}/${LOG_FILE}"
  fi

  # Extract filename and directory
  LOG_FILENAME=$(basename "$LOG_ABS_PATH")
  LOG_DIR=$(dirname "$LOG_ABS_PATH")

  # Verify log file exists
  if [ ! -f "${LOG_ABS_PATH}" ]; then
    echo "$LOG_ABS_PATH: No such file, try again"
    exit 1
  fi

  # Process additional arguments
  shift 2
  EXCEL_OUTPUT=""
  EXCEL_OUTPUT_FLAG=""
  CUSTOM_OUTPUT_DIR=""

  # Check for output directory parameter
  for i in "$@"; do
    if [[ $i == --output-dir=* ]]; then
      CUSTOM_OUTPUT_DIR="${i#*=}"
      # Remove this parameter from the args list
      set -- "${@/$i/}"
      break
    fi
  done

  # Use custom output directory if provided
  if [ ! -z "$CUSTOM_OUTPUT_DIR" ]; then
    # Handle absolute vs relative paths for output directory
    if [[ "$CUSTOM_OUTPUT_DIR" = /* ]]; then
      OUTPUT_DIR="$CUSTOM_OUTPUT_DIR"
    else
      OUTPUT_DIR="${MEMORY_DIR}/${CUSTOM_OUTPUT_DIR}"
    fi
    
    # Create the output directory if it doesn't exist
    mkdir -p ${OUTPUT_DIR}
    chmod 777 ${OUTPUT_DIR}
  fi

  # Check if --export parameter is present
  for arg in "$@"; do
    if [[ $arg == --export* ]]; then
      # Extract the excel filename
      if [[ $arg == *=* ]]; then
        EXCEL_OUTPUT="${arg#*=}"
      elif [ $(($# + 1)) -gt 0 ] && [[ ! $((1)) == --* ]]; then
        EXCEL_OUTPUT="$((1))"
        shift
      else
        EXCEL_OUTPUT="summary.xlsx"
      fi

      # If no path specified, use OUTPUT_DIR
      if [[ ! $EXCEL_OUTPUT == */* ]]; then
        EXCEL_OUTPUT="${OUTPUT_DIR}/${EXCEL_OUTPUT}"
      fi

      # Replace the --export parameter with one that points to /output in container
      EXCEL_OUTPUT_BASENAME=$(basename "$EXCEL_OUTPUT")
      EXCEL_OUTPUT_FLAG="--export /output/${EXCEL_OUTPUT_BASENAME}"
    fi
  done

  # Build command with options
  DOCKER_CMD=(docker run -it --rm)
  # Mount the directory containing the log file to /input in the container
  DOCKER_CMD+=(-v "${LOG_DIR}:/input")
  # Mount the output directory to /output in the container
  DOCKER_CMD+=(-v "${OUTPUT_DIR}:/output")
  # Mount the tizenrt directory
  DOCKER_CMD+=(-v "${TOPDIR}:/root/tizenrt")
  # Working directory is now tools/memory
  DOCKER_CMD+=(-w /root/tizenrt/tools/memory --privileged)

  DOCKER_CMD+=(${DOCKER_IMAGE}:${DOCKER_VERSION})
  DOCKER_CMD+=(bash -c)

  # Build the Python command with proper paths
  # Use the log file from the mounted input directory
  PYTHON_CMD="python3 -u ${PYTHON_SCRIPT} \"/input/${LOG_FILENAME}\""

  # Add the Excel output flag if specified
  if [ ! -z "$EXCEL_OUTPUT_FLAG" ]; then
    PYTHON_CMD+=" ${EXCEL_OUTPUT_FLAG}"
  else
    # Check if --export is in the remaining parameters
    EXPORT_FOUND=false
    for arg in "$@"; do
      if [[ $arg == --export* ]]; then
        EXPORT_FOUND=true
        break
      fi
    done

    # If not found, add default --export flag
    if [ "$EXPORT_FOUND" = false ]; then
      TIMESTAMP=$(date "+%Y%m%d_%H%M%S")
      EXCEL_OUTPUT="${OUTPUT_DIR}/summary_${TIMESTAMP}.xlsx"
      PYTHON_CMD+=" --export /output/summary_${TIMESTAMP}.xlsx"
    fi
  fi

  # Add any remaining parameters
  for arg in "$@"; do
    # Skip any --export parameters as we've handled them
    if [[ ! $arg == --export* ]] && [[ ! $arg == --output-dir=* ]]; then
      PYTHON_CMD+=" $arg"
    fi
  done

  # Add the Python command as a single quoted argument to bash -c
  DOCKER_CMD+=("$PYTHON_CMD")

  # Create output file
  TIMESTAMP=$(date "+%Y.%m.%d-%H.%M.%S")
  OUTPUTFILE+="analyze_${TIMESTAMP}.txt"
  OUTPUTFILE="${OUTPUT_DIR}/${OUTPUTFILE}"

  echo "Executing: $PYTHON_CMD" | tee "$OUTPUTFILE"
  echo "" >> "$OUTPUTFILE"

  # Execute analyzer script inside Docker container
  "${DOCKER_CMD[@]}" | tee -a "$OUTPUTFILE"
  echo "" | tee -a "$OUTPUTFILE"
  echo ">> Output is stored in $OUTPUTFILE" | tee -a "$OUTPUTFILE"

  if [ ! -z "$EXCEL_OUTPUT" ]; then
    echo ">> Excel report is available at: $EXCEL_OUTPUT" | tee -a "$OUTPUTFILE"
  fi
}

function ANALYZE_MENU()
{
  echo =============================================================================
  echo " \"Log Analyzer Tool\""
  echo =============================================================================
  echo " \"Enter the log file path (absolute or relative to tools/memory folder):\""
  read LOG_FILE

  # Handle absolute vs relative paths
  if [[ "$LOG_FILE" = /* ]]; then
    # Absolute path provided
    LOG_ABS_PATH="$LOG_FILE"
  else
    # Relative path - interpret as relative to memory directory
    LOG_ABS_PATH="${MEMORY_DIR}/${LOG_FILE}"
  fi

  if [ ! -f ${LOG_ABS_PATH} ]; then
    echo "$LOG_ABS_PATH: No such file, try again"
    return
  fi

  echo " \"Enter output directory path (leave empty for default, absolute or relative to tools/memory folder):\""
  read OUTPUT_DIR

  # If output directory is provided
  if [ ! -z "$OUTPUT_DIR" ]; then
    ANALYZE_LOG analyze "$LOG_FILE" "--output-dir=${OUTPUT_DIR}"
  else
    ANALYZE_LOG analyze "$LOG_FILE"
  fi
}

function HELP()
{
  echo "Usage: heapfailanalyzer.sh [menu] | [help] | [analyze log_file [options]]"
  echo ""
  echo "menu      Display interactive menu"
  echo "help      Display this help menu"
  echo "analyze   Analyze the specified log file"
  echo "          Ex: heapfailanalyzer.sh analyze logs/system.log"
  echo "          Ex: heapfailanalyzer.sh analyze /absolute/path/to/system.log"
  echo ""
  echo "Options:"
  echo "  --output-dir=PATH    Specify a custom output directory (absolute or relative to tools/memory folder)"
  echo "  --export=FILENAME    Specify Excel file name for output report"
  echo ""
  echo "Notes:"
  echo "  - Default output location is ./heapfailanalyzer_output/ in tools/memory folder"
  echo "  - Relative paths are interpreted as relative to the tools/memory folder"
  echo ""
}

function MENU()
{
  unset SELECTED_START

  while [ 1 ]; do
    OUTPUTFILE="loganalyzer_output_"
    if [ ! -z "$1" ];then
      SELECTED_START=$1
    else
      echo ======================================================
      echo " \"Select Option\""
      echo ======================================================
      echo " \"1. Analyze Log File\""
      echo " \"h. Help\""
      echo " \"x. Exit\""
      echo ======================================================
      read SELECTED_START
    fi

    case ${SELECTED_START,,} in
      1|analyze)
        ANALYZE_MENU
        ;;
      h|help)
        HELP
        ;;
      x|exit)
        exit 0
        ;;
      *)
        ;;
    esac
  done
}

# Main script execution
GET_SPECIFIC_DOCKER_IMAGE
echo "Docker Image Version : ${DOCKER_IMAGE}:${DOCKER_VERSION}"

if [ -z "$1" ]; then
  MENU
elif [ "$1" == "menu" ]; then
  MENU
elif [ "$1" == "analyze" ]; then
  ANALYZE_LOG $@
elif [ "$1" == "help" ]; then
  HELP
else
  HELP
fi

exit 0
