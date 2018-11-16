#!/bin/bash

#. "./env.conf"

cmake -DCMAKE_TOOLCHAIN_FILE=${FreeRTOSDir}"/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release  .
make -j4
