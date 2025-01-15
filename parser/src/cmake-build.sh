#! /bin/bash

mkdir -p cmake/build
pushd cmake/build
cmake ../..
make
popd
