#ifndef PLAYER_H_
#define PLAYER_H_

#include "constants.h"
//#include "Tile.h"
#include "rect.h"

class Player: public Rect
{
    private:

        static const int DEFAULT_PLAYER_SIZE = 50;

        // Multipliers for velocity
        // This value is multiplied by -1, 0, +1 to
        // determine where the player is going
        int xSpeed,
            ySpeed;

        // This is the rate at which 
        int xAccel;

        // Misc. information
        int health;

        // Ensures that if the player holds down jump, he or she will not jump
        // more than once.
        bool jumpReleased;

    public:

        // Constructors
        Player();
        Player( int xPos, int yPos, int world_size = DEFAULT_PLAYER_SIZE );

        // Updated version of player actions, uses variable time step
        void movePlayer( int moveDir[4], int curTime );

        int getPlayerHealth() { return health; }       
};


#endif
