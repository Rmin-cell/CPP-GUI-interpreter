PROTOC = protoc
PROTO_GO_OUT = .
PROTO_GRPC_OUT = .
PROTO_PATH = ../shared/config/proto

TARGET = proto

all: $(TARGET)

.PHONY: proto
proto:
	$(PROTOC) --proto_path=$(PROTO_PATH) --go_out=$(PROTO_GO_OUT) --go-grpc_out=$(PROTO_GRPC_OUT) mathexpr.proto


.PHONY: clean
clean:
	proto/*.pb.*
