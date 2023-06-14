#!/bin/bash 

################
# Library
################
Usage() {
    echo "Usage: $0 [Image2 Name] [Boot Image Name][option] [AP Image Name][option]"
}

# Parameter:
#     value, width, dest
function MakeFixedWidthHeaderString() {
    local __value=$1
    local __width=$2
    local __dest=$3
    local __reorder=$4
    local __header_raw
    local __header_raw_reorder
    local __header_array

    if [[ "$__dest" ]]; then

        __header_raw=$(printf "%0""$__width""x" $__value)
        # echo $__header_raw

        if [ "$__reorder" -eq 1 ]; then
                # 20000680 to 80060020
                for (( i=$__width; i > 0; i-=2 ))
                do
                    __header_raw_reorder+=$(echo ${__header_raw:(i-2):2})
                done
        else
                __header_raw_reorder=$__header_raw
        fi
        #echo $__header_raw_reorder

        __header_array=($(echo $__header_raw_reorder | sed 's/\(.\)/\1 /g'))

        for (( i=0; i < $__width; i+=2))
        do
            eval $__dest+='\\x'"${__header_array[$i]}${__header_array[$i+1]}"
        done
    fi
}

################
# Main
################
if [ "$#" -lt 1 ]; then
    Usage
    exit 1
fi

if [ "$#" -gt 4 ]; then
    Usage
    exit 1
fi

# Get Parameters
COMPILEOS=$(uname)

# Get Parameters
COMPILEOS=$(uname)
if [ "$COMPILEOS" != "Darwin" ]; then
	COMPILEOS=$(uname -o)
fi

cd ./gnu_utility
if [ "$COMPILEOS" == "GNU/Linux" ]; then
	CHECKSUMTOOL=./checksum
else
	CHECKSUMTOOL=./checksum.exe
fi

# Constant Variables
VERSION=0xFFFFFFFF
OTA_NUM=1
OTA_SIGN=0x4F544131
HEADER_LEN=24
CHECKSUM=0
IMAGE_LEN=0
OFFSET=32
IMAGE_ID=0xFFFFFFFF
IMAGE_FILENAME_1=""
IMAGE_FILENAME_2=""
IMAGE_FILENAME_1_NEW=""
IMAGE_FILENAME_2_NEW=""
IMAGE_LEN_1=0
IMAGE_LEN_2=0
ENDFLAG=0


if [ "$COMPILEOS" == "GNU/Linux" ]; then
	IMAGE2_FILENAME=$1
elif [ "$COMPILEOS" == "Darwin" ]; then
	IMAGE2_FILENAME=$(relative_to_path $(pwd) $1)
else
	IMAGE2_FILENAME=$(realpath --relative-to=$(pwd) $1)
fi

if [ "$#" == 2 ]; then
	IMAGE_FILENAME_1=$2
    echo "$IMAGE_FILENAME_1"
	IMAGE_FILENAME_1_NEW=$(basename $IMAGE_FILENAME_1)
	OFFSET=56
	OTA_NUM=2
elif [ "$#" == 3 ]; then
	IMAGE_FILENAME_1=$2
	IMAGE_FILENAME_2=$3
	IMAGE_FILENAME_1_NEW=$(basename $IMAGE_FILENAME_1)
	IMAGE_FILENAME_2_NEW=$(basename $IMAGE_FILENAME_2)
	OFFSET=80
	OTA_NUM=3
fi

CURR_PATH=$(dirname $IMAGE2_FILENAME)
#IMAGE_FILENAME_PREPEND="${IMAGE2_FILENAME%.*}"'_prepend.'"${IMAGE2_FILENAME##*.}"
IMAGE_FILENAME_PREPEND=$CURR_PATH/OTA_All.bin
IMAGE_FILENAME_NEW=$(basename $IMAGE2_FILENAME)

HEADER_FINAL=''
MakeFixedWidthHeaderString $VERSION   8  HEADER_FINAL 0
MakeFixedWidthHeaderString $OTA_NUM   8  HEADER_FINAL 1

if [ "$IMAGE_FILENAME_1_NEW" == "km4_boot_all.bin" ]; then
	IMAGE_ID=0
	if [ "$COMPILEOS" != "Darwin" ]; then
		IMAGE_LEN_1=$(du -b $IMAGE_FILENAME_1 | cut -f 1)
	else
		IMAGE_LEN_1=$(stat -f%z $IMAGE_FILENAME_1)
	fi
    MakeFixedWidthHeaderString $OTA_SIGN   8  HEADER_FINAL 0
    MakeFixedWidthHeaderString $HEADER_LEN   8  HEADER_FINAL 1
    MakeFixedWidthHeaderString $CHECKSUM    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $IMAGE_LEN_1    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $OFFSET    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $IMAGE_ID    8  HEADER_FINAL 1	
fi

if [ "$COMPILEOS" != "Darwin" ]; then
    IMAGE_LEN=$(du -b $IMAGE2_FILENAME | cut -f 1)
else
    IMAGE_LEN=$(stat -f%z $IMAGE2_FILENAME)
fi

IMAGE_ID=1
let OFFSET=$OFFSET+$IMAGE_LEN_1

