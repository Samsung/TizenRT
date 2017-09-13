#!/bin/sh

RELEASE_MODE=False
BASE_PATH=${PWD}
GBS_TOOL_PATH=${BASE_PATH}/../doc/Tizen\ Build/2_3
GBS_ROOT=${BASE_PATH}/../../../GBS_ROOT_OCF
#mkdir -p ${GBS_ROOT}

if [ -n "$2" ] ; then
    GBS_TOOL_PATH=$2
fi

if [ "$1" = "1" ] ; then
    RELEASE_MODE=True
fi

if [ -n "$3" ] ; then
    GBS_ROOT=$3
fi

cur_dir="${PWD}"
echo "cur_dir = ${cur_dir}"

spec=`ls ./build/tizen/packaging/*.spec`
version=`rpm --query --queryformat '%{version}\n' --specfile $spec`

name=`echo $name|cut -d" " -f 1`
version=`echo $version|cut -d" " -f 1`

name=oicda

#if tizen 2.2 - 0
#if tizen 2.3 - 1
#if tizen 2.4 - 2

echo $10
export TARGET_VERSION=$10

if [ "$9" ] ; then
    BUILD_SAMPLE=$9
fi
echo $BUILD_SAMPLE

rm -rf $name-$version

sourcedir=`pwd`
echo "sourcedir = ${sourcedir}"

cd $sourcedir

#unrpm build/tizen/iotivity-1.1.0-0.armv7l.rpm
#cp ./usr/lib/*so ./lib/tizen/armgnueabi
#rm -rf ./usr/

rm -rf $sourcedir/tmp
#rm -rf $cur_dir/inc/iotivity

mkdir -p ./tmp
mkdir -p ./tmp/con/
mkdir -p $cur_dir/inc/iotivity

#copying iotivity header files locally from dev/iotivity folder

if echo $TARGET_VERSION|grep -qi '^0$'; then

echo "This is tizen 2.2"

