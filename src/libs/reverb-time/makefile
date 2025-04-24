#PROJECT PATH
PROJECT_PATH = $(PROJECT_DIR)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -g -O0 -Wall -fPIC


#Library name and srcs
# Architecture
ARCH = $(shell arch)
ifeq ($(ARCH),arm64)
	LIB_NAME = libreverbtime.dylib
else
	LIB_NAME = libreverbtime.so
endif
LIB_SRCS = rtProc.cc
LIB_OBJ = rtProc.o
HEADERS = rtDefs.hh

#PATH TO LIBRARY
LIB_INSTALL_PATH = $(PROJECT_PATH)/lib
LIB_INCLUDE_PATH = $(PROJECT_PATH)/include

# Build shared library
shared: $(LIB_NAME)

$(LIB_NAME): $(LIB_OBJ)
	$(CXX) -shared -o $@ $^

# Compile object files
$(LIB_OBJ): $(LIB_SRCS) $(HEADER)
	$(CXX) $(CXXFLAGS) -c $(LIB_SRCS)

#clean
clean:
	rm -f $(LIB_OBJ) $(LIB_NAME)

#install
install:
	cp $(LIB_NAME) $(LIB_INSTALL_PATH)
	cp $(HEADERS) $(LIB_INCLUDE_PATH)

#uninstall
uninstall:
	rm -f $(LIB_INSTALL_PATH)/$(LIB_NAME)
	rm -f $(LIB_INCLUDE_PATH)/$(HEADERS)
