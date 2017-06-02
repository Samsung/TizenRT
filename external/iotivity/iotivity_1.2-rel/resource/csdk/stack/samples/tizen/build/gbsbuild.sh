#!/bin/sh

spec=`ls ./resource/csdk/stack/samples/tizen/build/packaging/*.spec`
version=`rpm --query --queryformat '%{version}\n' --specfile $spec`

name=`echo $name|cut -d" " -f 1`
version=`echo $version|cut -d" " -f 1`

name=oicri

echo $1
export TARGET_TRANSPORT=$1

echo $2
export SECURED=$2

echo $3
export BUILD_SAMPLE=$3

echo $4
export RELEASE=$4

echo $5
export LOGGING=$5

echo $6
export ROUTING=$6

echo $7
export WITH_TCP=$7

echo $8
export WITH_PROXY=$8

echo $9
export WITH_MQ=$9

echo $TARGET_TRANSPORT
echo $BUILD_SAMPLE
echo $WITH_MQ

rm -rf $name-$version

builddir=`pwd`
sourcedir=`pwd`

echo `pwd`

rm -rf ./tmp
mkdir ./tmp
mkdir ./tmp/extlibs/
mkdir ./tmp/packaging
cp -LR ./extlibs/tinycbor $sourcedir/tmp/extlibs
cp -Rf ./extlibs/mbedtls $sourcedir/tmp/extlibs
cp -R ./extlibs/cjson $sourcedir/tmp/extlibs
cp -R ./extlibs/timer $sourcedir/tmp/extlibs
cp -R ./extlibs/rapidxml $sourcedir/tmp/extlibs
cp -R ./extlibs/libcoap $sourcedir/tmp/extlibs
cp -LR ./extlibs/sqlite3 $sourcedir/tmp/extlibs
cp -R ./resource/csdk/stack/samples/tizen/build/packaging/*.spec $sourcedir/tmp/packaging
cp -R ./resource $sourcedir/tmp/
cp -R ./build_common/external_libs.scons $sourcedir/tmp/
mkdir -p $sourcedir/tmp/build_common/
cp -a ./build_common/*.scons $sourcedir/tmp/build_common/

# copy dependency RPMs and conf files for tizen build
cp ./tools/tizen/*.rpm $sourcedir/tmp
cp ./tools/tizen/*.rpm $sourcedir/tmp/resource/csdk/stack/samples/tizen/SimpleClientServer
cp ./tools/tizen/.gbs.conf ./tmp
cp ./tools/tizen/.gbs.conf ./tmp/resource/csdk/stack/samples/tizen/SimpleClientServer

cd $sourcedir
cd ./resource/csdk/stack/samples/tizen/build/

cp -R ./* $sourcedir/tmp/
rm -f $sourcedir/tmp/SConscript
cp SConstruct $sourcedir/tmp/
cp scons/SConscript $sourcedir/tmp/scons/

mkdir -p $sourcedir/tmp/iotivityconfig
cd $sourcedir/build_common/
cp -R ./iotivityconfig/* $sourcedir/tmp/iotivityconfig/
cp -R ./SConscript $sourcedir/tmp/

cp -R $sourcedir/iotivity.pc.in $sourcedir/tmp/

cd $sourcedir/tmp

echo `pwd`
# Prepare mbedTLS dependency
$SHELL ./extlibs/mbedtls/prep.sh

# Prepare TinyCBOR dependency
$SHELL ./extlibs/tinycbor/prep.sh

whoami
# Initialize Git repository
if [ ! -d .git ]; then
   git init ./
   git config user.email "you@example.com"
   git config user.name "Your Name"
   git add ./
   git commit -m "Initial commit"
fi

echo "Calling core gbs build command"
gbscommand="gbs build -A armv7l -B ~/GBS-ROOT-RI-OIC --include-all --repository ./ --define 'TARGET_TRANSPORT $1' --define 'SECURED $2' --define 'RELEASE $4' --define 'LOGGING $5' --define 'ROUTING $6' --define 'WITH_TCP $7' --define 'WITH_PROXY $8' --define 'WITH_MQ $9'"
echo $gbscommand
if eval $gbscommand; then
   echo "Core build is successful"
else
   echo "Core build failed. Try 'find . -type f -exec dos2unix {} \;' in the 'connectivity/' folder"
   cd $sourcedir
   rm -rf $sourcedir/tmp
   exit 1
fi

if echo $BUILD_SAMPLE|grep -qi '^ON$'; then
   cd resource/csdk/stack/samples/tizen/SimpleClientServer
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
   gbscommand="gbs build -A armv7l -B ~/GBS-ROOT-RI-OIC --include-all --repository ./ --define 'TARGET_TRANSPORT $1' --define 'SECURED $2' --define 'RELEASE $4' --define 'LOGGING $5' --define 'ROUTING $6' --define 'WITH_TCP $7' --define 'WITH_PROXY $8' --define 'WITH_MQ $9'"
   echo $gbscommand
   if eval $gbscommand; then
      echo "Sample build is successful"
   else
      echo "Sample build is failed. Try 'find . -type f -exec dos2unix {} \;' in the 'connectivity/' folder"
      exit 1
   fi
else
	echo "Sample build is not enabled"
fi

cd $sourcedir
rm -rf $sourcedir/tmp

exit 0
