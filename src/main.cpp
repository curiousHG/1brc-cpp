#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

long long tenth(std::string_view &s){
    long long num = 0;
    for(char c : s) {
        if (isdigit(c)) {
            num = num * 10 + (c - '0');
        }
    }
    return s[0] == '-' ? -num : num;
}
void printNum(long long n){
    if(n<0){
        std::cout<<"-";
        n = -n;
    }
    std::cout << n / 10 << "." << n % 10;
}

int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "measurements.txt";

    std::ifstream file(path);

    // 2. Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    std::string line;
    // 3. Read line-by-line until the end of the file
    char colon = ';';
    std::unordered_map<std::string, std::array<long long, 4>> mes;
    while (std::getline(file, line)) {
        std::string_view line_view(line);
        size_t pos = line_view.find(colon);
        if (pos == std::string_view::npos) {
            std::cerr << "Error: Invalid line format!" << std::endl;
            continue; // Skip this line and continue with the next
        }
        auto name = std::string(line_view.substr(0, pos));
        std::string_view value_view = line_view.substr(pos + 1);
        long long temp = tenth(value_view);
        if(mes.find(name)!=mes.end()){
            mes.find(name)->second[0] = std::min(mes.at(name)[0], temp);
            mes.find(name)->second[1]+=temp;
            mes.find(name)->second[2] = std::max(mes.at(name)[2], temp);
            mes.find(name)->second[3]++;
        }else{
            mes[name][0] = temp;
            mes[name][1] = temp;
            mes[name][2] = temp;
            mes[name][3] = 1;
        }
    }
    std::cout<<"{";
    std::vector<std::pair<std::string, std::array<long long, 4>>> sorted_mes(mes.begin(), mes.end());
    std::sort(sorted_mes.begin(), sorted_mes.end());
    for(const auto& [name, stats] : sorted_mes) {
        std::cout<<name<<"=";
        printNum(stats[0]);
        std::cout<<"/";
        printNum((long long)floor((double)stats[1] / stats[3] + 0.5));
        std::cout<<"/";
        printNum(stats[2]);
        if (&name != &sorted_mes.back().first) {
            std::cout<<", ";
        }
        
    }
    std::cout<<"}"<<std::endl;
    return 0;
}
