

#include <string>
#include <iostream>
#include "LTexture.h"
#include "SDL.h"
#include "SDL_image.h"


extern SDL_Renderer* mainRenderer;

LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    freeLTexture();
}

bool LTexture::loadFromFile( std::string path )
{
    // Make sure we're not overwriting the texture
    // Freeing first ensures that we don't have a memory leak
    freeLTexture();

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        std::cout << "Failed to load surface! SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Color key the image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0, 0 ) ); 

        mTexture = SDL_CreateTextureFromSurface( mainRenderer, loadedSurface );
        if( mTexture == NULL )
        {
            std::cout << "Failed to create texture! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            // Get dimensions of surface
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface( loadedSurface );
    }

    return mTexture != NULL;
}

void LTexture::freeLTexture()
{
    // Makes sure that there is a texture to destroy before destroying it.
    // We don't want to be deleting something random in memory.
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    // Modulate texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{

    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
    SDL_Rect renderRect = { x, y, mWidth, mHeight };
    
    if( clip != NULL )
    {
        renderRect.w = clip->w;
        renderRect.h = clip->h;
    }
    SDL_RenderCopy( mainRenderer, mTexture, clip, &renderRect );
}

int LTexture::getHeight()
{
    return mHeight;
}

int LTexture::getWidth()
{
    return mWidth;
}