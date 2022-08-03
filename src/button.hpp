#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "texture.hpp"

enum LButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	//BUTTON_SPRITE_MOUSE_DOWN = 2,
	//BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 2
};

//buttons
class LButton {
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		//get position of top left corner
		SDL_Point getPos();

		//Handles mouse event
		char* handleEvent( SDL_Event* e, int BUTTON_WIDTH, int BUTTON_HEIGHT );

		//set the id of a button
		void setID(char* a);

		//return the id of a button
		char* getID();

		//get current sprite
		LButtonSprite getCurrentSprite();

	private:
		//Top left position
		SDL_Point mPosition;

		//the id of the button
		char* buttonID;

		//Currently used sprite
		LButtonSprite mCurrentSprite;

		//Mouse button sprites
		SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
};

