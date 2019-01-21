# Introduction

Amazon avs-device-sdk has been integrated on TizenRT as a submodule. Therefore, it's necessary to update submodule and apply patches manually, before starting to build.

## Update submodule

```shell
cd TizenRT
git submodule update --init --recursive
```

## Apply patches

```shell
cd TizenRT/external/avs/avs-device-sdk/
git apply ../patches/*.patch
```
