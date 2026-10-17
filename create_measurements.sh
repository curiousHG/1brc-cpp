#!/bin/bash
set -euo pipefail

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release > /dev/null
cmake --build build --target create_measurements > /dev/null
exec ./build/create_measurements "$@"
