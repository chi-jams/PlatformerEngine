#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "SDL2/SDL.h"
#include "texture.h"

class Animation
{ 
    private:

        // A pointer to the sprite sheet that the animation will be using.
        // This will be an object defined outside the animation object,
        // and will be used for multiple animation objects.
        Texture* spriteSheet;

        // The first frame in the animation.
        // Also contains information for frame
        // width and height.
        SDL_Rect* startFrame;

        // The number of frames in an animations,
        // and the one that is currently being
        // rendered.
        int numFrames,
            curFrame;

        // The amount of time one fram should be
        // displayed before switching tot he next.
        // Also the last time the framed changed.
        int displayTime,
            lastChange;

    public:
        Animation();
        Animation( Texture* spriteSheet, SDL_Rect frame, int numFrames, int frameDelay );

        void initAnimation( Texture* spriteSheet, SDL_Rect frame, int numFrames, int frameDelay );

        ~Animation();

        // Gets the texture that is going to be used for the animation.
        // Note that this class will only look at a texture, and will
        // never manipulate it in any way.
        void setTexture( Texture* spriteSheet );

        void render( int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE );

};

#endif
