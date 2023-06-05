# Variables
CC = g++
CFLAGS = -Iinclude -fdiagnostics-color=always -g
DEPS = $(wildcard include/*.h)
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, bin/%.o, $(SRC))
TARGET = bin/$(notdir $(CURDIR))

# Default make
all: $(TARGET)

# Rule for creating the object files
bin/%.o: src/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule for creating the target
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Clean
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
