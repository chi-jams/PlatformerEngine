#ifndef RECT_H_
#define RECT_H_

#include <bitset>
#include "constants.h"

class Rect 
{
    protected:

        // Critical for game updating, we're using a variable time step
        // for updating the game, meaning that we move things according
        // to how many seconds have passed, rather than now many update
        // cycles have occured.
        //int timeLastPolled;

        // Position of the top left corner of the rectangle.
        float x, y;

        // Dimensions of the rectangle.
        int w, h;

        // Velocity of the object.
        float xVel, yVel;

        // What tile the top-left corner
        // of the rectangle is on on the map.
        int mapX, mapY;

        /* Stuff that's used with gravity */

        // Is gravity being used on this object?
        bool gravityEnabled;

        // If the rectangle is on the ground (poised to jump)
        // Also, check if the rectangle is still trying to jump upward.
        bool onGround,
            ascending;

        // Shows which side the object is contacting, important for jumping,
        // both on the ground jumping and wall jumping.
        bool onSurface[NUM_DIRECTIONS];

        int jumpSpeed;

        // For use with gravity. The fastest an object can go in the game.
        int terminalVelocity;

        // The rate at which objects are pulled downward.
        // It's a positive value because the window's y axis is 0 at the top
        // and grows downward.
        int gravity;

        // The directions that a specific tile will be contacted from. 
        std::bitset< NUM_DIRECTIONS > check_dir;

        // Which way the rectangle (if any) that the rectangular object
        // is looking.
        direction facing;
    public:

        Rect();
        Rect( int x, int y, int w, int h );

        // Does all the moving of the rectangle object.
        void updateRect( double deltaTime );

        // Updates how much time has elapsed since the last cycle, used in
        // all of the time based updating methods.
        //void updateTime( int curTime );

        // Checks if two rectangles are touching each other.
        bool intersects( Rect* inputRect );

        // Check if two rectangles are touching each other,
        // and if they are, checks which side is touching.
        direction collides( Rect* inputRect );

        // When two rectangles collide, ensure that neither of them
        // overlap. Moves this rectangle to just touching the input
        // rectangle. Additionally notifies which side of this
        // rectangle touched the input rectangle.
        direction collidesNoOverlap( Rect* inputRect );

        // If gravity is enabled on the object, make that object fall.
        void updateGravity( double deltaTime );

        // If gravity applies, and the rectangle can jump
        void jump();
        void stopAscending();        

        // Setters
        void setCheckDirAll();
        void setCheckDir( direction dir, int enabled );

        void setX( int x ) { this->x = x; }
        void sety( int y ) { this->y = y; }
        void setW( int w ) { this->w = w; }
        void setH( int h ) { this->h = h; }

        void setMapX( int x ) { mapX = x; }
        void setMapY( int y ) { mapY = y; }

        void setFacing( direction input ) { facing = input; }

        void setOnGround( bool input = true ) { onGround = input; }
        void setOnSurface( direction dir, bool input = true ) { onSurface[dir] = input; }
        // Sets all varialbes in onSurface to false
        void setNoSurfaces();

        // Getters
        int getXPos() { return x; }
        int getYPos() { return y; }
        int getWidth() { return w; }
        int getHeight() { return h; }

        int getXVel() { return xVel; }
        int getYVel() { return yVel; }

        int getMapX() { return mapX; }
        int getMapY() { return mapY; }

        int getCheckDir( direction dir ) { return check_dir[ dir ]; }

        direction getFacing() { return facing; }

        // Checks if functionality with gravity is enabled.
        bool isGravityEnabled() { return gravityEnabled; }

        // :V
        bool isOnGround() { return onGround; }

        // Returns if a player is contacting a wall or a floor on a particular side.
        bool isOnSurface( direction dir ) { return onSurface[dir]; }

        // Check if the rectangle is actively trying to ascend
        bool isAscending() { return ascending; } 

};

#endif
