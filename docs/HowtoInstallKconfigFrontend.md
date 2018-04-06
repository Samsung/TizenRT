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

### [Trouble Shooting](https://github.com/Samsung/TizenRT/blob/master/docs/TroubleShooting.md)
