#!/bin/bash

DISTRO=$(awk -F= '/^ID=/{print $2}' /etc/os-release)
GRPC_VERSION=1.35.0

printf "Detected $DISTRO\nTrying to install necessary packages...\n"

if [ "$DISTRO" == "manjaro" ]; then
	sudo pacman -Sy openssl cmake autoconf libtool pkg-config clang
elif [ "$DISTRO" == "ubuntu" ]; then
	sudo apt-get update && sudo apt-get install -y cmake libssl-dev build-essential autoconf libtool pkg-config libc-ares-dev libabsl-dev
elif [ "$DISTRO" == "centos" ]; then
	sudo yum -y install openssl cmake autoconf libtool pkg-config
else
	printf 'Unsupported Linux distro\n'
	exit 0
fi

if [ "$1" == "d" ]; then
    sudo rm -rf grpc
    git clone --recurse-submodules -b v$GRPC_VERSION https://github.com/grpc/grpc

    if [ $? -ne 0 ]; then
        echo "failed fo download"
        exit 1
    fi
fi

cd grpc/

# Install absl
printf "Trying to install abseil...\n"
mkdir -p third_party/abseil-cpp/cmake/build
cd third_party/abseil-cpp/cmake/build
sudo cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ../..
sudo make -j4 install
cd ../../../../

# Install c-ares
# If the distribution provides a new-enough version of c-ares,
# this section can be replaced with:
# apt-get install -y libc-ares-dev
printf "Trying to install cares...\n"
mkdir -p third_party/cares/cares/cmake/build
cd third_party/cares/cares/cmake/build
sudo cmake -DCMAKE_BUILD_TYPE=Release ../..
sudo make -j4 install
cd ../../../../../

# Install protobuf
printf "Trying to install protobuf...\n"
mkdir -p third_party/protobuf/cmake/build
cd third_party/protobuf/cmake/build
sudo cmake -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release ..
sudo make -j4 install
cd ../../../../

# Install re2
printf "Trying to install re2...\n"
mkdir -p third_party/re2/cmake/build
cd third_party/re2/cmake/build
sudo cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ../..
sudo make -j4 install
cd ../../../../

# Install zlib
printf "Trying to install zlib...\n"
mkdir -p third_party/zlib/cmake/build
cd third_party/zlib/cmake/build
sudo cmake -DCMAKE_BUILD_TYPE=Release ../..
sudo make -j4 install
cd ../../../../

# Install gRPC
printf "Trying to install gRPC...\n"
mkdir -p cmake/build
cd cmake/build
sudo cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DgRPC_INSTALL=ON \
  -DgRPC_BUILD_TESTS=OFF \
  -DgRPC_CARES_PROVIDER=package \
  -DgRPC_ABSL_PROVIDER=package \
  -DgRPC_PROTOBUF_PROVIDER=package \
  -DgRPC_RE2_PROVIDER=package \
  -DgRPC_SSL_PROVIDER=package \
  -DgRPC_ZLIB_PROVIDER=package \
  ../..
sudo make -j4 install
cd ../..

# sudo rm -rf grpc
