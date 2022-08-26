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

        void setName(string newname);
        void setLocation(string newlocation);
        void setDate(string newdate);
        void setTime(float newtime);
        void setAffection(std::vector<std::string> newAffectionLevels);
        void setSwitches(std::vector<std::string>  newSwitches);
        void setInventory(std::vector<std::string> inventoryList);

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

class GameState {
    public:
        //initialise game state
        GameState();

        //update the gameData
        void updateGameData(GameData update);

        //get the gameData from the gameState
        GameData getGameData();

        //get the current state
        string getState();

        //change the state
        void setState(string state);

        //convert to string for saving
        string gameStateToString();

        void setTypeData(std::vector<std::string> newTypeData);

        //change the name within the game data
        void setName(string name);

        //change the location within the game data
        void setLocation(string location);

        //change the date
        void setDate(string date);

        //change the time
        void setTime(string time);

        //change the affection
        void setAffection(std::vector<std::string> affection);

        //change the switches
        void setSwitches(std::vector<std::string> switches);

        //change the inventory
        void setInventory(std::vector<std::string> items);

        //increment the time
        void incrementTime(string increment);

    private:
        string type;
        string typeData [4];
        GameData currentGameData;
};