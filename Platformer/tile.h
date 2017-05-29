#ifndef TILE_H_
#define TILE_H_

#include <bitset>
#include "constants.h"
#include "rect.h"

// One square that is draw on the map.
// These compose all of the terrain in the world.
class Tile: public Rect
{
    public:

        Tile();
        Tile( int x, int y, tileType type );
        Tile( const Tile& inputTile );

        void Copy( const Tile& inputTile );

        tileType getTileType() { return type; }

        Tile& operator=( const Tile& inputTile );

        void setHitBox( int hitBoxNum ) { hitBox = hitBoxNum; }

        int getHitboxNum() { return hitBox; }

    private:

        const int DEFAULT_TILE_SIZE = 50;

        tileType type;

        // The hitbox that the specific tile is tied
        // to. The hitbox is a combination between all
        // adjecent tiles in a straight line.
        int hitBox;
};


#endif