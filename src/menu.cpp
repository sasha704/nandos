/*THE GAME*/

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "button.hpp"
#include "gameState.hpp"
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

//--------------CHARACTER SPRITES:--------------------------

LTexture characters;
SDL_Rect charSprites[ 6 ];
//mash, wrap, nata, fgc, chips, yog

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

		if(!characters.loadFromFile(renderer, "../images/characters.png")){
			printf("failed to load neutral character sprites");
			success = false;
		}

		//Set mash
		charSprites[ 0 ].x =   0;
		charSprites[ 0 ].y =   0;
		charSprites[ 0 ].w = 512;
		charSprites[ 0 ].h = 820;

		//Set wrap
		charSprites[ 1 ].x = 512;
		charSprites[ 1 ].y =   0;
		charSprites[ 1 ].w = 654;
		charSprites[ 1 ].h = 820;
		
		//Set nata
		charSprites[ 2 ].x = 1166;
		charSprites[ 2 ].y = 0;
		charSprites[ 2 ].w = 656;
		charSprites[ 2 ].h = 820;

		//Set fgc
		charSprites[ 3 ].x = 1822;
		charSprites[ 3 ].y =   0;
		charSprites[ 3 ].w = 635;
		charSprites[ 3 ].h = 820;

		//Set chips
		charSprites[ 4 ].x = 2457;
		charSprites[ 4 ].y =   0;
		charSprites[ 4 ].w = 454;
		charSprites[ 4 ].h = 820;
		
		//Set yog
		charSprites[ 5 ].x = 2911;
		charSprites[ 5 ].y = 0;
		charSprites[ 5 ].w = 570;
		charSprites[ 5 ].h = 820;
		
		//Set sprites for buttons
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
void saveGame(int file) {
	string fileName = "../saves/save1.txt";

	if(file==1){
		fileName = "../saves/save1.txt";
	}else if (file==2){
		fileName = "../saves/save2.txt";
	}else if( file==3){
		fileName = "../saves/save3.txt";
	}

	// Create and open a text file
	ofstream MyFile(fileName);

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
GameState loadState(string saveData){

	GameState tempState;

	std::stringstream test (saveData);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, ','))
	{
		seglist.push_back(segment);
	}

	tempState.setName(seglist[0]);
	tempState.setLocation(seglist[1]);
	tempState.setDate(seglist[2]);
	tempState.setTime(seglist[3]);
	tempState.setAffection(splitList(seglist[4]));
	tempState.setSwitches(splitList(seglist[5]));

	tempState.setInventory(splitList(seglist[6]));
	tempState.setState(seglist[7]);
	tempState.setTypeData(splitList(seglist[8]));

	return tempState;
}

//load the game in a specified save file
GameState loadGame(int save) {
	string saveData;
	GameState tempState;
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
		tempState = loadState(saveData);
	}

	// Close the file
	MyReadFile.close();

	return tempState;

}

LTexture getSaveData(int save){
	LTexture savedata;
	loadGame(save);

	string saveDataString = "Name: " + gameState.getGameData().getName() +" Location: "+gameState.getGameData().getLocation()+"Day: "+std::to_string(gameState.getGameData().getDate());

	//Render text
	SDL_Color textColorLight = { 255, 255, 255 };
	if (! (savedata.loadFromRenderedText( renderer, gFont, saveDataString, textColorLight ))){
		printf( "Failed to render text texture!\n" );
	}

	return savedata;
	
}

LTexture getSaveDataName(int save){
	LTexture savedata;
	GameState loadedGame = loadGame(save);

	string saveDataString = "Name: " + loadedGame.getGameData().getName();

	//Render text
	SDL_Color textColorLight = { 255, 255, 255 };
	if (! (savedata.loadFromRenderedText( renderer, gFont, saveDataString, textColorLight ))){
		printf( "Failed to render text texture!\n" );
	}

	return savedata;
}

