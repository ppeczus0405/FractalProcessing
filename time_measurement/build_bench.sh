#!/bin/bash

TIME_MEASUREMENT_ROOT_DIR="$(
  cd $(dirname ${0}) &> /dev/null
  pwd -P
)"

(cd "${TIME_MEASUREMENT_ROOT_DIR}/.." && ./build.sh)
rm -f "${TIME_MEASUREMENT_ROOT_DIR}/fractal_benchmark"
install "${TIME_MEASUREMENT_ROOT_DIR}/../BUILD/out/Release/bin/time_measurement" "${TIME_MEASUREMENT_ROOT_DIR}/fractal_benchmark"
