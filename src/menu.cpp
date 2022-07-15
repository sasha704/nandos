/* Display a "main menu" to the user*/

#include <SDL.h>
#include <stdio.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//the window
SDL_Window* window = NULL;

//the surface contained by the window
SDL_Surface* screenSurface = NULL;

//The menu image
SDL_Surface* menuImage = NULL;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Create window
		window = SDL_CreateWindow("Working Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		} else {
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return success;
}

bool loadMedia() {
	//Loading success flag
	bool success = true;

	//Load bitmap image (the menu background)
	menuImage = SDL_LoadBMP("../images/menu.bmp");

	if (menuImage == NULL) {
		printf("Unable to load menu image %s! SDL Error: %s\n", "../images/menu.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

/**
 * @brief deallocate memory when closing program
 * 
 */
void close() {
	//Deallocate surface
	SDL_FreeSurface(menuImage);
	menuImage = NULL;

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[]) {

	//display message to console
	printf("Menu opened.\n");

	//try to create window
	if(!init()) {
		printf("Failed to initialize!\n");
	} else {
		//Load media
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			//Apply the image
			SDL_BlitSurface(menuImage, NULL, screenSurface, NULL);
			
			//Update the surface
			SDL_UpdateWindowSurface(window);

			//Wait 4 seconds
			SDL_Delay(4000);
		}
	}

	//Free resources and close SDL
	close();

	//display message to console
	printf("Menu closed.");

	return 0;
}