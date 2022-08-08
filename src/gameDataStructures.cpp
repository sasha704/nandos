#include <iostream>
#include <string>
#include "gameDataStructures.hpp"

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


Storage::Storage() {
	//initialise
}

GameData::GameData() {
    //initialise
    location = NULL;
    date = NULL;
    time = NULL;
    name = NULL;
    affectionLevels = new float[6] {0,0,0,0,0,0};
    inventory = Storage();
    switches = new bool[1] {false};
}



GameState::GameState(){
    //initialise
    type = NULL;
    typeData = new char[0];
    currentGameData = GameData();
}

//change the gameState to menu
void GameState::openMenu(){
    type = "menu";
}

//change the gameState to game
void GameState::openGame(){
    type = "game";
}

//change the gameState to choice
void GameState::openChoice(char choices[]){
    type = "choice";
    typeData = choices;
}

void GameState::openLoadSave(){
    type = "loadSave";
}

void GameState::updateGameData(GameData update){
    currentGameData = update;
}

GameData GameState::getGameData(){
    return currentGameData;
}

char* GameState::getState(){
    return type;
}

void GameState::quitGame(){
    type = "quit";
}

void GameState::setState(char* state){
    type = state;
}

