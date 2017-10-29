
#include <algorithm>
#include <utility>
#include "constants.h"
#include "tile.h"

Tile::Tile()
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;

    xVel = 0;
    yVel = 0;

    type = NULL_TILE;

    facing = NONE;
    
    //gravityEnabled = false;
}


Tile::Tile( int x, int y, tileType type )
{
    this->x = x;
    this->y = y;
    w = DEFAULT_TILE_SIZE;
    h = DEFAULT_TILE_SIZE;
    this->type = type;

    xVel = 0;
    yVel = 0;

    facing = NONE;

    //gravityEnabled = false;
}

Tile::Tile( const Tile& inputTile )
{
    Copy( inputTile );
}

void Tile::Copy( const Tile& inputTile )
{
    this->x = inputTile.x;
    this->y = inputTile.y;
    this->w = inputTile.w;
    this->h = inputTile.h;
    this->type = inputTile.type;

    this->xVel = inputTile.xVel;
    this->yVel = inputTile.yVel;

    this->facing = inputTile.facing;
}

Tile& Tile::operator=( const Tile& inputTile )
{
    if( &inputTile == this )
    {
        return *this;
    }
    else
    {
        Copy( inputTile );
        return *this;
    }
}