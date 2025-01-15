CC = gcc

# Wall: Enables most compiler warnings
# Wshadow: Warns about variables that shadow other variables.
# O2: optimize the code for speed (level2)
CCFLAGS = -Wall -Wshadow -O2

# linker flag:
# 	lm: link the math library (libm)
LFLAGS = -lm

PROTOC = protoc
PROTO_CPP_OUT = ./proto
PROTO_GRPC_OUT = ./proto
PROTO_PATH = ../../shared/config/proto
PROTOC_PLUGIN_GEN = $(shell which grpc_cpp_plugin)

TARGET = example

all: $(TARGET)

.PHONY: example
example: example.o tinyexpr.o
	$(CC) $(CCFLAGS) -o $@.a $^ $(LFLAGS)
#                       || ||
#                       || ||-> special variable that represents the all dependencies (example.o tinyexpr.o)
#                       ||-> special variable that represents the target (example)

.PHONY: proto
proto:
	$(PROTOC) --proto_path=$(PROTO_PATH) --cpp_out=$(PROTO_CPP_OUT) --grpc_out=$(PROTO_GRPC_OUT) --plugin=protoc-gen-grpc=$(PROTOC_PLUGIN_GEN) mathexpr.proto

.PHONY: .c.o
.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@
#                       ||
#                       ||-> special automatic variable that refers to the first dependency (the .c file)

.PHONY: clean
clean:
	rm *.o *.exe *.a proto/*.pb.*
