#!/bin/bash
source ./scripts/setEnv.sh
mkdir $PROJECT_DIR/bin 
mkdir $PROJECT_DIR/lib

#Build armadillo/portaudio/etc.
mkdir $PROJECT_DIR/external

mkdir $PROJECT_DIR/external/arma
curl -L -0 https://sourceforge.net/projects/arma/files/latest/ > $PROJECT_DIR/external/arma/armadillo-latest.tar.xz
cd $PROJECT_DIR/external/arma/
tar -xvf armadillo-latest.tar.xz -C . --strip-components=1
cmake .
make install

# Build local dependencies
cd $PROJECT_DIR/src/libs/reverb-time
make shared
make install

#Build project
cd $PROJECT_DIR/src
make clean
make
