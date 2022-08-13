#include <iostream>
#include <string>
#include "gameDataStructures.hpp"
using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::string;

int NO_OF_SWITCHES = 1;
int NO_OF_DATES = 6;

/* game state:
 - type of screen being shown (menu, choice, game, load/save)

 menu = ["menu", [gameData]]
 choice = ["choice", "choice text", [options], [gameData]]
 game = ["game", [gameData]] 
 load/save = ["loadSave", [gameData]]

 gameData = ["location", "date", "time", "name", [affection levels], [inventory], [switches]]
 (switches originally all set to false but can be changed via player choices)
 
 */

/* switch key:
    0 - carrying key
*/

GameData::GameData() {
    //initialise
    location = "outside";
    date = "0";
    time = "0";
    name = "name";
    affectionLevels = new float[6] {0,0,0,0,0,0};
    inventory = Storage();
    switches = new int[1] {0};
}

void GameData::setName(string newname){
    name = newname;
}

void GameData::setLocation(string newlocation){
    location = newlocation;
}

void GameData::setDate(string newdate){
    date = newdate;
}

void GameData::setTime(string newtime){
    time = newtime;
}

//UNFINISHED
void GameData::setAffection(float newAffectionLevels[]){
    affectionLevels = newAffectionLevels;
}

//UNFINISHED
void GameData::setSwitches(int newSwitches[]){
    switches = newSwitches;
}

//UNFINISHED
void GameData::setInventory(char* inventoryList){
    //add items to inventory (Storage object)
}


string convertSwitches(int switches[]){
    string stringSwitches = "";

    for (int i = 0; i < NO_OF_SWITCHES-1; i++){
        stringSwitches += std::to_string(switches[i]);
        stringSwitches += "-";
    }
    
    stringSwitches += std::to_string(switches[NO_OF_SWITCHES-1]);
    stringSwitches += ",";

    return stringSwitches;
}

string convertAffection(float affectionLevels[]){
    string stringAffection = "";

    for (int i = 0; i < NO_OF_DATES-1; i++) {
        stringAffection += std::to_string(affectionLevels[i]);
        stringAffection += "-";
    }

    stringAffection += std::to_string(affectionLevels[NO_OF_DATES-1]);
    stringAffection += ",";

    return stringAffection;
}

string GameData::data2String(){
    string stringData = "";
    stringData += name;
    stringData += ",";
    stringData += location;
    stringData += ",";
    stringData += date;
    stringData += ",";
    stringData += time;
    stringData += ",";
    stringData += convertAffection(affectionLevels);
    stringData += convertSwitches(switches);
    stringData += inventory.convertStorage();
    return stringData;

}

GameState::GameState(){
    //initialise
    type = "menu";
    typeData = new char[0];
    currentGameData = GameData();
}

void GameState::updateGameData(GameData update){
    currentGameData = update;
}

GameData GameState::getGameData(){
    return currentGameData;
}

string GameState::getState(){
    return type;
}

void GameState::setState(string state){
    type = state;
}

//UNFINISHED
string convertTypeData(string typeData){
    return "";
}

//UNFINISHED
void GameState::setTypeData(string* newTypeData){

}

string GameState::gameStateToString(){
    string stringData = "";

    stringData += currentGameData.data2String();
    stringData += ",";
    stringData += type;
    stringData += ",";
    stringData += convertTypeData(typeData);
   

    return stringData;
}

