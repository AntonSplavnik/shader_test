NAME = raytracer
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
SRC_DIR = .
OBJ_DIR = obj
MLX42_PATH = ./MLX42
MLX42_LIB = $(MLX42_PATH)/build/libmlx42.a
MLX42_INCLUDE = -I$(MLX42_PATH)/include

# Source files
SRCS = main.c
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Check for macOS or Linux
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# macOS
	PLATFORM_LIBS = -framework Cocoa -framework OpenGL -framework IOKit
	# Add GLFW library - try different paths for Homebrew and MacPorts
	GLFW_PATH := $(shell brew --prefix glfw 2>/dev/null || echo "/opt/local")
	PLATFORM_LIBS += -L$(GLFW_PATH)/lib -lglfw
	CFLAGS += -I$(GLFW_PATH)/include
else
	# Linux and others
	PLATFORM_LIBS = -ldl -pthread -lm
	PLATFORM_LIBS += -lglfw
endif

# Default target
all: $(OBJ_DIR) $(NAME)

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Rule to build the executable
$(NAME): $(MLX42_LIB) $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLX42_LIB) $(PLATFORM_LIBS)
	@echo "Build complete: $(NAME)"

# Rule to compile .c files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(MLX42_INCLUDE) -c $< -o $@

# Rule to build the MLX42 library
$(MLX42_LIB):
	@echo "Building MLX42 library..."
	@cd $(MLX42_PATH) && cmake -B build && cmake --build build -j4
	@echo "MLX42 library built successfully"

# Clean object files
clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)

# Clean everything
fclean: clean
	@echo "Cleaning executable and libraries..."
	@rm -f $(NAME)
	@cd $(MLX42_PATH) && rm -rf build
	@echo "Clean complete"

# Rebuild everything
re: fclean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all     : Build the project (default)"
	@echo "  clean   : Remove object files"
	@echo "  fclean  : Remove object files, executable, and libraries"
	@echo "  re      : Rebuild everything"
	@echo "  help    : Show this help message"

.PHONY: all clean fclean re help 