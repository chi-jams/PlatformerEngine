
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <set>
#include "SDL.h"
#include "SDL_image.h"
#include "texture.h"
#include "animation.h"
#include "constants.h"
#include "world.h"
#include "Player.h"


const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

enum gameState { TITLE_SCREEN,
                 IN_GAME,
                 LOADING_SCREEN };

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

SDL_Surface* loadSurface( std::string path );

SDL_Texture* loadTexture( std::string path );


///////////////////
// Main function //
///////////////////

// All keyboard input
const Uint8* currentKeyStates;

//The window we'll be rendering to
SDL_Window* mainWindow = NULL;
	
//The surface contained by the window
SDL_Surface* mainWindowSurface = NULL;

// Renders things for mainWindow
SDL_Renderer* mainRenderer = NULL;

Texture* tileTextures = NULL;

void drawEverything( Player* player, World* level, int cameraX = 0, int cameraY = 0 );

void drawWorld( World* level, int cameraX = 0, int cameraY = 0 );

// Items for drawing the player.
enum playerTextures { PLAYER_IDLE,
                      PLAYER_WALKING,
                      NUM_PLAYER_ANIMATIONS
                      };

Texture* playerSprites = NULL;
Animation* playerAnimations = NULL;

void drawPlayer( Player* player, int cameraX = 0, int cameraY = 0 );

// For debugging purposes, draws all of the hitboxes over the tiles.
void drawHitBoxes( World* level, int cameraX = 0, int cameraY = 0 );


unsigned int curTime = 0;

unsigned int lastTime = 0;

unsigned int deltaTime = 0;

