#!/bin/bash

ROOT_DIR="$(
  cd $(dirname ${0}) &> /dev/null
  pwd -P
)"

pushd "${ROOT_DIR}"

rm -rf BUILD
rm -rf compile_commands.json
mkdir BUILD

CXX="clang++ -stdlib=libc++" cmake -B BUILD -S . -DCMAKE_BUILD_TYPE=Release
CXX="clang++ -stdlib=libc++" bear -- cmake --build BUILD --parallel --config Release
