# 1BRC in C++

A C++ take on [The One Billion Row Challenge](https://github.com/gunnarmorling/1brc) by Gunnar Morling.

## The challenge

`measurements.txt` holds one measurement per line, `<station>;<temperature>`:

```
Hamburg;12.0
Bulawayo;8.9
Palembang;38.8
```

Compute min, mean and max temperature per station and print them sorted alphabetically by station name, rounded to one decimal:

```
{Abha=-23.0/18.0/59.2, Abidjan=-16.2/26.0/67.3, Abéché=-10.0/29.4/69.0, ...}
```

Input rules:

- Station name: non-empty UTF-8, at most 100 bytes, never contains `;` or `\n`
- Temperature: -99.9 to 99.9, always exactly one fractional digit
- At most 10,000 unique station names
- Lines end with `\n`
- Output values round half up to one decimal, as the reference does with Java's `Math.round(x * 10) / 10.0`

## Layout

| Path | Purpose |
|------|---------|
| `src/main.cpp` | Your solution (`calculate_average`), currently a stub |
| `tools/create_measurements.cpp` | Measurement generator |
| `data/weather_stations.csv` | The 413 official stations with their mean temperature (`name;mean`) |
| `tests/samples/` | Edge-case inputs (`*.txt`) with expected outputs (`*.out`) |
| `test.sh` | Builds and checks the solution against every sample |
| `tocsv.sh` | Normalizes output to one station per line for readable diffs |

## Usage

Build (Release, `-O3 -march=native`):

```
cmake -S . -B build
cmake --build build
```

Generate data. Each reading is drawn from a normal distribution around the station mean with a standard deviation of 10, the same as the official generator. 1 billion rows is about 13 GB.

```
./create_measurements.sh 1_000_000_000
./create_measurements.sh 1_000_000 measurements-1m.txt
```

Run:

```
./build/calculate_average measurements.txt
```

Test against all samples, or a single one:

```
./test.sh
./test.sh tests/samples/measurements-rounding.txt
```

Benchmark:

```
hyperfine --warmup 1 './build/calculate_average measurements.txt'
```

## License

Apache 2.0, see [LICENSE.txt](LICENSE.txt). Station data and test samples come from the original 1BRC repository.
