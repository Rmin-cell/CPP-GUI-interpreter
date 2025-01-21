CC = g++

# Wall: Enables most compiler warnings
# Wshadow: Warns about variables that shadow other variables.
# O2: optimize the code for speed (level2)
CCFLAGS = -Wall -Wshadow -O2

# linker flag:
# 	lm: link the math library (libm)
LFLAGS = -lm

TARGET = example

all: $(TARGET)

.PHONY: example
example: my-tinyexpr.o
	$(CC) $(CCFLAGS) -g -o $@.out $^ $(LFLAGS)

.PHONY: .cpp.o
.cpp.o:
	$(CC) -c $(CCFLAGS) $< -o $@

.PHONY: clean
clean:
	rm *.o *.exe *.a proto/*.pb.*
