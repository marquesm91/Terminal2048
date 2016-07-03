# Compiler, Flags, Directory Name and Executable Name
CC= g++
CFLAGS= -std=c++11
EFLAGS= -lncurses
ODIR= ./obj
EDIR= ./bin
EXEC1= t2048

# Source codes and objects
SRCS= t2048_Linux.cpp
OBJS= $(patsubst %.cpp,$(ODIR)/%.o,$(SRCS))

all: $(EDIR)/$(EXEC1)

# Create paste for Objects
$(ODIR):
	@mkdir -p $@

# Concatenate objects with your new directory
$(OBJS): | $(ODIR)

# Special dependencies
main.o: t2048_Linux.h

# Compile and create objects
$(ODIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(EFLAGS)

# Generate the executables
$(EDIR)/$(EXEC1): $(OBJS)
	@mkdir -p $(EDIR) # Create bin paste
	$(CC) $(CFLAGS) $^ -o $@ $(EFLAGS)
	rm -f *.settings

# Delete objects, executables and new directories
clean:
	rm -rf $(OBJS) $(ODIR) $(EDIR)/* $(EDIR)