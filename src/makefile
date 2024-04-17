#PROJECT PATH
PROJECT_PATH = $(WORKING_DIR)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

#Architecture
ARCH = $(shell uname -m)

#INSTALL PATH
INSTALL_PATH = $(PROJECT_PATH)/bin

# Source files
SRCS = main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = roomAcoustics_$(ARCH)

# Include path
INC = -I$(PROJECT_PATH)/include -I/opt/homebrew/Cellar/fftw/3.3.10_1/include

# Library path
LIB = -L/opt/homebrew/Cellar/fftw/3.3.10_1/lib

# Build rule
all: $(INSTALL_PATH)/$(EXEC)

# Target rule to link the executable
$(INSTALL_PATH)/$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(LIB) -lfftw3 -lm

# Rule to compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<
	
# Clean rule
clean:
	rm -f $(INSTALL_PATH)/$(EXEC) $(OBJS)
