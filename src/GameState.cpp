#include <iostream>
#include <string>
#include "gameState.hpp"
using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::string;

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

int GameState::getChoices(){
    return choices;
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
    currentGameData.setDate(std::stof(date));
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

void GameState::setChoices(int choice){
    choices = choice;
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

