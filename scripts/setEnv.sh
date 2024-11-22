#!/bin/bash
# FILEPATH: /Users/aaron_escbr/gitRepos/room-acoustics/setEnv.sh

#export WORKING_DIR=$(pwd)
#export PROJECT_DIR=$(dirname "$(realpath "$BASH_SOURCE")")
export SCRIPT_DIR=$(dirname "$(realpath "$BASH_SOURCE")")
export PROJECT_DIR=$SCRIPT_DIR/..

export LD_LIBRARY_PATH=$PROJECT_DIR/lib:$LD_LIBRARY_PATH

echo "PROJECT_DIR set to $PROJECT_DIR"



