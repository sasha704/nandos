#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <sstream>
#include "GameData.hpp"
using namespace std;

class GameState {
    public:
        //initialise game state
        GameState();

        //update the gameData
        void updateGameData(GameData update);

        //convert to string for saving
        string gameStateToString();

        //getters
        GameData getGameData();
        string getState();
        int getChoices();

        //setters
        void setState(string state);
        void setTypeData(std::vector<std::string> newTypeData);
        void setName(string name);
        void setLocation(string location);
        void setDate(string date);
        void setTime(string time);
        void setAffection(std::vector<std::string> affection);
        void setSwitches(std::vector<std::string> switches);
        void setInventory(std::vector<std::string> items);
        void setChoices(int choice);

        //increment the time
        void incrementTime(string increment);

    private:
        string type;
        string typeData [4];
        GameData currentGameData;
        int choices;
};