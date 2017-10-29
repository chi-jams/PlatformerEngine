
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include "world.h"


World::World()
{
    levelWidth = 0;
    levelHeight = 0;
    tileSize = 0;
    level = NULL;
}


World::World( std::string inputFile )
{
    // This stays constant. This is how big we want the Tile
    // to be when it comes to interaction. It has nothing to do
    // with the size that it'll be when it is drawn on the screen
    tileSize = 50;

    loadLevel( inputFile );
}

World::~World()
{

    // Destroy the level BAHAHAHAHAHAHA
    for( int i = 0; i < levelWidth; i++ )
    {
        delete [] level[i];
    }
    delete [] level;

}

void World::loadLevel( std::string inputFile )
{
    // Take a text file, open it.
    std::ifstream fin;

    fin.open( inputFile );
    if( fin.fail() )
    {
        std::cerr << "Error!" << inputFile << " failed to open!" << std::endl;
    }
    else
    {
        // The first two values in the file will be a levelWidth and a levelHeight.
        fin >> levelWidth;
        fin >> levelHeight;

        // After that, read in all the other values,
        // which are tiles for the level.
        level = new Tile*[levelWidth];
        for( int i = 0; i < levelWidth; i++ )
        {
            level[i] = new Tile[levelHeight];
        }
        
        for( int y = 0; y < levelHeight; y++ )
        {
            for( int x = 0; x < levelWidth; x++ )
            {
                int input;
                fin >> input;

                if( input == 9 )
                {
                    // Set where the player is supposed to start.
                    playerStartX = x * tileSize;
                    playerStartY = y * tileSize;
                    
                    // Make the tile where the player starts and empty tile,
                    // so nothing funky happens.
                    input = 0;
                }

                level[x][y] = Tile( x * tileSize, y * tileSize, (tileType)input );

            }
        }

        // This set of loops is how all of the collision detection was originally done.
        // Basically what happened here is that we checked each SOLID_TILE and its four
        // adjacent tiles, and whichever side had an open tile (one that the player could
        // possibly move into), we indicated that we need to check that side for collision
        // detection through turning a bit on or off.
        for( int y = 0; y < levelHeight; y++ )
        {
            for( int x = 0; x < levelWidth; x++ )
            {                
                if( level[x][y].getTileType() == SOLID_TILE )
                {
                    // Using getTile rather than referencing the level is both safer
                    // and easier, because getTile will check if you try to get something
                    // that is off the level ( such as level[-1][0] ).
                    
                    // If it's an empty tile, the it's something the player
                    // can walk through, so we need to be able to check if
                    // the player is touching that side.
                    if( getTile( x, y - 1 ) != NULL && getTile( x, y - 1 )->getTileType() == EMPTY_TILE )
                    {
                       level[x][y].setCheckDir( UP, ENABLED );
                    }
                    // Otherwise, we don't need to check that direction.
                    else
                        level[x][y].setCheckDir( UP, DISABLED );

                    // Check if we need to check the bottom side in collision detection.
                    if( getTile( x, y + 1 ) != NULL && getTile( x, y + 1 )->getTileType() == EMPTY_TILE )
                        level[x][y].setCheckDir( DOWN, ENABLED );
                    else
                        level[x][y].setCheckDir( DOWN, DISABLED );

                    // Check if we need to check the left side in collision detection.
                    if( getTile( x - 1, y ) != NULL && getTile( x - 1, y )->getTileType() == EMPTY_TILE )
                        level[x][y].setCheckDir( LEFT, ENABLED );
                    else
                        level[x][y].setCheckDir( LEFT, DISABLED );

                    // Check if we need to check the right side in collision detection.
                    if( getTile( x + 1, y ) != NULL && getTile( x + 1, y )->getTileType() == EMPTY_TILE )
                        level[x][y].setCheckDir( RIGHT, ENABLED );
                    else
                        level[x][y].setCheckDir( RIGHT, DISABLED );

                }
            }
        }

        
        // This set of loops creates all of the vertical hitboxes.
        for( int x = 0; x < levelWidth; x++ )
        {
            for( int y = 0; y < levelHeight; y++ )
            {
                // When we find a solid tile, we want to find all of the solid tiles that are vertically
                // adjacent to it. 
                if( level[x][y].getTileType() == SOLID_TILE )
                {
                    // The number of tiles that are horizontally adjacent.
                    int numBoxes = 1;

                    // Get all the tiles  that are adjacent to each other in the same row.
                    while( getTile( x, y + numBoxes ) != NULL && level[x][ y + numBoxes ].getTileType() == SOLID_TILE )
                    {
                        numBoxes++;
                    }
                    
                    // If we have more than one adjacent box, then we want to make a larger hitbox out of it.
                    if( numBoxes > 1 )
                    {
                        // Once we find the last horizontally adjacent solid tile, create a hit box that
                        // covers all of those horizontally adjacent tiles.
                        hitboxes.push_back( Rect( x * tileSize, y * tileSize, tileSize, numBoxes * tileSize ) );
                        hitboxes[ hitboxes.size() - 1 ].setCheckDirAll();

                        for( int i = 0; i < numBoxes; i++ )
                        {
                            // Get the last hit box on the hit box vector, because it's the one that we just
                            // just created above this, and get all of the tiles contained in the hit box to point
                            // to this hit box. 
                            level[x][ y + i ].setHitBox( hitboxes.size() - 1 );
                        }

                        // When everything's done, we don't want to go over those tiles to be on another hit box,
                        // so we skip over them.
                        y += numBoxes;
                    }

                }
            }

        }

        // This set of loops creates all of the horizontal hitboxes.
        for( int y = 0; y < levelHeight; y++ )
        {
            for( int x = 0; x < levelWidth; x++ )
            {
                // When we find a solid tile, we want to find all of the solid tiles that are horizontally
                // adjacent to it. 
                if( level[x][y].getTileType() == SOLID_TILE )
                {
                    // The number of tiles that are horizontally adjacent.
                    int numBoxes = 1;

                    // Get all the tiles  that are adjacent to each other in the same row.
                    while( getTile( x + numBoxes, y ) != NULL && level[ x + numBoxes ][y].getTileType() == SOLID_TILE )
                    {
                        numBoxes++;
                    }
                    
                    // If we have more than one hit box, then we want to make a larger hit box out of it.
                    if( numBoxes > 1 )
                    {
                        // Once we find the last horizontally adjacent solid tile, create a hit box that
                        // covers all of those horizontally adjacent tiles.
                        hitboxes.push_back( Rect( x * tileSize, y * tileSize, numBoxes * tileSize, tileSize ) );
                        hitboxes[ hitboxes.size() - 1 ].setCheckDirAll();

                        for( int i = 0; i < numBoxes; i++ )
                        {
                            // Get the last hit box on the hit box vector, because it's the one that we just
                            // just created above this, and get all of the tiles contained in the hit box to point
                            // to this hit box. 
                            level[ x + i ][y].setHitBox( hitboxes.size() - 1 );
                        }

                        // When everything's done, we don't want to go over those tiles to be on another hit box,
                        // so we skip over them.
                        x += numBoxes;
                    }
                }
            }
        }
        
    }
}

