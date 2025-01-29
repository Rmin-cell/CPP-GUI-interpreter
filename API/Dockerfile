FROM golang:1.22-alpine AS grpc-builder

RUN apk add --no-cache \
    build-base \
    cmake \
    autoconf \
    libtool \
    pkgconfig \
    openssl-dev \
    c-ares-dev \
    git \
    make \
    protobuf-dev \
    protobuf

RUN go install google.golang.org/protobuf/cmd/protoc-gen-go@latest && \
    go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@latest

# --- --- # --- --- #
FROM golang:1.22-alpine AS grpc-runtime

RUN apk add --no-cache \
    openssl-dev \
    c-ares-dev \
    make \
    protobuf-dev

COPY --from=grpc-builder /usr/local /usr/local
COPY --from=grpc-builder /go/bin/protoc-gen-go /go/bin/protoc-gen-go
COPY --from=grpc-builder /go/bin/protoc-gen-go-grpc /go/bin/protoc-gen-go-grpc

FROM grpc-runtime AS app-builder

WORKDIR /app/src

COPY src/go.mod src/go.sum ./


RUN go mod download


COPY src .
COPY shared /shared

RUN make proto

RUN CGO_ENABLED=0 GOOS=linux go build -o api .

FROM alpine:latest
WORKDIR /app/src

COPY --from=app-builder /app/src/api .

COPY src/static ./static

EXPOSE 8080

# Command to run the application
CMD ["./api"]

