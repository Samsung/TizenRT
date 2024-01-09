
#!/bin/bash 

################
# Library
################
Usage() {
    echo "Usage: $0 [Image Name] [aligned Bytes]"
}

################
# Main
################
if [ "$#" -lt 2 ]; then
    Usage
    exit 1
fi

# Get Parameters
IMAGE_FILENAME=$1
Aligned_Bytes=$2
filesize=$(stat -c "%s" $IMAGE_FILENAME)
newsize=$((((($filesize - 1) / $Aligned_Bytes) + 1) * $Aligned_Bytes))
padcount=$(($newsize - $filesize))

for (( i=$padcount; i > 0; i-- ))
do
	echo -n -e "\xFF" >> $IMAGE_FILENAME
done
