# PROJECT PATH
PROJECT_PATH = $(PROJECT_DIR)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -g -O0 -Wall

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
INC = -I$(PROJECT_PATH)/include \
	  -I/opt/homebrew/Cellar/fftw/3.3.10_1/include \
	  -I/usr/include \
	  -I/usr/include/portaudio

# Library path
LIB = 	-L/usr/lib/aarch64-linux-gnu \
		-L/usr/lib64 \
		-L$(PROJECT_PATH)/lib

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