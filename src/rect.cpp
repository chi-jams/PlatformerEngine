
#include <iostream>
#include "constants.h"
#include "rect.h"

Rect::Rect()
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}


Rect::Rect( int x, int y, int w, int h )
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void Rect::updateRect( int deltaTime )
{    
    // the 1000 is for 1000 miliseconds, we multipy
    x += ( xVel / 1000.0 ) * deltaTime;
    y += ( yVel / 1000.0 ) * deltaTime;
    
    //std::cout << x << " " << y << std::endl;
}

/*
void Rect::updateTime( int curTime )
{
    deltaTime = curTime - timeLastPolled;
    timeLastPolled = curTime;

    // If for some reason the delta time has exceeded MAX_DELTA_TIME, change delta time to 
    // MAX_DELTA_TIME, because something unnatural has happened (either major computer lag,
    // or the user has moved the window around)
    if( deltaTime > MAX_DELTA_TIME )
    {
        deltaTime = MAX_DELTA_TIME;
    }
}
*/

// This function only checks if two rectangles overlap
// each other.
bool Rect::intersects( Rect* inputRect )
{
    // corner positions for the input Rectangle.
    int inX1 = inputRect->x,
        inY1 = inputRect->y,
        inX2 = inX1 + inputRect->w,
        inY2 = inY1 + inputRect->h;

    // Bottom left corner position for the class's rectangle.
    int x2 = x + w,
        y2 = y + h;

    return ( x < inX2 && x2 > inX1 && y < inY2 && y2 > inY1 );
}

// This function first checks if two rectangles overlap eachother,
// and if they do, checks which side of this rectangle is contacting
// the input rectangle.
direction Rect::collides( Rect* inputRect )
{
    const float MINIMUM_OVERLAP = 0.2;

    if( intersects( inputRect ) )
    {
        // xC means "x center," it indicates the location of the center
        // of the rectangle
        int xC = x + ( w / 2 ),
            yC = y + ( h / 2 );

        // Center of the input rectangle.
        int iXC = inputRect->x + ( inputRect->w / 2 ),
            iYC = inputRect->y + ( inputRect->h / 2 );

        // The distance between the centers of each rectangle.
        int dx = xC - iXC,
            dy = yC - iYC;


        // All of the overlapping sides that are going to be compared to each other
        // each comparison is labeled as this rectangle's side followed by the input
        // rectangle's side
        int topBot = inputRect->y + inputRect->h - y,
            botTop = y + h - inputRect->y,
            leftRight = inputRect->x + inputRect->w - x,
            rightLeft = x + w - inputRect->x;

        // The smaller of the two's widths/heights.
        int minH = h,
            minW = w;

        if( inputRect->h < h )
            minH = inputRect->h;
        
        if( inputRect->w < w )
            minW = inputRect->w;

        // How much two rectangles are overlapping each other in a particular axis
        // Percentages are multiplied by 100, and are out of the thinnest/shortest
        // rectangle.
        float xOverlap,
              yOverlap;

        // Check if the rectangles are totally overlapping vertically        
        if( topBot > botTop )
            yOverlap = (float)botTop / minH;
        else
            yOverlap = (float)topBot / minH;
        

        // Check if the rectangles are totally overlapping horizontally        
        if( leftRight > rightLeft )
            xOverlap = (float)rightLeft / minW;
        else
            xOverlap = (float)leftRight / minW;
        
        //std::cout << xOverlap << " " << yOverlap << std::endl;
        //std::cout << topBot << " " << botTop << " " << leftRight << " " << rightLeft << std::endl;

        direction xDir = NONE,
                  yDir = NONE;
        
        // Check if we we have checking the right side enabled, then check if the center of the
        // input rectangle is to the right of this rectangle.
        if( dx < 0 )
            xDir = RIGHT;
        // Do the same for the left.
        else if( dx > 0 )
            xDir = LEFT;

        if( dy < 0 )
            yDir = DOWN;
        else if( dy > 0 )
            yDir = UP;

        if( xDir != NONE && check_dir[ xDir ] == ENABLED && yOverlap > MINIMUM_OVERLAP && yOverlap > xOverlap )
            return xDir;
        else if( yDir != NONE && check_dir[ yDir ] == ENABLED && xOverlap > MINIMUM_OVERLAP )
            return yDir;
        else
            return NONE;
    }
    else
    {
        return NONE;
    }
}

