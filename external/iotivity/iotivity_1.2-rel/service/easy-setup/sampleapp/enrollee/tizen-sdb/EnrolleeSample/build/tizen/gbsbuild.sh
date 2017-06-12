#!/bin/sh

cur_dir="./service/easy-setup"
spec=`ls ./service/easy-setup/sampleapp/enrollee/tizen-sdb/EnrolleeSample/packaging/*.spec`
version=`rpm --query --queryformat '%{version}\n' --specfile $spec`

name=`echo $name|cut -d" " -f 1`
version=`echo $version|cut -d" " -f 1`

name=iotivity

rm -rf $name-$version

builddir=`pwd`
sourcedir=`pwd`

echo `pwd`

# Clean tmp directory.
rm -rf ./tmp

# Create directory structure for GBS Build
mkdir ./tmp
mkdir ./tmp/con/
mkdir ./tmp/extlibs/
mkdir ./tmp/packaging
mkdir ./tmp/logger
cp -R ./build_common $sourcedir/tmp
cp -R ./examples $sourcedir/tmp

# tinycbor is available as soft-link, so copying with 'dereference' option.
cp -LR ./extlibs/tinycbor $sourcedir/tmp/extlibs

cp -R ./extlibs/cjson $sourcedir/tmp/extlibs
cp -R ./extlibs/mbedtls $sourcedir/tmp/extlibs
cp -R ./extlibs/gtest $sourcedir/tmp/extlibs
cp -LR ./extlibs/sqlite3 $sourcedir/tmp/extlibs
cp -R ./extlibs/timer $sourcedir/tmp/extlibs
cp -R ./extlibs/rapidxml $sourcedir/tmp/extlibs
cp -R ./extlibs/libcoap $sourcedir/tmp/extlibs
cp -R ./resource $sourcedir/tmp
cp -R ./service $sourcedir/tmp
cp ./extra_options.scons $sourcedir/tmp
cp ./tools/tizen/*.spec ./tmp/packaging
cp ./tools/tizen/*.manifest ./tmp/packaging
cp ./SConstruct ./tmp
cp ./LICENSE.md ./tmp

# copy dependency RPMs and conf files for tizen build
cp ./tools/tizen/*.rpm ./tmp
cp ./tools/tizen/.gbs.conf ./tmp
cp ./tools/tizen/*.rpm $sourcedir/tmp/service/easy-setup/sampleapp/enrollee/tizen-sdb/EnrolleeSample
cp ./tools/tizen/.gbs.conf ./tmp/service/easy-setup/sampleapp/enrollee/tizen-sdb/EnrolleeSample

cp -R $sourcedir/iotivity.pc.in $sourcedir/tmp

cd $sourcedir/tmp

echo `pwd`
# Prepare mbedTLS dependency
$SHELL ./extlibs/mbedtls/prep.sh

# Prepare TinyCBOR dependency
$SHELL ./extlibs/tinycbor/prep.sh

# Build IoTivity
# Initialize Git repository
if [ ! -d .git ]; then
   git init ./
   git config user.email "you@example.com"
   git config user.name "Your Name"
   git add ./
   git commit -m "Initial commit"
fi

buildoption="--define 'TARGET_TRANSPORT '$1 --define 'SECURED '$2 --define 'ROUTING '$3 --define 'RELEASE '$4 --define 'LOGGING '$5 --define 'ES_TARGET_ENROLLEE '$6 --define 'WITH_TCP '$7 --define 'WITH_CLOUD '$8"
echo "Calling core gbs build command"
gbscommand="gbs build -A armv7l -B ~/GBS-ROOT-OIC $buildoption --include-all --repository ./"
echo $gbscommand
if eval $gbscommand; then
   echo "Core build is successful"
else
   echo "Core build failed."
   cd $sourcedir
   rm -rf $sourcedir/tmp
   exit 1
fi

# Build Enrollee Sample App
cd service/easy-setup/sampleapp/enrollee/tizen-sdb/EnrolleeSample
echo `pwd`
# Initialize Git repository
if [ ! -d .git ]; then
  git init ./
  git config user.email "you@example.com"
  git config user.name "Your Name"
  git add ./
  git commit -m "Initial commit"
fi
echo "Calling sample gbs build command"
gbscommand="gbs build -A armv7l -B ~/GBS-ROOT-OIC $buildoption --include-all --repository ./"
echo $gbscommand
if eval $gbscommand; then
  echo "Sample build is successful"
else
  echo "Sample build is failed."
  exit 1
fi
rm -rf tmp
cd $sourcedir
rm -rf $sourcedir/tmp

exit 0
