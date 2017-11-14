

#include <iostream>
#include "player.h"
#include "constants.h"

Player::Player()
{
    // See above.
    x = 0;
    y = 0;

    xVel = 0;
    yVel = 0;

    w = DEFAULT_PLAYER_SIZE;
    h = w * 2;

    // We want to check all four sides of the
    // player to see if he's colliding with
    // anything.
    for( int i = 0; i < NUM_DIRECTIONS; i++ )
    {
        check_dir[i] = ENABLED;
    }

    // Movement is now based off of units per second.
    xSpeed = 550;
    ySpeed = 2000;

    xAccel = 4000;

    health = 100;

    jumpReleased = false;

    // See rect.h for variable notes.
    gravityEnabled = true;
    onGround = false;
    ascending = false;

    setNoSurfaces();

    jumpSpeed = 2000;
    terminalVelocity = 2000;
    gravity = 8000;

    facing = RIGHT;

    timeLastPolled = 0;

    // 34 miliseconds is the max amount of time between updates allowed.
    // This is the amount that would cause the game to run at 30 fps.
    MAX_DELTA_TIME = 34;
}

Player::Player( int x, int y, int world_size )
{
    // Give the player all of its default values
    *this = Player();

    this->x = x;
    this->y = y;

    w = world_size;
    h = w * 2;
}

// OLD move player function, use the one that has curTime as a function
void Player::movePlayer( int moveDir[4] )
{
    // Move the player left or right.
    xVel = moveDir[ RIGHT ] * xSpeed - moveDir[ LEFT ] * xSpeed;

    // If the up button is pressed, then jump
    if( moveDir[ UP ] && jumpReleased )
    {
        jump();
        jumpReleased = false;
    }
    // If the player wants to make the jump shorter,
    // slow down significantly and begin falling.
    else if( !moveDir[ UP ] && ascending )
    {
        stopAscending();
    }
    // If the player has landed and has released the jump button,
    // then allow them to jump the next time they try to jump
    else if( !moveDir[ UP ] && onGround )
    {
        jumpReleased = true;
    }

    updateGravity();

    if( xVel > 0 )
        facing = RIGHT;
    else if( xVel < 0 )
        facing = LEFT;

    updateRect();
}


void Player::movePlayer( int moveDir[4], int curTime )
{
    updateTime( curTime );

    //WALKING//
    
    // The player will change his movement based on a multiple of this value.
    float runSpeed = ( (float)xAccel / 1000 ) * deltaTime;

    // If the player wants to move right, start accelerating to the right,
    // and maintain their speed if they're going as fast or faster than the
    // normal walking speed.
    if( moveDir[RIGHT] )
    {
        // If the player is moving the opposite direction,
        // then we want to stop at double speed.
        if( xVel < 0 )
            xVel += 2 * runSpeed;
        // Otherwise, accelerate until we're moving as fast as we can.
        if( xVel < xSpeed )
            xVel += runSpeed;
    }
    // Same thing as above, excpet move left.
    else if( moveDir[LEFT] )
    {
        // Read above comments.
        if( xVel > 0 )
            xVel += -2 * runSpeed;
        if( xVel > -xSpeed )
            xVel += -runSpeed;
    }
    // If the player desires to stop...
    else
    {
        // If they are within a small margin around stopped, stop the player.
        if( std::abs( xVel ) < runSpeed )
        {
            xVel = 0;
        }
        // Otherwise, slow the player down based on xAccel.
        else
        {
			if( xVel != 0.0 )
				xVel += -( xVel / std::abs( xVel ) ) * runSpeed / 2;
        }

    }


    //JUMPING

    // If the up button is pressed, then jump
    if( moveDir[ UP ] && jumpReleased )
    {
        jump();
        jumpReleased = false;
    }
    // If the player wants to make the jump shorter,
    // slow down significantly and begin falling.
    else if( !moveDir[ UP ] && ascending )
    {
        stopAscending();
    }
    // If the player has landed and has released the jump button,
    // then allow them to jump the next time they try to jump
    else if( !moveDir[ UP ] && ( onSurface[DOWN] || onSurface[LEFT] || onSurface[RIGHT] ) )
    {
        jumpReleased = true;
    }

    // environmental conditions/updating

    // Make the player grip onto surfaces, so he'll slide down walls slower.
    if( onSurface[LEFT] || onSurface[RIGHT] )
    {
        terminalVelocity = 500;
    }
    else
    {
        terminalVelocity = 2000;
    }

    updateGravity();

    if( xVel > 0 )
        facing = RIGHT;
    else if( xVel < 0 )
        facing = LEFT;

    updateRect();
}
