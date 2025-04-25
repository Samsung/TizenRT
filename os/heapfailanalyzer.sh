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

OSDIR=`test -d ${0%/*} && cd ${0%/*}; pwd`
CONFIGFILE="${OSDIR}/.config"
TOPDIR="${OSDIR}/.."
DOCKER_IMAGE=
DOCKER_PUBLIC_IMAGE="tizenrt/tizenrt"
DOCKER_VERSION="2.0.1"
OUTPUTFILE="loganalyzer_output_"
PYTHON_SCRIPT="/root/tizenrt/tools/memory/heapfailanalyzer.py"
# Create output directory for generated files
OUTPUT_DIR="${OSDIR}/../build/output/heapfailanalyzer_output"
mkdir -p ${OUTPUT_DIR}

#Allow write permission to the directory
chmod 777 -p ${OUTPUT_DIR}

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
LOG_FILENAME=$(basename "$LOG_FILE")
LOG_ABS_PATH=$(realpath "$LOG_FILE")

LOG_DIR=$(dirname "$LOG_ABS_PATH")

# Verify log file exists
if [ ! -f "${LOG_FILE}" ]; then
echo "$LOG_FILE: No such file, try again"
exit 1
fi

# mount the logfile directory as well
# LOG_FILE_ABS=(realpath "$2")
# LOG_FILE_ABS="$(cd "$dirname "$2")" && pwd)/$(basename "$2")"

# Process additional arguments
shift 2
EXCEL_OUTPUT=""
EXCEL_OUTPUT_FLAG=""

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
DOCKER_CMD+=(-w /root/tizenrt/os --privileged)


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
if [[ ! $arg == --export* ]]; then
PYTHON_CMD+=" $arg"
fi
done

# Add the Python command as a single quoted argument to bash -c
DOCKER_CMD+=("$PYTHON_CMD")
# echo "Executing: $PYTHON_CMD"
# stdbuf -oL "${DOCKER_CMD[@]}"

# echo ">> if export was successful, output Excel be in: $EXPORT_DIR_HOST/"

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
echo " \"Enter the log file path relative to os folder: (ex: ../logs/system.log)\""
read LOG_FILE

if [ ! -f ${LOG_FILE} ]; then
echo "$LOG_FILE: No such file, try again"
return
fi

# echo " \"Any additional options for the analyzer? (leave empty if none)\""
# echo " \"Example options: --export summary.xlsx --format json --detailed\""
# read -e ADDITIONAL_OPTS

# # Use eval to preserve quoted parameters correctly
# if [ -z "$ADDITIONAL_OPTS" ]; then
ANALYZE_LOG analyze "$LOG_FILE"
# else
# # Convert the input string to an array to preserve argument structure
# eval "ARGS=($ADDITIONAL_OPTS)"
# ANALYZE_LOG analyze "$LOG_FILE" "${ARGS[@]}"
# fi
}

function HELP()
{
echo "Usage: loganalyzer.sh [menu] | [help] | [analyze log_file [options]]"
echo ""
echo "menu Display interactive menu"
echo "help Display this help menu"
echo "analyze Analyze the specified log file"
echo " Ex: loganalyzer.sh analyze ../logs/system.log"
echo ""
echo "Parameters:"
echo " All parameters after the log file path are passed directly to the Python analyzer script."
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
