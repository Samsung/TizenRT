# st_things sample

This is an example for a st_things.

## How to build
Configure the build from $TIZENRT_BASEDIR/os/tools directory.
```bash
./configure.sh artik053/st_things
cd ..
make
```

## Download
Add 'privatekey', 'certificate', and json file(ex. sampleDevice.json) to $TIZENRT_BASEDIR/tools/fs/contents.
```bash
TIZENRT_BASEDIR/os$ make download ALL
```
