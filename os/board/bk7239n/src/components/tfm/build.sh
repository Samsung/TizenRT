#! /bin/bash

export PATH=/opt/gcc-arm-none-eabi-9-2020-q2-update/bin:$PATH
NINJA_BUILD=1
BUILD_DIR=cmake_build

cd tfm
current_dir=$(pwd)
armino_path=${current_dir}/../../../

if [ "$NINJA_BUILD" == "1" ]; then
    cmake -G Ninja -S . -B ../$BUILD_DIR -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTFM_PROFILE=profile_medium -DCMAKE_BUILD_TYPE=Debug -DTEST_S=ON -DTEST_NS=ON -DUSER=$USER -DBL2=OFF -DOTP_NV_COUNTERS_RAM_EMULATION=ON -DPLATFORM_DEFAULT_OTP_WRITEABLE=ON  -DARMINO_SDK_PATH=${armino_path} -DARM_CE_DUBHE=1 -DDUBHE_SECURE=1 -DARM_CE_DUBHE_SCA=1 -DARM_CE_DUBHE_TRNG=1 -DARM_CE_DUBHE_OTP=1  -DARM_CE_DUBHE_HASH=1  -DARM_CE_DUBHE_ACA=1 -DCONFIG_TODO=1
    cd ../$BUILD_DIR
    ninja -- install
else
    cmake -S . -B ../$BUILD_DIR -DTFM_PLATFORM=beken/bk7236 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake  -DTFM_PROFILE=profile_medium -DCMAKE_BUILD_TYPE=Debug -DTEST_S=ON -DTEST_NS=ON -DBL2_HEADER_SIZE=0 -DUSER=$USER  -DBL2=OFF -DOTP_NV_COUNTERS_RAM_EMULATION=ON -DPLATFORM_DEFAULT_OTP_WRITEABLE=ON -DARMINO_SDK_PATH=${armino_path}  -DARM_CE_DUBHE=1 -DDUBHE_SECURE=1 -DARM_CE_DUBHE_SCA=1 -DARM_CE_DUBHE_TRNG=1 -DARM_CE_DUBHE_OTP=1  -DARM_CE_DUBHE_HASH=1  -DARM_CE_DUBHE_ACA=1 -DCONFIG_TODO=1
    cmake --build ../$BUILD_DIR -- install -j8
fi

cd ..
if [ ! -d build ]; then
	git clone "ssh://ming.liu@192.168.0.46:29418/wifi/bk7236/build"
	cd build
	git clone "ssh://ming.liu@192.168.0.46:29418/iot/wcn/tools/imgtools"
fi

${current_dir}/build/fpga_build.sh $current_dir $1 $2


