# Output Target
TARGET = main

# Optimization
OPT = -O1

# Build Directory
BUILD_DIR = build

# C and C++ Sources
C_SOURCES = $(wildcard src/*.c)
CPP_SOURCES = $(wildcard src/*.cpp)

# C header directories
C_INCLUDES = \
-Iinc
# C++ header directories
CPP_INCLUDES = \
$(C_INCLUDES)

# compiler / linker
CC = gcc
CXX = g++

# C and C++ flags
CFLAGS = --std=c17 $(OPT) -Wall -Wextra $(C_INCLUDES) -g
CXXFLAGS = --std=c++17 $(OPT) -Wall -Wextra $(CPP_INCLUDES) -g

#######################################
# define linker
#######################################
# C linker
LINK = $(CC)
LINKFLAGS = $(CFLAGS)

# use C++ linker if given C++ sources
ifneq ($(CPP_SOURCES) , )
LINK = $(CXX)
LINKFLAGS = $(CXXFLAGS)
endif



# list of C objects
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of C++ objects
OBJS += $(addprefix $(BUILD_DIR)/, $(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

all : $(TARGET).exe

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@ 

$(BUILD_DIR)/%.o: %.cpp $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@ 

$(TARGET).exe : $(OBJS)
	$(LINK) $(LINKFLAGS) $(OBJS) -o $@

run : $(TARGET).exe
	./$(TARGET).exe

clean : 
	rm -fR $(BUILD_DIR) *.exe

