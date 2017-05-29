#ifndef WORLD_H_
#define WORLD_H_

#include <string>
#include <vector>
#include "constants.h"
#include "rect.h"
#include "tile.h"


// World is the environment around the player.
// It is a grid of tiles.
class World
{
    public:
        World();
        World( std::string inputFile );
        World( const World& inputWorld );

        ~World();

        void loadLevel( std::string inputFile );

        Tile* getTile( int x, int y );
        
        // Does all collision detection with no overlap for an
        // input rectangular object (such as the player) over an
        // area of tiles that is w x h tiles.
        void worldCollisionsWith( Rect* object, int w, int h );

        // Getters
        int getWidth() { return levelWidth; }
        int getHeight() { return levelHeight; }
        int getTileSize() { return tileSize; }

        int getPlayerStartX() { return playerStartX; }
        int getPlayerStartY() { return playerStartY; }

        int getNumHitboxes() { return hitboxes.size(); }
        Rect* getHitboxRect( int hitboxNum );
        Rect* getHitboxFromTile( int x, int y );

    private:
        

        // The size of the level.
        int levelWidth,
            levelHeight;

        // The width of a tile in pixels.
        int tileSize;

        // The space the player moves around on.
        // It is a 2 dimensional grid of tiles.
        Tile** level;
        
        int playerStartX,
            playerStartY;

        // boxes used for collision detection
        std::vector<Rect> hitboxes;
};

#endif