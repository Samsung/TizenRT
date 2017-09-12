#!/bin/sh

cur_dir="./resource/csdk/connectivity/"

spec=`ls ./resource/csdk/connectivity/build/tizen/packaging/*.spec`
version=`rpm --query --queryformat '%{version}\n' --specfile $spec`

name=`echo $name|cut -d" " -f 1`
version=`echo $version|cut -d" " -f 1`

name=oicca

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
export WITH_TCP=$6

echo $TARGET_TRANSPORT
echo $BUILD_SAMPLE
echo $WITH_TCP

rm -rf $name-$version

builddir=`pwd`
sourcedir=`pwd`

echo `pwd`

rm -rf ./tmp
mkdir ./tmp
mkdir ./tmp/con/
mkdir -p $sourcedir/tmp/con/extlibs/

cp -R $cur_dir/* $sourcedir/tmp/con
cp -R $cur_dir/SConscript $sourcedir/tmp/con
cp -R $cur_dir/src/ip_adapter/SConscript $sourcedir/tmp/con/src/ip_adapter/
cp -R $cur_dir/src/bt_le_adapter/SConscript $sourcedir/tmp/con/src/bt_le_adapter/
cp -R $cur_dir/src/bt_edr_adapter/SConscript $sourcedir/tmp/con/src/bt_edr_adapter/
cp -R $cur_dir/common/SConscript $sourcedir/tmp/con/common/
cp -R $cur_dir/samples/tizen/ $sourcedir/tmp/con/sample/
mkdir -p $sourcedir/tmp/con/sample/external/inc
cp -R $cur_dir/external/inc/* $sourcedir/tmp/con/sample/external/inc/

cp -R ./extlibs/mbedtls/ $sourcedir/tmp/con/mbedtls/
cp -R ./extlibs/libcoap/ $sourcedir/tmp/con/extlibs/
mkdir -p $sourcedir/tmp/con/c_common
cp -R ./resource/c_common/* $sourcedir/tmp/con/c_common/
cp -R ./resource/csdk/logger/include/* $sourcedir/tmp/con/common/inc/
mkdir ./tmp/con/logger/
cp -R ./resource/csdk/logger/* $sourcedir/tmp/con/logger

# copy dependency RPMs and conf files for tizen build
cp ./tools/tizen/*.rpm $sourcedir/tmp
cp ./tools/tizen/*.rpm $sourcedir/tmp/con/sample
cp ./tools/tizen/.gbs.conf ./tmp
cp ./tools/tizen/.gbs.conf ./tmp/con/sample

cd $sourcedir
cd $cur_dir/build/tizen
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
gbscommand="gbs build -A armv7l -B ~/GBS-ROOT-OIC --include-all  --repository ./ --define 'TARGET_TRANSPORT $1' --define 'SECURED $2' --define 'RELEASE $4' --define 'LOGGING $5' --define 'WITH_TCP $6'"
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
   cd con/sample
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
   gbscommand="gbs build -A armv7l -B ~/GBS-ROOT-OIC --include-all --define 'TARGET_TRANSPORT $1' --define 'SECURED $2' --define 'RELEASE $4' --define 'LOGGING $5' --repository ./"
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
