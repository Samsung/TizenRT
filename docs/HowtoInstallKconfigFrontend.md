# Kconfig-frontends Installation

## Prerequisites
The *bison* (or *byacc* if supported), *flex*, *gperf*, *libncurses5-dev*, *zlib1g-dev*, *gettext* and *g++* packages should be installed:
```bash
sudo apt-get install bison byacc flex gperf libncurses5-dev zlib1g-dev gettext g++
```

## Kconfig-frontend
1. Download and untar *kconfig-frontends* package.  
One of site is [Yann Morin's Project](http://ymorin.is-a-geek.org/projects/kconfig-frontends).
```bash
tar -xvf kconfig-frontends-x.xx.x.x.tar.bz2
```

2. Go to *kconfig-frontends* folder.
```bash
cd kconfig-frontends-x.xx.x.x
```

3. Configure and Build.
```bash
./configure --enable-mconf --disable-gconf --disable-qconf
make
sudo make install
```
## nuttx tool installation
1. Download nuttx tool from https://bitbucket.org/nuttx/tools/downloads/

2. Click on Download repository, and download nuttx-tools-891510d25342.zip, upzip after download

3. While in ~/nuttx-tools-98e04974b213/kconfig-frontends folder (cd kconfig-frontends-x.xx.x.x), configure and build
```
./configure --enable-mconf --disable-gconf --disable-qconf
make
sudo make install
```
```
./configure --disable-shared --enable-static --enable-mconf --disable-nconf --disable-gconf --disable-qconf --disable-nconf --disable-utils
make
sudo make install
```
```
./configure --prefix=/usr
make
sudo make install
```

### [Trouble Shooting](https://github.com/Samsung/TizenRT/blob/master/docs/TroubleShooting.md)
