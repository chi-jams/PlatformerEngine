

#include <ctime>
#include "SDL.h"
#include "animation.h"

Animation::Animation()
{
    spriteSheet = NULL;

    startFrame = new SDL_Rect();
    startFrame->x = 0;
    startFrame->y = 0;
    startFrame->w = 0;
    startFrame->h = 0;

    numFrames = 0;
    curFrame = 0;

    displayTime = 0;
    lastChange = 0;

}

Animation::Animation( Texture* spriteSheet, SDL_Rect frame, int numFrames, int frameDelay )
{
    initAnimation( spriteSheet, frame, numFrames, frameDelay );
}

void Animation::initAnimation( Texture* spriteSheet, SDL_Rect frame, int numFrames, int frameDelay )
{
    setTexture( spriteSheet );
    
    startFrame = new SDL_Rect( frame );

    // Get how many frames there are and set it tot he first frame.
    this->numFrames = numFrames;
    curFrame = 0;

    // Get how long of a time we want between frames.
    displayTime = frameDelay;
    lastChange = 0;
}

Animation::~Animation()
{
    delete startFrame;
}

void Animation::setTexture( Texture* spriteSheet )
{
    this->spriteSheet = spriteSheet;
}

void Animation::render( int x, int y, SDL_RendererFlip flip )
{
    SDL_Rect outFrame = *startFrame;

    outFrame.x += outFrame.w * curFrame;

    if( numFrames > 1 )
    {
        // If the frame has been shown its designated time, change to the next frame.
        int curTime = clock();
        if( lastChange + displayTime < clock() )
        {
            lastChange = curTime;
            curFrame++;
            curFrame %= numFrames;
        }
    }

    spriteSheet->render( x, y, &outFrame, flip );
}
