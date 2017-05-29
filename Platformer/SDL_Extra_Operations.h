#ifndef SDL_EXT_OPS_H_
#define SDL_EXT_OPS_H_

#include <iostream>
#include "SDL.h"

bool operator==( const SDL_Rect one, const SDL_Rect two )
{
    return ( one.x == two.x && one.y == two.y && one.w == two.w && one.h == two.h ); 
}

#endif