// This functions checks which side has collided, then moves this
// rectangle so that the two rectangles are just touching each other.
direction Rect::collidesNoOverlap( Rect* inputRect )
{
    // Return whatever side the inputRect hit.
    direction contactSide;

    switch( contactSide = collides( inputRect ) )
    {
        case NONE:
            // If the rectangles didn't collide
            // on a valid side then don't do
            // anything.
            break;
        case UP:
            // If the top of this rectangle hits the input rectangle,
            // then move this rectangle so that the top edge of this
            // rectangle is touching the bottom edge of the input 
            // rectangle
            y = inputRect->getYPos() + inputRect->getHeight();
            
            // If the object is ascending and it hits an object, stop it.
            if( yVel < 0 )
                yVel = 0;

            break;
        case DOWN:
            // If the bottom of this rectangle hits the input rectangle,
            // then move this rectangle so that the bottom edge of this
            // rectangle is touching the top edge of the input rectangle
            // +1 is to ensure that they're still counting as touching
            // during the next game loop.
            y = inputRect->getYPos() - h + 1;

            // If the player is falling, make the player stop falling.
            if( yVel > 0 )
                yVel = 0;

            break;
        case LEFT:
            // If the left side of this rectangle hits the input rectangle,
            // move this rectangle so that the left edge of this rectangle
            // is touching the right edge of the input rectangle.
            x = inputRect->getXPos() + inputRect->getWidth() - 1;

            // If the the player is walking to the left, stop letting him
            // walk to the left.
            if( xVel < 0 )
                xVel = 0;

            break;
        case RIGHT:
            // If the right side of this rectangle hits the input rectangle,
            // move this rectangle so that the left edge of this rectangle is
            // touching the right edge of the input rectangle.
            x = inputRect->getXPos() - w + 1;

            // If the player is walking to the right, stop letting him walk
            // to the right.
            if( xVel > 0 )
                xVel = 0;
            break;
    }

    return contactSide;
}

/* Gravity-enabled functionality */

// Literally just making stuff fall. Seriously, not that hard to figure out.
// Now based off of time rather than update speed.
void Rect::updateGravity( int deltaTime )
{
    // Check if gravity is enabled, if it is, then enable this functionality
    if( gravityEnabled )
    {
        // If we're not on the ground, and we haven't reached terminal velocity
        // yet, continue to speed up fall1Eing.

        // ^has been changed to just constantly be pulling the rectangle towards
        // the ground. What happened is that there's a circular relationship
        // between an object being on the ground and the object falling, causing
        // the hitboxes below to only register half of the time, which is a problem.

        // Constantly pulls objects towards the bottom of the screen.
        if( yVel < terminalVelocity )
        {
            yVel += ( gravity / 1000 ) * deltaTime;
        }
        else 
        {
            yVel = terminalVelocity;
        }
        
        // If the player has already slowed down past the point that the player
        // would be at if he or she canceled his jump, don't allow him to
        // "stop ascending" at that point to perform a small second hop.
        if( yVel > -jumpSpeed / 2 )
        {
            ascending = false;
        }
    }
}

// Makes a rectangle jump at its jump speed, primarily for the player, but it's
// there just in case other rectangles need it.
void Rect::jump()
{
    // Check if gravity is enabled, if it is, then enable this functionality
    if( gravityEnabled )
    {
        // If we're on the ground, then start jumping into the air.
        if( onSurface[DOWN] )
        {
            onSurface[DOWN] = false;
            ascending = true;

            yVel = -jumpSpeed;
        }
        // If's we're on a wall, we want to wall jump, going diagonally upward
        // into the air.
        else if( onSurface[LEFT] )
        {
            onSurface[LEFT] = false;
            ascending = true;

            // Instead of going straight up, we want the object to be jumping
            // away from the wall at a 45 degree angle.
            yVel = -jumpSpeed;
            xVel = jumpSpeed / 2;
        }
        else if( onSurface[RIGHT] )
        {
            onSurface[RIGHT] = false;
            ascending = true;

            // Instead of going straight up, we want the object to be jumping
            // away from the wall at a 45 degree angle.
            yVel = -jumpSpeed;
            xVel = -jumpSpeed / 2;
        }
    }
}

// stopAscending is mainly for the player, but it what it does is it cancels
// a rectangle's jump mid-air, leaving a little bit of velocity so the jump
// still looks smooth.
void Rect::stopAscending()
{
    // Check if gravity is enabled, if it is, then enable this functionality
    if( gravityEnabled )
    {
        // If we are still ascending, then we want to stop ascending
        if( ascending )
        {
            ascending = false;

            yVel = -jumpSpeed / 2;
        }
    }
}

// Sets whether or not a particular direction is checked
// during collision detection.
void Rect::setCheckDir( direction dir, int enabled )
{
    check_dir[dir] = enabled;
}

void Rect::setCheckDirAll()
{
    for( int i = 0; i < NUM_DIRECTIONS; i++ )
    {
        check_dir[i] = ENABLED;
    }
}

void Rect::setNoSurfaces()
{
    for( int i = 0; i < NUM_DIRECTIONS; i++ )
    {
        onSurface[i] = false;
    }
}
