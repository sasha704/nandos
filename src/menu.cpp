/*THE GAME*/

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "button.hpp"
#include "gameDataStructures.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


//-------------------CONSTANTS:-------------------

//size of window
const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 768;

//button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 100;
const int TOTAL_BUTTONS = 3;

//-----------------GLOBAL VARIABLES:---------------

//global gamestate
GameState gameState;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ]; 
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];

//--------------------BUTTON TEXTURES:-------------------

//menu sprite textures
LTexture buttonSpriteSheetTexture;

//---------------BACKGROUNDS:----------------------------

//menu texture
LTexture backgroundTexture;

//--------------BUTTON TEXT:--------------------------------

//darktext
LTexture quitButtonTextDark;
LTexture playButtonTextDark;
LTexture loadButtonTextDark;

//lighttext
LTexture quitButtonTextLight;
LTexture playButtonTextLight;
LTexture loadButtonTextLight;

//-----------------METHODS:----------------------------------


//starts up SDL and creates window
bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Set texture filtering to linear
		if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1") ) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		window = SDL_CreateWindow( "Extra Spicy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
		if (window == NULL) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if ( renderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				//Initialize SDL_ttf
				if( TTF_Init() == -1 ) {
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

//load media
bool loadMedia() {
	//Loading success flag
	bool success = true;

	//Load sprites
	if ( !buttonSpriteSheetTexture.loadFromFile( renderer, "../images/buttons.png" ) ) {
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else {

		//Load background texture
		if( !backgroundTexture.loadFromFile( renderer, "../images/start_screen_nandoos.png" ) ) {
			printf( "Failed to load background texture image!\n" );
			success = false;
		}
		
		//Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i ) {
			gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i * 885;
			gSpriteClips[ i ].w = 1700;
			gSpriteClips[ i ].h = 885;
		}
		

		//Set button positions
		gButtons[0].setPosition( 0, 0 );
		gButtons[0].setID("PLAY");
		
		gButtons[1].setPosition( 0, WINDOW_HEIGHT - ((BUTTON_HEIGHT * 2) + 50) );
		gButtons[1].setID("LOAD");

		gButtons[2].setPosition( 0, WINDOW_HEIGHT - BUTTON_HEIGHT );
		gButtons[2].setID("QUIT");

	}

	//Open the font
	gFont = TTF_OpenFont( "../fonts/SeaweedScript-Regular.ttf", 28 );
	if (gFont == NULL) {
		printf( "Failed to load seaweed font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else {
		//Render text
		SDL_Color textColorDark = { 0, 0, 0 };
		SDL_Color textColorLight = { 255, 255, 255 };
		if (! (quitButtonTextDark.loadFromRenderedText( renderer, gFont, "Quit", textColorDark ) && loadButtonTextDark.loadFromRenderedText( renderer, gFont, "Load", textColorDark ) && playButtonTextDark.loadFromRenderedText( renderer, gFont, "New Game", textColorDark ) && quitButtonTextLight.loadFromRenderedText( renderer, gFont, "Quit", textColorLight ) && loadButtonTextLight.loadFromRenderedText( renderer, gFont, "Load", textColorLight ) && playButtonTextLight.loadFromRenderedText( renderer, gFont, "New Game", textColorLight ))) {
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

//frees media and shuts down SDL
void close() {
	//Free loaded images
	buttonSpriteSheetTexture.free();
	backgroundTexture.free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	
}

//-------------SAVE AND LOAD FUNCTIONS:------------------


//save a game to save1.txt
void saveGame() {
	// Create and open a text file
	ofstream MyFile("../saves/save1.txt");

	// Write to the file
	MyFile << gameState.gameStateToString();

	// Close the file
	MyFile.close();
}

std::vector<std::string> splitList(string list){
	std::stringstream test (list);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, '-'))
	{
		seglist.push_back(segment);
	}

	return seglist;
}

//takes in a string and converts it to a gameState object
void loadState(string saveData){

	std::stringstream test (saveData);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, ','))
	{
		seglist.push_back(segment);
	}

	gameState.setName(seglist[0]);
	gameState.setLocation(seglist[1]);
	gameState.setDate(seglist[2]);
	gameState.setTime(seglist[3]);
	gameState.setAffection(splitList(seglist[4]));
	gameState.setSwitches(splitList(seglist[5]));

	gameState.setInventory(splitList(seglist[6]));
	gameState.setState(seglist[7]);
	gameState.setTypeData(splitList(seglist[8]));
}

//load the game in a specified save file
void loadGame(int save) {
	string saveData;
	string fileName = "../saves/save1.txt";

	if(save==2){
		fileName = "../saves/save2.txt";
	}else if(save==3){
		fileName = "../saves/save3.txt";
	}

	// Read from the text file
	ifstream MyReadFile(fileName);

	// Use a while loop together with the getline() function to read the file line by line
	while (getline (MyReadFile, saveData)) {
		// Output the text from the file
		loadState(saveData);
	}

	// Close the file
	MyReadFile.close();

}

LTexture getSaveData(int save){
	LTexture savedata;
	loadGame(save);
	gameState.setState("load");

	string saveDataString = "Name: " + gameState.getGameData().getName() +" Location: "+gameState.getGameData().getLocation()+"Day: "+gameState.getGameData().getDate();

	//Render text
	SDL_Color textColorLight = { 255, 255, 255 };
	if (! (savedata.loadFromRenderedText( renderer, gFont, saveDataString, textColorLight ))){
		printf( "Failed to render text texture!\n" );
	}

	return savedata;
	
}

//----------------PLOT CALCULATION METHODS:----------------------

//return a texture of the text that should be on screen
//UNFINISHED
LTexture getText(int date, int time, string location){
	LTexture dialogueTexture;
	string dialogueText = "";

	//intro scene
	if(date==0){
		if(time==0){
			dialogueText = "this is example dialogue";
		}
	}

	//day 1
	if(date==1){
		if(time==0){

		}
	}

	
	//Render text
	SDL_Color textColorDark = { 0, 0, 0 };
	SDL_Color textColorLight = { 255, 255, 255 };
	if (! (dialogueTexture.loadFromRenderedText( renderer, gFont, dialogueText, textColorDark ))){
		printf( "Failed to render text texture!\n" );
	}

	return dialogueTexture;
	
}

//----------------MAIN GAME LOOP:-------------------------------

int main( int argc, char* args[] ) {
	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		//Load media
		if ( !loadMedia() ) {
			printf( "Failed to load media!\n" );
		}
		else {	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit) {
				
				
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 ) {
					//User requests quit
					if ((e.type == SDL_QUIT) || (gameState.getState().compare("quit")==0)) {
						quit = true;
						//std::cout << gameState.gameStateToString() << "\n";
						//saveGame();
						//loadGame();
						//std::cout << gameState.getGameData().getName() << "\n";
						
					}

					//Handle button events
					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {
						char* result = gButtons[ i ].handleEvent( &e, BUTTON_WIDTH, BUTTON_HEIGHT);

						if(strcmp(result,"play")!=0){
							gameState.setState(result);
							
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );

				
				

				if (gameState.getState().compare("menu") == 0) {

					//Render background texture to screen
					backgroundTexture.render( renderer, 0, 0, 1);
					
					//Render buttons
					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {

						SDL_Point mPosition = gButtons[i].getPos();

						buttonSpriteSheetTexture.render( renderer, mPosition.x, mPosition.y, 2, &gSpriteClips[ gButtons[i].getCurrentSprite()]);
					}
					
					//put text on play button
					if(gButtons[0].getCurrentSprite()==BUTTON_SPRITE_MOUSE_OUT){
						playButtonTextDark.render( renderer, ((BUTTON_WIDTH - playButtonTextDark.getWidth() ) / 2) , (( BUTTON_HEIGHT- playButtonTextDark.getHeight() ) / 2), 0);
					}else{
						playButtonTextLight.render( renderer, ((BUTTON_WIDTH - playButtonTextDark.getWidth() ) / 2) , (( BUTTON_HEIGHT- playButtonTextDark.getHeight() ) / 2), 0);

					}

					//put text on load button
					if(gButtons[1].getCurrentSprite()==BUTTON_SPRITE_MOUSE_OUT){
						loadButtonTextDark.render( renderer, ((BUTTON_WIDTH - loadButtonTextDark.getWidth() ) / 2) , ((WINDOW_HEIGHT - ((BUTTON_HEIGHT*2) + 50)) + ((( BUTTON_HEIGHT- loadButtonTextDark.getHeight() ) / 2))), 0);
					}else{
						loadButtonTextLight.render( renderer, ((BUTTON_WIDTH - loadButtonTextDark.getWidth() ) / 2) , ((WINDOW_HEIGHT - ((BUTTON_HEIGHT*2) + 50)) + ((( BUTTON_HEIGHT- loadButtonTextDark.getHeight() ) / 2))), 0);
					}

					//put text on quit button
					if(gButtons[2].getCurrentSprite()==BUTTON_SPRITE_MOUSE_OUT){
						quitButtonTextDark.render( renderer, ((BUTTON_WIDTH - quitButtonTextDark.getWidth() ) / 2) , ((WINDOW_HEIGHT - BUTTON_HEIGHT )+ (( BUTTON_HEIGHT- quitButtonTextDark.getHeight() ) / 2)), 0);
					}else{
						quitButtonTextLight.render( renderer, ((BUTTON_WIDTH - quitButtonTextDark.getWidth() ) / 2) , ((WINDOW_HEIGHT - BUTTON_HEIGHT )+ (( BUTTON_HEIGHT- quitButtonTextDark.getHeight() ) / 2)), 0);
					}
					

				}
				else{
					//set buttons as inactive
					gButtons[0].setID("INACTIVE");
					gButtons[1].setID("INACTIVE");
					gButtons[2].setID("INACTIVE");
				}

				if (gameState.getState().compare("game")==0) {

					//render the current background
					/*
					
					case gameState.getGameData().getLocation():
					kitchen -> load kitchen
					main room -> load main room
					etc etc

					*/


					//render the current characters
					/*

					characters = getCharacters(date, time, location);
					
					up to 3 shown on screen

					left, middle, right

					if characters.length = 3

					character1.load(left)
					character2.load(middle)
					character3.load(right)

					if characters.length = 2

					character1.load(left)
					character2.load(right)

					if characters.length = 1

					charachert1.load(middle)

					*/


					//add dialogue on screen:

					//display box on screen
					SDL_Rect dialogueRect;
					dialogueRect.x = (WINDOW_WIDTH-800)/2;
					dialogueRect.y = (WINDOW_HEIGHT-200);
					dialogueRect.w = 800;
					dialogueRect.h = 200;
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					SDL_RenderFillRect(renderer, &dialogueRect);

					//get text to display
					LTexture dialogueText = getText(0,0,"kitchen");

					//display text
					dialogueText.render( renderer, ((WINDOW_WIDTH-800)/2) , (WINDOW_HEIGHT-200), 0);



					//add buttons to screen
					/*

					add forward button
					add save button
					add options button


					*/



					//activate buttons

					

				}

				else if (gameState.getState().compare("choices")==0) {
					//load choices
				}
				
				else if (gameState.getState().compare("load")==0){
					//show background
					backgroundTexture.render( renderer, 0, 0, 1);

					//show 3 rectangles

					SDL_Rect saveRect;
					saveRect.x = (WINDOW_WIDTH-800)/2;
					saveRect.y = (WINDOW_HEIGHT-200);
					saveRect.w = 800;
					saveRect.h = 200;
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

					SDL_RenderFillRect(renderer, &saveRect);
					saveRect.y = (WINDOW_HEIGHT/3);
					SDL_RenderFillRect(renderer, &saveRect);
					saveRect.y = 0;
					SDL_RenderFillRect(renderer, &saveRect);


					//set up save buttons
					gButtons[0].setPosition( 0, 0 );
					gButtons[0].setID("SAVE1");
					
					gButtons[1].setPosition( 0, (WINDOW_HEIGHT/3) );
					gButtons[1].setID("SAVE2");

					gButtons[2].setPosition( 0, WINDOW_HEIGHT - BUTTON_HEIGHT );
					gButtons[2].setID("SAVE3");


					//Render buttons
					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {

						SDL_Point mPosition = gButtons[i].getPos();

						buttonSpriteSheetTexture.render( renderer, mPosition.x, mPosition.y, 2, &gSpriteClips[ gButtons[i].getCurrentSprite()]);
					}


					//get text for saves
					LTexture savedata1 = getSaveData(1);
					LTexture savedata2 = getSaveData(2);
					LTexture savedata3 = getSaveData(3);

					//display text
					savedata1.render( renderer, ((WINDOW_WIDTH-800)/2) , 0, 0);
					savedata2.render( renderer, ((WINDOW_WIDTH-800)/2) , (WINDOW_HEIGHT/3), 0);
					savedata3.render( renderer, ((WINDOW_WIDTH-800)/2) , (WINDOW_HEIGHT-200), 0);

					//render button text
					//light
					LTexture lightLoad;
					SDL_Color textColorDark = { 0, 0, 0 };
					SDL_Color textColorLight = { 255, 255, 255 };

					if (! (lightLoad.loadFromRenderedText( renderer, gFont, "Load", textColorLight ))){
						printf( "Failed to render text texture!\n" );
					}

					//dark
					LTexture darkLoad;

					if (! (darkLoad.loadFromRenderedText( renderer, gFont, "Load", textColorDark))){
						printf( "Failed to render text texture!\n" );
					}

					//display text
					

					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {

						SDL_Point mPosition = gButtons[i].getPos();
						if(gButtons[i].getCurrentSprite()==BUTTON_SPRITE_MOUSE_OUT){
							darkLoad.render( renderer, mPosition.x, mPosition.y, 0);
						}else{
							lightLoad.render( renderer, mPosition.x, mPosition.y, 0);
						}

						
					}




				}else if (gameState.getState().compare("save1")==0){
					loadGame(1);
				}else if (gameState.getState().compare("save1")==0){
					loadGame(2);
				}else if (gameState.getState().compare("save1")==0){
					loadGame(3);
				}

				//Update screen
				SDL_RenderPresent(renderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}