#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Storage {
    public:
        //create instance of storage
        Storage();

        //convert list of objects to string
        string convertStorage();

        //set the items in a storage object
        void setItems(std::vector<std::string> itemList);

        //add an item to the storage
        void addItem(string item);

    private:
        std::vector<std::string> items;
        int size;
};