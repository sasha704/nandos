#include <iostream>
#include <string>
using namespace std;

class Storage {
    public:
        //create instance of storage
        Storage();

        //convert list of objects to string
        string convertStorage();

    private:
        char* items = NULL;
        int size;
};