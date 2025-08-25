# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lm -levent -luuid
SRCDIR = src
OBJDIR = obj
INCDIR = include

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = HydraSync

# Default target
all: $(TARGET)

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link main.c with object files to create executable
$(TARGET): main.c $(OBJECTS)
	$(CC) $(CFLAGS) main.c $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Show help
help:
	@echo "Targets disponibles:"
	@echo "  all     - Compile le projet (default)"
	@echo "  clean   - Supprime les fichiers de build"
	@echo "  rebuild - Clean puis compile"
	@echo "  run     - Compile et exécute le programme"
	@echo "  debug   - Compile en mode debug"
	@echo "  help    - Affiche cette aide"

.PHONY: all clean rebuild run debug help