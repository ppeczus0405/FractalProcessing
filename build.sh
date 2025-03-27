#!/bin/bash

ROOT_DIR="$(
  cd $(dirname ${0}) &> /dev/null
  pwd -P
)"

pushd "${ROOT_DIR}"

rm -rf BUILD
mkdir BUILD

# Commands Json
if [[ "${1}" == "--commands-json" ]]; then
  rm -rf compile_commands.json
  CC="clang" CXX="clang++ -stdlib=libc++" cmake -B BUILD -S . -G "Ninja Multi-Config" -DQT6_INSTALL_DIR="/home/p3ki/.local/lib/Qt6.8/"
  CC="clang" CXX="clang++ -stdlib=libc++" bear -- cmake --build BUILD --parallel --config Debug
else
  CC="gcc" CXX="g++" cmake -B BUILD -S . -G "Ninja Multi-Config" -DQT6_INSTALL_DIR="/home/p3ki/.local/lib/Qt6.8/"
  CC="gcc" CXX="g++" bear -- cmake --build BUILD --parallel --config Debug
fi
