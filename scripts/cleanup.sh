#!/bin/bash
if [ -z "$PROJECT_DIR" ]; then
    echo "PROJECT_DIR is not set. Please source setEnv.sh first."
    exit 1
fi
rm -rf $PROJECT_DIR/bin
rm -rf $PROJECT_DIR/lib
rm -rf $PROJECT_DIR/external
rm -rf $PROJECT_DIR/obj
rm $PROJECT_DIR/src/libs/reverb-time/*.o
rm $PROJECT_DIR/src/libs/reverb-time/*.a
rm $PROJECT_DIR/src/libs/reverb-time/*.so

echo "Clean up done"
