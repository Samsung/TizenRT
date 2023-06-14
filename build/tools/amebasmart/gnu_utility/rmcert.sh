#!/bin/bash 

################
# Library
################
Usage() {
    echo "Usage: $0 [Image Name]"
}

################
# Main
################
if [ "$#" -lt 1 ]; then
    Usage
    exit 1
fi

# Get Parameters
file=$1

dd if="$file" of="$file~" bs=1 skip=4096
mv "$file~" "$file"
