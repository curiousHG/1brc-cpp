#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

struct Station {
    std::string name;
    double mean;
};

static std::vector<Station> load_stations(const char* path) {
    std::ifstream in(path);
    std::vector<Station> stations;
    std::string line;
    while (std::getline(in, line)) {
        auto separator = line.rfind(';');
        if (line.empty() || line[0] == '#' || separator == std::string::npos) {
            continue;
        }
        stations.push_back({line.substr(0, separator), std::stod(line.substr(separator + 1))});
    }
    return stations;
}

static bool parse_count(std::string_view text, std::uint64_t& count) {
    std::string digits;
    for (char c : text) {
        if (c != '_') {
            digits += c;
        }
    }
    auto [end, error] = std::from_chars(digits.data(), digits.data() + digits.size(), count);
    return error == std::errc() && end == digits.data() + digits.size() && count > 0;
}

static char* write_temperature(char* out, double value) {
    long tenths = static_cast<long>(std::floor(value * 10.0 + 0.5));
    tenths = std::clamp(tenths, -999L, 999L);
    if (tenths < 0) {
        *out++ = '-';
        tenths = -tenths;
    }
    out = std::to_chars(out, out + 4, tenths / 10).ptr;
    *out++ = '.';
    *out++ = static_cast<char>('0' + tenths % 10);
    return out;
}

int main(int argc, char** argv) {
    std::uint64_t count = 0;
    if (argc < 2 || argc > 4 || !parse_count(argv[1], count)) {
        std::fprintf(stderr,
                     "Usage: %s <number of records> [output=measurements.txt] "
                     "[stations=data/weather_stations.csv]\n"
                     "       underscores are allowed, e.g. 1_000_000_000\n",
                     argv[0]);
        return 1;
    }
    const char* output_path = argc > 2 ? argv[2] : "measurements.txt";
    const char* stations_path = argc > 3 ? argv[3] : "data/weather_stations.csv";

    auto stations = load_stations(stations_path);
    if (stations.empty()) {
        std::fprintf(stderr, "No stations loaded from %s\n", stations_path);
        return 1;
    }

    std::FILE* out = std::fopen(output_path, "wb");
    if (!out) {
        std::perror(output_path);
        return 1;
    }

    std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<std::size_t> pick(0, stations.size() - 1);
    std::normal_distribution<double> deviation(0.0, 10.0);

    constexpr std::size_t buffer_size = 1 << 22;
    constexpr std::size_t max_line = 128;
    std::vector<char> buffer(buffer_size);
    char* cursor = buffer.data();
    char* flush_at = buffer.data() + buffer_size - max_line;

    for (std::uint64_t i = 1; i <= count; ++i) {
        const Station& station = stations[pick(rng)];
        cursor = std::copy(station.name.begin(), station.name.end(), cursor);
        *cursor++ = ';';
        cursor = write_temperature(cursor, station.mean + deviation(rng));
        *cursor++ = '\n';

        if (cursor >= flush_at) {
            std::fwrite(buffer.data(), 1, cursor - buffer.data(), out);
            cursor = buffer.data();
        }
        if (i % 50'000'000 == 0) {
            std::fprintf(stderr, "Wrote %llu measurements\n", static_cast<unsigned long long>(i));
        }
    }
    std::fwrite(buffer.data(), 1, cursor - buffer.data(), out);
    std::fclose(out);

    std::fprintf(stderr, "Created %s with %llu measurements from %zu stations\n", output_path,
                 static_cast<unsigned long long>(count), stations.size());
    return 0;
}
