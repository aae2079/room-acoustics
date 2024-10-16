#PROJECT PATH
PROJECT_PATH = $(PROJECT_DIR)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -O2 -fPIC


#Library name and srcs
LIB_NAME = libreverbtime.so 
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
	sudo rm -f $(LIB_OBJ) $(LIB_NAME)

#install
install:
	sudo mkdir -p $(LIB_INSTALL_PATH)
	sudo cp $(LIB_NAME) $(LIB_INSTALL_PATH)
	sudo cp $(HEADERS) $(LIB_INCLUDE_PATH)

#uninstall
uninstall:
	sudo rm -f $(LIB_INSTALL_PATH)/$(LIB_NAME)
	sudo rm -f $(LIB_INCLUDE_PATH)/$(HEADERS)
