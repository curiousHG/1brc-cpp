#include <cstdio>
#include <bits/stdc++.h>
#include <iomanip>

using namespace std;
int main(int argc, char** argv) {
    const char* path = argc > 1 ? argv[1] : "measurements.txt";

    ifstream file(path);

    // 2. Check if the file opened successfully
    if (!file.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return 1;
    }

    string line;
    // 3. Read line-by-line until the end of the file
    char colon = ';';
    unordered_map<string, array<double, 4>> mes;
    while (getline(file, line)) {
        int pos = line.find(colon);
        string name = line.substr(0, pos);
        double temp = stod(line.substr(pos+1));
        if(mes.find(name)!=mes.end()){
            mes[name][0] = min(mes[name][0], temp);
            mes[name][1]+=temp;
            mes[name][2] = max(mes[name][2], temp);
            mes[name][3]++;
        }else{
            mes[name][0] = temp;
            mes[name][1] = temp;
            mes[name][2] = temp;
            mes[name][3] = 1;
        }
    }
    cout<<"{";
    // Abha=-23.0/18.0/59.2
    // have 1.0 precision for min, 1.0 precision for average, and 1.0 precision for max
    // set precision once to 1
    cout<<fixed<<setprecision(1);
    for(auto it = mes.begin(); it != mes.end(); ++it) {
        cout<<it->first<<"="<<it->second[0]<<"/"<<it->second[1]/it->second[3]<<"/"<<it->second[2];
        if(next(it) != mes.end()) {
            cout<<", ";
        }

    }
    cout<<"}"<<endl;
    return 0;
}
