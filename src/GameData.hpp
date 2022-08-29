#include <iostream>
#include <string>
#include "Storage.hpp"
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class GameData {
    public:
        //initialise gameData
        GameData();

        //convert game data to a string
        string data2String();

        //setters
        void setName(string newname);
        void setLocation(string newlocation);
        void setDate(string newdate);
        void setTime(float newtime);
        void setAffection(std::vector<std::string> newAffectionLevels);
        void setSwitches(std::vector<std::string>  newSwitches);
        void setInventory(std::vector<std::string> inventoryList);

        //getters
        string getName();
        string getLocation();
        string getDate();
        int getTime();

    private:
        string location;
        string date;
        float time;
        string name;
        float* affectionLevels;
        Storage inventory;
        int* switches;
};
