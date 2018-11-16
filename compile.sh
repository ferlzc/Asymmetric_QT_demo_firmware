#!/bin/bash

path=$(pwd)
build_path=$path/control-freertos/armgcc/
deploy_path=/srv/tftp

env() {
    export ARMGCC_DIR=$path/gcc-arm-none-eabi-4_9-2015q3/
    export FreeRTOSDir=$path/Colibri-iMX7-M4_FreeRTOS_1.2-20170407/
}

build() {
    echo "Build"
    echo $build_path
    cd $build_path
    $build_path/clean.sh
    $build_path/build_release.sh
}

deploy() {
    echo "Deploy"
    sudo cp  $build_path/release/Control.elf $deploy_path
}

msg() {
    echo "wrong usage!"
}

while getopts ":bd" flag; do
     case $flag in
         b)
            env
            build
             ;;
         d)
            deploy
            ;;
#        *)
#            msg
#            ;;
     esac
 done
