#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>

//size of window
const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 768;

//button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 100;
const int TOTAL_BUTTONS = 3;


enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		
		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, int background, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//buttons
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e );
	
		//Shows button sprite
		void render();

		//set the id of a button
		void setID(char* a);

		//return the id of a button
		char* getID();

	private:
		//Top left position
		SDL_Point mPosition;

		//the id of the button
		char* buttonID;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Mouse button sprites
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];

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

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}


bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}




void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, int background, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (background==1){
		renderQuad = { x, y, WINDOW_WIDTH, WINDOW_HEIGHT };
	}
	

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	buttonID = "name";

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::setID(char* x)
{
	buttonID = x;
}

char* LButton::getID(){
	return buttonID;
}

void LButton::handleEvent( SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				if (buttonID == "QUIT"){
					exit(4);
				}

				if (buttonID == "PLAY"){
					gameState = "game";
				}
				break;
				
				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}
	
void LButton::render()
{
	//Show current button sprite
	buttonSpriteSheetTexture.render( mPosition.x, mPosition.y, 0, &gSpriteClips[ mCurrentSprite ]);
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		window = SDL_CreateWindow( "Extra Spicy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia() {
	//Loading success flag
	bool success = true;

	//Load sprites
	if( !buttonSpriteSheetTexture.loadFromFile( "../images/newButton.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		

		//Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
		{
			gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i * 101;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
		}

		//Load background texture
		if( !backgroundTexture.loadFromFile( "../images/menu.png" ) )
		{
			printf( "Failed to load background texture image!\n" );
			success = false;
		}
		
		

		//Set button positions
		gButtons[ 0 ].setPosition( 0, 0 );
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
		if (! (quitButtonText.loadFromRenderedText( "Quit", textColor ) && loadButtonText.loadFromRenderedText( "Load", textColor ) && playButtonText.loadFromRenderedText( "New Game", textColor ))) {
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
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


int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit) {
				
				
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle button events
					for( int i = 0; i < TOTAL_BUTTONS; ++i )
					{
						gButtons[ i ].handleEvent( &e );
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );

				//Render background texture to screen
				backgroundTexture.render( 0, 0, 1);

				if (gameState=="menu") {
					
					//Render buttons
					for( int i = 0; i < TOTAL_BUTTONS; ++i )
					{
						gButtons[ i ].render();
					}

					//Render current frame
					quitButtonText.render( (0+( (BUTTON_WIDTH - quitButtonText.getWidth() ) / 2)) , (WINDOW_HEIGHT - (( BUTTON_HEIGHT- quitButtonText.getHeight() ) / 2)),0);
					loadButtonText.render( (0+( (BUTTON_WIDTH - loadButtonText.getWidth() ) / 2)) , (WINDOW_HEIGHT - ((( BUTTON_HEIGHT- loadButtonText.getHeight() ) / 2) + (BUTTON_HEIGHT) + 100)),0 );
					playButtonText.render( (0+( (BUTTON_WIDTH - playButtonText.getWidth() ) / 2)) , (0 + (( BUTTON_HEIGHT- playButtonText.getHeight() ) / 2)),0 );

				}
				else{
					//set buttons as inactive
					gButtons[0].setID("INACTIVE");
					gButtons[2].setID("INACTIVE");
				}

				if (gameState == "game") {


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