Tile* World::getTile( int x, int y )
{
    if( x < 0 || x >= levelWidth || y < 0 || y >= levelHeight )
        return NULL;
    else
        return &level[x][y];

}

void World::worldCollisionsWith( Rect* object, int w, int h )
{
    // All of the hitboxes that will be checked for the object.
    std::set<int> toCheck;
    for( int i = object->getMapX(); i < object->getMapX() + w; i++ )
    {
        for( int j = object->getMapY(); j < object->getMapY() + h; j++ )
        {
            if( getTile( i, j ) != NULL && getTile( i, j )->getTileType() == SOLID_TILE )
            {
                toCheck.insert( getTile( i, j )->getHitboxNum() );
            }
        }
    }
                
    //object->setOnGround( false );
    object->setNoSurfaces();
    std::set<int>::iterator it;
    for( it = toCheck.begin(); it != toCheck.end(); it++ )
    {
        direction outDir;

        // If the object touches a block below, say that the object is on the ground.
        if( ( outDir = object->collidesNoOverlap( getHitboxRect( *it ) ) ) == DOWN )
        {
            //object->setOnGround();
            object->setOnSurface( DOWN );
        }
        // If the object touches a block to the left, say so.
        else if( ( outDir = object->collidesNoOverlap( getHitboxRect( *it ) ) ) == LEFT )
        {
            object->setOnSurface( LEFT );
        }
        
        // If the object touches a block below, say so.
        else if( ( outDir = object->collidesNoOverlap( getHitboxRect( *it ) ) ) == RIGHT )
        {
            object->setOnSurface( RIGHT );
        }

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
        //std::cout << output << " ";
    }
    //std::cout << std::endl;

    toCheck.clear();
}

Rect* World::getHitboxRect( int hitboxNum )
{
    // If we try to get a hitbox that doesn't exist,
    // return null
    if( hitboxNum < 0 || hitboxNum >= hitboxes.size() )
        return NULL;
    // If it's a valid hitbox index, return the
    // proper hitbox.
    else
        return &hitboxes[ hitboxNum ];
}

Rect* World::getHitboxFromTile( int x, int y )
{
    // Make sure that the tile exists and is a solid tile, then return the hit box
    // that the solid tile is tied to.
    if( getTile( x, y ) != NULL && getTile( x, y )->getTileType() == SOLID_TILE )
    {
        return &hitboxes[ getTile( x, y )->getHitboxNum() ];
    }
    // If the tile we're trying to retrive doesn't exist,
    // return null
    else
    {
        return NULL;
    }
}