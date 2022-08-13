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
GameState gameState = GameState();

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
		gButtons[1].setID("SAVELOAD");

		gButtons[2].setPosition( 0, WINDOW_HEIGHT - BUTTON_HEIGHT );
		gButtons[2].setID("QUIT");

	}

	//Open the font
	gFont = TTF_OpenFont( "../fonts/SeaweedScript-Regular.ttf", 28 );
	if (gFont == NULL) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
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

//save a game to save1.txt
void saveGame() {
	// Create and open a text file
	ofstream MyFile("../saves/save1.txt");

	// Write to the file
	MyFile << gameState.gameStateToString();

	// Close the file
	MyFile.close();
}

//takes in a string and converts it to a gameState object
void loadState(string saveData){
	//
	

	std::stringstream test (saveData);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, ','))
	{
		seglist.push_back(segment);
	}

	
	for (string i: seglist) {
		std::cout << i << ' ';
	}

	gameState.getGameData().setName(seglist[0]);
	gameState.getGameData().setLocation(seglist[1]);
	gameState.getGameData().setDate(seglist[2]);
	gameState.getGameData().setTime(seglist[3]);

	//gameState.getGameData().setAffection(seglist[4]);

	//gameState.getGameData().setSwitches(seglist[5]);

	//gameState.getGameData().setInventory(seglist[6]);

	gameState.setState(seglist[7]);

	//gameState.setTypeData(seglist[8]);
}

//load the game in save1.txt
void loadGame() {
	string saveData;

	// Read from the text file
	ifstream MyReadFile("../saves/save1.txt");

	// Use a while loop together with the getline() function to read the file line by line
	while (getline (MyReadFile, saveData)) {
		// Output the text from the file
		loadState(saveData);
	}

	// Close the file
	MyReadFile.close();

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
						loadGame();
						
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

				//Render background texture to screen
				backgroundTexture.render( renderer, 0, 0, 1);
				

				if (gameState.getState().compare("menu") == 0) {
					
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

					//printf("flag1");

					//add buttons to screen

					//activate buttons

					//add any applicable characters on screen

					//add dialogue on screen

				}
				
				if (gameState.getState().compare("loadSave")==0){
					//read all 3 files as strings

					//interpret string as 
					
					
					//show the load/save screen
					
					//list all files in the saves folder

					//allow user to select one to load

					//open and read save files


					//list names on screen
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