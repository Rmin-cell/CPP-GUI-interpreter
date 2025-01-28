# --- --- --- ^&^ --- --- --- #
#           Stage 1
# --- --- --- ^&^ --- --- --- #

FROM ubuntu:20.04 AS grpc-builder

ARG GRPC_VERSION=1.35.0
ENV DEBIAN_FRONTEND=noninteractive

# --- --- # --- --- #
RUN apt-get update

RUN apt-get install -y --no-install-recommends \
    build-essential cmake autoconf libtool pkg-config \
    libssl-dev libc-ares-dev git ca-certificates

RUN rm -rf /var/lib/apt/lists/*

# --- --- # --- --- #
RUN git clone --recurse-submodules -b v${GRPC_VERSION} https://github.com/grpc/grpc /grpc

WORKDIR /grpc

# Install absl
# --- --- # --- --- #
RUN mkdir -p third_party/abseil-cpp/cmake/build && \
    cd third_party/abseil-cpp/cmake/build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ../.. && \
    make -j$(nproc) install

# Install protobuf
# --- --- # --- --- #
RUN mkdir -p third_party/protobuf/cmake/build && \
    cd third_party/protobuf/cmake/build && \
    cmake -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) install

# Install re2
# --- --- # --- --- #
RUN mkdir -p third_party/re2/cmake/build && \
    cd third_party/re2/cmake/build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ../.. && \
    make -j$(nproc) install

# Install zlib
# --- --- # --- --- #
RUN mkdir -p third_party/zlib/cmake/build && \
    cd third_party/zlib/cmake/build && \
    cmake -DCMAKE_BUILD_TYPE=Release ../.. && \
    make -j$(nproc) install

# Install gRPC
# --- --- # --- --- #
RUN mkdir -p cmake/build && \
    cd cmake/build && \
    cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DgRPC_INSTALL=ON \
    -DgRPC_BUILD_TESTS=OFF \
    -DgRPC_ABSL_PROVIDER=package \
    -DgRPC_CARES_PROVIDER=package \
    -DgRPC_PROTOBUF_PROVIDER=package \
    -DgRPC_RE2_PROVIDER=package \
    -DgRPC_SSL_PROVIDER=package \
    -DgRPC_ZLIB_PROVIDER=package \
    ../.. && \
    make -j$(nproc) install && \
    ldconfig

# --- --- --- ^&^ --- --- --- #
#           Stage 2
# --- --- --- ^&^ --- --- --- #
FROM ubuntu:20.04 AS grpc-runtime

ENV DEBIAN_FRONTEND=noninteractive

# --- --- # --- --- #
RUN apt-get update

RUN apt-get install -y --no-install-recommends \
    libssl-dev libc-ares-dev build-essential cmake

RUN rm -rf /var/lib/apt/lists/*

# --- --- # --- --- #
COPY --from=grpc-builder /usr/local /usr/local

# --- --- --- ^&^ --- --- --- #
#           Stage 3
# --- --- --- ^&^ --- --- --- #
FROM grpc-runtime AS app-builder

WORKDIR /app
COPY . .

WORKDIR /app/src
RUN ./cmake-build.sh

WORKDIR /app

CMD ["./src/cmake/build/server"]
