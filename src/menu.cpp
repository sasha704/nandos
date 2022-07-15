/* Display a "main menu" to the user*/

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>

// Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 480;

// the window
SDL_Window *window = NULL;

// the window renderer
SDL_Renderer *windowRenderer = NULL;

// the texture being displayed
SDL_Texture *currentTexture = NULL;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image as a texture (the menu background)
SDL_Texture *loadTexture(std::string path);

bool init() {
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{

		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		// create window
		window = SDL_CreateWindow("Working Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create renderer for window
			windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (windowRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// set renderer colour
				SDL_SetRenderDrawColor(windowRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// load the png
				int imgFlags = IMG_INIT_PNG;

				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				
			}
		}
	}

	return success;
}

bool loadMedia() {
	// Loading success flag
	bool success = true;

	// Load menu image as texture
	currentTexture = loadTexture("../images/menu2.png");

	if (currentTexture == NULL)
	{
		printf("Failed to load menu image as texture!\n");
		success = false;
	}

	return success;
}

/**
 * @brief deallocate memory when closing program
 *
 */
void close() {
	//free loaded image
	SDL_DestroyTexture(currentTexture);
	currentTexture = NULL;

	//destroy window
	SDL_DestroyRenderer(windowRenderer);
	SDL_DestroyWindow(window);
	window = NULL;
	windowRenderer = NULL;

	//quit SDL subsystems
	SDL_Quit();
	IMG_Quit();
}

SDL_Texture* loadTexture(std::string path) {
	//the final texture
	SDL_Texture* newTexture = NULL;

	//load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		//create texture from surface
		newTexture = SDL_CreateTextureFromSurface(windowRenderer, loadedSurface);
		
		if (newTexture == NULL)
		{
			printf("Unable to create texture from image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//free old surface once loaded
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int main(int argc, char *args[])
{

	// display message to console
	printf("Menu opened.\n");

	// try to create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		// Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			// Main loop flag
			bool quit = false;

			// Event handler
			SDL_Event e;

			// While application is running
			while (!quit)
			{
				//handle queued events
				while (SDL_PollEvent(&e) != 0)
				{
					//if user tries to quit using window x
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				//clear screen
				SDL_RenderClear(windowRenderer);

				//render texture to screen
				SDL_RenderCopy(windowRenderer, currentTexture, NULL, NULL);

				//update screen (display new texture)
				SDL_RenderPresent(windowRenderer);
			}
		}
	}

	// Free resources and close SDL
	close();

	// display message to console
	printf("Menu closed.");

	return 0;
}