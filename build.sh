#!/bin/bash

git clone https://github.com/google/libprotobuf-mutator.git
sed -i "/CONFIGURE_COMMAND/a \ \ \ \ \ \ \ \ -DCMAKE_NO_SYSTEM_FROM_IMPORTED=TRUE" libprotobuf-mutator/cmake/external/protobuf.cmake
mkdir -p external
mkdir -p build

pushd build
cmake ../libprotobuf-mutator -DLIB_PROTO_MUTATOR_DOWNLOAD_PROTOBUF=1 -DCMAKE_INSTALL_PREFIX=../external/ -DCMAKE_C_FLAGS="-fPIC" -DCMAKE_CXX_FLAGS="-fPIC"
make -j 12
make install
popd

mkdir -p ./external/bin ./external/include ./external/lib/
cp -r ./build/external.protobuf/include/* ./external/include
cp ./build/external.protobuf/bin/protoc ./external/bin/

