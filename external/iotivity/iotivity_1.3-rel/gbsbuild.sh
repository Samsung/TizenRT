#!/bin/bash -e

spec=`ls tools/tizen/*.spec`
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
mkdir ./tmp/extlibs/
mkdir ./tmp/packaging
cp -R ./build_common $sourcedir/tmp
cp -R ./examples $sourcedir/tmp

# tinycbor is available as soft-link, so copying with 'dereference' option.
cp -LR ./extlibs/tinycbor $sourcedir/tmp/extlibs

cp -R ./extlibs/cjson $sourcedir/tmp/extlibs
cp -R ./extlibs/mbedtls $sourcedir/tmp/extlibs
cp -R ./extlibs/gtest $sourcedir/tmp/extlibs
cp -LR ./extlibs/sqlite3 $sourcedir/tmp/extlibs
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

secured=1
gbsarch=${gbsarch:=armv7l}
gbsprofile=${gbsprofile:=profile.tizen}
gbscommand="gbs build -A ${gbsarch} -P ${gbsprofile}"
nproc=$(expr 1 + $(expr `nproc --ignore=1` / 2 ) )
gbscommand=$gbscommand" --define '_smp_mflags -j$nproc'"

for ARGUMENT_VALUE in $*
do
   echo $ARGUMENT_VALUE
   if [[ "$ARGUMENT_VALUE" = *"="* ]]; then
      optionname=$(echo $ARGUMENT_VALUE | cut -f1 -d=)
      optionvalue=$(echo $ARGUMENT_VALUE | cut -f2 -d=)
      echo -- "# $optionname $optionvalue"
      gbscommand=${gbscommand}" --define '$optionname $optionvalue'"

      if [ "SECURED" = "$optionname" ]; then
         secured=$optionvalue
      fi
   else
      echo "'$ARGUMENT_VALUE' does not contain '='";
   fi
done

gbscommand="${gbscommand} -B ~/GBS-ROOT-OIC --include-all --repository ./"
pwd

# Prepare mbedTLS dependency (also for unsecured as used by connectivity)
$SHELL ./extlibs/mbedtls/prep.sh

# Prepare TinyCBOR dependency
$SHELL ./extlibs/tinycbor/prep.sh

# Remove modules history for gbs export (Applies to tinycbor, mbedtls...)
rm -rf ./extlibs/*/*/.git

# Initialize Git repositoryㅣ
if [ ! -d .git ]; then
   git init ./
   git config user.email "you@example.com"
   git config user.name "Your Name"
   git add ./
   git commit -m "Initial commit"
fi

echo "Calling core gbs build command"
echo $gbscommand
if eval $gbscommand; then
    echo "Build is successful"
else
    echo "Build failed!"
    exit 1
fi


rm -rf tmp
cd $sourcedir
rm -rf $sourcedir/tmp

exit 0
