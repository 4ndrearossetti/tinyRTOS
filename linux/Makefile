# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Libraries
LIBS = 

# Target executable
TARGET = main

# Source files
SRCS = main.c scheduler.c context.c timer.c

# Object files (now in build/)
OBJS = $(addprefix build/,$(SRCS:.c=.o))

# Default target
all: build/$(TARGET)

# Create build directory
build:
	mkdir -p build

# Linking the target executable
build/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o build/$(TARGET) $(LIBS)

# Compiling source files
build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf build

# Run the program
run: build/$(TARGET)
	@./build/$(TARGET)

