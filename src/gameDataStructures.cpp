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
    time = 0;
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

void GameData::setTime(float newtime){
    time = newtime;
}

void GameData::setAffection(std::vector<std::string> newAffectionLevels){
    for (int i = 0; i< NO_OF_DATES;  i++){
        affectionLevels[i] = std::stof(newAffectionLevels[i]);
    }
}

void GameData::setSwitches(std::vector<std::string> newSwitches){
    for (int i = 0; i< NO_OF_SWITCHES;  i++){
        switches[i] = stoi(newSwitches[i]);
    }
}

void GameData::setInventory(std::vector<std::string> inventoryList){
    inventory.setItems(inventoryList);
}

string GameData::getName(){
    return name;
}

string GameData::getLocation(){
    return location;
}

string GameData::getDate(){
    return date;
}

int GameData::getTime(){
    return time;
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

void GameState::setName(string name){
    currentGameData.setName(name);
}

void GameState::setLocation(string location){
    currentGameData.setLocation(location);
}

void GameState::setDate(string date){
    currentGameData.setDate(date);
}

void GameState::setTime(string time){
    currentGameData.setTime(std::stof(time));
}

void GameState::setAffection(std::vector<std::string> affection){
    currentGameData.setAffection(affection);
}

void GameState::setSwitches(std::vector<std::string> switches){
    currentGameData.setSwitches(switches);
}

void GameState::setInventory(std::vector<std::string> items){
    currentGameData.setInventory(items);
}

string convertTypeData(string typeData[]){
    string stringData = "";
    
    for(int i = 0; i < 3; i++){
        stringData += typeData[i];
        stringData += "-";
    }

    stringData += typeData[4];

    return stringData;
}

void GameState::setTypeData(std::vector<std::string> newTypeData){
    for (int i = 0; i < 4; i++){
        typeData[i] = newTypeData[i];
    }
    
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

void GameState::incrementTime(string increment){
    currentGameData.setTime(currentGameData.getTime() + std::stof(increment));
}

