# Define the compiler
CC = gcc

# Define the compiler flags
CFLAGS = -Wall -Wextra -O2

# Define the target executable name
TARGET = posix_assignment

# Define the source files
SRC = posix_assignment.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Rule to clean up the build files
clean:
	rm -f $(TARGET)
