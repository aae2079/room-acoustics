# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

#Architecture
ARCH = $(shell uname -m)

#INSTALL PATH
INSTALL_PATH = ../bin

# Source files
SRCS = main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = roomAcoustics_$(ARCH)

# Build rule
all: $(INSTALL_PATH)/$(EXEC)

# Target rule to link the executable
$(INSTALL_PATH)/$(EXEC): $(OBJS)
	$(CXX) -o $@ $^

# Rule to compile source files
%.o: %.cpp
	$(CXX) -c -o $@ $<
	
# Clean rule
clean:
	rm -f $(OBJS) $(EXEC)
