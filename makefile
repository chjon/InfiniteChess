# Names of tools to use when building (make sure /c/MinGW/bin/ is in the PATH)
CXX = mingw32-g++
LD  = mingw32-g++
RM  = rm -rf

# Directories containing C headers.
INCLUDE_DIRS = -I"inc/freeglut/include" -I"inc/glew/include" -I"inc/glm/include"

# Directories containing import libraries.
LIB_DIRS = -L"lib/freeglut/lib" -L"lib/glew/lib"

# Warnings to be raised by the CPP compiler
WARNS = -Wall

# Compiler and linker flags
CPPFLAGS = -O3 -std=c++14 -D _WIN32_IE=0x0500 -D WINVER=0x500 $(WARNS) $(INCLUDE_DIRS)
LDFLAGS = -s -lfreeglut -lopengl32 -lglew32 -Wl,--subsystem,windows $(LIB_DIRS)

# Source files
SRC_DIR = ./src
SRCS = \
	$(wildcard $(SRC_DIR)/*.cpp) \
	$(wildcard $(SRC_DIR)/graphics/*.cpp) \
	$(wildcard $(SRC_DIR)/keyboard/*.cpp) \
	$(wildcard $(SRC_DIR)/math/*.cpp) \
	$(wildcard $(SRC_DIR)/util/*.cpp)

# Output directory
OUT_DIR = ./bin
OBJ_DIR = ./obj
OBJS = $(addprefix $(OBJ_DIR)/, $(foreach SRC, $(subst .cpp,.o,$(SRCS)), $(SRC:$(SRC_DIR)/%=%)))

TARGET = main.exe
TARGET := $(OUT_DIR)/$(TARGET)

.PHONY: all clean

all: $(TARGET)

run: $(TARGET)
	$(TARGET)

$(TARGET): $(OBJS) | $(OUT_DIR)
	mkdir -p $(dir $@)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJ_DIR)/*
