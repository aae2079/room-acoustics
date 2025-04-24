# PROJECT PATH
PROJECT_PATH = $(PROJECT_DIR)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -g -O0 -Wall -D_WAV_READER_

# Architecture
ARCH = $(shell arch)

# INSTALL PATH
INSTALL_PATH = $(PROJECT_PATH)/bin

# Source files
SRCS = $(PROJECT_PATH)/src/main.cpp

# Create Object Dir
$(shell mkdir -p $(PROJECT_PATH)/obj)

# Object Dir
ODIR = $(PROJECT_PATH)/obj

# Object files
OBJS = $(SRCS:$(PROJECT_PATH)/src/%.cpp=$(ODIR)/%.o)

# Executable name
EXEC = roomAcoustics_$(ARCH)

# Include paths
INC = -I$(PROJECT_PATH)/external/arma/include/armadillo_bits
ifeq ($(ARCH),arm64)
	INC += -I$(PROJECT_PATH)/include \
	  -I/opt/homebrew/include 
else
	INC += -I$(PROJECT_PATH)/include \
	  -I/usr/include 
endif

# Library path
LIB = -L$(PROJECT_PATH)/external/arma/
ifeq ($(ARCH),arm64)
	LIB += -L/opt/homebrew/lib \
		-L$(PROJECT_PATH)/lib \
		-Wl,-rpath,$(PROJECT_PATH)/external/arma/
else
	LIB += 	-L/usr/lib/x86_64-linux-gnu \
		-L/usr/lib64 \
		-L$(PROJECT_PATH)/lib
endif



# Build rule
all: $(INSTALL_PATH)/$(EXEC)

# Target rule to link the executable
$(INSTALL_PATH)/$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB) -lm -larmadillo -lreverbtime -lportaudio 

# Rule to compile source files
$(ODIR)/%.o: $(PROJECT_PATH)/src/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

# Clean rule
clean: $(INSTALL_PATH)/$(EXEC)
	rm -f $(INSTALL_PATH)/$(EXEC) $(OBJS)