#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// Different types of tiles used in world.
enum tileType 
{
    NULL_TILE = -1,     // Tile that doesn't exist
    EMPTY_TILE = 0,     // Tile that player can move through
    SOLID_TILE = 1,     // Tile that player cannot move through
}; 

// Directions for everything to move.
enum direction
{
    NONE = -1,
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    NUM_DIRECTIONS
};

// Goes with direction, tells if there is something input to move
// a pariticular direction.
const int STOPPED = 0;
const int MOVING = 1;

const bool DISABLED = false;
const bool ENABLED = true;

#endif