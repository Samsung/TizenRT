# light sample for st_things

This is a light example for a st_things.

1. [Create a cloud-connected device](https://smartthings.developer.samsung.com/develop/workspace/ide/create-a-cloud-connected-device.html)
2. [Edit device info json file](#edit-device-info-json-file)
3. [Build light sample](#build-light-sample)
4. [Download(Flash)](#downloadflash)

## Create a cloud-connected device.
To connect the cloud, create device information on [devworkspace](https://devworkspace.developer.samsung.com/smartthingsconsole/iotweb/site/index.html).
Find more details [here](https://smartthings.developer.samsung.com/develop/workspace/ide/create-a-cloud-connected-device.html).

## Edit device info json file.
1. Move into $TIZENRT_BASEDIR/apps/examples/st_things/light/contents/.

	```bash
	cd  [TizenRT]/apps/examples/st_things/light/contents/
	```

2. Copy to device_def.json to [TizenRT]/tools/fs/contents/.

	```bash
	cp device_def.json ../../../../../tools/fs/contents/
	```

3. Move to [TizenRT]/tools/fs/contents/.

	```bash
	cd ../../../../../tools/fs/contents/
	```

4. Edit device_def.json.
	Replace vendorId(VID) and manufacturerName(MNID) values with the created a cloud-connected device information from above.

## Build light sample.
Configure the build from $TIZENRT_BASEDIR/os/tools directory.

```bash
./configure.sh artik053/st_things
cd ..
make
```

## Download(Flash)
Add 'privatekey', 'certificate', and json file(ex. device_def.json) to $TIZENRT_BASEDIR/tools/fs/contents.

```bash
make download ALL
```