cp ${cur_dir}/../iotivity_tizen2_2/resource/oc_logger/include/targets/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/oc_logger/include/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/include/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/csdk/logger/include/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/csdk/stack/include/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/c_common/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/c_common/oic_malloc/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/c_common/oic_string/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/csdk/security/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/csdk/security/provisioning/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/csdk/security/provisioning/include/internal/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/csdk/security/provisioning/include/oxm/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity_tizen2_2/resource/csdk/connectivity/api/*.h $cur_dir/inc/iotivity
cp -R ${cur_dir}/../iotivity_tizen2_2/resource/csdk/connectivity/lib/libcoap-4.1.1/include $cur_dir/inc/iotivity
cp -R ${cur_dir}/../iotivity_tizen2_2/extlibs/tinydtls  $cur_dir/inc/iotivity
cp -R ${cur_dir}/../iotivity_tizen2_2/extlibs/tinycbor  $cur_dir/inc/iotivity
cp -R ${cur_dir}/../iotivity_tizen2_2/extlibs/cjson  $cur_dir/inc/iotivity

else

echo "This is tizen 2.3 and above"

cp ${cur_dir}/../iotivity/resource/oc_logger/include/targets/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/oc_logger/include/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/include/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/logger/include/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/stack/include/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/c_common/*.h  $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/c_common/oic_malloc/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/c_common/oic_string/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/c_common/oic_time/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/c_common/ocrandom/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/security/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/security/include/internal/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/security/provisioning/include/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/security/provisioning/include/internal/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/security/provisioning/include/oxm/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/connectivity/api/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/connectivity/inc/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/connectivity/inc/pkix/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/connectivity/common/inc/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/connectivity/util/inc/*.h $cur_dir/inc/iotivity
cp ${cur_dir}/../iotivity/resource/csdk/resource-directory/include/*.h $cur_dir/inc/iotivity
cp -R ${cur_dir}/../iotivity/resource/csdk/connectivity/lib/libcoap-4.1.1/include $cur_dir/inc/iotivity
cp -R ${cur_dir}/../iotivity/extlibs/mbedtls/mbedtls/include/mbedtls $cur_dir/inc/iotivity
cp -R ${cur_dir}/../iotivity/extlibs/tinydtls  $cur_dir/inc/iotivity
cp -R ${cur_dir}/../iotivity/extlibs/tinycbor/tinycbor/src/*  $cur_dir/inc/iotivity/
cp -R ${cur_dir}/../iotivity/extlibs/cjson  $cur_dir/inc/iotivity

fi


cp -R $cur_dir/build $sourcedir/tmp/
cp -R $cur_dir/inc $sourcedir/tmp/
cp -R $cur_dir/samples $sourcedir/tmp/
cp -R $cur_dir/src $sourcedir/tmp/
cp -R SConstruct $sourcedir/tmp

mkdir -p ./tmp/samples/tizen/lite/src
mkdir -p ./tmp/samples/tizen/lite/inc
mkdir -p ./tmp/samples/tizen/lite/lib
cp -R $cur_dir/src/* $sourcedir/tmp/samples/tizen/lite/src/
cp -R $cur_dir/inc/* $sourcedir/tmp/samples/tizen/lite/inc/
cp -R $cur_dir/lib/* $sourcedir/tmp/samples/tizen/lite/lib/

mkdir -p ./tmp/src
mkdir -p ./tmp/inc
mkdir -p ./tmp/lib/tizen/armeabi-v7a
cp -R $cur_dir/src/* $sourcedir/tmp/src/
cp -R $cur_dir/inc/* $sourcedir/tmp/inc/
cp -R $cur_dir/lib/linux/armgnueabi/* $sourcedir/tmp/lib/tizen/armeabi-v7a/



cd $sourcedir
cd $cur_dir/build/tizen

cp -R ./* $sourcedir/tmp/



cd $sourcedir/tmp

if echo $BUILD_SAMPLE|grep -qi '^OFF$'; then
    echo `pwd`
    echo $HOME

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
    gbscommand="gbs -c \"${GBS_TOOL_PATH}/.gbs.conf\" build -A armv7l -B \"${GBS_ROOT}\" -D \"${GBS_TOOL_PATH}/build.conf\" --include-all --define 'Transporttype $4' --define 'Securetype $5' --define 'Buildtype ${RELEASE_MODE}' --define 'Logtype $6' --define 'TargetDevice $7' --define 'Withtcptype $8' --repository ./"
    echo $gbscommand
    if eval $gbscommand; then
       echo "Core build is successful"
    else
       echo "Core build failed. Try 'sudo find . -type f -exec dos2unix {} \;' in the 'connectivity/' folder"
       cd $sourcedir
       rm -rf $sourcedir/tmp
       exit 1
    fi
fi

if echo $BUILD_SAMPLE|grep -qi '^ON$'; then
   cd samples/tizen/lite
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
   gbscommand="gbs -c \"${GBS_TOOL_PATH}/.gbs.conf\" build -A armv7l -B \"${GBS_ROOT}\" -D \"${GBS_TOOL_PATH}/build.conf\" --include-all --define 'Transporttype $4' --define 'Securetype $5' --define 'Buildtype ${RELEASE_MODE}' --define 'Logtype $6' --define 'TargetDevice $7' --define 'Withtcptype $8' --repository ./"
   echo $gbscommand
   if eval $gbscommand; then
      echo "Sample build is successful"
   else
      echo "Sample build is failed. Try 'sudo find . -type f -exec dos2unix {} \;' in the 'connectivity/' folder"
      cd $sourcedir
      rm -rf $sourcedir/tmp
      rm -rf $sourcedir/inc/iotivity
      exit 1
   fi
else
   echo "Sample build is not enabled"
fi


cd $sourcedir
rm -rf $sourcedir/tmp
rm -rf $sourcedir/inc/iotivity

exit 0
