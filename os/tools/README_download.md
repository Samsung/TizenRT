# Zipdownload / Download

It is possible now to program binaries directly, without having to compile from source. 
This feature is targeted towards users who directly program the hardware board with TizenRT binaries.

This guide shows two parts, one is for who makes binary, another is for who programs binary.  

## Person of Binary Maker

Even though who makes binaries, downloader can not program only with binaries.  
The *zipdownload* script zips mandatory files including binaries, download scripts and tools.  
This zipped file supports to program TizenRT binaries without whole source codes.

You can run it as shown below:
```bash
os$ ./tools/zipdownload.sh
```

You can find the options of *zipdownload.sh* through *--help*.
```bash
os$ ./tools/zipdownload.sh --help
```

*tizenrt_<BOARD_NAME>_<COMMIT_HASH>.tar* is the name of zip file by default.

## Person of Binary Downloader

After extracting zipped file received from binary maker, downloader can program binaries as shown below:  
##### On Linux
```bash
$ ./os/tools/download.sh
```
##### On Windows
```
Double click of *download.bat* at *os/tools* folder
```
Download script and batch file are in *os/tools* folder.

Note: Each board requires USB driver installation on Windows.
For example, ARTIK requires [configuring USB drivers](https://github.com/SamsungARTIK/tizenrt#configure-usb-drivers).
