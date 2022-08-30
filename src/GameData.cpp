/*
GameData structure holds information about the game data.
*/
#include <iostream>
#include <string>
#include "GameData.hpp"
using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::string;

//GLOBAL VARIABLES
int NO_OF_SWITCHES = 1;
int NO_OF_DATES = 6;

//-----------------------------------INITIALIZE:-------------------
GameData::GameData() {
    //initialise
    location = "outside";
    date = 0;
    time = 0;
    name = "name";
    affectionLevels = new float[6] {0,0,0,0,0,0};
    inventory = Storage();
    switches = new int[1] {0};
}

//-----------------------------------SETTERS:----------------------
void GameData::setName(string newname){
    name = newname;
}

void GameData::setLocation(string newlocation){
    location = newlocation;
}

void GameData::setDate(int newdate){
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

//------------------------------------GETTERS:----------------------
string GameData::getName(){
    return name;
}

string GameData::getLocation(){
    return location;
}

int GameData::getDate(){
    return date;
}

int GameData::getTime(){
    return time;
}

//------------------CONVERSION TO STRING:---------------------------
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
    stringData += std::to_string(date);
    stringData += ",";
    stringData += std::to_string(time);
    stringData += ",";
    stringData += convertAffection(affectionLevels);
    stringData += convertSwitches(switches);
    stringData += inventory.convertStorage();
    return stringData;

}