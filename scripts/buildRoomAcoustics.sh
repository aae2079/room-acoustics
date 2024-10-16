source ./setEnv.sh
mkdir $PROJECT_DIR/bin 
mkdir $PROJECT_DIR/lib

# Build dependencies
cd $PROJECT_DIR/src/libs/reverb-time
make shared
make install

#Build project
cd $PROJECT_DIR/src
make clean
