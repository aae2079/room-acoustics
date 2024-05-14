#PROJECT PATH
PROJECT_PATH = $(PROJECT_DIR)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -g -O0 -Wall 

#Architecture
ARCH = $(shell arch)

#INSTALL PATH
INSTALL_PATH = $(PROJECT_PATH)/bin

# Source files
SRCS = main.cpp

#Create Object Dir
$(shell mkdir -p $(PROJECT_PATH)/obj)

#Object Dir
ODIR = $(PROJECT_PATH)/obj

# Object files
OBJS = $(addprefix $(ODIR)/, $(SRCS:.cpp=.o))

# Executable name
EXEC = roomAcoustics_$(ARCH)

# Include path

ifeq ($(ARCH),arm64)
INC = -I$(PROJECT_PATH)/include \
	  -I/opt/homebrew/Cellar/fftw/3.3.10_1/include \
	  -I/usr/include
else
INC = -I$(PROJECT_PATH)/include \
	  -I/opt/homebrew/Cellarsu/fftw/3.3.10_1/include \
	  -I/usr/include \
	  -I/usr/include/portaudio
endif

# Library path
LIB = -L/opt/homebrew/Cellar/fftw/3.3.10_1/lib -L/usr/lib/aarch64-linux-gnu -L/usr/lib/portaudio

# Build rule
all: $(INSTALL_PATH)/$(EXEC)

# Target rule to link the executable
$(INSTALL_PATH)/$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS)-o $@ $^ $(LIB) -lfftw3 -lm -larmadillo -lportaudio

# Rule to compile source files
$(ODIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<
	
# Clean rule
clean:
	rm -f $(INSTALL_PATH)/$(EXEC) $(OBJS)


