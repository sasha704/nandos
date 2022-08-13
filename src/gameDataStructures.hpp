#include <iostream>
#include <string>
#include "Storage.hpp"
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
        void setTime(string newtime);
        void setAffection(float newAffectionLevels[]);
        void setSwitches(int newSwitches[]);
        void setInventory(char* inventoryList);

    private:
        string location;
        string date;
        string time;
        string name;
        float* affectionLevels = new float[6];
        Storage inventory;
        int* switches = new int[1];
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

        void setTypeData(string* newTypeData);

    private:
        string type;
        char* typeData = NULL;
        GameData currentGameData;
};