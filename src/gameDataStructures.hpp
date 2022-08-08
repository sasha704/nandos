#include <iostream>
#include <string>

class Storage {
    public:
        //create instance of storage
        Storage();

    private:
        char* items = NULL;
};

class GameData {
    public:
        //initialise gameData
        GameData();

    private:
        char* location;
        char* date;
        char* time;
        char* name;
        float* affectionLevels = new float[6];
        Storage inventory;
        bool* switches = new bool[1];
};

class GameState {
    public:
        //initialise game state
        GameState();

        //change the gameState to menu
        void openMenu();
        
        //change the gameState to game
        void openGame();

        //change the gameState to choice
        void openChoice(char choices[]);

        //change the gameState to load/save
        void openLoadSave();

        //update the gameData
        void updateGameData(GameData update);

        //get the gameData from the gameState
        GameData getGameData();

        //get the current state
        char* getState();

        //change the state to quit
        void quitGame();

        void setState(char* state);

    private:
        char* type;
        char* typeData = NULL;
        GameData currentGameData;
};