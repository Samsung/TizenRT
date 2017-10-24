# Create and Update a dat file
Convert SVR DB *.json file to *.dat file using **json2cbor** tool

## Build IoTivity on Linux Environment
```sh
scons -j8 RELEASE=False SECURED=1 RD_MODE=SERVER LOG_LEVEL=ERROR TARGET_TRANSPORT=IP
```

## Move to build result directory
```sh
cd iotivity/out/linux/x86_64/debug/resource/csdk/security/tool
```

## Run json2cbor tool
```sh
./json2cbor {project-repo}/json/default_svr_db.json {project-repo}/json/default_svr_db.dat
```

# Generate "ocf_mylight_reset_db.c" file
Convert *.dat file to C unsigned char array (**xxd** and **sed** tools are required)

```sh
cd {project-repo}/json
./generate-c.sh
```
