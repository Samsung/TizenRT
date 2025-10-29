#!/bin/bash

curr_path=$(dirname `realpath $0`)
tfm_build_dir=$(realpath $curr_path/../../output/bin/tfm_build)
USER_UID=$1
if [ -d $tfm_build_dir ];then
    chown -R $USER_UID $tfm_build_dir
    find $curr_path -type d -name '__pycache__' -exec chown -R $USER_UID {} +
    components_dir=$(realpath $curr_path/../../../os/board/bk7239n/src/components)
    chown -R $USER_UID $components_dir/tfm/cmake-3.15.7
    find $components_dir/tfm -type d -name '__pycache__' -exec chown -R $USER_UID {} +
    find $components_dir/tfm_mbedtls -type d -name '__pycache__' -exec chown -R $USER_UID {} +
fi

install_for_deployment_dir=$(realpath "$curr_path/../../output/bin/install_for_deployment")
if [ -d $install_for_deployment_dir ];then
    chown -R $USER_UID $install_for_deployment_dir
fi

if [ -d $curr_path/bk_py_libs ];then
    chown -R $USER_UID $curr_path/bk_py_libs
fi

if [ -d $curr_path/beken_utils/input_dir ];then
    chown -R $USER_UID $curr_path/beken_utils/input_dir
fi

compression_dir=$(realpath $curr_path/../../../os/compression)
if [ -d $compression_dir/miniz ];then
    chown -R $USER_UID $compression_dir/miniz
    tools_compression_dir=$(realpath $curr_path/../../../os/tools/compression)
    chown -R $USER_UID $tools_compression_dir/obj
    chown -R $USER_UID $tools_compression_dir/dep
    build_user_dir=$(realpath $curr_path/../../output/bin/user)
    chown -R $USER_UID $build_user_dir
fi
