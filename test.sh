#!/bin/bash
set -euo pipefail

INPUT=${1:-"tests/samples/*.txt"}

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release > /dev/null
cmake --build build --target calculate_average > /dev/null

echo "Compiled"
failed=0
for sample in $INPUT; do
  if diff --color=always <(./build/calculate_average "$sample" | ./tocsv.sh) <(./tocsv.sh < "${sample%.txt}.out"); then
    echo "PASS $sample"
  else
    echo "FAIL $sample"
    failed=$((failed + 1))
  fi
done

if [ "$failed" -ne 0 ]; then
  echo "$failed sample(s) failed"
  exit 1
fi
