#include <iostream>
#include <string>
#include "Storage.hpp"
using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::string;

Storage::Storage() {
	//initialise
    size = 0;
}

string Storage::convertStorage(){
    string stringStorage = "";

    for (int i = 0; i < size-1; i++) {
        stringStorage += std::to_string(stringStorage[i]);
        stringStorage += "-";
    }

    stringStorage += std::to_string(stringStorage[size-1]);

    return stringStorage;
}
