#!/bin/bash
WORKING_DIR=`pwd`
FDK_AAC_REPO="https://github.com/akanchi/fdk-aac.git"
FDK_AAC_DIR="${WORKING_DIR}/objs"
FDK_AAC_SRC_DIR="${WORKING_DIR}/thirdparty/fdk-aac"
EXAMPLE_DIR="${WORKING_DIR}/trunk"
FDK_AAC_LIB_INSTALL_DIR="${EXAMPLE_DIR}"

if [ ! -f "${FDK_AAC_LIB_INSTALL_DIR}/lib/libfdk-aac.a" ]; then
    echo 'libfdk-aac.a not found'
    echo 'start to clone fdk-aac.git'
    rm -rf ${FDK_AAC_SRC_DIR}
    git clone ${FDK_AAC_REPO} ${FDK_AAC_SRC_DIR}
    cd ${FDK_AAC_SRC_DIR}
    ./autogen.sh
    ./configure --prefix=${FDK_AAC_LIB_INSTALL_DIR} 
    make
    make install
fi

cd $EXAMPLE_DIR
make