if [ "$IMAGE_FILENAME_NEW" == "km0_km4_app.bin" ]; then
    MakeFixedWidthHeaderString $OTA_SIGN   8  HEADER_FINAL 0
    MakeFixedWidthHeaderString $HEADER_LEN   8  HEADER_FINAL 1
    MakeFixedWidthHeaderString $CHECKSUM    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $IMAGE_LEN    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $OFFSET    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $IMAGE_ID    8  HEADER_FINAL 1
fi

if [ "$IMAGE_FILENAME_1_NEW" == "ap_image_all.bin" ]; then
	IMAGE_ID=2
	let OFFSET=$OFFSET+$IMAGE_LEN
	if [ "$COMPILEOS" != "Darwin" ]; then
		IMAGE_LEN_1=$(du -b $IMAGE_FILENAME_1 | cut -f 1)
	else
		IMAGE_LEN_1=$(stat -f%z $IMAGE_FILENAME_1)
	fi
    MakeFixedWidthHeaderString $OTA_SIGN   8  HEADER_FINAL 0
    MakeFixedWidthHeaderString $HEADER_LEN   8  HEADER_FINAL 1
    MakeFixedWidthHeaderString $CHECKSUM    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $IMAGE_LEN_1    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $OFFSET    8  HEADER_FINAL 1
	MakeFixedWidthHeaderString $IMAGE_ID    8  HEADER_FINAL 1	
	ENDFLAG=1
fi

#echo "$ENDFLAG"

if [ $ENDFLAG != 1 ]; then
	if [ "$IMAGE_FILENAME_2_NEW" == "ap_image_all.bin" ]; then
		IMAGE_ID=2
		let OFFSET=$OFFSET+$IMAGE_LEN
		if [ "$COMPILEOS" != "Darwin" ]; then
			IMAGE_LEN_2=$(du -b $IMAGE_FILENAME_2 | cut -f 1)
		else
			IMAGE_LEN_2=$(stat -f%z $IMAGE_FILENAME_2)
		fi
		MakeFixedWidthHeaderString $OTA_SIGN   8  HEADER_FINAL 0
		MakeFixedWidthHeaderString $HEADER_LEN   8  HEADER_FINAL 1
		MakeFixedWidthHeaderString $CHECKSUM    8  HEADER_FINAL 1
		MakeFixedWidthHeaderString $IMAGE_LEN_2    8  HEADER_FINAL 1
		MakeFixedWidthHeaderString $OFFSET    8  HEADER_FINAL 1
		MakeFixedWidthHeaderString $IMAGE_ID    8  HEADER_FINAL 1	
	fi
fi

if [ "$#" == 2 ]; then
	IMAGE_FILENAME_1=$2
    echo "$IMAGE_FILENAME_1"
	IMAGE_FILENAME_1_NEW=$(basename $IMAGE_FILENAME_1)
	OFFSET=56
	OTA_NUM=2
	if [ "$IMAGE_FILENAME_1_NEW" == "km4_boot_all.bin" ]; then
		echo -n -e $HEADER_FINAL | cat - $IMAGE_FILENAME_1 $IMAGE2_FILENAME > $IMAGE_FILENAME_PREPEND
		#checksum
		$CHECKSUMTOOL $IMAGE_FILENAME_1 $IMAGE_FILENAME_PREPEND 16
		$CHECKSUMTOOL $IMAGE2_FILENAME $IMAGE_FILENAME_PREPEND 40
	else
		echo -n -e $HEADER_FINAL | cat - $IMAGE2_FILENAME $IMAGE_FILENAME_1  > $IMAGE_FILENAME_PREPEND
		#checksum
		$CHECKSUMTOOL $IMAGE2_FILENAME $IMAGE_FILENAME_PREPEND 16
		$CHECKSUMTOOL $IMAGE_FILENAME_1 $IMAGE_FILENAME_PREPEND 40
	fi
elif [ "$#" == 3 ]; then
	if [ "$IMAGE_FILENAME_1_NEW" == "km4_boot_all.bin" ]; then
		echo -n -e $HEADER_FINAL | cat - $IMAGE_FILENAME_1 $IMAGE2_FILENAME $IMAGE_FILENAME_2 > $IMAGE_FILENAME_PREPEND
		#checksum
		$CHECKSUMTOOL $IMAGE_FILENAME_1 $IMAGE_FILENAME_PREPEND 16
		$CHECKSUMTOOL $IMAGE2_FILENAME $IMAGE_FILENAME_PREPEND 40
		$CHECKSUMTOOL $IMAGE_FILENAME_2 $IMAGE_FILENAME_PREPEND 64
	else
		echo -n -e $HEADER_FINAL | cat - $IMAGE2_FILENAME $IMAGE_FILENAME_1 $IMAGE_FILENAME_2 > $IMAGE_FILENAME_PREPEND
		#checksum
		$CHECKSUMTOOL $IMAGE2_FILENAME $IMAGE_FILENAME_PREPEND 16
		$CHECKSUMTOOL $IMAGE_FILENAME_1 $IMAGE_FILENAME_PREPEND 40
		$CHECKSUMTOOL $IMAGE_FILENAME_2 $IMAGE_FILENAME_PREPEND 64
	fi
else
	echo -n -e $HEADER_FINAL | cat - $IMAGE2_FILENAME > $IMAGE_FILENAME_PREPEND
	#checksum
	$CHECKSUMTOOL $IMAGE2_FILENAME $IMAGE_FILENAME_PREPEND 16	
fi




