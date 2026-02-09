#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct StoreInstruction {
    int lineNumber;
    string fullLine;
    string adress;
    bool isDead;
};

bool isStoreLine(const string& s){
    return s.find("store") != string::npos;
}

bool isLoadForAddress(const string& line, const string& address) {
    return (line.find("load") != string::npos) && 
           (line.find(address) != string::npos);
}

string extractAdress(const string& line){
    auto pointerPosition = line.find("i32*");

    if(pointerPosition == string::npos){
        return "";
    }

    auto startPosition = pointerPosition + 5;

    auto endPosition = line.find_first_of(" ,\t", startPosition);

    return line.substr(startPosition, endPosition - startPosition);
}

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

    vector<string> lines;
    string line;
    int storeCount = 0;

    while(getline(inputFile, line)){
            lines.push_back(line);
    }

    cout << "Total store instructions found:" << storeCount << endl;

    vector<StoreInstruction> stores;

    for(int i = 0; i < lines.size(); i++){
        if(isStoreLine(lines[i])) {
            StoreInstruction store;
            store.lineNumber = i + 1;
            store.fullLine = lines[i];
            store.adress = extractAdress(lines[i]);
            store.isDead = false;

            stores.push_back(store);
        }
    }

    for(int i = 0; i < stores.size(); i++){
        string currentAdress = stores[i].adress;
        int currentIndex = stores[i].lineNumber - 1;

        for(int j = currentIndex + 1; j < lines.size(); j++){
            string nextLine = lines[j];

            if(isStoreLine(nextLine) && extractAdress(nextLine) == currentAdress){
                stores[i].isDead = true;
                break;
            }
            if(isLoadForAddress(nextLine, currentAdress)){
                stores[i].isDead = false;
                break;
            }

            if(nextLine.find("ret") != string::npos){
                if(nextLine.find(currentAdress) == string::npos){
                    stores[i].isDead = true;
                }
                break;
            }
        }
    }

    for(const auto& store : stores){
        if(store.isDead){
            cout << "DEAD LINE: " << store.lineNumber << ": "
                << store.fullLine << endl;
        }
        else {
            cout << "[LIVE] Line " << store.lineNumber << ": " 
                 << store.fullLine << endl;
        }
    }

    return 0;
} 