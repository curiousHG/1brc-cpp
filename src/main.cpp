#include <cstdio>

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "measurements.txt";

    std::FILE* file = std::fopen(path, "rb");
    if (!file) {
        std::perror(path);
        return 1;
    }
    std::fclose(file);

    std::puts("{}");
    return 0;
}
