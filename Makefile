# Compiler, Flags, Directory Name and Executable Name
CC= g++
CFLAGS= -std=c++11
ODIR= ./Objects
EXEC1= t2048

# Source codes and objects
SRCS= Tile.cpp main.cpp
OBJS= $(patsubst %.cpp,$(ODIR)/%.o,$(SRCS))

all: $(EXEC1)

# Create paste for Objects
$(ODIR):
	@mkdir -p $@

# Concatenate objects with your new directory
$(OBJS): | $(ODIR)

# Special dependencies
main.o: Tiles.h

# Compile and create objects
$(ODIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Generate the executables
$(EXEC1): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	rm -f *.settings

# Delete objects, executables and new directories
clean:
	rm -rf $(OBJS) $(EXEC1) $(ODIR)