int main( int argc, char* args[] )
{
    if( !init() )
    {
        std::cout<< "Failed to initialize! SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        if( !loadMedia() )
        {
            std::cout << "failed to load resources! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            // Whether or not the user wants to get out of the program.
            bool quit = false;

            gameState curState = IN_GAME;

			std::string loadedLevel = "Levels/level7.txt";

            World* newWorld = new World( loadedLevel );

            int playerStartX = newWorld->getPlayerStartX();
            int playerStartY = newWorld->getPlayerStartY();
            Player* player = new Player( playerStartX, playerStartY );

            int moveDir[ NUM_DIRECTIONS ];
                moveDir[ UP ] = STOPPED;
                moveDir[ DOWN ] = STOPPED;
                moveDir[ LEFT ] = STOPPED;
                moveDir[ RIGHT ] = STOPPED;
 
            int camX = 0,
                camY = 0,
                camSpeed = 10;

            ////////////////////////////////////////
            // This is where the game loop starts //
            ////////////////////////////////////////
            while( !quit )
            {
                SDL_Event e;

                // Handle all joystick, gamepad, keyboard stuff before actually
                // doing any updating in the game.
                while( SDL_PollEvent( &e ) != 0 )
                {
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                currentKeyStates = SDL_GetKeyboardState( NULL );

                moveDir[ UP ] = currentKeyStates[ SDL_SCANCODE_W ] || currentKeyStates[ SDL_SCANCODE_SPACE ];
                moveDir[ DOWN ] = currentKeyStates[ SDL_SCANCODE_S ];
                moveDir[ LEFT ] = currentKeyStates[ SDL_SCANCODE_A ];
                moveDir[ RIGHT ] = currentKeyStates[ SDL_SCANCODE_D ];
                /*
                if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
                {
                    camX += camSpeed;
                }
                else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
                {
                    camX -= camSpeed;
                }
                
                if( currentKeyStates[ SDL_SCANCODE_UP ] )
                {
                    camY -= camSpeed;
                }
                else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
                {
                    camY += camSpeed;
                }
                */
                //camX += camSpeed * moveDir[ X ];
                //camY += camSpeed * moveDir[ Y ];

                ////////////////////////////////////////////////////
                // Put all game updating related stuff below here //
                ////////////////////////////////////////////////////

                curTime = (int)clock();

                //player->updateRect();
                player->movePlayer( moveDir, curTime );
                player->setMapX( player->getXPos() / newWorld->getTileSize() );
                player->setMapY( player->getYPos() / newWorld->getTileSize() );
                
                /*
                for( int i = 0; i < newWorld->getNumHitboxes(); i++ )
                {
                    direction outDir;
                    if( ( outDir = player->collidesNoOverlap( newWorld->getHitboxRect(i) ) ) == DOWN )
                        player->setOnGround();

                    std::string output;

                    switch( outDir )
                    {
                        case NONE:
                            output = "NONE";
                            break;
                        case UP:
                            output = "UP";
                            break;
                        case DOWN:
                            output = "DOWN";
                            break;
                        case LEFT:
                            output = "LEFT";
                            break;
                        case RIGHT:
                            output = "RIGHT";
                            break;
                    }
                    std::cout << output << std::endl;
                }
                */
                //std::cout << player->getMapX() << " " << player->getMapY() << std::endl;
                
                // We're going to check a set of tiles 3 wide and 4 tall for the player.
                newWorld->worldCollisionsWith( player, 2, 3 );

                //////////////////////////////////////////////
                // Put all drawing related stuff below here //
                //////////////////////////////////////////////

                SDL_RenderClear( mainRenderer );                
                
                camX = player->getXPos() + player->getWidth() / 2;
                camY = player->getYPos() + player->getHeight() / 2;
                
                drawEverything( player, newWorld, camX, camY );

                SDL_RenderPresent( mainRenderer );
                /* For when this is made into a proper game
                if( curState == TITLE_SCREEN )
                {

                }
                else if( curState = IN_GAME )
                {

                

                }
                */

            }
        }
    }

    close();

    return 0;
}


////////////////////////
// Stuff for the Game //
////////////////////////

void drawEverything( Player* player, World* level, int camX, int camY )
{   
    int tileSize = level->getTileSize();

    // Make the cam position based off of the center of the screen,
    // rather than the top-left corner.
    camX -= SCREEN_WIDTH / 2;
    camY -= SCREEN_HEIGHT / 2;
  
    // If the level is narrower than the screen, then center the camera along
    // the y axis
    if( SCREEN_WIDTH >= level->getWidth() * tileSize )
        camX = ( level->getWidth() * tileSize ) / 2 - SCREEN_WIDTH / 2 ;
    // Avoid showing unnesessary black area, only go to the egdes of the map,
    // with a small extra margin, the size of one tile.
    else if( camX < -tileSize )
        camX = -tileSize;
    else if( camX + SCREEN_WIDTH - tileSize > level->getWidth() * tileSize )
        camX =  level->getWidth() * tileSize - SCREEN_WIDTH + tileSize;

    if( SCREEN_HEIGHT >= level->getHeight() * tileSize )
        camY = ( level->getHeight() * tileSize ) / 2 - SCREEN_HEIGHT / 2 ;
    else if( camY < -tileSize )
        camY = -tileSize;
    else if( camY + SCREEN_HEIGHT - tileSize > level->getHeight() * tileSize )
        camY =  level->getHeight() * tileSize - SCREEN_HEIGHT + tileSize;
    
    //std::cout << camX << " " << camY << std::endl;

    drawWorld( level, camX, camY );
    drawHitBoxes( level, camX, camY );
    drawPlayer( player, camX, camY );
}

void drawWorld( World* level, int camX, int camY )
{
    int tileSize = level->getTileSize();

    for( int y = 0; y < level->getHeight(); y++ )
    {   
        for( int x = 0; x < level->getWidth(); x++ )
        {
            int curTile = level->getTile( x, y )->getTileType();

            SDL_Rect outputTile;
            outputTile.x = curTile * tileSize;
            outputTile.y = 0;
            outputTile.w = tileSize;
            outputTile.h = tileSize;

            tileTextures->render( x * tileSize - camX, y * tileSize - camY, &outputTile );
        }
    }
}

void drawPlayer( Player* player, int camX, int camY )
{
    /*
    SDL_Rect outputSprite;
    
    // The width and height of the sprites will always be the same.
    outputSprite.w = player->getPlayerSize();
    outputSprite.h = player->getPlayerSize() * 2;
    */

    // The location on the screen that the player will be drawn at.
    int renderX = player->getXPos() - camX,
        renderY = player->getYPos() - camY;

    if( player->getFacing() == RIGHT )
    {
        if( player->getXVel() == 0 )
        {
            playerAnimations[PLAYER_IDLE].render( renderX, renderY );
        }
        else if( player->getXVel() > 0 )
        {
        
            playerAnimations[PLAYER_WALKING].render( renderX, renderY );
        
        }
    }
    else
    {
        if( player->getXVel() == 0 )
        {
            playerAnimations[PLAYER_IDLE].render( renderX, renderY, SDL_FLIP_HORIZONTAL );
        }
        else if( player->getXVel() < 0 )
        {
        
            playerAnimations[PLAYER_WALKING].render( renderX, renderY, SDL_FLIP_HORIZONTAL );
        }
    }

    
}

void drawHitBoxes( World* level, int camX, int camY ) {

    int tileSize = level->getTileSize();

    SDL_SetRenderDrawColor( mainRenderer, 0x39, 0xFF, 0x14, 0xFF );

    for( int i = 0; i < level->getNumHitboxes(); i++ )
    {
        SDL_Rect outputRect;
        outputRect.x = level->getHitboxRect(i)->getXPos() - camX;
        outputRect.y = level->getHitboxRect(i)->getYPos() - camY;
        outputRect.w = level->getHitboxRect(i)->getWidth();
        outputRect.h = level->getHitboxRect(i)->getHeight();

        SDL_RenderDrawRect( mainRenderer, &outputRect );
    }
    
    SDL_SetRenderDrawColor( mainRenderer, 0, 0, 0, 0xFF );
}

/////////////////////////
// Universal functions //
/////////////////////////

SDL_Surface* loadSurface( std::string path )
{
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        std::cout<< "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        optimizedSurface = SDL_ConvertSurface( loadedSurface, mainWindowSurface->format, NULL );
        if( optimizedSurface == NULL )
        {
            std::cout<< "Unable to optimize image! SDL Error: " << SDL_GetError() << std::endl;
        }

        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

SDL_Texture* loadTexture( std::string path )
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        std::cout<< "Failed to load surface! SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface( mainRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            std::cout<< "Failed to create texture! SDL Error: " << SDL_GetError() << std::endl;
        }

        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout<< "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		//Create window
		mainWindow = SDL_CreateWindow( "Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( mainWindow == NULL )
		{
			std::cout<< "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
            // Create a renderer
            mainRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RENDERER_ACCELERATED );
            if( mainRenderer == NULL )
            {
                std::cout<< "Failed to create renderer! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                
                SDL_SetRenderDrawColor( mainRenderer, 0, 0, 0, 0xFF );

                // Initialize image loading.
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    std::cout<< "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
                }
            }
            
            //Get window surface
            mainWindowSurface = SDL_GetWindowSurface( mainWindow );
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

    // Load all the tile textures for the environment.
    tileTextures = new Texture();
    if( !tileTextures->loadFromFile( "Assets/Tiles.png" ) )
    {
        std::cout << "Failed to load tile textures!" << std::endl;
        success = false;
    }

    // Load all the sprites for player movement.
    playerSprites = new Texture();
    if( !playerSprites->loadFromFile( "Assets/Player.png" ) )
    {
        std::cout << "Failed to load player sprites!" << std::endl;
        success = false;
    }
    playerAnimations = new Animation[ NUM_PLAYER_ANIMATIONS ];

    // Every single animation the player has will have these dimenions.
    SDL_Rect playerFrame;
    playerFrame.w = 50;
    playerFrame.h = 100;

    // Initialize the idle animation.
    playerFrame.x = 0;
    playerFrame.y = 0;
    playerAnimations[ PLAYER_IDLE ].initAnimation( playerSprites, playerFrame, 1, 0 );

    // Initialize the walking animation.
    playerFrame.x = 50;
    playerFrame.y = 0;
    playerAnimations[ PLAYER_WALKING ].initAnimation( playerSprites, playerFrame, 4, 100 );

	return success;
}

void close()
{

	//Destroy window, and the renderer for the window
    SDL_DestroyRenderer( mainRenderer );
	SDL_DestroyWindow( mainWindow );
    mainRenderer = NULL;
	mainWindow = NULL;

	//Quit SDL subsystems
    IMG_Quit();
	SDL_Quit();
}