LTexture getSaveDataLocation(int save){
	LTexture savedata;
	GameState loadedGame = loadGame(save);

	string saveDataString = "Location: " + loadedGame.getGameData().getLocation();
	
	//Render text
	SDL_Color textColorLight = { 255, 255, 255 };
	if (! (savedata.loadFromRenderedText( renderer, gFont, saveDataString, textColorLight ))){
		printf( "Failed to render text texture!\n" );
	}

	return savedata;
}

LTexture getSaveDataDate(int save){
	LTexture savedata;
	GameState loadedGame = loadGame(save);

	string saveDataString = "Day: " + loadedGame.getGameData().getDate();
	
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
			dialogueText = "we saw you across the bar and we like your vibe";
		}else if(time==1){
			dialogueText = "did you know humans are naturally not monogamous";
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

//return a list of characters that appear in the current scene
std::vector<int> getCharacters(int date, int time, string location){
	//mash, wrap, nata, fgc, chips, yog
	int mash = 0;
	int wrap = 1;
	int nata = 2;
	int fgc = 3;
	int chips = 4;
	int yog = 5;

	std::vector<int> characterList;

	//intro scene
	if(date==0){
		if(time==0){
			characterList.push_back(mash);
		}else if(time==1){
			characterList.push_back(mash);
			characterList.push_back(wrap);
		}
	}

	//day 1
	if(date==1){
		if(time==0){

		}
	}

	return characterList;
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
						string increment = "increment";
						string stringResult(result);
						if(strcmp(result,"play")!=0 && stringResult.rfind(increment, 0) != 0){
							gameState.setState(result);
						}else if (stringResult.rfind("increment", 0) == 0){
							
							//split the string by spaces
							std::stringstream test (result);
							std::string segment;
							std::vector<std::string> seglist;

							while(std::getline(test, segment, ' ')) {
								seglist.push_back(segment);
							}

							//increment the time
							gameState.incrementTime(seglist[1]);

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
					
					//DISPLAY BACKGROUND:

					if (gameState.getGameData().getLocation().compare("apartment") == 0){
						//apartment.render( renderer, 0, 0, 1);
						backgroundTexture.render( renderer, 0, 0, 1);
					} else if (gameState.getGameData().getLocation().compare("inside") == 0){
						//inside.render( renderer, 0, 0, 1);
						backgroundTexture.render( renderer, 0, 0, 1);
					} else if (gameState.getGameData().getLocation().compare("kitchen") == 0){
						//kitchen.render( renderer, 0, 0, 1);
						backgroundTexture.render( renderer, 0, 0, 1);
					} else if (gameState.getGameData().getLocation().compare("bathroom") == 0){
						//bathroom.render( renderer, 0, 0, 1);
						backgroundTexture.render( renderer, 0, 0, 1);
					} else if (gameState.getGameData().getLocation().compare("storage") == 0){
						//storage.render( renderer, 0, 0, 1);
						backgroundTexture.render( renderer, 0, 0, 1);
					} else if (gameState.getGameData().getLocation().compare("office") == 0){
						//office.render( renderer, 0, 0, 1);
						backgroundTexture.render( renderer, 0, 0, 1);
					} else if (gameState.getGameData().getLocation().compare("bins") == 0){
						//bins.render( renderer, 0, 0, 1);
						backgroundTexture.render( renderer, 0, 0, 1);
					}

					//DISPLAY CHARACTERS
					
					//get all the visible characters
					std::vector<int> characterList = getCharacters(gameState.getGameData().getDate(), gameState.getGameData().getTime(), gameState.getGameData().getLocation());

					//show on screen
					if(characterList.size()==6){
						characters.render( renderer, 200, 0, 0, &charSprites[ 0 ]);
						characters.render( renderer, 300, 0, 0, &charSprites[ 1 ]);
						characters.render( renderer, 500, 0, 0, &charSprites[ 2 ]);
						characters.render( renderer, 700, 0, 0, &charSprites[ 3 ]);
						characters.render( renderer,   0, 0, 0, &charSprites[ 4 ]);
						characters.render( renderer, 800, 0, 0, &charSprites[ 5 ]);

					}else if(characterList.size()==5){
						characters.render( renderer, 200, 0, 0, &charSprites[ characterList[0] ]);
						characters.render( renderer, 300, 0, 0, &charSprites[ characterList[1] ]);
						characters.render( renderer, 500, 0, 0, &charSprites[ characterList[2] ]);
						characters.render( renderer, 700, 0, 0, &charSprites[ characterList[3] ]);
						characters.render( renderer,   0, 0, 0, &charSprites[ characterList[4] ]);

					} else if(characterList.size()==4){
						characters.render( renderer, 200, 0, 0, &charSprites[ characterList[0] ]);
						characters.render( renderer, 300, 0, 0, &charSprites[ characterList[1] ]);
						characters.render( renderer, 500, 0, 0, &charSprites[ characterList[2] ]);
						characters.render( renderer, 700, 0, 0, &charSprites[ characterList[3] ]);
						
					} else if(characterList.size()==3){
						characters.render( renderer, 200, 0, 0, &charSprites[ characterList[0] ]);
						characters.render( renderer, 300, 0, 0, &charSprites[ characterList[1] ]);
						characters.render( renderer, 500, 0, 0, &charSprites[ characterList[2] ]);
						
					} else if(characterList.size()==2){
						characters.render( renderer, 200, 0, 0, &charSprites[ characterList[0] ]);
						characters.render( renderer, 700, 0, 0, &charSprites[ characterList[1] ]);
						
					} else if(characterList.size()==1){
						characters.render( renderer, 300, 0, 0, &charSprites[ characterList[0] ]);
					}

					

					

					
					if(gameState.getChoices()==1){

					}else{

						


						//add dialogue on screen:

						//display box on screen
						SDL_Rect dialogueRect;
						dialogueRect.x = (WINDOW_WIDTH-800)/2;
						dialogueRect.y = (WINDOW_HEIGHT-200);
						dialogueRect.w = 800;
						dialogueRect.h = 200;
						SDL_SetRenderDrawColor(renderer, 220, 222, 164, 255);
						SDL_RenderFillRect(renderer, &dialogueRect);

						//get text to display
						LTexture dialogueText = getText(0,gameState.getGameData().getTime(),"kitchen");

						//display text
						dialogueText.render( renderer, ((WINDOW_WIDTH-800)/2) , (WINDOW_HEIGHT-200), 0);



						//add buttons to screen:
						
						//add forward button
						gButtons[0].setID("NEXT");
						gButtons[0].setPosition( WINDOW_WIDTH-BUTTON_WIDTH, WINDOW_HEIGHT - BUTTON_HEIGHT );
						SDL_Point mPosition = gButtons[0].getPos();
						buttonSpriteSheetTexture.render( renderer, mPosition.x, mPosition.y, 2, &gSpriteClips[ gButtons[0].getCurrentSprite()]);

						//add a save button
						gButtons[1].setID("SAVE");
						gButtons[1].setPosition( 0, WINDOW_HEIGHT - BUTTON_HEIGHT );
						mPosition = gButtons[1].getPos();
						buttonSpriteSheetTexture.render( renderer, mPosition.x, mPosition.y, 2, &gSpriteClips[ gButtons[1].getCurrentSprite()]);

						//add text to buttons
						//Render text
						SDL_Color textColorDark = { 0, 0, 0 };
						SDL_Color textColorLight = { 255, 255, 255 };
						
						LTexture darkSave;
						LTexture lightSave;
						LTexture darkNext;
						LTexture lightNext;

						if (! (((darkSave.loadFromRenderedText( renderer, gFont, "Save", textColorDark )))&& (lightSave.loadFromRenderedText( renderer, gFont, "Save", textColorLight))&& (darkNext.loadFromRenderedText( renderer, gFont, "Next", textColorDark ))&& (lightNext.loadFromRenderedText( renderer, gFont, "Next", textColorLight )))){
							printf( "Failed to render text texture!\n" );
						}

						//put text on next button
						mPosition = gButtons[0].getPos();
						if(gButtons[0].getCurrentSprite()==BUTTON_SPRITE_MOUSE_OUT){
							darkNext.render( renderer, (mPosition.x+((BUTTON_WIDTH-darkNext.getWidth())/2)), (mPosition.y+((BUTTON_HEIGHT-darkNext.getHeight())/2)), 0);
						}else{
							lightNext.render( renderer, (mPosition.x+((BUTTON_WIDTH-lightNext.getWidth())/2)), (mPosition.y+((BUTTON_HEIGHT-lightNext.getHeight())/2)), 0);
						}
						

						//put text on save button
						mPosition = gButtons[1].getPos();
						if(gButtons[1].getCurrentSprite()==BUTTON_SPRITE_MOUSE_OUT){
							darkSave.render( renderer, (mPosition.x+((BUTTON_WIDTH-darkSave.getWidth())/2)), (mPosition.y+((BUTTON_HEIGHT-darkSave.getHeight())/2)), 0);
						}else{
							lightSave.render( renderer, (mPosition.x+((BUTTON_WIDTH-darkSave.getWidth())/2)), (mPosition.y+((BUTTON_HEIGHT-darkSave.getHeight())/2)), 0);
						}





					}
					

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
					gButtons[0].setID("LOAD1");
					
					gButtons[1].setPosition( 0, (WINDOW_HEIGHT/3) );
					gButtons[1].setID("LOAD2");

					gButtons[2].setPosition( 0, WINDOW_HEIGHT - BUTTON_HEIGHT );
					gButtons[2].setID("LOAD3");


					//Render buttons
					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {

						SDL_Point mPosition = gButtons[i].getPos();

						buttonSpriteSheetTexture.render( renderer, mPosition.x, mPosition.y, 2, &gSpriteClips[ gButtons[i].getCurrentSprite()]);
					}


					//get text for saves
					LTexture savedata1Name = getSaveDataName(1);
					LTexture savedata1Location = getSaveDataLocation(1);
					LTexture savedata1Date = getSaveDataDate(1);

					LTexture savedata2Name = getSaveDataName(2);
					LTexture savedata2Location = getSaveDataLocation(2);
					LTexture savedata2Date = getSaveDataDate(2);

					LTexture savedata3Name = getSaveDataName(3);
					LTexture savedata3Location = getSaveDataLocation(3);
					LTexture savedata3Date = getSaveDataDate(3);

					//display text
					savedata1Name.render( renderer, ((WINDOW_WIDTH-700)/2) , 0, 0);
					savedata1Location.render( renderer, ((WINDOW_WIDTH-700)/2) , 70, 0);
					savedata1Date.render( renderer, ((WINDOW_WIDTH-700)/2) , 140, 0);

					savedata2Name.render( renderer, ((WINDOW_WIDTH-700)/2) , (WINDOW_HEIGHT/3), 0);
					savedata2Location.render( renderer, ((WINDOW_WIDTH-700)/2) , ((WINDOW_HEIGHT/3)+70), 0);
					savedata2Date.render( renderer, ((WINDOW_WIDTH-700)/2) , ((WINDOW_HEIGHT/3)+140), 0);

					savedata3Name.render( renderer, ((WINDOW_WIDTH-700)/2) , (WINDOW_HEIGHT-200), 0);
					savedata3Location.render( renderer, ((WINDOW_WIDTH-700)/2) , (WINDOW_HEIGHT-130), 0);
					savedata3Date.render( renderer, ((WINDOW_WIDTH-700)/2) , (WINDOW_HEIGHT-60),0);

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
							darkLoad.render( renderer, (mPosition.x+((BUTTON_WIDTH-darkLoad.getWidth())/2)), (mPosition.y+((BUTTON_HEIGHT-darkLoad.getHeight())/2)), 0);
						}else{
							lightLoad.render( renderer, (mPosition.x+((BUTTON_WIDTH-darkLoad.getWidth())/2)), (mPosition.y+((BUTTON_HEIGHT-lightLoad.getHeight())/2)), 0);
						}

						
					}




				}else if (gameState.getState().compare("load1")==0){
					gameState = loadGame(1);
				}else if (gameState.getState().compare("load2")==0){
					gameState = loadGame(2);
				}else if (gameState.getState().compare("load3")==0){
					gameState = loadGame(3);
				}else if(gameState.getState().compare("save")==0){
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
					LTexture savedata1Name = getSaveDataName(1);
					LTexture savedata1Location = getSaveDataLocation(1);
					LTexture savedata1Date = getSaveDataDate(1);

					LTexture savedata2Name = getSaveDataName(2);
					LTexture savedata2Location = getSaveDataLocation(2);
					LTexture savedata2Date = getSaveDataDate(2);

					LTexture savedata3Name = getSaveDataName(3);
					LTexture savedata3Location = getSaveDataLocation(3);
					LTexture savedata3Date = getSaveDataDate(3);

					//display text
					savedata1Name.render( renderer, ((WINDOW_WIDTH-700)/2) , 0, 0);
					savedata1Location.render( renderer, ((WINDOW_WIDTH-700)/2) , 70, 0);
					savedata1Date.render( renderer, ((WINDOW_WIDTH-700)/2) , 140, 0);

					savedata2Name.render( renderer, ((WINDOW_WIDTH-700)/2) , (WINDOW_HEIGHT/3), 0);
					savedata2Location.render( renderer, ((WINDOW_WIDTH-700)/2) , ((WINDOW_HEIGHT/3)+70), 0);
					savedata2Date.render( renderer, ((WINDOW_WIDTH-700)/2) , ((WINDOW_HEIGHT/3)+140), 0);

					savedata3Name.render( renderer, ((WINDOW_WIDTH-700)/2) , (WINDOW_HEIGHT-200), 0);
					savedata3Location.render( renderer, ((WINDOW_WIDTH-700)/2) , (WINDOW_HEIGHT-130), 0);
					savedata3Date.render( renderer, ((WINDOW_WIDTH-700)/2) , (WINDOW_HEIGHT-60),0);

					//render button text
					//light
					LTexture lightLoad;
					SDL_Color textColorDark = { 0, 0, 0 };
					SDL_Color textColorLight = { 255, 255, 255 };

					if (! (lightLoad.loadFromRenderedText( renderer, gFont, "Save", textColorLight ))){
						printf( "Failed to render text texture!\n" );
					}

					//dark
					LTexture darkLoad;

					if (! (darkLoad.loadFromRenderedText( renderer, gFont, "Save", textColorDark))){
						printf( "Failed to render text texture!\n" );
					}

					//display text
					

					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {

						SDL_Point mPosition = gButtons[i].getPos();
						if(gButtons[i].getCurrentSprite()==BUTTON_SPRITE_MOUSE_OUT){
							darkLoad.render( renderer, (mPosition.x+((BUTTON_WIDTH-darkLoad.getWidth())/2)), (mPosition.y+((BUTTON_HEIGHT-darkLoad.getHeight())/2)), 0);
						}else{
							lightLoad.render( renderer, (mPosition.x+((BUTTON_WIDTH-darkLoad.getWidth())/2)), (mPosition.y+((BUTTON_HEIGHT-lightLoad.getHeight())/2)), 0);
						}

						
					}
				}else if(gameState.getState().compare("save1")==0){
					gameState.setState("game");
					saveGame(1);
					loadGame(1);
				}else if(gameState.getState().compare("save2")==0){
					gameState.setState("game");
					saveGame(2);
					loadGame(2);
				}else if(gameState.getState().compare("save3")==0){
					gameState.setState("game");
					saveGame(3);
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