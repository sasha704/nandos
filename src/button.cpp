#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "button.hpp"

LButton::LButton() {
	mPosition.x = 0;
	mPosition.y = 0;
	buttonID = "name";

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;

	//Set sprites
	for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i ) {
		gSpriteClips[ i ].x = 0;
		gSpriteClips[ i ].y = i * 1250;
		gSpriteClips[ i ].w = 300;
		gSpriteClips[ i ].h = 100;;
	}
}

void LButton::setPosition( int x, int y ) {
	mPosition.x = x;
	mPosition.y = y;
}

SDL_Point LButton::getPos(){
	return mPosition;
}

void LButton::setID(char* x) {
	buttonID = x;
}

char* LButton::getID(){
	return buttonID;
}

LButtonSprite LButton::getCurrentSprite(){
	return mCurrentSprite;
}

char* LButton::handleEvent( SDL_Event* e, int BUTTON_WIDTH, int BUTTON_HEIGHT ) {
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ) {
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x ) {
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH ) {
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y ) {
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT ) {
			inside = false;
		}

		//Mouse is outside button
		if ( !inside ) {
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else {
			//Set mouse over sprite
			switch( e->type ) {
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				
				if (strcmp(buttonID,"QUIT")==0) {
					return "quit";
				}else if (strcmp(buttonID,"PLAY")==0){
					return "game";

				}else if (strcmp(buttonID,"LOAD")==0){
					return "load";
				}else if (strcmp(buttonID,"LOAD1")==0){
					return "load1";
				}else if (strcmp(buttonID,"LOAD2")==0){
					return "load2";
				}else if (strcmp(buttonID,"LOAD3")==0){
					return "load3";
				}else if (strcmp(buttonID,"NEXT")==0){
					return "increment 1";
				}else if (strcmp(buttonID,"SAVE")==0){
					return "save";
				}else if (strcmp(buttonID,"SAVE1")==0){
					return "save1";
				}else if (strcmp(buttonID,"SAVE2")==0){
					return "save2";
				}else if (strcmp(buttonID,"SAVE3")==0){
					return "save3";
				}
				
				
				break;
				
				case SDL_MOUSEBUTTONUP:
				break;
			}
		}
	}
	return "play";
}
	