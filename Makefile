# Source and build directories
SRC_DIR = src
BUILD_DIR = build/debug
OBJ_NAME = play.exe  # Explicitly include the .exe extension for Windows

# Compiler and flags
CC = g++
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g
INCLUDE_PATHS = -Iinclude -IC:/msys64/ucrt64/include
LIBRARY_PATHS = -Llib -LC:/msys64/ucrt64/lib
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Source files and corresponding object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Dependency files for automatic tracking
DEP_FILES = $(OBJ_FILES:.o=.d)

# Target executable
all: $(BUILD_DIR)/$(OBJ_NAME)
	@echo "Build complete: $(BUILD_DIR)/$(OBJ_NAME)"

# Rule to link the final executable
$(BUILD_DIR)/$(OBJ_NAME): $(OBJ_FILES)
	@echo "Linking: $(BUILD_DIR)/$(OBJ_NAME)"
	$(CC) $(COMPILER_FLAGS) $(LIBRARY_PATHS) $(OBJ_FILES) $(LINKER_FLAGS) -o $@

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $< -> $@"
	mkdir -p $(BUILD_DIR)
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Include dependency files if they exist
-include $(DEP_FILES)

# Rule to clean build artifacts
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)
