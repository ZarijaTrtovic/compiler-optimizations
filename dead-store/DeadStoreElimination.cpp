#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv){
    if(argc < 2){
        cout << "User didnt enter any file name" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);

    if(!inputFile.is_open()){
        cerr << "Error: Could not open file" << argv[1] << endl;
        return 1;
    }

    string line;
    int storeCount = 0;

    while(getline(inputFile, line)){
        if(line.find("store") != string::npos){
            storeCount++;
            cout << "STORE # " << storeCount << "] " << line << endl;
        }
    }

    cout << "Total store instructions found:" << storeCount << endl;

    return 0;
} 