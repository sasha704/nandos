#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
//#include "texture.hpp"
#include "button.hpp"

//size of window
const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 768;

//button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 100;
const int TOTAL_BUTTONS = 3;

//so button is clicked, and that moves the game onto the next state

/*
 game state where a->b->c -> (d1||d2)
 d1 -> e1
 d2 -> e2

 game state:
 - location
 - action moment/ disalogue moment
 - options/ diaglogue
 - characters on screen and sprites used for them

 greater game state:
 - which menu is being shown/if the game is running


*/








//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;



//global gamestate
char* gameState = "menu";

//menu sprite textures
LTexture buttonSpriteSheetTexture;

//menu texture
LTexture backgroundTexture;

//Button text
LTexture quitButtonText;
LTexture playButtonText;
LTexture loadButtonText;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ]; 

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
	if ( !buttonSpriteSheetTexture.loadFromFile( renderer, "../images/newButton.png" ) ) {
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else {

		//Load background texture
		if( !backgroundTexture.loadFromFile( renderer, "../images/menu.png" ) ) {
			printf( "Failed to load background texture image!\n" );
			success = false;
		}
		
		

		//Set button positions
		gButtons[0].setPosition( 0, 0 );
		gButtons[0].setID("PLAY");
		
		gButtons[ 1 ].setPosition( 0, WINDOW_HEIGHT - ((BUTTON_HEIGHT * 2) + 50) );
		gButtons[ 2 ].setPosition( 0, WINDOW_HEIGHT - BUTTON_HEIGHT );
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
		SDL_Color textColor = { 0, 0, 0 };
		if (! (quitButtonText.loadFromRenderedText( renderer, gFont, "Quit", textColor ) && loadButtonText.loadFromRenderedText( renderer, gFont, "Load", textColor ) && playButtonText.loadFromRenderedText( renderer, gFont, "New Game", textColor ))) {
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
					if ((e.type == SDL_QUIT) || (strcmp(gameState,"quit")==0)) {
						quit = true;
					}

					//Handle button events
					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {
						char* result = gButtons[ i ].handleEvent( &e, BUTTON_WIDTH, BUTTON_HEIGHT);

						if(strcmp(result,"play")!=0){
							gameState = result;
							
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );

				//Render background texture to screen
				backgroundTexture.render( renderer, 0, 0, 1);
				

				if (strcmp(gameState, "menu") == 0) {
					
					//Render buttons
					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {
						gButtons[ i ].render(renderer, buttonSpriteSheetTexture);
					}
					
					

					//Render current frame
					quitButtonText.render( renderer, (0+( (BUTTON_WIDTH - quitButtonText.getWidth() ) / 2)) , (WINDOW_HEIGHT - (( BUTTON_HEIGHT- quitButtonText.getHeight() ) / 2)),0);
					loadButtonText.render( renderer, (0+( (BUTTON_WIDTH - loadButtonText.getWidth() ) / 2)) , (WINDOW_HEIGHT - ((( BUTTON_HEIGHT- loadButtonText.getHeight() ) / 2) + (BUTTON_HEIGHT) + 100)),0 );
					playButtonText.render( renderer, (0+( (BUTTON_WIDTH - playButtonText.getWidth() ) / 2)) , (0 + (( BUTTON_HEIGHT- playButtonText.getHeight() ) / 2)),0 );


				}
				else{
					//set buttons as inactive
					//gButtons[0].setID("INACTIVE");
					//gButtons[2].setID("INACTIVE");
					printf("flag2");
				}

				if (gameState == "game") {

					printf("flag1");

					//add buttons to screen

					//activate buttons

					//add any applicable characters on screen

					//add dialogue on screen

				}
				
				//if (gameState == "pause"){}
				//if (gameState == "settings"){}
				//if (gameState == "load_save"){}

				//Update screen
				SDL_RenderPresent(renderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}